////
//// Created by william on 2023/12/12.
////
//
//#include "debugger/jsDebugger.h"
//
//#include <iostream>
//#include <libplatform/libplatform.h>
//#include <v8.h>
//
//std::string getRootDir()
//{
//    static std::string path = SOURCE_DIR;
//    return path;
//}
//
//static std::string getFileContents(const std::filesystem::path& filename)
//{
//    std::ifstream in{ filename, std::ios::in | std::ios::binary };
//    if (in)
//    {
//        std::string contents;
//        in.seekg(0, std::ios::end);
//        auto size = in.tellg();
//        if (size > 0)
//        {
//            contents.resize((std::string::size_type)size);
//            in.seekg(0, std::ios::beg);
//            in.read(&contents[0], contents.size());
//        }
//        in.close();
//        return contents;
//    }
//    return {};
//}
//
//void printCallback(const v8::FunctionCallbackInfo<v8::Value>& args)
//{
//    if (args.Length() < 1) return;
//    v8::Isolate* isolate = args.GetIsolate();
//    v8::HandleScope scope(isolate);
//    v8::String::Utf8Value value(isolate, args[0]);
//    std::cout << *value << std::endl;
//}
//
//int jsTest2(int argc, char* argv[])
//{
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
//    v8::Isolate::Scope isolate_scope(isolate);
//    JsDebugger::getInstance().getDebugger() = std::make_shared<js::debugger::DebugJS>(isolate, nullptr, 5067);
//    {
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
//        auto path = getRootDir() + "/test/js/jsScript/test.js";
//        std::string jsStr = getFileContents(path);
//        auto source = v8::String::NewFromUtf8(isolate, jsStr.data(), v8::NewStringType::kNormal).ToLocalChecked();
//
//        // 编译源代码
//        v8::Local<v8::Script> script = v8::Script::Compile(context, source).ToLocalChecked();
//        JsDebugger::getInstance().getDebugger()->addScriptInfo(script->GetUnboundScript()->GetId(), path.c_str());
//        // 运行脚本
//        script->Run(context).ToLocalChecked();
//
//        // 获取add函数
//        v8::Local<v8::Value> add_fn_val = context->Global()->Get(context, v8::String::NewFromUtf8(isolate, "add", v8::NewStringType::kNormal).ToLocalChecked()).ToLocalChecked();
//        v8::Local<v8::Function> add_fn = v8::Local<v8::Function>::Cast(add_fn_val);
//
//        // 创建参数
//        const int argc = 2;
//        // 使用for循环调用函数
//        int index = 0;
//        while (true)
//        {
//            auto& debugger = JsDebugger::getInstance().getDebugger();
//            if (debugger && debugger->remoteCmdValid() && !JsDebugger::getInstance().path().empty())
//            {
//                debugger->checkEnterRemoteDebug();
//                std::this_thread::sleep_for(std::chrono::milliseconds(10));
//            }
//            v8::Local<v8::Value> argv[argc] = { v8::Number::New(isolate, ++index), v8::Number::New(isolate, 1) };
//            // 调用函数
//            v8::Local<v8::Value> result = add_fn->Call(context, context->Global(), argc, argv).ToLocalChecked();
//            // 输出结果
//            v8::String::Utf8Value utf8(isolate, result);
//            std::cout << "Result[" << index << "]: " << *utf8 << std::endl;
//        }
//    }
//    // 清理V8
//    isolate->Dispose();
//    v8::V8::Dispose();
//    v8::V8::ShutdownPlatform();
//    delete create_params.array_buffer_allocator;
//
//    return 0;
//}