//
// Created by william on 2023/12/13.
//
#include <iostream>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>

class SocketServer
{
public:
    SocketServer()
    {
        m_remoteListen = socket(AF_INET, SOCK_STREAM, 0);
        if (m_remoteListen < 0)
        {
            std::cerr << "Failed to create socket" << std::endl;
            return;
        }
        sockaddr_in serv_addr;
        memset(&serv_addr, 0, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        serv_addr.sin_port = htons(5067);

        if (bind(m_remoteListen, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
        {
            std::cerr << "Failed to bind socket" << std::endl;
            return;
        }
        listen(m_remoteListen, 5);
    }

    ~SocketServer()
    {
        if (m_remoteListen != -1)
        {
            close(m_remoteListen);
        }
        if (m_remoteConnect != -1)
        {
            close(m_remoteConnect);
        }
    }

    void run()
    {
        m_isConnected = true;
        sockaddr_in cli_addr;
        socklen_t clientLen = sizeof(cli_addr);
        m_remoteConnect = accept(m_remoteListen, (struct sockaddr*)&cli_addr, &clientLen);
        if (m_remoteConnect < 0)
        {
            std::cerr << "Failed to accept connection" << std::endl;
        }
        char buffer[1024];
        memset(buffer, 0, 1024);
        int n = recv(m_remoteConnect, buffer, 1024, 0);
        if (n < 0)
        {
            std::cerr << "Failed to read from socket" << std::endl;
        }
        else
        {
            std::cout << "Received message: " << buffer << std::endl;
        }
        close(m_remoteConnect);
        m_remoteConnect = -1;
    }

    void sendMessage(const std::string& message)
    {
        if (m_remoteConnect != -1)
        {
            send(m_remoteConnect, message.c_str(), message.size(), 0);
        }
    }

    bool isConnected() const
    {
        return m_isConnected;
    }

private:
    intptr_t m_remoteConnect = -1;
    intptr_t m_remoteListen = -1;
    bool m_isConnected = false;
};

int socketTest()
{
    SocketServer server;
    std::thread thread = std::thread([&]() { server.run(); });
    while (true)
    {
        if (server.isConnected())
        {
            server.sendMessage("Content-Length:128\r\n\r\n{\"type\":\"response\",\"request_seq\":1,\"success\":true,\"command\":\"initialize\",\"message\":\"\",\"body\":{\"supportsEvaluateForHovers\":true}}Content-Length:38\r\n\r\n{\"type\":\"event\",\"event\":\"initialized\"}");
            server.sendMessage("Content-Length:82\r\n\r\n{\"type\":\"response\",\"request_seq\":2,\"success\":true,\"command\":\"attach\",\"message\":\"\"}Content-Length:222\r\n\r\n{\"type\":\"event\",\"event\":\"breakpoint\",\"body\":{\"breakpoint\":{\"id\":1,\"verified\":true,\"source\":{\"name\":\"test.lua\",\"path\":\"/Users/william/developer/demo/XScript/samplers/SimpleTest/lua/test.lua\"},\"line\":72},\"reason\":\"changed\"}}Content-Length:222\r\n\r\n{\"type\":\"event\",\"event\":\"breakpoint\",\"body\":{\"breakpoint\":{\"id\":2,\"verified\":true,\"source\":{\"name\":\"test.lua\",\"path\":\"/Users/william/developer/demo/XScript/samplers/SimpleTest/lua/test.lua\"},\"line\":74},\"reason\":\"changed\"}}Content-Length:222\r\n\r\n{\"type\":\"event\",\"event\":\"breakpoint\",\"body\":{\"breakpoint\":{\"id\":3,\"verified\":true,\"source\":{\"name\":\"test.lua\",\"path\":\"/Users/william/developer/demo/XScript/samplers/SimpleTest/lua/test.lua\"},\"line\":75},\"reason\":\"changed\"}}Content-Length:222\r\n\r\n{\"type\":\"event\",\"event\":\"breakpoint\",\"body\":{\"breakpoint\":{\"id\":4,\"verified\":true,\"source\":{\"name\":\"test.lua\",\"path\":\"/Users/william/developer/demo/XScript/samplers/SimpleTest/lua/test.lua\"},\"line\":77},\"reason\":\"changed\"}}Content-Length:222\r\n\r\n{\"type\":\"event\",\"event\":\"breakpoint\",\"body\":{\"breakpoint\":{\"id\":5,\"verified\":true,\"source\":{\"name\":\"test.lua\",\"path\":\"/Users/william/developer/demo/XScript/samplers/SimpleTest/lua/test.lua\"},\"line\":79},\"reason\":\"changed\"}}Content-Length:900\r\n\r\n{\"type\":\"response\",\"request_seq\":3,\"success\":true,\"command\":\"setBreakpoints\",\"message\":\"\",\"body\":{\"breakpoints\":{\"breakpoint\":{\"id\":1,\"verified\":true,\"source\":{\"name\":\"test.lua\",\"path\":\"/Users/william/developer/demo/XScript/samplers/SimpleTest/lua/test.lua\"},\"line\":72},\"breakpoint\":{\"id\":2,\"verified\":true,\"source\":{\"name\":\"test.lua\",\"path\":\"/Users/william/developer/demo/XScript/samplers/SimpleTest/lua/test.lua\"},\"line\":74},\"breakpoint\":{\"id\":3,\"verified\":true,\"source\":{\"name\":\"test.lua\",\"path\":\"/Users/william/developer/demo/XScript/samplers/SimpleTest/lua/test.lua\"},\"line\":75},\"breakpoint\":{\"id\":4,\"verified\":true,\"source\":{\"name\":\"test.lua\",\"path\":\"/Users/william/developer/demo/XScript/samplers/SimpleTest/lua/test.lua\"},\"line\":77},\"breakpoint\":{\"id\":5,\"verified\":true,\"source\":{\"name\":\"test.lua\",\"path\":\"/Users/william/developer/demo/XScript/samplers/SimpleTest/lua/test.lua\"},\"line\":79}}}}");
            server.sendMessage("Content-Length:78\r\n\r\n{\"type\":\"event\",\"event\":\"stopped\",\"body\":{\"threadId\":1,\"reason\":\"breakpoint\"}}");
            server.sendMessage("Content-Length:284\r\n\r\n{\"type\":\"response\",\"request_seq\":5,\"success\":true,\"command\":\"stackTrace\",\"message\":\"\",\"body\":{\"totalFrames\":1,\"stackFrames\":[{\"id\":0,\"name\":\"<unknow>\",\"line\":72,\"column\":0,\"source\":{\"path\":\"/Users/william/developer/demo/XScript/samplers/SimpleTest/lua/test.lua\",\"name\":\"test.lua\"}}]}}");
            server.sendMessage("Content-Length:284\r\n\r\n{\"type\":\"response\",\"request_seq\":6,\"success\":true,\"command\":\"stackTrace\",\"message\":\"\",\"body\":{\"totalFrames\":1,\"stackFrames\":[{\"id\":0,\"name\":\"<unknow>\",\"line\":72,\"column\":0,\"source\":{\"path\":\"/Users/william/developer/demo/XScript/samplers/SimpleTest/lua/test.lua\",\"name\":\"test.lua\"}}]}}");
            server.sendMessage("Content-Length:400\r\n\r\n{\"type\":\"response\",\"request_seq\":7,\"success\":true,\"command\":\"scopes\",\"message\":\"\",\"body\":{\"scopes\":[{\"name\":\"Global\",\"variablesReference\":2,\"namedVariables\":56,\"indexedVariables\":0,\"expensive\":false},{\"name\":\"UpValue\",\"variablesReference\":4,\"namedVariables\":3,\"indexedVariables\":0,\"expensive\":false},{\"name\":\"Local\",\"variablesReference\":3,\"namedVariables\":2,\"indexedVariables\":0,\"expensive\":false}]}}");
            server.sendMessage("Content-Length:6664\r\n\r\n{\"type\":\"response\",\"request_seq\":8,\"success\":true,\"command\":\"variables\",\"message\":\"\",\"body\":{\"variables\":[{\"variablesReference\":0,\"name\":\"\",\"value\":\"table: 0x600001825580\",\"namedVariables\":0,\"indexedVariables\":0},{\"variablesReference\":36,\"name\":\"AligenUp\",\"value\":\"function: 0x600002924180\",\"namedVariables\":3,\"indexedVariables\":0},{\"variablesReference\":0,\"name\":\"BTrace\",\"value\":\"function: 0x600000326370\",\"namedVariables\":0,\"indexedVariables\":0},{\"variablesReference\":34,\"name\":\"CApplication\",\"value\":\"table: 0x600001824b80\",\"namedVariables\":10,\"indexedVariables\":0},{\"variablesReference\":59,\"name\":\"CApplicationHandler\",\"value\":\"table: 0x600001825f00\",\"namedVariables\":8,\"indexedVariables\":0},{\"variablesReference\":37,\"name\":\"CBufferStream\",\"value\":\"table: 0x600001824800\",\"namedVariables\":8,\"indexedVariables\":0},{\"variablesReference\":38,\"name\":\"ClassCast\",\"value\":\"function: 0x6000018252c0\",\"namedVariables\":3,\"indexedVariables\":0},{\"variablesReference\":0,\"name\":\"DebugPrint\",\"value\":\"function: 0x6000003272d0\",\"namedVariables\":0,\"indexedVariables\":0},{\"variablesReference\":49,\"name\":\"IApplicationHandler\",\"value\":\"table: 0x6000018255c0\",\"namedVariables\":10,\"indexedVariables\":0},{\"variablesReference\":20,\"name\":\"SAddress\",\"value\":\"table: 0x600001825780\",\"namedVariables\":10,\"indexedVariables\":0},{\"variablesReference\":31,\"name\":\"SApplicationConfig\",\"value\":\"table: 0x600001825940\",\"namedVariables\":10,\"indexedVariables\":0},{\"variablesReference\":32,\"name\":\"StartApplication\",\"value\":\"function: 0x600001826c80\",\"namedVariables\":3,\"indexedVariables\":0},{\"variablesReference\":0,\"name\":\"Test\",\"value\":\"function: 0x600000338cf0\",\"namedVariables\":0,\"indexedVariables\":0},{\"variablesReference\":33,\"name\":\"_G\",\"value\":\"table: 0x6000018240c0\",\"namedVariables\":56,\"indexedVariables\":0},{\"variablesReference\":0,\"name\":\"_VERSION\",\"value\":\"Lua 5.1\",\"namedVariables\":0,\"indexedVariables\":0},{\"variablesReference\":0,\"name\":\"assert\",\"value\":\"function: 0x6000003241b0\",\"namedVariables\":0,\"indexedVariables\":0},{\"variablesReference\":40,\"name\":\"class\",\"value\":\"function: 0x600002e2c190\",\"namedVariables\":5,\"indexedVariables\":0},{\"variablesReference\":0,\"name\":\"collectgarbage\",\"value\":\"function: 0x6000003241e0\",\"namedVariables\":0,\"indexedVariables\":0},{\"variablesReference\":48,\"name\":\"coroutine\",\"value\":\"table: 0x600001824280\",\"namedVariables\":6,\"indexedVariables\":0},{\"variablesReference\":43,\"name\":\"debug\",\"value\":\"table: 0x600001824680\",\"namedVariables\":14,\"indexedVariables\":0},{\"variablesReference\":0,\"name\":\"dofile\",\"value\":\"function: 0x600000327360\",\"namedVariables\":0,\"indexedVariables\":0},{\"variablesReference\":0,\"name\":\"error\",\"value\":\"function: 0x600000324270\",\"namedVariables\":0,\"indexedVariables\":0},{\"variablesReference\":22,\"name\":\"g_App\",\"value\":\"CApplication: 0x600001826940->0x108019a30\",\"namedVariables\":5,\"indexedVariables\":0},{\"variablesReference\":25,\"name\":\"g_handler\",\"value\":\"table: 0x600001826800\",\"namedVariables\":2,\"indexedVariables\":0},{\"variablesReference\":0,\"name\":\"gcinfo\",\"value\":\"function: 0x6000003242a0\",\"namedVariables\":0,\"indexedVariables\":0},{\"variablesReference\":0,\"name\":\"gdb\",\"value\":\"function: 0x600000326340\",\"namedVariables\":0,\"indexedVariables\":0},{\"variablesReference\":0,\"name\":\"getfenv\",\"value\":\"function: 0x6000003242d0\",\"namedVariables\":0,\"indexedVariables\":0},{\"variablesReference\":0,\"name\":\"getmetatable\",\"value\":\"function: 0x600000324300\",\"namedVariables\":0,\"indexedVariables\":0},{\"variablesReference\":23,\"name\":\"io\",\"value\":\"table: 0x6000018244c0\",\"namedVariables\":14,\"indexedVariables\":0},{\"variablesReference\":55,\"name\":\"ipairs\",\"value\":\"function: 0x600001824180\",\"namedVariables\":1,\"indexedVariables\":0},{\"variablesReference\":0,\"name\":\"load\",\"value\":\"function: 0x6000003243c0\",\"namedVariables\":0,\"indexedVariables\":0},{\"variablesReference\":0,\"name\":\"loadfile\",\"value\":\"function: 0x600000327390\",\"namedVariables\":0,\"indexedVariables\":0},{\"variablesReference\":0,\"name\":\"loadstring\",\"value\":\"function: 0x6000003243f0\",\"namedVariables\":0,\"indexedVariables\":0},{\"variablesReference\":42,\"name\":\"math\",\"value\":\"table: 0x600001824640\",\"namedVariables\":31,\"indexedVariables\":0},{\"variablesReference\":0,\"name\":\"module\",\"value\":\"function: 0x600000324bd0\",\"namedVariables\":0,\"indexedVariables\":0},{\"variablesReference\":46,\"name\":\"newproxy\",\"value\":\"function: 0x600001824240\",\"namedVariables\":1,\"indexedVariables\":0},{\"variablesReference\":0,\"name\":\"next\",\"value\":\"function: 0x600000324450\",\"namedVariables\":0,\"indexedVariables\":0},{\"variablesReference\":12,\"name\":\"os\",\"value\":\"table: 0x600001824580\",\"namedVariables\":11,\"indexedVariables\":0},{\"variablesReference\":9,\"name\":\"package\",\"value\":\"table: 0x600001824300\",\"namedVariables\":8,\"indexedVariables\":0},{\"variablesReference\":52,\"name\":\"pairs\",\"value\":\"function: 0x6000018241c0\",\"namedVariables\":1,\"indexedVariables\":0},{\"variablesReference\":0,\"name\":\"pcall\",\"value\":\"function: 0x600000324480\",\"namedVariables\":0,\"indexedVariables\":0},{\"variablesReference\":0,\"name\":\"print\",\"value\":\"function: 0x600000338270\",\"namedVariables\":0,\"indexedVariables\":0},{\"variablesReference\":0,\"name\":\"rawequal\",\"value\":\"function: 0x6000003244e0\",\"namedVariables\":0,\"indexedVariables\":0},{\"variablesReference\":0,\"name\":\"rawget\",\"value\":\"function: 0x600000324540\",\"namedVariables\":0,\"indexedVariables\":0},{\"variablesReference\":0,\"name\":\"rawset\",\"value\":\"function: 0x600000324570\",\"namedVariables\":0,\"indexedVariables\":0},{\"variablesReference\":0,\"name\":\"require\",\"value\":\"function: 0x600000324c00\",\"namedVariables\":0,\"indexedVariables\":0},{\"variablesReference\":0,\"name\":\"select\",\"value\":\"function: 0x6000003245a0\",\"namedVariables\":0,\"indexedVariables\":0},{\"variablesReference\":0,\"name\":\"setfenv\",\"value\":\"function: 0x6000003245d0\",\"namedVariables\":0,\"indexedVariables\":0},{\"variablesReference\":0,\"name\":\"setmetatable\",\"value\":\"function: 0x600000324600\",\"namedVariables\":0,\"indexedVariables\":0},{\"variablesReference\":7,\"name\":\"string\",\"value\":\"table: 0x6000018245c0\",\"namedVariables\":15,\"indexedVariables\":0},{\"variablesReference\":45,\"name\":\"table\",\"value\":\"table: 0x600001824400\",\"namedVariables\":9,\"indexedVariables\":0},{\"variablesReference\":0,\"name\":\"tonumber\",\"value\":\"function: 0x600000324660\",\"namedVariables\":0,\"indexedVariables\":0},{\"variablesReference\":0,\"name\":\"tostring\",\"value\":\"function: 0x6000003382a0\",\"namedVariables\":0,\"indexedVariables\":0},{\"variablesReference\":0,\"name\":\"type\",\"value\":\"function: 0x600000324720\",\"namedVariables\":0,\"indexedVariables\":0},{\"variablesReference\":0,\"name\":\"unpack\",\"value\":\"function: 0x600000324750\",\"namedVariables\":0,\"indexedVariables\":0},{\"variablesReference\":0,\"name\":\"xpcall\",\"value\":\"function: 0x600000324780\",\"namedVariables\":0,\"indexedVariables\":0}]}}");
            server.sendMessage("Content-Length:499\r\n\r\n{\"type\":\"response\",\"request_seq\":9,\"success\":true,\"command\":\"variables\",\"message\":\"\",\"body\":{\"variables\":[{\"variablesReference\":64,\"name\":\"address\",\"value\":\"SAddress: 0x6000018269c0->0x600001826a68\",\"namedVariables\":2,\"indexedVariables\":0},{\"variablesReference\":63,\"name\":\"buffer\",\"value\":\"table: 0x600001826c40\",\"namedVariables\":1,\"indexedVariables\":0},{\"variablesReference\":62,\"name\":\"config\",\"value\":\"SApplicationConfig: 0x600001826b40->0x600002321528\",\"namedVariables\":2,\"indexedVariables\":0}]}}");
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    thread.join();
}