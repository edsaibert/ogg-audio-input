#ifndef AUDIO_H
#define AUDIO_H

#define SAMPLE_RATE 48000
#define FRAMES_PER_BUFFER 1024

#include "../includes.h"

class pa{
public:
    void portAudioInicialize();

    void portAudioTerminate();

    void getDevices();

    void readMicrophoneInput();

    void encodeAudio();

    void FFT();

private:
    void checkError(PaError err);
};

#endif