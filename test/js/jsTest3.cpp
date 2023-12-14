
//
// Created by william on 2023/12/12.
//

#include "debugJS.h"

#include <iostream>
#include <libplatform/libplatform.h>
#include <v8.h>

using namespace XS;

static std::string getFilePath()
{
    static std::string path = std::string(SOURCE_DIR) + "/test/js/jsScript/test3.js";
    return path;
}

extern std::string getFileContents(const std::filesystem::path& filename);

extern void printCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

void jsTest3(int argc, char* argv[])
{
    // 初始化V8
    v8::V8::InitializeICUDefaultLocation(argv[0]);
    v8::V8::InitializeExternalStartupData(argv[0]);
    std::unique_ptr<v8::Platform> platform = v8::platform::NewDefaultPlatform();
    v8::V8::InitializePlatform(platform.get());
    v8::V8::Initialize();

    // 创建一个新的Isolate和作用域
    v8::Isolate::CreateParams create_params;
    create_params.array_buffer_allocator =
        v8::ArrayBuffer::Allocator::NewDefaultAllocator();
    v8::Isolate* isolate = v8::Isolate::New(create_params);
    v8::Isolate::Scope isolateScope(isolate);
    v8::HandleScope handleScope(isolate);
    v8::Local<v8::Context> context = v8::Context::New(isolate);
    v8::Persistent<v8::Context> preContext(isolate, context);
    JSDebugger::getInstance().getDebugger() = std::make_unique<CDebugJS>(preContext, nullptr, 5067);
    v8::Context::Scope contextScope(context);
    v8::Local<v8::Object> globalObj = context->Global();
    v8::Local<v8::External> ScriptContext = v8::External::New(isolate, nullptr);
    auto console = globalObj->Get(context, v8::String::NewFromUtf8(isolate, "console").ToLocalChecked()).ToLocalChecked();
    v8::Local<v8::Object> consoleObj = console->ToObject(isolate->GetCurrentContext()).ToLocalChecked();
    v8::Local<v8::Function> funLog = v8::Function::New(isolate->GetCurrentContext(), printCallback, ScriptContext).ToLocalChecked();
    consoleObj->Set(context, v8::String::NewFromUtf8(isolate, "log").ToLocalChecked(), funLog);
    // JavaScript源代码
    std::string jsStr = getFileContents(getFilePath());
    auto source = v8::String::NewFromUtf8(isolate, jsStr.data(), v8::NewStringType::kNormal).ToLocalChecked();

    // 编译源代码
    v8::Local<v8::Script> script = v8::Script::Compile(context, source).ToLocalChecked();
    auto scriptInfo = script->GetUnboundScript();
    int32 nID = scriptInfo->GetId();
    JSDebugger::getInstance().getDebugger()->AddScriptInfo(nID, getFilePath().c_str());

    // 运行脚本
    script->Run(context).ToLocalChecked();

    // 获取add函数
    v8::Local<v8::Value> add_fn_val = context->Global()->Get(context, v8::String::NewFromUtf8(isolate, "testLog", v8::NewStringType::kNormal).ToLocalChecked()).ToLocalChecked();
    v8::Local<v8::Function> add_fn = v8::Local<v8::Function>::Cast(add_fn_val);

    // 创建参数
    // 使用for循环调用函数
    int index = 0;
    while (true)
    {
        if (JSDebugger::getInstance().getDebugger()->RemoteCmdValid())
            JSDebugger::getInstance().getDebugger()->CheckEnterRemoteDebug();
        // 调用函数
        v8::Local<v8::Value> result = add_fn->Call(context, context->Global(), 0, nullptr).ToLocalChecked();
        // 输出结果
        v8::String::Utf8Value utf8(isolate, result);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    // 清理V8
    isolate->Dispose();
    v8::V8::Dispose();
    v8::V8::ShutdownPlatform();
    delete create_params.array_buffer_allocator;
}