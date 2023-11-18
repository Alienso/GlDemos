//
// Created by Alienson on 30.11.2022..
//

#ifndef AUDIOFREQUENCY_AUDIO_H
#define AUDIOFREQUENCY_AUDIO_H

#include <vector>

class Audio {

public:
    Audio();
    ~Audio();
    static void init();
    static void* processAudio();
    static int calculateDominantFrequency();

    static std::vector<double>* magnitudes;
    static std::vector<float>* audioData;
    static void applyHann();
    static int gNumNoInputs;
    static int framesPerBuffer;

private:
    static int sampleRate;
};


#endif //AUDIOFREQUENCY_AUDIO_H
