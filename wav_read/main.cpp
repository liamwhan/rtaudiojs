#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include <cstdlib>
#include "../lib/rtaudio/RtAudio.h"

using std::cin;
using std::cout;
using std::endl;
using std::fstream;
using std::string;

#define global_variable static;

typedef struct WAV_HEADER
{
    /* RIFF Chunk Descriptor */
    uint8_t RIFF[4];
    uint32_t ChunkSize; // RIFF Chunk Size
    uint8_t WAVE[4];    // WAVE Header
    /* "fmt" sub-chunk */
    uint8_t fmt[4];         // FMT header
    uint32_t Subchunk1Size; // Size of the fmt chunk
    uint16_t AudioFormat;   // Audio format 1=PCM,6=mulaw,7=alaw,     257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM
    uint16_t nChannels;     // Number of channels 1=Mono 2=Sterio
    uint32_t sampleRate;    // Sampling Frequency in Hz
    uint32_t bytesPerSec;   // bytes per second
    uint16_t blockAlign;    // 2=16-bit mono, 4=16-bit stereo
    uint16_t bitsPerSample; // Number of bits per sample
    /* "data" sub-chunk */
    uint8_t Subchunk2ID[4]; // "data"  string
    uint32_t Subchunk2Size; // Sampled data length

} WavHeader;

typedef struct OUTPUT_DATA
{
    FILE *File;
    unsigned int NChannels;
    unsigned short *rawData;
    long FileSize;
    int numSamples;
    int lastSample;
} OutputData;

global_variable WavHeader wavHeader;
global_variable long long currentSample = 0;

static int getFileSize(FILE *inFile);
static void printHeaderInfo();

int playback16bit(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
             double streamTime, RtAudioStreamStatus status, void *userData)
{

    if(status) {
        cout << "Stream underflow detected!" << endl;
    }

    OutputData *odata = (OutputData *)userData;
    unsigned short *buffer = (unsigned short *)outputBuffer;

    unsigned int count;
    if ((currentSample + nBufferFrames) > odata->numSamples)
    {
        count = odata->numSamples;
    } else {
        count = currentSample + nBufferFrames;
    }

    for(uint32_t i=currentSample; i<count;++i)
    {
        *buffer++ = odata->rawData[i];
        ++currentSample;
    }

    if(currentSample >= odata->numSamples)
    {
        return 1;
    } else {

        return 0;
    }

    

}

int main(int argc, char *argv[])
{

    // fprintf(stdout, "sizeof long : %zi\nsizeof wavHeader: %zi\nsizeof uint8_t: %zi\n", sizeof(long), sizeof(wavHeader), sizeof(uint8_t));

    int fileLength = 0;

    const char *filePath;
    OutputData outputData;

    filePath = argv[1];
    cout << "Reading WAV file at " << filePath << endl;

    outputData.File = fopen(filePath, "rb");
    if (outputData.File == nullptr)
    {
        fprintf(stderr, "Unable to open wave file: %s\n", filePath);
        return 1;
    }

    // Read the header into the wavHeader struct
    // reads size*count bytes so in this case 1*headerSize
    // wavFile is already a pointer so we pass it in as is.
    // fread(void * ptr, size_t size, size_t count, FILE * stream);
    size_t bytesRead = fread(&wavHeader, 1, sizeof(wavHeader), outputData.File); // Reads 44bytes into wavHeader and returns 44 to bytesRead
    printHeaderInfo();
    cout << "Header Size: " << bytesRead << " bytes." << endl;


    uint16_t bytesPerSample = wavHeader.bitsPerSample / 8;      //Number of bytes per sample
    static const uint64_t numSamples = wavHeader.Subchunk2Size / bytesPerSample; //How many samples are in the wav file?

    outputData.rawData = new unsigned short[numSamples];
    outputData.numSamples = numSamples;
    outputData.NChannels = wavHeader.nChannels;

    cout << "Size of short: " << sizeof(short) << " bytes." << endl;
    cout << "Size of short*nChannels: " << (sizeof(short) * wavHeader.nChannels) << " bytes." << endl;
    cout << "Number of Samples: " << numSamples << "." << endl;
    cout << "Number of Samples * element size: " << (numSamples * 2) << " bytes." << endl;
    // We seek in bytes so find out how many bytes the stride needs to be
    while((bytesRead = fread(outputData.rawData, bytesPerSample*wavHeader.nChannels, numSamples, outputData.File)) > 0)
    {
        cout << "WAV Data Size: " << bytesRead << " bytes." << endl;
    }


    RtAudio dac;
    RtAudio::StreamParameters parameters;
    parameters.deviceId = dac.getDefaultOutputDevice();
    parameters.nChannels = wavHeader.nChannels;
    uint32_t sampleRate = wavHeader.sampleRate;
    uint32_t bufferFrames = 512;



    try {
        dac.openStream( &parameters, NULL, RTAUDIO_SINT16, sampleRate
        , &bufferFrames, &playback16bit, (void *)&outputData);
        dac.startStream();
    } catch (RtAudioError &e) {
        cout << '\n' << e.getMessage() << '\n' << endl;
        exit(0);
    }

    char keyInput;
    cout << "Playing " << filePath << " press <enter> to quit." << endl;
    cin.get( keyInput );

    try {
        dac.stopStream();
        fclose(outputData.File);
    } catch (RtAudioError &e)
    {
        e.printMessage();
    }
    
   
    return 0;
}

int getFileSize(FILE *inFile)
{
    int fileSize = 0;
    fseek(inFile, 0, SEEK_END);

    fileSize = ftell(inFile);

    fseek(inFile, 0, SEEK_SET);
    return fileSize;
}

void printHeaderInfo()
{
    cout << "RIFF header                :" << wavHeader.RIFF[0] << wavHeader.RIFF[1] << wavHeader.RIFF[2] << wavHeader.RIFF[3] << endl;
    cout << "WAVE header                :" << wavHeader.WAVE[0] << wavHeader.WAVE[1] << wavHeader.WAVE[2] << wavHeader.WAVE[3] << endl;
    cout << "FMT                        :" << wavHeader.fmt[0] << wavHeader.fmt[1] << wavHeader.fmt[2] << wavHeader.fmt[3] << endl;
    cout << "ChunkSize                  :" << wavHeader.ChunkSize << endl;

    // Display the sampling Rate from the header
    cout << "sampleRate                 :" << wavHeader.sampleRate << endl;
    cout << "bitsPerSample              :" << wavHeader.bitsPerSample << endl;
    cout << "nChannels                  :" << wavHeader.nChannels << endl;
    cout << "Number of bytes per second :" << wavHeader.bytesPerSec << endl;
    cout << "Audio Format               :" << wavHeader.AudioFormat << endl;

    // Audio format 1=PCM,6=mulaw,7=alaw, 257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM
    cout << "Block align                :" << wavHeader.blockAlign << " bytes" << endl;
    cout << "Data string                :" << wavHeader.Subchunk2ID[0] << wavHeader.Subchunk2ID[1] << wavHeader.Subchunk2ID[2] << wavHeader.Subchunk2ID[3] << endl;
    cout << "Data length                :" << wavHeader.Subchunk2Size << endl;
}