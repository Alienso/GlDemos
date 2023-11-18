//
// Created by Alienson on 30.11.2022..
//

#include "Audio.h"

#include <complex>
#include <valarray>
#include <cmath>

#include <portaudio.h>
#include <fft.hpp>

#include "iostream"

#define PA_SAMPLE_TYPE      paFloat32

int Audio::gNumNoInputs;
int Audio::framesPerBuffer;
int Audio::sampleRate;
std::vector<float>* Audio::audioData;
std::vector<double>* Audio::magnitudes;

void Audio::init() {
    sampleRate = 44100;
    framesPerBuffer = 2048;
    gNumNoInputs = 1;

    magnitudes = new std::vector<double>(framesPerBuffer);
    audioData = new std::vector<float>(2 * framesPerBuffer);
}

int Audio::calculateDominantFrequency(){
    int bufferSize = 2*framesPerBuffer;
    double magnitude[bufferSize/2];
    std::valarray<Complex> fftArray(bufferSize);

    for (int i=0;i<bufferSize;i++)
        fftArray[i] = Complex((*audioData)[i],0);

    fft(fftArray);

    for(int i=0; i<(bufferSize/2);++i){
        double real = fftArray[i].real();
        double im = fftArray[i].imag();
        magnitude[i] = sqrt(real*real + im*im);
        (*magnitudes)[i] = (10*log10(magnitude[i]*magnitude[i]));
    }

    double max = magnitude[0];
    //double it = min(bufferSize/2,bufferSize/(SAMPLE_RATE*255));
    int index = 0;
    for (int i=0; i<bufferSize/2 ;i+=2){
        //if (10*log10(magnitude[i]*magnitude[i])>-200){ //*2???
            if(magnitude[i]>max){
                max = magnitude[i];
                index = i;
            }
        //}
    }

    return sampleRate * index / bufferSize;
}

static int fuzzCallback( const void *inputBuffer, void *outputBuffer,
                         unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo* timeInfo,
                         PaStreamCallbackFlags statusFlags,
                         void *userData ){

    const float *in = (const float*)inputBuffer;
    float *out = (float*)outputBuffer;

    unsigned int i;
    if( inputBuffer == NULL ){
        for( i=0; i<framesPerBuffer; i++ ){
            *out++ = 0;
            *out++ = 0;
        }
        Audio::gNumNoInputs += 1;
    }
    else{
        float input;
        for( i=0; i<framesPerBuffer; i++ ){
            input = *in++;
            (*Audio::audioData)[i] = input;
            (*Audio::audioData)[i + 1] = input;
            //*out++ = input;
            //*out++ = input;
        }

        //Audio::applyHann();

        int rez = Audio::calculateDominantFrequency();
        std::cout<<rez<<std::endl;
    }

    return paContinue;
}

void* Audio::processAudio(){

    PaStreamParameters inputParameters, outputParameters;
    PaStream *stream;
    PaError err;

    err = Pa_Initialize();
    if (err != paNoError){
        std::cout << "Pa_init";
        return nullptr;
    }

    inputParameters.device = Pa_GetDefaultInputDevice();
    if (inputParameters.device == paNoDevice) {
        std::cout << "Error: No default input device.\n";
        return nullptr;
    }
    inputParameters.channelCount = 1;
    inputParameters.sampleFormat = PA_SAMPLE_TYPE;
    inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultLowInputLatency;
    inputParameters.hostApiSpecificStreamInfo = NULL;

    outputParameters.device = Pa_GetDefaultOutputDevice();
    if (outputParameters.device == paNoDevice) {
        std::cout << "Error: No default output device.\n";
        return nullptr;
    }
    outputParameters.channelCount = 2;
    outputParameters.sampleFormat = PA_SAMPLE_TYPE;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;


    err = Pa_OpenStream(
            &stream,
            &inputParameters,
            &outputParameters,
            sampleRate,
            framesPerBuffer,
            0,
            fuzzCallback,
            NULL );
    if( err != paNoError ) return nullptr;

    err = Pa_StartStream( stream );
    if( err != paNoError ) return nullptr;

    std::cout << "Hit ENTER to stop program.\n";
    getchar();
    err = Pa_CloseStream( stream );
    if( err != paNoError ) return nullptr;

    std::cout << "Finished. gNumNoInputs = " << gNumNoInputs << '\n';
    Pa_Terminate();

    return (void*)1;
}

void Audio::applyHann(){

    for (int i=0;i<2*framesPerBuffer;i++)
        (*audioData)[i] = (*audioData)[i] * pow(sin((M_PI * i)/(2*framesPerBuffer)),2);

    /*for (int i=0;i<2*FRAMES_PER_BUFFER;i++)
        data[i] = 1-data[i];
    return;*/

}

Audio::Audio() {
}

Audio::~Audio() {

}
