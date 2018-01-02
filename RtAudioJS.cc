#include <node.h>
#include <node_object_wrap.h>
#include <v8.h>
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

using namespace v8;
using namespace std;


RtAudio::StreamParameters streamParamsRt;
Persistent<Number> sampleRate;

void InitPoly(Local<Object> exports)
{
    RtStreamParams::Init(exports);
}

void Probe(const FunctionCallbackInfo<Value> &args)
{
    Isolate *isolate = args.GetIsolate();

    // RtAudio operations:
    // Instantiate the RtAudio Library, allocate a DeviceInfo struct to store the probe results
    // and get the number of available devices on the host
    RtAudio dac;
    RtAudio::DeviceInfo info;
    unsigned int devices = dac.getDeviceCount();

    // Throw a detailed excpetion if no audio devices detected.
    if (devices < 1)
    {
        isolate->ThrowException(Exception::Error(
            String::NewFromUtf8(isolate,
                                "RtAudio could not detect any audio devices on the host.\n"
                                "This is most commonly caused by:\n"
                                "- Running in a virtualised environment (a VM, or Windows "
                                "Subsystem for Linux) that does not have direct access to hardware.\n"
                                "- Driver issues on the host OS.")));
        return;
    }
    // Create the JS Array of DeviceInfo objects we will be returning
    Local<Array> jsDevices = Array::New(isolate, devices);

    // Probe each available device
    for (unsigned int i = 0; i < devices; i++)
    {
        // Get an assigned DeviceInfo struct for device i
        info = dac.getDeviceInfo(i);
        Local<Object> jsInfo = Object::New(isolate);

        jsInfo->Set(
            String::NewFromUtf8(isolate, "probed"),
            Boolean::New(isolate, info.probed));

        if (info.probed == true)
        {
            jsInfo->Set(
                String::NewFromUtf8(isolate, "id"),
                Number::New(isolate, i));

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

    args.GetReturnValue().Set(jsDevices);
}

struct AsyncWork
{
    uv_work_t request;
    Persistent<Function> callback;

    std::vector<uint> locations;
    std::vector<uint> results;
};

static void AsyncDoWork(uv_work_t *req)
{
    AsyncWork *work = static_cast<AsyncWork *>(req->data);

    uint count = work->locations.size();
    work->results.resize(count);
    for (uint i = 0; i < count; ++i)
    {
        work->results[i] = work->locations[i] * work->locations[i];
    }

    this_thread::sleep_for(chrono::seconds(3));
}

// called by libuv in event loop when async function completes
static void AsyncWorkComplete(uv_work_t *req, int status)
{
    Isolate *isolate = Isolate::GetCurrent();

    // Fix for Node 4.x - thanks to https://github.com/nwjs/blink/commit/ecda32d117aca108c44f38c8eb2cb2d0810dfdeb
    v8::HandleScope handleScope(isolate);

    Local<Array> result_list = Array::New(isolate);
    AsyncWork *work = static_cast<AsyncWork *>(req->data);

    // the work has been done, and now we pack the results
    // vector into a Local array on the event-thread's stack.
    uint count = work->results.size();
    for (unsigned int i = 0; i < count; i++)
    {
        uint result = work->results[i];
        result_list->Set(i, Number::New(isolate, result));
    }

    // set up return arguments
    Handle<Value> argv[] = {Null(isolate), result_list};

    // execute the callback
    // https://stackoverflow.com/questions/13826803/calling-javascript-function-from-a-c-callback-in-v8/28554065#28554065
    Local<Function>::New(isolate, work->callback)->Call(isolate->GetCurrentContext()->Global(), 2, argv);

    // Free up the persistent function callback
    work->callback.Reset();
    delete work;
}

void AsyncTestJS(const FunctionCallbackInfo<Value> &args)
{
    Isolate *isolate = args.GetIsolate();

    // we will start a worker thread to do the actual work
    // and call the callback passed from JS here.
    AsyncWork *work = new AsyncWork();
    work->request.data = work;

    //convert some dummy Js data in to C++ data
    if (!args[0]->IsArray())
    {
        isolate->ThrowException(Exception::TypeError(
            String::NewFromUtf8(isolate, "Must pass an array of ints at 1st argument to act as dummy data")));
        return;
    }

    if (!args[1]->IsFunction())
    {
        isolate->ThrowException(Exception::TypeError(
            String::NewFromUtf8(isolate, "Must pass a callback function as 2nd argument.")));
        return;
    }

    //Cast Js args to their v8 types
    Local<Array> input = Local<Array>::Cast(args[0]);
    Local<Function> callback = Local<Function>::Cast(args[1]);

    // Init the Persistent<>
    work->callback.Reset(isolate, callback);

    //Copy Js Array vals to vector on the heap
    uint count = input->Length();
    for (uint i = 0; i < count; ++i)
    {
        work->locations.push_back(input->Get(i)->Uint32Value());
    }

    uv_queue_work(uv_default_loop(), &work->request, AsyncDoWork, AsyncWorkComplete);

    args.GetReturnValue().Set(Undefined(isolate));
}



// NOTE(liam): Setup the javascript module object and adds the above
// Method fn to it
void Init(Local<Object> exports)
{
    NODE_SET_METHOD(exports, "deviceProbe", Probe);
    NODE_SET_METHOD(exports, "asyncTest", AsyncTestJS);
    RtStreamParams::Init(exports);

}

// NOTE(liam): Registers the init function with node gyp
NODE_MODULE(RtAudioJs, Init);
}