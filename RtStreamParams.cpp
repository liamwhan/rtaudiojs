#include "RtStreamParams.h"

namespace RtAudioJs
{
extern bool IsUndefinedOrNotNumber(v8::Local<v8::Value> val)
{
    return val->IsNullOrUndefined() || !val->IsNumber() || !val->IsUint32();
} // IsUndefinedOrNotNumber

NAN_MODULE_INIT(RtStreamParams::Init)
{
    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(RtStreamParams::New);

    //Prepare constructor template
    tpl->SetClassName(Nan::New<v8::String>("RtStreamParams").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    v8::Local<v8::ObjectTemplate> instanceTmpl = tpl->InstanceTemplate();

    SetAccessor(instanceTmpl, Nan::New<v8::String>("deviceId").ToLocalChecked(), RtStreamParams::GetProperty, RtStreamParams::SetProperty);
    SetAccessor(instanceTmpl, Nan::New<v8::String>("nChannels").ToLocalChecked(), RtStreamParams::GetProperty, RtStreamParams::SetProperty);
    SetAccessor(instanceTmpl, Nan::New<v8::String>("firstChannel").ToLocalChecked(), RtStreamParams::GetProperty, RtStreamParams::SetProperty);
    SetAccessor(instanceTmpl, Nan::New<v8::String>("sampleRate").ToLocalChecked(), RtStreamParams::GetProperty, RtStreamParams::SetProperty);

    Nan::Set(target,
             Nan::New<v8::String>("RtStreamParams").ToLocalChecked(),
             tpl->GetFunction());
}

NAN_METHOD(RtStreamParams::New)
{
    // Throw an exception if used without JS `new` keyword.
    if (!info.IsConstructCall())
    {
        Nan::ThrowSyntaxError(
            Nan::New<v8::String>(
                "RtStreamParams must be instantiated.\n"
                "This class can only be used with the `new` keyword e.g.\n"
                "`let streamParams = new RtAudioJs.RtStreamParams(0, 2, 0, 48000);`")
                .ToLocalChecked());
        return;
    }

    // Intermediates for convenience
    v8::Local<v8::Number> deviceIdNum;
    v8::Local<v8::Number> nChannelsNum;
    v8::Local<v8::Number> firstChannelNum;
    v8::Local<v8::Number> sampleRateNum;

    // The uint values that we'll store in the end
    uint deviceIdInt;
    uint nChannelsInt;
    uint firstChannelInt;
    uint sampleRateInt;

    // Was an options object passed?
    if (info[0]->IsObject())
    {
        v8::Local<v8::Object> params = info[0]->ToObject();
        v8::Local<v8::String> deviceIdKey = Nan::New<v8::String>("deviceId").ToLocalChecked();
        v8::Local<v8::String> nChannelsKey = Nan::New<v8::String>("nChannels").ToLocalChecked();
        v8::Local<v8::String> firstChannelKey = Nan::New<v8::String>("firstChannel").ToLocalChecked();
        v8::Local<v8::String> sampleRateKey = Nan::New<v8::String>("sampleRate").ToLocalChecked();

        // Don't default to a deviceId if none was passed, as this could result in difficult to locate bugs for the EU.
        if (IsUndefinedOrNotNumber(params->Get(deviceIdKey)))
        {
            Nan::ThrowTypeError(
                Nan::New<v8::String>("The \"deviceId\" property is required and must be integer value").ToLocalChecked());
            return;
        }

        deviceIdNum = Nan::New<v8::Number>(params->Get(deviceIdKey)->NumberValue());
        nChannelsNum = (IsUndefinedOrNotNumber(params->Get(nChannelsKey))) ? Nan::New<v8::Number>(2) : Nan::New<v8::Number>(params->Get(nChannelsKey)->NumberValue());
        firstChannelNum = (IsUndefinedOrNotNumber(params->Get(firstChannelKey))) ? Nan::New<v8::Number>(0) : Nan::New<v8::Number>(params->Get(firstChannelKey)->NumberValue());
        sampleRateNum = (IsUndefinedOrNotNumber(params->Get(sampleRateKey))) ? Nan::New<v8::Number>(48000) : Nan::New<v8::Number>(params->Get(sampleRateKey)->NumberValue());
    }
    else
    {
        // Assume parameters passed separately.

        // Don't default to a deviceId if none was passed, as this could result in difficult to locate bugs for the EU.
        if (IsUndefinedOrNotNumber(info[0]))
        {
            Nan::ThrowTypeError(
                Nan::New<v8::String>("The \"deviceId\" argument is required and must be integer value").ToLocalChecked());
            return;
        }
        deviceIdNum = Nan::New<v8::Number>(info[0]->NumberValue());

        //Get values or set Defaults for everything else
        uint argsLength = info.Length();
        nChannelsNum = ((argsLength >= 2) && (!IsUndefinedOrNotNumber(info[1]))) ? Nan::New<v8::Number>(info[1]->NumberValue()) : Nan::New<v8::Number>(2);
        sampleRateNum = ((argsLength >= 3) && (!IsUndefinedOrNotNumber(info[2]))) ? Nan::New<v8::Number>(info[2]->NumberValue()) : Nan::New<v8::Number>(48000);
        firstChannelNum = ((argsLength > 3) && (!IsUndefinedOrNotNumber(info[3]))) ? Nan::New<v8::Number>(info[3]->NumberValue()) : Nan::New<v8::Number>(0);
    }

    // Convert to C++ types
    deviceIdInt = deviceIdNum->Uint32Value();
    nChannelsInt = nChannelsNum->Uint32Value();
    firstChannelInt = firstChannelNum->Uint32Value();
    sampleRateInt = sampleRateNum->Uint32Value();

    // Create Wrapped Object and return
    RtStreamParams *obj = new RtStreamParams(deviceIdInt, nChannelsInt, firstChannelInt, sampleRateInt);
    obj->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
}

// Get Accessor. Allows for property access via dot syntax in JS e.g. `if(steamParams.deviceId == 0)`
NAN_GETTER(RtStreamParams::GetProperty)
{
    v8::Isolate *isolate = info.GetIsolate();
    // Unwrap (Cast) &info as an RtStreamParams object
    RtStreamParams *obj = ObjectWrap::Unwrap<RtStreamParams>(info.This());

    // Cast the property as a C string
    v8::String::Utf8Value s(property);
    std::string str(*s);

    //Identify the property and return the correct value;
    if (str == "deviceId")
    {
        info.GetReturnValue().Set(Nan::New<v8::Number>(obj->deviceId_));
    }
    else if (str == "nChannels")
    {
        info.GetReturnValue().Set(Nan::New<v8::Number>(obj->nChannels_));
    }
    else if (str == "firstChannel")
    {
        info.GetReturnValue().Set(Nan::New<v8::Number>(obj->firstChannel_));
    }
    else if (str == "sampleRate")
    {
        info.GetReturnValue().Set(Nan::New<v8::Number>(obj->sampleRate_));
    }
} // RtStreamParams::GetProperty

NAN_SETTER(RtStreamParams::SetProperty)
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

} // namespace RtAudioJs
