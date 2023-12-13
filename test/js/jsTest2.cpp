//
// Created by william on 2023/12/12.
//

#include "cJson.h"
#include "tStrStream.h"

#include <arpa/inet.h>
#include <fcntl.h>
#include <iostream>
#include <libplatform/libplatform.h>
#include <map>
#include <set>
#include <sstream>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>
#include <v8-inspector.h>
#include <v8.h>

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define INVALID_16BITID 0xffff
#define INVALID_32BITID 0xffffffff
#define ELEM_COUNT(_array) ((uint32)(sizeof(_array) / sizeof(_array[0])))

class CDebugCmd;

typedef TConstString<char> const_string;
class CBreakPoint : public const_string
{
    uint32 m_breakPointID;
    uint32 m_nFileNameStart;
    uint32 m_nLineNum;

public:
    CBreakPoint(uint32 nID, const char* szFileName, bool bRef, uint32 uLineNum);
    bool operator<(const CBreakPoint& ano) const;
    bool operator==(const CBreakPoint& ano) const;
    const char* GetModuleName() const { return c_str() + m_nFileNameStart; }
    uint32 GetBreakPointID() const { return m_breakPointID; }
    uint32 GetLineNum() const { return m_nLineNum; }
};

typedef std::set<CBreakPoint> CBreakPointList;
typedef std::vector<std::string> CFileLines;
typedef std::map<std::string, CFileLines> CFileMap;
typedef std::map<std::string, std::string> CPathRedirectMap;
typedef TList<CDebugCmd> CDebugCmdList;
typedef CDebugCmdList::CListNode CDebugNode;
class CDebugCmd : public CDebugNode, public CJson
{};

std::string getRootDir()
{
    static std::string path = SOURCE_DIR;
    return path;
}

static std::string getFileContents(const std::filesystem::path& filename)
{
    std::ifstream in{ filename, std::ios::in | std::ios::binary };
    if (in)
    {
        std::string contents;
        in.seekg(0, std::ios::end);
        auto size = in.tellg();
        if (size > 0)
        {
            contents.resize((std::string::size_type)size);
            in.seekg(0, std::ios::beg);
            in.read(&contents[0], contents.size());
        }
        in.close();
        return contents;
    }
    return {};
}

void printCallback(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    if (args.Length() < 1) return;
    v8::Isolate* isolate = args.GetIsolate();
    v8::HandleScope scope(isolate);
    v8::String::Utf8Value value(isolate, args[0]);
    std::cout << *value << std::endl;
}

enum class AttachType
{
    Detach,
    Waiting,
    Launch,
    Attach
};
typedef std::unique_ptr<v8_inspector::V8InspectorSession> CInsSessionPtr;

static intptr_t s_remoteListener = 0;
static intptr_t s_remoteConnecter = 0;
static bool s_bEnterDebug = false;
static AttachType s_attachType = AttachType::Attach;
static CBreakPointList s_setBreakPoint;
static CFileMap s_mapFileBuffer;
static uint32 s_messageID = 1;
typedef std::map<uint32, std::string> IDStringMap;
static IDStringMap s_mapBreakPoint;
CInsSessionPtr s_session;
static CPathRedirectMap s_mapRedirectPath;
static bool m_expectStep = false;
static std::string s_sendStr;
std::mutex s_cmdMutex;
CDebugCmdList s_listDebugCmd;
std::thread s_hThread;

int32 s_curLine;

static void sendNetData(CJson* pJson)
{
    if (s_remoteConnecter != INVALID_SOCKET)
    {
        std::stringstream ss;
        pJson->Save(ss, INVALID_32BITID);
        const std::string& strResult = ss.str();
        auto length = (uint32_t)strResult.size();
        char szBuffer[256];
        snprintf(szBuffer, ELEM_COUNT(szBuffer) - 1, "Content-Length:%d\r\n\r\n", length);
        s_sendStr.append(szBuffer);
        s_sendStr.append(strResult);
    }
    delete pJson;
}

static void sendRespone(CJson* pBody, const char* szSequence, bool bSucceeded, const char* szCommand, const char* szMsg = "")
{
    CJson* pRespone = new CJson;
    pRespone->AddChild("type", "response");
    pRespone->AddChild("request_seq", szSequence);
    pRespone->AddChild("success", bSucceeded ? "true" : "false");
    pRespone->AddChild("command", szCommand);
    pRespone->AddChild("message", szMsg);
    if (pBody)
        pRespone->AddChild(pBody);

    sendNetData(pRespone);
}

