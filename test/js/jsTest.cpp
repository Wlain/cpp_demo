//
// Created by william on 2023/12/11.
//
#include <libplatform/libplatform.h>
#include <v8-context.h>
#include <v8-initialization.h>
#include <v8-isolate.h>
#include <v8-local-handle.h>
#include <v8-primitive.h>
#include <v8-script.h>

void jsTest(int argc, char* argv[])
{
    // Initialize V8.
    v8::V8::InitializeICUDefaultLocation(argv[0]);
    v8::V8::InitializeExternalStartupData(argv[0]);
    std::unique_ptr<v8::Platform> platform = v8::platform::NewDefaultPlatform();
    v8::V8::InitializePlatform(platform.get());
    v8::V8::Initialize();

    // Create a new Isolate and make it the current one.
    v8::Isolate::CreateParams createParams;
    createParams.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();
    v8::Isolate* isolate = v8::Isolate::New(createParams);
    {
        v8::Isolate::Scope isolateScope(isolate);
        // Create a stack-allocated handle scope.
        v8::HandleScope handleScope(isolate);
        // Create a new context.
        v8::Local<v8::Context> context = v8::Context::New(isolate);
        // Enter the context for compiling and running the hello world script.
        v8::Context::Scope contextScope(context);
        {
            // Create a string containing the JavaScript source code.
            v8::Local<v8::String> source = v8::String::NewFromUtf8Literal(isolate, "'Hello' + ', World!'");
            // Compile the source code.
            v8::Local<v8::Script> script = v8::Script::Compile(context, source).ToLocalChecked();
            // Run the script to get the result.
            v8::Local<v8::Value> result = script->Run(context).ToLocalChecked();
            // Convert the result to an UTF8 string and print it.
            v8::String::Utf8Value utf8(isolate, result);
            printf("%s\n", *utf8);
        }
        {
            constexpr const char jsSource[] = R"javascript(
                let bytes = new Uint8Array([
                  0x00, 0x61, 0x73, 0x6d, 0x01, 0x00, 0x00, 0x00, 0x01, 0x07, 0x01,
                  0x60, 0x02, 0x7f, 0x7f, 0x01, 0x7f, 0x03, 0x02, 0x01, 0x00, 0x07,
                  0x07, 0x01, 0x03, 0x61, 0x64, 0x64, 0x00, 0x00, 0x0a, 0x09, 0x01,
                  0x07, 0x00, 0x20, 0x00, 0x20, 0x01, 0x6a, 0x0b
                ]);
                let module = new WebAssembly.Module(bytes);
                let instance = new WebAssembly.Instance(module);
                instance.exports.add(3, 4);
            )javascript";

            // Create a string containing the JavaScript source code.
            v8::Local<v8::String> source = v8::String::NewFromUtf8Literal(isolate, jsSource);
            // Compile the source code.
            v8::Local<v8::Script> script = v8::Script::Compile(context, source).ToLocalChecked();
            // Run the script to get the result.
            v8::Local<v8::Value> result = script->Run(context).ToLocalChecked();
            // Convert the result to a uint32 and print it.
            uint32_t number = result->Uint32Value(context).ToChecked();
            printf("3 + 4 = %u\n", number);
        }
    }
    // Dispose the isolate and tear down V8.
    isolate->Dispose();
    v8::V8::Dispose();
    v8::V8::DisposePlatform();
    delete createParams.array_buffer_allocator;
}