#ifndef AUDIO_H
#define AUDIO_H

#include "../includes.h"

class pa{
public:
    std::vector<float> audioBufferLeft;
    std::vector<float> audioBufferRight;

    pa(size_t bufferSize);

    void portAudioInicialize();

    void portAudioTerminate();

    void openStream();

    void startStream();

    void stopStream();

    void closeStream();

    void getDevices();

    std::vector<float> getAudioBufferLeft();

    std::vector<float> getAudioBufferRight();

    void encodeAudio();

    void FFT();

private:
    int numDevices;
    const PaDeviceInfo* deviceInfo;
    PaStreamParameters inputParameters;
    PaStreamParameters outputParameters;
    PaDeviceIndex inputDevice;
    PaDeviceIndex outputDevice;
    PaStream* stream;

    void checkError(PaError err);
};

#endif