static void onNetData(CDebugCmd* pCmd)
{
    std::lock_guard<std::mutex> lock(s_cmdMutex);
    s_listDebugCmd.PushBack(*pCmd);
}

static void TeminateRemote(const char* sequence)
{
    s_attachType = AttachType::Detach;
    if (sequence)
        sendRespone(nullptr, sequence, true, "terminate");

    CDebugCmd* pCmd = new CDebugCmd;
    pCmd->AddChild("seq", "0");
    pCmd->AddChild("type", "request");
    pCmd->AddChild("command", "_clearall");
    onNetData(pCmd);
    close(s_remoteListener);
    s_remoteListener = INVALID_SOCKET;
    s_sendStr.clear();
}

static bool reciveRemoteData(char (&szBuffer)[2048], int32 nCurSize)
{
    std::string strBuffer;
    bool quit = false;
    while (s_attachType != AttachType::Detach && nCurSize > 0 && !quit)
    {
        strBuffer.append(szBuffer, nCurSize);
        while (!quit)
        {
            std::string::size_type nFlagPos = strBuffer.find("Content-Length");
            if (nFlagPos == std::string::npos)
                break;
            std::string::size_type nStartPos = strBuffer.find(':', nFlagPos);
            if (nStartPos == std::string::npos)
                break;
            std::string::size_type nEndPos = strBuffer.find("\r\n\r\n", nStartPos);
            if (nEndPos == std::string::npos)
                break;
            const char* szStr = strBuffer.c_str() + nStartPos + 1;
            uint32 nDataSize = (uint32)strtol(szStr, NULL, 0);
            if (strBuffer.size() < nEndPos + 4 + nDataSize)
                break;
            CDebugCmd* pCmd = new CDebugCmd;
            pCmd->Load(strBuffer.c_str() + nEndPos + 4, nDataSize);
            onNetData(pCmd);
            strBuffer.erase(0, nEndPos + 4 + nDataSize);
        }

        while (!quit)
        {
            fd_set fdValid;
            FD_ZERO(&fdValid);
            FD_SET(s_remoteConnecter, &fdValid);

            struct timeval tv;
            tv.tv_sec = 0;
            tv.tv_usec = 100000;
            if (!select((int32)(s_remoteConnecter + 1), &fdValid, NULL, NULL, &tv))
                continue;
            nCurSize = (int32)recv(s_remoteConnecter, szBuffer, 2048, 0);
            break;
        }
    }
    return true;
}

static void socketRun()
{
    bool quit = false;
    while (!quit)
    {
        fd_set fdValid;
        FD_ZERO(&fdValid);
        FD_SET(s_remoteListener, &fdValid);

        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 100000;
        if (!select((int32_t)(s_remoteListener + 1), &fdValid, NULL, NULL, &tv))
            continue;

        sockaddr_in Address;
        socklen_t nSize = sizeof(sockaddr_in);
        s_remoteConnecter = accept(s_remoteListener, (sockaddr*)&Address, &nSize);
        if (s_remoteConnecter == INVALID_SOCKET)
            continue;
        s_attachType = AttachType::Waiting;
        m_expectStep = false;
        char szBuffer[2048];
        auto nResult = (int32_t)recv(s_remoteConnecter, szBuffer, 2048, 0);
        if (reciveRemoteData(szBuffer, nResult))
        {
            TeminateRemote(nullptr);
            continue;
        }
        close(s_remoteConnecter);
        s_remoteConnecter = INVALID_SOCKET;
        s_sendStr.clear();
    }
}

static bool RemoteCmdValid()
{
    return !s_listDebugCmd.IsEmpty();
}

static void sendEvent(CJson* pBody, const char* szEvent)
{
    CJson* pEvent = new CJson;
    pEvent->AddChild("type", "event");
    pEvent->AddChild("event", szEvent);
    if (pBody)
        pEvent->AddChild(pBody);
    sendNetData(pEvent);
}

static void delBreakPoint(uint32 breakPointID)
{
    for (auto it = s_setBreakPoint.begin(); it != s_setBreakPoint.end(); ++it)
    {
        const CBreakPoint& BreakPoint = *it;
        if (breakPointID != BreakPoint.GetBreakPointID())
            continue;
        s_setBreakPoint.erase(it);
        return;
    }

    auto it = s_mapBreakPoint.find(breakPointID);
    if (it == s_mapBreakPoint.end())
        return;
    char szCommand[4096];
    char_stream(szCommand)
        << "{\"id\":" << s_messageID++ << ","
        << "\"method\":\"Debugger.removeBreakpoint\","
        << "\"params\":{\"breakpointId\":\"" << it->second << "\"}}";
    v8_inspector::StringView view((const uint8_t*)szCommand, strlen(szCommand));
    s_mapBreakPoint.erase(it);
    s_session->dispatchProtocolMessage(view);
}

