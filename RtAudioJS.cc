#include <vector>
#include <node.h>
#include "lib/rtaudio/RtAudio.h"

namespace demo
{

using namespace v8;

/**
 * Persistent experiment
 * 1. create a Persistent<Object> streamParams
 * 2. create and expose a method setStreamParams() to transfer JS object 
 * to the persisten streamParams v8 Object
 * 3. Create and Expose a getStreamParams object to return the persisted values
 * 4. The value should return if it has not been disposed in the GC.
 * 
 **/
void Probe(const FunctionCallbackInfo<Value> &args)
{
    Isolate *isolate = args.GetIsolate();

    // RtAudio operations:
    // Instantiate the RtAudio Library, allocate a DeviceInfo struct to store the probe results
    // and get the number of available devices on the
    RtAudio dac;
    RtAudio::DeviceInfo info;
    unsigned int devices = dac.getDeviceCount();
    if (devices < 1) {
        isolate->ThrowException(Exception::Error(
            String::NewFromUtf8(isolate, 
            "RtAudio could not detect any audio devices on the host.\n"
            "This is most commonly caused by:\n"
            "- Running in a virtualised environment (a VM, or Windows "
            "Subsystem for Linux) that does not have direct access to hardware.\n"
            "- Driver issues on the host OS."
            )));
            return;
    }
    // The JS Array of DeviceInfo objects we will be returning on completion
    Local<Array> jsDevices = Array::New(isolate, devices);
    // Probe each available device
    for (unsigned int i = 0; i < devices; i++)
    {
        // Get the DeviceInfo for device i
        info = dac.getDeviceInfo(i);
        Local<Object> jsInfo = Object::New(isolate);

        jsInfo->Set(
            String::NewFromUtf8(isolate, "probed"),
            Boolean::New(isolate, info.probed));

        if (info.probed == true)
        {
            jsInfo->Set(
                String::NewFromUtf8(isolate, "id"),
                Number::New(isolate, i)
            );

            jsInfo->Set(
                String::NewFromUtf8(isolate, "name"),
                String::NewFromUtf8(isolate, info.name.c_str()));

            jsInfo->Set(
                String::NewFromUtf8(isolate, "outputChannels"),
                Number::New(isolate, info.outputChannels));

            jsInfo->Set(
                String::NewFromUtf8(isolate, "inputChannels"),
                Number::New(isolate, info.inputChannels));

            jsInfo->Set(
                String::NewFromUtf8(isolate, "duplexChannels"),
                Number::New(isolate, info.duplexChannels));

            jsInfo->Set(
                String::NewFromUtf8(isolate, "isDefaultOutput"),
                Boolean::New(isolate, info.isDefaultOutput));

            jsInfo->Set(
                String::NewFromUtf8(isolate, "isDefaultInput"),
                Boolean::New(isolate, info.isDefaultInput));

            unsigned int nSampleRates = (unsigned int)info.sampleRates.size();
            Local<Array> sampleRates = Array::New(isolate, nSampleRates);
            for (unsigned int j = 0; j < nSampleRates; ++j)
            {
                sampleRates->Set(Number::New(isolate, j), Number::New(isolate, info.sampleRates[j]));
            }
            jsInfo->Set(
                String::NewFromUtf8(isolate, "sampleRates"),
                sampleRates);

            jsInfo->Set(
                String::NewFromUtf8(isolate, "preferredSampleRate"),
                Number::New(isolate, info.preferredSampleRate));

            // TODO(liam): RtAudioFormat nativeFormats (its a bitmask and probably contains useful information so we should include it)
        }
        else
        {
            //Probe failed
            jsInfo->Set(
                String::NewFromUtf8(isolate, "name"),
                v8::Null(isolate));

            jsInfo->Set(
                String::NewFromUtf8(isolate, "outputChannels"),
                v8::Null(isolate));

            jsInfo->Set(
                String::NewFromUtf8(isolate, "inputChannels"),
                v8::Null(isolate));

            jsInfo->Set(
                String::NewFromUtf8(isolate, "duplexChannels"),
                v8::Null(isolate));

            jsInfo->Set(
                String::NewFromUtf8(isolate, "isDefaultOutput"),
                v8::Null(isolate));

            jsInfo->Set(
                String::NewFromUtf8(isolate, "isDefaultInput"),
                v8::Null(isolate));

            jsInfo->Set(
                String::NewFromUtf8(isolate, "sampleRates"),
                Array::New(isolate));

            jsInfo->Set(
                String::NewFromUtf8(isolate, "preferredSampleRate"),
                v8::Null(isolate));
        }

        // Add the DeviceInfo object to the return array.
        jsDevices->Set(i, jsInfo);

    }

    // 
    args.GetReturnValue().Set(jsDevices);
}



// NOTE(liam): Setup the javascript module object and adds the above
// Method fn to it
void Init(Local<Object> exports)
{
    NODE_SET_METHOD(exports, "deviceProbe", Probe);
}

// NOTE(liam): Registers the init function with node gyp
NODE_MODULE(NODE_GYP_MODULE_NAME, Init);
}