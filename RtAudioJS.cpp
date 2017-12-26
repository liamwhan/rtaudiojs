#include <node.h>
#include "lib/rtaudio/RtAudio.h"

namespace demo {

    using v8::Exception;
    using v8::FunctionCallbackInfo;
    using v8::Isolate;
    using v8::Local;
    using v8::Number;
    using v8::Object;
    using v8::String;
    using v8::Value;
    using v8::Array;


    // NOTE(liam): This is the implementation of a method
    // that will be exposed on the exported JS obect.
    void Add(const FunctionCallbackInfo<Value>& args)
    {
        Isolate *isolate = args.GetIsolate();
        
        // Check the number of arguments passed
        if(args.Length() < 2)
        {
            isolate->ThrowException(Exception::TypeError(
                String::NewFromUtf8(
                    isolate, 
                    "Incorrect number of arguments. Expected 2, recieved" + args.Length())));
                return;
        }
        
        // Check the argument types
        if (!args[0]->IsNumber() || !args[1]->IsNumber()) {
            isolate->ThrowException(Exception::TypeError(
                String::NewFromUtf8(isolate, "Incorrect argument type, both arguments must be a Number")));
            return;
        }
        
        double value = args[0]->NumberValue() + args[1]->NumberValue();
        Local<Number> num = Number::New(isolate, value);

        args.GetReturnValue().Set(num);
    }

    // NOTE(liam): Setup the javascript module object and adds the above
    // Method fn to it
    void Init(Local<Object> exports)
    {
        NODE_SET_METHOD(exports, "add", Add);
    }

    // NOTE(liam): Registers the init function with node gyp
    NODE_MODULE(NODE_GYP_MODULE_NAME, Init);

}