bool ProcessCommand(CDebugCmd* pCmd)
{
    char szBuf[256];
    const char* szCommand = pCmd->At<const char*>("command");
    const char* szSequence = pCmd->At<const char*>("seq");
    if (!strcasecmp(szCommand, "initialize"))
    {
        CJson* pBody = new CJson("body");
        pBody->AddChild("supportsEvaluateForHovers", "true");
        //pBody->AddChild( "supportsExceptionInfoRequest", "true" );
        //pBody->AddChild( "supportsExceptionOptions", "true" );
        sendRespone(pBody, szSequence, true, szCommand);
        sendEvent(nullptr, "initialized");
        return true;
    }
    else if (!strcasecmp(szCommand, "launch"))
    {
        CJson* pArg = pCmd->GetChild("arguments");
        bool bNoDebug = pArg->At<bool>("noDebug");
        s_attachType = bNoDebug ? AttachType::Detach : AttachType::Launch;
        sendRespone(nullptr, szSequence, true, szCommand);
        return true;
    }
    else if (!strcasecmp(szCommand, "attach"))
    {
        s_mapRedirectPath.clear();
        s_attachType = AttachType::Attach;
        CJson* pArg = pCmd->GetChild("arguments");
        std::string strPaths = pArg->At<std::string>("cwd");
        auto listPaths = SeparateString(strPaths.c_str(), '|');
        for (auto strPathPair : listPaths)
        {
            auto vecPathPair = SeparateString(strPathPair.c_str(), '<');
            if (vecPathPair.size() != 2)
                continue;
            s_mapRedirectPath[vecPathPair[0]] = vecPathPair[1];
        }
        sendRespone(nullptr, szSequence, true, szCommand);
        return true;
    }
    else if (!strcasecmp(szCommand, "loadedSources"))
    {
        CJson* pBody = new CJson("body");
        CJson* pSourceArray = pBody->AddChild("sources");
        CFileMap::iterator it = s_mapFileBuffer.begin();
        for (; it != s_mapFileBuffer.end(); ++it)
        {
            CJson* pSource = pSourceArray->AddChild("");
            std::string strpath = it->first.c_str();
            for (auto Path : s_mapRedirectPath)
            {
                const char* pstr = strstr(strpath.c_str(), Path.first.c_str());
                if (!pstr)
                    continue;
                strpath = Path.second + (pstr + Path.first.length());
                break;
            }

            pSource->AddChild("path", strpath.c_str());
            pSource->AddChild("name", GetFileNameFromPath(it->first.c_str()));
            if (it->first.c_str()[0] != '/' && it->first.find(':') == INVALID_32BITID)
            {
                pSource->AddChild("presentationHint", "normal");
                pSource->AddChild("sourceReference", "1");
            }
        }
        sendRespone(pBody, szSequence, true, szCommand);
        return true;
    }
    else if (!strcasecmp(szCommand, "terminate") ||
             !strcasecmp(szCommand, "disconnect"))
    {
        auto attachType = s_attachType;
        TeminateRemote(szSequence);
        if (attachType != AttachType::Launch)
            return true;
#ifdef _WIN32
        TerminateProcess(GetCurrentProcess(), 0);
#else
        exit(0);
#endif
        return true;
    }
    else if (!strcasecmp(szCommand, "setBreakpoints"))
    {
        std::cout << "setBreakpoints" << std::endl;
        return {};
    }
    else if (!strcasecmp(szCommand, "threads"))
    {
        std::cout << "threads" << std::endl;
        return {};
    }
    else if (!strcasecmp(szCommand, "pause"))
    {
        std::cout << "pause" << std::endl;
        return {};
    }
    else if (!strcasecmp(szCommand, "setExceptionBreakpoints"))
    {
        std::cout << "setExceptionBreakpoints" << std::endl;
        return {};
    }
    else if (!strcasecmp(szCommand, "_clearall"))
    {
        std::cout << "_clearall" << std::endl;
        return {};
    }
    if (!strcasecmp(szCommand, "stackTrace"))
    {
        std::cout << "stackTrace" << std::endl;
        return {};
    }
    else if (!strcasecmp(szCommand, "scopes"))
    {
        std::cout << "scopes" << std::endl;
        return {};
    }
    return true;
}

