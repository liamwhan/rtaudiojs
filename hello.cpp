#include <node.h>

namespace demo {

    using v8::FunctionCallbackInfo;
    using v8::Isolate;
    using v8::Local;
    using v8::Object;
    using v8::String;
    using v8::Value;

    // NOTE(liam): this seems to be the handler for a Javascript method, 
    // Looks like its Getting a pointer to the Return Value var, 
    // and setting the return value to "World"
    void Method(const FunctionCallbackInfo<Value>& args)
    {
        Isolate *isolate = args.GetIsolate();
        args.GetReturnValue().Set(String::NewFromUtf8(isolate, "world"));
    }

    // NOTE(liam): Setup the javascript module object and adds the above
    // Method fn to it
    void init(Local<Object> exports)
    {
        NODE_SET_METHOD(exports, "hello", Method);
    }

    // NOTE(liam): Registers the init function with node gyp
    NODE_MODULE(NODE_GYP_MODULE_NAME, init);

}