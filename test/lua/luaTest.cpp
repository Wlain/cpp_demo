//
// Created by william on 2021/9/24.
//
#include "base.h"
extern "C" {
#include "lauxlib.h"
#include "lua.h"
#include "lualib.h"
#include "ldebug.h"
}

class luaTest
{
public:
    void testFunc()
    {
        std::cout << m_number;
        std::cout << "func called" << std::endl;
    }

private:
    int m_number = 0;
};

static int print(lua_State* L)
{
    int n = lua_gettop(L);
    int i;
    for (i = 1; i <= n; i++)
    {
        if (i > 1) printf("\t");
        if (lua_isstring(L, i))
            printf("%s", lua_tostring(L, i));
        else if (lua_isnil(L, i))
            printf("%s", "nil");
        else if (lua_isboolean(L, i))
            printf("%s", lua_toboolean(L, i) ? "true" : "false");
        else
            printf("%s:%p", luaL_typename(L, i), lua_topointer(L, i));
    }
    printf("\n");
    return 0;
}


void printLuaTraceback(lua_State* L)
{
    int top = lua_gettop(L);
    for (int index = 1; index < top; ++index)
    {
        auto type = lua_type(L, index);
        std::cout << "params" << index << ": type is:";
        switch (type)
        {
        case LUA_TNIL:
            std::cout << "nil" << std::endl;
            break;
        case LUA_TBOOLEAN:
            std::cout << "boolean, value is:" << (lua_toboolean(L, index) ? "true" : "false") << std::endl;
            break;
        case LUA_TLIGHTUSERDATA:
        case LUA_TUSERDATA:
            std::cout << "userdata, address is:" << lua_topointer(L, index) << std::endl;
            break;
        case LUA_TSTRING:
            std::cout << "string, value is:" << lua_tostring(L, index) << std::endl;
            break;
        case LUA_TTABLE:
            std::cout << "table, size is:" << lua_objlen(L, index) << ", address is:" << lua_topointer(L, type) << std::endl;
            break;
        case LUA_TNUMBER:
            std::cout << "number, value is:" << lua_tonumber(L, index) << std::endl;
            break;
        default:;
        }
    }
    lua_getglobal(L, "debug");
    lua_getfield(L, -1, "traceback");
    lua_pushstring(L, "\t");
    lua_pushinteger(L, 1);
    const char* m_traceback = "cannot get Lua traceback ";
    if (!lua_pcall(L, 2, 1, 0))
    {
        m_traceback = lua_tostring(L, -1);
        lua_pop(L, 1);
    }
    std::cout << m_traceback << std::endl;
}


void crashTestImp(int param1, double param2, float param3, const char* param4)
{
    luaTest* test = nullptr;
    test->testFunc();
    std::cout << "param1:" << param1 << " param2:" << param2 << " param3:" << param3 << " param4:" << param4 << std::endl;
}

int crashTest(lua_State* L)
{
    std::cout << "crashTest called" << std::endl;
    std::cout << "test API arg count:" << lua_gettop(L) << std::endl;
    lua_getglobal(L, "crashTest");
    auto param1 = (int)lua_tonumber(L, 1);
    auto param2 = (double)lua_tonumber(L, 2);
    auto param3 = (float)lua_tonumber(L, 3);
    auto param4 = (const char*)lua_tostring(L, 4);
    crashTestImp(param1, param2, param3, param4);
    return 0;
}

int callTest(lua_State* L)
{
    std::cout << "printTest called" << std::endl;
    return 0;
}

void luaTest()
{
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    lua_pushcfunction(L, callTest);
    lua_setglobal(L, "callTest");
    lua_pushcfunction(L, crashTest);
    lua_setglobal(L, "crashTest");
    if (luaL_dofile(L, "/Users/william/Developor/cpp_demo/test/lua/test.lua"))
    {
        std::cout << "error:" << lua_tostring(L, -1) << std::endl;
    }
    lua_close(L);
}