static bool CheckRemoteCmd()
{
    if (s_remoteConnecter == -1)
        return false;

    bool bContinue = true;
    while (RemoteCmdValid() && bContinue)
    {
        std::lock_guard<std::mutex> lock(s_cmdMutex);
        CDebugCmd* pCmd = s_listDebugCmd.GetFirst();
        pCmd->CDebugNode::Remove();

        if (!pCmd)
            break;
        if (!ProcessCommand(pCmd))
            bContinue = false;
        delete pCmd;
    }
    return bContinue;
}

static bool checkEnterRemoteDebug()
{
    if (s_remoteConnecter == -1)
        return false;
    if (s_bEnterDebug)
        return false;
    CheckRemoteCmd();
    return true;
}

static void CheckDebugCmd()
{
    if (s_listDebugCmd.IsEmpty())
        return;
    checkEnterRemoteDebug();
}

template <typename FunType, typename... Param>
bool RunFunction(nullptr_t, FunType pFun, Param... p)
{
    CheckDebugCmd();
    void* aryParam[sizeof...(p) + 1] = { &p... };
}

int jsTest2(int argc, char* argv[])
{
    // socket监听
    s_remoteListener = (uint32_t)socket(AF_INET, SOCK_STREAM, 0);
    if (s_remoteListener == INVALID_SOCKET)
        std::cout << "socket error" << std::endl;
    int flags = fcntl(s_remoteListener, F_GETFL, 0);
    if (flags == -1)
        std::cout << "fcntl error" << std::endl;
    flags &= ~O_NONBLOCK;
    if (-1 == fcntl(s_remoteListener, F_SETFL, flags))
        std::cout << "fcntl error" << std::endl;
    int32_t nVal = 1;
    if (setsockopt(s_remoteListener, SOL_SOCKET, SO_REUSEADDR, (const char*)(&nVal), sizeof(nVal)) == SOCKET_ERROR)
    {
        close(s_remoteListener);
        s_remoteListener = INVALID_SOCKET;
    }
    sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_addr.s_addr = 0;
    serverAddr.sin_port = htons(5067);
    serverAddr.sin_family = AF_INET;

    if (::bind(s_remoteListener, (sockaddr*)(&serverAddr), sizeof(sockaddr)))
    {
        close(s_remoteListener);
        s_remoteListener = INVALID_SOCKET;
    }

    if (listen(s_remoteListener, INVALID_16BITID))
    {
        close(s_remoteListener);
        s_remoteListener = INVALID_SOCKET;
    }
    s_hThread = std::thread(&socketRun);

//    // 初始化V8
//    v8::V8::InitializeICUDefaultLocation(argv[0]);
//    v8::V8::InitializeExternalStartupData(argv[0]);
//    std::unique_ptr<v8::Platform> platform = v8::platform::NewDefaultPlatform();
//    v8::V8::InitializePlatform(platform.get());
//    v8::V8::Initialize();
//
//    // 创建一个新的Isolate和作用域
//    v8::Isolate::CreateParams create_params;
//    create_params.array_buffer_allocator =
//        v8::ArrayBuffer::Allocator::NewDefaultAllocator();
//    v8::Isolate* isolate = v8::Isolate::New(create_params);
//    {
//        v8::Isolate::Scope isolate_scope(isolate);
//        v8::HandleScope handle_scope(isolate);
//
//        // 创建print函数
//        v8::Local<v8::ObjectTemplate> global = v8::ObjectTemplate::New(isolate);
//        global->Set(v8::String::NewFromUtf8(isolate, "print", v8::NewStringType::kNormal).ToLocalChecked(),
//                    v8::FunctionTemplate::New(isolate, printCallback));
//
//        v8::Local<v8::Context> context = v8::Context::New(isolate, NULL, global);
//        v8::Context::Scope context_scope(context);
//
//        // JavaScript源代码
//        std::string jsStr = getFileContents(getRootDir() + "/test/js/jsScript/test.js");
//        auto source = v8::String::NewFromUtf8(isolate, jsStr.data(), v8::NewStringType::kNormal).ToLocalChecked();
//
//        // 编译源代码
//        v8::Local<v8::Script> script =
//            v8::Script::Compile(context, source).ToLocalChecked();
//
//        // 运行脚本
//        v8::Local<v8::Value> result = script->Run(context).ToLocalChecked();
//    }

//    while (true)
//    {
//        RunFunction(nullptr, "StartApplication", "sampler", 12345);
//        std::this_thread::sleep_for(std::chrono::milliseconds(10));
//    }

    // 清理V8
//    isolate->Dispose();
//    v8::V8::Dispose();
//    v8::V8::ShutdownPlatform();
//    delete create_params.array_buffer_allocator;

    return 0;
}