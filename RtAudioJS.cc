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

void find_primes(int limit, vector<int> &primes)
{
    std::vector<int> is_prime(limit, true);
    for (int n = 2; n < limit; n++)
    {
        if (is_prime[n])
        {
            primes.push_back(n);
        }
        for (int i = n * n; i < limit; i += n)
        {
            is_prime[i] = false;
        }
    }
}

class PrimeWorker : public Nan::AsyncWorker
{

  public:
    PrimeWorker(Callback *callback, int limit) : Nan::AsyncWorker(callback), limit(limit) {}

    //Executes in worker thread
    void Execute()
    {
        find_primes(limit, primes);
    }

    // Executes in event loop
    void HandleOKCallback()
    {
        v8::Local<v8::Array> results = New<v8::Array>(primes.size());
        for (unsigned int i = 0; i < primes.size(); i++)
        {
            Nan::Set(results, i, New<v8::Number>(primes[i]));
        }
        v8::Local<v8::Value> argv[] = {results};
        callback->Call(1, argv);
    }

  private:
    int limit;
    vector<int> primes;
};

class PrimeProgressWorker : public AsyncProgressWorker
{
  public:
    PrimeProgressWorker(Callback *callback, Callback *progress, int limit) : AsyncProgressWorker(callback), progress(progress), limit(limit)
    {
    }

    void Execute(const AsyncBareProgressWorker::ExecutionProgress &progress)
    {
        std::vector<int> is_prime(limit, true);
        for (int n = 2; n < limit; n++)
        {
            double p = (100.0 * n) / limit;
            progress.Send(reinterpret_cast<const char *>(&p),
                          sizeof(double));
            if (is_prime[n])
                primes.push_back(n);
            for (int i = n * n; i < limit; i += n)
            {
                is_prime[i] = false;
            }
            std::this_thread::sleep_for(
                chrono::milliseconds(100));
        }
    }

    void HandleOKCallback()
    {
        v8::Local<v8::Array> results = New<v8::Array>(primes.size());
        for (unsigned int i = 0; i < primes.size(); i++)
        {
            Nan::Set(results, i, New<v8::Number>(primes[i]));
        }
        v8::Local<v8::Value> argv[] = {results};
        callback->Call(1, argv);
    }

    void HandleProgressCallback(const char *data, size_t size)
    {
        // Required, this is not created automatically
        Nan::HandleScope scope;
        v8::Local<v8::Value> argv[] = {
            New<v8::Number>(*reinterpret_cast<double *>(
                const_cast<char *>(data)))};
        progress->Call(1, argv);
    }

  private:
    Callback *progress;
    int limit;
    vector<int> primes;

};


NAN_METHOD(StreamParamsInit)
{

}

NAN_METHOD(Primes)
{
    int limit = To<int>(info[0]).FromJust();
    Callback *callback = new Callback(info[1].As<v8::Function>());
    AsyncQueueWorker(new PrimeWorker(callback, limit));
}

NAN_METHOD(PrimesProgress)
{
    int limit = To<int>(info[0]).FromJust();
    Callback *callback = new Callback(info[1].As<v8::Function>());
    Callback *progress = new Callback(info[2].As<v8::Function>());
    AsyncQueueWorker(new PrimeProgressWorker(callback, progress, limit));
}

// NOTE(liam): target is defined by the NAN_MODULE_INIT macro
NAN_MODULE_INIT(Init)
{
    Nan::Set(
        target,
        New<v8::String>("deviceProbe").ToLocalChecked(),
        GetFunction(New<v8::FunctionTemplate>(Probe)).ToLocalChecked());

    Nan::Set(
        target,
        New<v8::String>("primes").ToLocalChecked(),
        GetFunction(New<v8::FunctionTemplate>(Primes)).ToLocalChecked());

    Nan::Set(
        target,
        New<v8::String>("primes_progress").ToLocalChecked(),
        GetFunction(New<v8::FunctionTemplate>(PrimesProgress)).ToLocalChecked());

    RtStreamParams::Init(target);
}

// NOTE(liam): Registers the module
NODE_MODULE(NODE_GYP_MODULE_NAME, Init);
}
