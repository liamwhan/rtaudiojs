#include "../lib/rtaudio/RtAudio.h"
#include <iostream>
#include <cstdlib>

#ifndef __MATH_LONG_DOUBLE_CONSTANTS
    #define __MATH_LONG_DOUBLE_CONSTANTS
#endif

#include <math.h>

static const double TAU = (double) M_PI * 2.0;
static const double A = 1.0;
static const double F = 400.0;
static unsigned int sampleRate = 44100;
static unsigned int bufferFrames = 256; // 256 sample frames
static unsigned int nChannels = 2;

// Two-channel sawtooth wave generator.
int saw( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
        double streamTime, RtAudioStreamStatus status, void *userData )
{
    
    unsigned int i, j;
    double *buffer = (double *) outputBuffer;
    // double *lastValues = (double *) userData;
    if ( status )
        std::cout << "Stream underflow detected!" << std::endl;
    // Write interleaved audio data.
    for ( i=0; i<nBufferFrames; i++ ) {
        for ( j=0; j<2; j++ ) {
            double sample = (A * sin((TAU * F) * (streamTime * i)));
            std::cout << sample << std::endl;
            // *buffer++ = lastValues[j];
            *buffer++ = sample;

        }
    }
    return 0;
}
int main()
{
    RtAudio dac;
    if ( dac.getDeviceCount() < 1 ) {
        std::cout << "\nNo audio devices found!\n";
        exit( 0 );
    }
    RtAudio::StreamParameters parameters;
    parameters.deviceId = dac.getDefaultOutputDevice();
    parameters.nChannels = 2;
    parameters.firstChannel = 0;
    
    double data[2];
    try {
        dac.openStream( &parameters, NULL, RTAUDIO_FLOAT64,
                       sampleRate, &bufferFrames, &saw, (void *)&data );
        dac.startStream();
    }
    catch ( RtAudioError& e ) {
        e.printMessage();
        exit( 0 );
    }
    
    char input;
    std::cout << "\nPlaying ... press <enter> to quit.\n";
    std::cin.get( input );
    try {
        // Stop the stream
        dac.stopStream();
    }
    catch (RtAudioError& e) {
        e.printMessage();
    }
    if ( dac.isStreamOpen() ) dac.closeStream();
    return 0;
}