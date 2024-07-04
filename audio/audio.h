#ifndef AUDIO_H
#define AUDIO_H

#include "../includes.h"

class pa
{
public:
    using complex = std::complex<float>; // definição de tipo para números complexos
    const double pi = acos(-1);          // pi

    std::vector<float> audioBufferLeft;  // buffer de áudio para o canal esquerdo
    std::vector<float> audioBufferRight; // buffer de áudio para o canal direito

    pa(size_t bufferSize); // construtor

    void portAudioInicialize(); // inicializa o PortAudio
    void portAudioTerminate();  // termina o PortAudio

    void openStream();  // abre o stream de áudio
    void startStream(); // inicia o stream de áudio
    void stopStream();  // para o stream de áudio
    void closeStream(); // fecha o stream de áudio

    void getDevices(); // obtém os dispositivos de áudio

    std::vector<float> getAudioBufferLeft();  // obtém o buffer de áudio do canal esquerdo
    std::vector<float> getAudioBufferRight(); // obtém o buffer de áudio do canal direito

    float calculateCoefficients(); // calcula os coeficientes do filtro
    void lowpassFilter();          // aplica o lowpass filter

    void FFT(); // aplica a transformada rápida de Fourier

private:
    int numDevices;
    const PaDeviceInfo *deviceInfo;
    PaStreamParameters inputParameters;
    PaStreamParameters outputParameters;
    PaDeviceIndex inputDevice;
    PaDeviceIndex outputDevice;
    PaStream *stream;

    float cutoff = 1000.0f; // to-do

    void checkError(PaError err);
};

#endif