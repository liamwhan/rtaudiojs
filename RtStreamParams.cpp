#include "RtStreamParams.h"
#include <node.h>
#include <node_object_wrap.h>
#include <v8.h>

namespace RtAudioJs
{

bool RtStreamParams::IsUndefinedOrNotNumber(Local<Value> val)
{
    return val->IsNullOrUndefined() || !val->IsNumber() || !val->IsUint32();

} // RtStreamParams::IsUndefinedOrNotNumber

void RtStreamParams::New(const FunctionCallbackInfo<Value> &args)
{
    Isolate *isolate = args.GetIsolate();

    // Throw an exception if used without JS `new` keyword.
    if (!args.IsConstructCall())
    {
        isolate->ThrowException(Exception::SyntaxError(
            String::NewFromUtf8(isolate,
                                "RtStreamParams must be instantiated.\n"
                                "This class can only be used with the `new` keyword e.g.\n"
                                "`let streamParams = new RtAudioJs.RtStreamParams(0, 2, 0, 48000);`")));
        return;
    }

   // Intermediates for convenience
    Local<Number> deviceIdNum;
    Local<Number> nChannelsNum;
    Local<Number> firstChannelNum;
    Local<Number> sampleRateNum;

    // The uint values that we'll store in the end
    uint deviceIdInt;
    uint nChannelsInt;
    uint firstChannelInt;
    uint sampleRateInt;

    // Was an options object passed?
    if (args[0]->IsObject())
    {
        Local<Object> params = args[0]->ToObject();
        Local<String> deviceIdKey = String::NewFromUtf8(isolate, "deviceId");
        Local<String> nChannelsKey = String::NewFromUtf8(isolate, "nChannels");
        Local<String> firstChannelKey = String::NewFromUtf8(isolate, "firstChannel");
        Local<String> sampleRateKey = String::NewFromUtf8(isolate, "sampleRate");

         // Don't default to a deviceId if none was passed, as this could result in difficult to locate bugs for the EU.
        if (IsUndefinedOrNotNumber(params->Get(deviceIdKey)))
        {
            isolate->ThrowException(Exception::TypeError(
                String::NewFromUtf8(isolate, "The \"deviceId\" property is required and must be integer value")));
            return;
        }

        deviceIdNum = Number::New(isolate, params->Get(deviceIdKey)->NumberValue());
        nChannelsNum = (IsUndefinedOrNotNumber(params->Get(nChannelsKey))) ? Number::New(isolate, 2) : Number::New(isolate, params->Get(nChannelsKey)->NumberValue());
        firstChannelNum = (IsUndefinedOrNotNumber(params->Get(firstChannelKey))) ? Number::New(isolate, 0) :  Number::New(isolate, params->Get(firstChannelKey)->NumberValue());
        sampleRateNum = (IsUndefinedOrNotNumber(params->Get(sampleRateKey))) ? Number::New(isolate, 48000) :Number::New(isolate, params->Get(sampleRateKey)->NumberValue());
    }
    else
    {
        // Assume parameters passed separately.
        
        // Don't default to a deviceId if none was passed, as this could result in difficult to locate bugs for the EU.
        if (IsUndefinedOrNotNumber(args[0]))
        {
            isolate->ThrowException(Exception::TypeError(
                String::NewFromUtf8(isolate, "The \"deviceId\" argument is required and must be integer value")));
            return;
        }
        deviceIdNum = Number::New(isolate, args[0]->NumberValue());
        
        //Get values or set Defaults for everything else
        uint argsLength = args.Length();
        nChannelsNum = ((argsLength >= 2) && (!IsUndefinedOrNotNumber(args[1]))) ? Number::New(isolate, args[1]->NumberValue()) : Number::New(isolate, 2);
        sampleRateNum = ((argsLength >= 3) && (!IsUndefinedOrNotNumber(args[2]))) ? Number::New(isolate, args[2]->NumberValue()) : Number::New(isolate, 48000);
        firstChannelNum = ((argsLength > 3) && (!IsUndefinedOrNotNumber(args[3]))) ? Number::New(isolate, args[3]->NumberValue()) : Number::New(isolate, 0);
    }

    // Convert to C++ types
    deviceIdInt = deviceIdNum->Uint32Value();
    nChannelsInt = nChannelsNum->Uint32Value();
    firstChannelInt = firstChannelNum->Uint32Value();
    sampleRateInt = sampleRateNum->Uint32Value();

    // Create Wrapped Object and return
    RtStreamParams *obj = new RtStreamParams(deviceIdInt, nChannelsInt, firstChannelInt, sampleRateInt);
    obj->Wrap(args.This());
    args.GetReturnValue().Set(args.This());

} // RtStreamParams::New


// Get Accessor. Allows for property access via dot syntax in JS e.g. `if(steamParams.deviceId == 0)`
void RtStreamParams::GetProperty(Local<String> property, const PropertyCallbackInfo<Value> &info)
{
    Isolate *isolate = info.GetIsolate();
    // Unwrap (Cast) &info as an RtStreamParams object
    RtStreamParams *obj = ObjectWrap::Unwrap<RtStreamParams>(info.This());

    // Cast the property as a C string
    v8::String::Utf8Value s(property);
    std::string str(*s, s.length());

    //Identify the property and return the correct value;
    if (str == "deviceId")
    {
        info.GetReturnValue().Set(Number::New(isolate, obj->deviceId_));
    }
    else if (str == "nChannels")
    {
        info.GetReturnValue().Set(Number::New(isolate, obj->nChannels_));
    }
    else if (str == "firstChannel")
    {
        info.GetReturnValue().Set(Number::New(isolate, obj->firstChannel_));
    }
    else if (str == "sampleRate")
    {
        info.GetReturnValue().Set(Number::New(isolate, obj->sampleRate_));
    }
} // RtStreamParams::GetProperty

void RtStreamParams::SetProperty(Local<String> property, Local<Value> value, const PropertyCallbackInfo<void> &info)
{

    RtStreamParams *obj = ObjectWrap::Unwrap<RtStreamParams>(info.This());

    v8::String::Utf8Value s(property);
    std::string str(*s, s.length());

    if (str == "deviceId")
    {
        obj->deviceId_ = value->Uint32Value();
    }
    else if (str == "nChannels")
    {
        obj->nChannels_ = value->Uint32Value();
    }
    else if (str == "firstChannel")
    {
        obj->firstChannel_ = value->Uint32Value();
    }
    else if (str == "sampleRate")
    {
        obj->sampleRate_ = value->Uint32Value();
    }

} // RtStreamParams::SetProperty

void RtStreamParams::Init(Local<Object> exports)
{

    Isolate *isolate = exports->GetIsolate();

    //Prepare constructor template
    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
    tpl->SetClassName(String::NewFromUtf8(isolate, "RtStreamParams"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    tpl->InstanceTemplate()->SetAccessor(String::NewFromUtf8(isolate, "deviceId"), GetProperty, SetProperty);
    tpl->InstanceTemplate()->SetAccessor(String::NewFromUtf8(isolate, "nChannels"), GetProperty, SetProperty);
    tpl->InstanceTemplate()->SetAccessor(String::NewFromUtf8(isolate, "firstChannel"), GetProperty, SetProperty);
    tpl->InstanceTemplate()->SetAccessor(String::NewFromUtf8(isolate, "sampleRate"), GetProperty, SetProperty);

    exports->Set(
        String::NewFromUtf8(isolate, "RtStreamParams"),
        tpl->GetFunction());

} // RtStreamParams::Init


} // namespace RtAudioJs
