/**@file  		CDebugJS.h
 * @brief		V8 debugger interface
 * @author		Daphnis Kau
 * @date			2019-06-24
 * @version		V1.0
 */

#ifndef __DEBUG_JS_H__
#define __DEBUG_JS_H__

#include "common/tbrTree.h"
#include "core/CDebugBase.h"
#include "v8-inspector.h"
#include "v8-platform.h"
#include "v8.h"

namespace v8 {
    class Isolate;

    class Context;
} // namespace v8

namespace XS {
    typedef v8::Local<v8::Context> CV8Context;
    typedef std::unique_ptr<v8_inspector::V8Inspector> CInspectorPtr;
    typedef std::unique_ptr<v8_inspector::V8InspectorSession> CInsSessionPtr;

    class CDebugJS
            : public CDebugBase, public v8_inspector::V8InspectorClient, public v8_inspector::V8Inspector::Channel {
        enum {
            eScopeID = 1
        };
        struct SObjectInfo;
        typedef tbrTree<SObjectInfo> CObjectMap;

        struct SLocation {
            int32 nScriptId = 0;
            int32 nLineNum = -1;
            int32 nColumnNum = -1;

            void ReadFromJson(CJson *pJson);
        };

        struct SObjectInfo : public CObjectMap::CRBTreeNode {
            std::string strID;
            std::string strType;
            std::string strClassName;
            std::string strDesc;
            std::string strValue;
            std::vector<uint32> vecName;
            std::vector<uint32> vecIndex;
            bool bChildrenFetched = false;

            operator const std::string &() const { return strID; }

            bool operator<(const std::string &strKey) { return strID < strKey; }

            void ReadFromJson(CJson *pJson);
        };

        struct SScopeInfo {
            std::string strType;
            SObjectInfo *ObjectInfo;
            SLocation StartLocation;
            SLocation EndLocation;

            void ReadFromJson(CJson *pJson);
        };

        struct SObjectRefInfo {
            std::string strFieldName;
            SObjectInfo *pObjectInfo;
        };

        struct SFrameInfo {
            uint32 nVariableID = eScopeID;
            uint32 nMaxScopeID = 0;
            std::string strCallFrameID;
            std::string strFunctionName;
            std::string strScriptUrl;
            SLocation FunctionLocation;
            SLocation PauseLocation;
            SObjectInfo *ThisInfo;
            std::vector<SScopeInfo> vecScope;
            std::map<uint32, SObjectRefInfo> mapObjRefs;
        };
        typedef std::vector<SFrameInfo> FrameArray;
        typedef std::map<uint32, std::string> IDStringMap;

        uint16 m_nDebugPort;
        CInspectorPtr m_Inspector;
        CInsSessionPtr m_Session;
        std::string m_strUtf8Buffer;

        uint32 m_nMessageID;
        FrameArray m_aryFrame;
        IDStringMap m_mapBreakPoint;
        IDStringMap m_mapScriptInfo;
        CObjectMap m_mapObjects;
        v8::Isolate *m_isolate{};

        bool ReceiveRemoteData(char (&szBuffer)[2048], int32 nCurSize) override;

        bool CheckRemoteCmd() override;

        void SendWebSocketData(uint8 nId, const char *pData, uint32 nSize);

        uint32 GenBreakPointID(const char *szFileName, int32 nLine) override;

        void ClearVariables();

        void FetchChildren(SObjectInfo &ObjInfo);

        uint32
        AddFrameObject(SFrameInfo &FrameInfo, SObjectInfo &ObjInfo, std::string strField, std::string strParentID = "");

    public:
        CDebugJS(const v8::Persistent<v8::Context> &context, const char *strDebugHost, uint16 nDebugPort);

        ~CDebugJS();

        void AddScriptInfo(int32 nID, const char *szFileName);

        void DelBreakPoint(uint32 nBreakPointID) override;

        uint32 GetFrameCount() override;

        bool GetFrameInfo(int32 nFrame, int32 *nLine, const char **szFunction, const char **szSource) override;

        int32 SwitchFrame(int32 nCurFrame) override;

        uint32 EvaluateExpression(int32 nCurFrame, const char *szExpression) override;

        uint32 GetScopeChainID(int32 nCurFrame) override { return eScopeID; }

        uint32 GetChildrenID(uint32 nParentID, bool bIndex, uint32 nStart, uint32 *aryChild, uint32 nCount) override;

        SValueInfo GetVariable(uint32 nID) override;

        void Stop() override;

        void Continue() override;

        void StepIn() override;

        void StepNext() override;

        void StepOut() override;

        void runMessageLoopOnPause(int contextGroupId) override;

        void quitMessageLoopOnPause() override;

        void runIfWaitingForDebugger(int contextGroupId) override;

        CV8Context ensureDefaultContextInGroup(int context_group_id) override;

        double currentTimeMS() override;

        void sendResponse(int callId, std::unique_ptr<v8_inspector::StringBuffer> message) override;

        void sendNotification(std::unique_ptr<v8_inspector::StringBuffer> message) override;

        void flushProtocolNotifications() override;
    };
} // namespace XS

class JSDebugger final {
public:
    static JSDebugger &getInstance() {
        static std::mutex mutex;
        std::lock_guard<std::mutex> lock(mutex);
        static auto *instance = new JSDebugger();
        return *instance;
    }

    ~JSDebugger();

    JSDebugger(const JSDebugger &) = delete;

    JSDebugger &operator=(const JSDebugger &) = delete;

    std::shared_ptr<XS::CDebugJS> &getDebugger() { return debugger; }

    std::string &path() { return m_path; }

private:
    JSDebugger() {}

public:
    std::shared_ptr<XS::CDebugJS> debugger;
    std::string m_path;
};

#endif
