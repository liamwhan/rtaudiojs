#include <vector>
#include <node.h>
#include "lib/rtaudio/RtAudio.h"

namespace demo
{
    
    using namespace v8;  

    RtAudio::StreamParameters streamParamsRt;
    Persistent<Number> sampleRate;

    void SetStreamParams(const FunctionCallbackInfo<Value> &args) {
        Isolate *isolate = args.GetIsolate();

        if(!args[0]->IsObject())
        {
            isolate->ThrowException(Exception::TypeError(
                String::NewFromUtf8(isolate, "An RtStreamParams object is required:\nRtAudio.setStreamParams({deviceId: 0, nChannels: 2, firstChannel: 0, sampleRate: 48000 });")));
            return;
        }

        // TODO(liam): Check all Props,

        Local<Object> _streamParamsJs = Local<Object>::New(isolate, args[0]->ToObject());

        // NOTE(liam): Cleanest way I have found to create object keys to access JS Hashmap 
        Local<String> deviceIdKey       = String::NewFromUtf8(isolate, "deviceId");
        Local<String> nChannelsKey      = String::NewFromUtf8(isolate, "nChannels");
        Local<String> firstChannelKey   = String::NewFromUtf8(isolate, "firstChannel");
        Local<String> sampleRateKey     = String::NewFromUtf8(isolate, "sampleRate");


        if(
            _streamParamsJs->Get(deviceIdKey)->IsNullOrUndefined() || 
            !_streamParamsJs->Get(deviceIdKey)->IsNumber() || 
            !_streamParamsJs->Get(deviceIdKey)->IsUint32()
        ) {
            isolate->ThrowException(Exception::TypeError(
                String::NewFromUtf8(isolate, "The \"deviceId\" property is required and must be integer value")));
            return;
        }

        if(
            _streamParamsJs->Get(nChannelsKey)->IsNullOrUndefined() || 
            !_streamParamsJs->Get(nChannelsKey)->IsNumber() || 
            !_streamParamsJs->Get(nChannelsKey)->IsUint32()
        ) {
            isolate->ThrowException(Exception::TypeError(
                String::NewFromUtf8(isolate, "The \"nChannels\" property is required and must be integer value")));
            return;
        }

        if(
            _streamParamsJs->Get(sampleRateKey)->IsNullOrUndefined() || 
            !_streamParamsJs->Get(sampleRateKey)->IsNumber() || 
            !_streamParamsJs->Get(sampleRateKey)->IsUint32()
        ) {
            isolate->ThrowException(Exception::TypeError(
                String::NewFromUtf8(isolate, "The \"sampleRate\" property is required and must be integer value")));
            return;
        }

        // NOTE(liam): Don't check for undefined for firstChannel as it has a default value
        if( !_streamParamsJs->Get(firstChannelKey)->IsNullOrUndefined() && 
            (!_streamParamsJs->Get(firstChannelKey)->IsNumber() || 
            !_streamParamsJs->Get(firstChannelKey)->IsUint32())
        ) {
            isolate->ThrowException(Exception::TypeError(
                String::NewFromUtf8(isolate, "The \"firstChannel\" property must be an integer")));
            return;
        } else {
            if (!_streamParamsJs->Get(firstChannelKey)->IsNullOrUndefined())
            {
                streamParamsRt.firstChannel = _streamParamsJs->Get(firstChannelKey)->Uint32Value();
            }
        }

        streamParamsRt.deviceId = _streamParamsJs->Get(deviceIdKey)->Uint32Value();
        streamParamsRt.nChannels = _streamParamsJs->Get(nChannelsKey)->Uint32Value();
        sampleRate.Reset(isolate, _streamParamsJs->Get(sampleRateKey)->ToNumber(isolate)); //sampleRate isn't actually a member of RtAudio::StreamParameters we just allow JS users to set it here for convenience

        args.GetReturnValue().Set(Boolean::New(isolate, true));
    }

    
    void GetStreamParams(const FunctionCallbackInfo<Value> &args)
    {
       
       Isolate *isolate = args.GetIsolate();
       Local<Object> _streamParams = Object::New(isolate);

       _streamParams->Set(
           String::NewFromUtf8(isolate, "deviceId"),
           Number::New(isolate, streamParamsRt.deviceId)
       );
       _streamParams->Set(
           String::NewFromUtf8(isolate, "nChannels"),
           Number::New(isolate, streamParamsRt.nChannels)
       );
       _streamParams->Set(
           String::NewFromUtf8(isolate, "firstChannel"),
           Number::New(isolate, streamParamsRt.firstChannel)
       );
       _streamParams->Set(
           String::NewFromUtf8(isolate, "sampleRate"),
           Local<Number>::New(isolate, sampleRate)
       );

       args.GetReturnValue().Set(_streamParams);
    }



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
        NODE_SET_METHOD(exports, "setStreamParams", SetStreamParams);
        NODE_SET_METHOD(exports, "getStreamParams", GetStreamParams);
    }
    
    // NOTE(liam): Registers the init function with node gyp
    NODE_MODULE(NODE_GYP_MODULE_NAME, Init);
}