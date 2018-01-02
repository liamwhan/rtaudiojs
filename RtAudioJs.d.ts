namespace RtAudioJs {
    export interface IRtStreamParams {
        deviceId:Number;
        nChannels:Number;
        sampleRate:Number;
        firstChannel?:Number;
    }

    export interface IRtSetSearchParams{
        (params:IRtSearchParams)
    }

    export interface IRtDeviceInfo {
        probed: boolean;
        id: Number;
        name: String;
        outputChannels: Number;
        inputChannels: Number;
        isDefaultOutput: boolean;
        isDefaultInput: boolean;
        sampleRates: [Number];
        preferredSampleRate: Number;
    }
    
    declare function setSearchParams(params:IRtSetSearchParams):boolean;
    declare function getStreamParams():IRtStreamParams;
    declare function deviceProbe():IRtDeviceInfo;


    
}