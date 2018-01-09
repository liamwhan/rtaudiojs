#include <nan.h>
#include <uv.h>

#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <thread>

#include "RtStreamParams.h"
#include "lib/rtaudio/RtAudio.h"

namespace RtAudioJs
{
using namespace Nan;
// using namespace v8;
using namespace std;

RtAudio::StreamParameters streamParamsRt;

struct AsyncWork
{
    uv_async_t async;
    std::vector<string> msgArr;
    std::vector<v8::Persistent<v8::Function, v8::CopyablePersistentTraits<v8::Function>>> handlers;
    uv_rwlock_t lock;
};

// NOTE: Not a NAN_METHOD
// this is a C++ land method that is called by the Worker Thread when
// there is a buffer available for write.
void RequestBufferJS(uv_async_t *async)
{
    v8::Isolate *isolate = v8::Isolate::GetCurrent();
    v8::HandleScope handleScope(isolate);

    // async->data is whatever we define it to be so here we are casting it to an AsyncWork struct *defined above
    AsyncWork *work = static_cast<AsyncWork*>(async->data);

    // Lock the work object for read write
    uv_rwlock_wrlock(&work->lock);

    // Make a copy of the vector (we may not do this)
    std::vector<string> msgArr = work->msgArr;
    work->msgArr.clear();

    // Unlock the mutex. 
    uv_rwlock_wrunlock(&work->lock);

    unsigned int msgCount = msgArr.size();
    for (unsigned int i = 0; i < msgCount; ++i) {
        // Processing
    }


    // Lock the 
}

NAN_METHOD(Probe)
{

    // RtAudio operations:
    // Instantiate the RtAudio Library, allocate a DeviceInfo struct to store the probe results
    // and get the number of available devices on the host
    RtAudio dac;
    RtAudio::DeviceInfo rtDeviceInfo;
    unsigned int devices = dac.getDeviceCount();

    // Throw a detailed excpetion if no audio devices detected.
    if (devices < 1)
    {
        Nan::ThrowError(
            Nan::Error("RtAudio could not detect any audio devices on the host.\n"
                       "This is most commonly caused by:\n"
                       "- Running in a virtualised environment (a VM, or Windows "
                       "Subsystem for Linux) that does not have direct access to hardware.\n"
                       "- Driver issues on the host OS."));
        return;
    }
    // Create the JS Array of DeviceInfo objects we will be returning
    v8::Local<v8::Array> jsDevices = Nan::New<v8::Array>(devices);

    // Probe each available device
    for (unsigned int i = 0; i < devices; i++)
    {
        // Get an assigned DeviceInfo struct for device i
        rtDeviceInfo = dac.getDeviceInfo(i);
        v8::Local<v8::Object> jsInfo = Nan::New<v8::Object>();

        jsInfo->Set(
            Nan::New<v8::String>("probed").ToLocalChecked(),
            Nan::New<v8::Boolean>(rtDeviceInfo.probed));

        if (rtDeviceInfo.probed == true)
        {
            jsInfo->Set(
                Nan::New<v8::String>("id").ToLocalChecked(),
                Nan::New<v8::Number>(i));

            jsInfo->Set(
                Nan::New<v8::String>("name").ToLocalChecked(),
                Nan::New<v8::String>(rtDeviceInfo.name.c_str()).ToLocalChecked());

            jsInfo->Set(
                Nan::New<v8::String>("outputChannels").ToLocalChecked(),
                Nan::New<v8::Number>(rtDeviceInfo.outputChannels));

            jsInfo->Set(
                Nan::New<v8::String>("inputChannels").ToLocalChecked(),
                Nan::New<v8::Number>(rtDeviceInfo.inputChannels));

            jsInfo->Set(
                Nan::New<v8::String>("duplexChannels").ToLocalChecked(),
                Nan::New<v8::Number>(rtDeviceInfo.duplexChannels));

            jsInfo->Set(
                Nan::New<v8::String>("isDefaultOutput").ToLocalChecked(),
                Nan::New<v8::Boolean>(rtDeviceInfo.isDefaultOutput));

            jsInfo->Set(
                Nan::New<v8::String>("isDefaultInput").ToLocalChecked(),
                Nan::New<v8::Boolean>(rtDeviceInfo.isDefaultInput));

            unsigned int nSampleRates = (unsigned int)rtDeviceInfo.sampleRates.size();
            v8::Local<v8::Array> sampleRates = Nan::New<v8::Array>(nSampleRates);
            for (unsigned int j = 0; j < nSampleRates; ++j)
            {
                Nan::Set(sampleRates, j, Nan::New<v8::Number>(rtDeviceInfo.sampleRates[j]));
            }
            jsInfo->Set(
                Nan::New<v8::String>("sampleRates").ToLocalChecked(),
                sampleRates);

            jsInfo->Set(
                Nan::New<v8::String>("preferredSampleRate").ToLocalChecked(),
                Nan::New<v8::Number>(rtDeviceInfo.preferredSampleRate));

            // TODO(liam): RtAudioFormat nativeFormats (its a bitmask and probably contains useful information so we should include it)
        }
        else
        {
            //Probe failed
            jsInfo->Set(
                Nan::New<v8::String>("name").ToLocalChecked(),
                Nan::Null());

            jsInfo->Set(
                Nan::New<v8::String>("outputChannels").ToLocalChecked(),
                Nan::Null());

            jsInfo->Set(
                Nan::New<v8::String>("inputChannels").ToLocalChecked(),
                Nan::Null());

            jsInfo->Set(
                Nan::New<v8::String>("duplexChannels").ToLocalChecked(),
                Nan::Null());

            jsInfo->Set(
                Nan::New<v8::String>("isDefaultOutput").ToLocalChecked(),
                Nan::Null());

            jsInfo->Set(
                Nan::New<v8::String>("isDefaultInput").ToLocalChecked(),
                Nan::Null());

            jsInfo->Set(
                Nan::New<v8::String>("sampleRates").ToLocalChecked(),
                Nan::New<v8::Array>(0));

            jsInfo->Set(
                Nan::New<v8::String>("preferredSampleRate").ToLocalChecked(),
                Nan::Null());
        }

        // Add the DeviceInfo v8::Object to the return v8::Array.
        jsDevices->Set(i, jsInfo);
    }

    info.GetReturnValue().Set(jsDevices);
}

// NOTE(liam): target is defined by the NAN_MODULE_INIT macro
NAN_MODULE_INIT(Init)
{
    Nan::Set(
        target,
        New<v8::String>("deviceProbe").ToLocalChecked(),
        GetFunction(New<v8::FunctionTemplate>(Probe)).ToLocalChecked());

    RtStreamParams::Init(target);
}

// NOTE(liam): Registers the module
NODE_MODULE(NODE_GYP_MODULE_NAME, Init);
}
