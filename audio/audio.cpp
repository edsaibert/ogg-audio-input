#include "audio.h"

// function to get the maximum of two numbers
static inline float max(float a, float b){
    return a > b ? a : b;
}

// function to get the absolute value of a number
static inline float paAbs(float a){
    return a > 0 ? a : -a;
}

// Callback function for PortAudio
static int paTestCallback(
    const void* inputBuffer, void* outputBuffer, 
    unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, 
    PaStreamCallbackFlags statusFlags, void* userData
){
    float* in = (float*)inputBuffer;
    (void) outputBuffer;

    pa* audio = (pa*)userData;

    // float vol_l = 0;
    // float vol_r = 0;

    // 512 samples per channel
    for (unsigned long i = 0; i < framesPerBuffer; i++){
        audio->audioBufferLeft[i] = in[i * 2];
        audio->audioBufferRight[i] = in[(i * 2) + 1];
        // audio->audioBuffer
        // vol_l += max(vol_l, paAbs(in[i * 2]));
        // vol_r += max(vol_r, paAbs(in[i * 2 + 1]));
    }

    return paContinue;
}

void pa::checkError(PaError err) {
    if (err != paNoError) {
        std::cout << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
        exit(EXIT_FAILURE);
    }
}

// Constructor
pa::pa(size_t bufferSize) : audioBufferLeft(bufferSize, 0.0f) , audioBufferRight(bufferSize, 0.0f){

}

void pa::portAudioInicialize() {
    PaError err;
    err = Pa_Initialize();

    checkError(err);
}

void pa::portAudioTerminate() {
    PaError err;
    err = Pa_Terminate();

    checkError(err);
}

void pa::openStream(){
    // print de input e output parameters
    PaError err = Pa_OpenStream(
        &stream,
        &inputParameters,
        &outputParameters,
        SAMPLE_RATE,
        FRAMES_PER_BUFFER,
        paNoFlag,
        paTestCallback,
        this);
    checkError(err);
}

void pa::startStream(){
    PaError err;
    err = Pa_StartStream(stream);
    checkError(err);
}

void pa::stopStream(){
    PaError err;
    err = Pa_StopStream(stream);
    checkError(err);
}

void pa::closeStream(){
    PaError err;
    err = Pa_CloseStream(stream);
    checkError(err);
}

void pa::getDevices() {
    numDevices = Pa_GetDeviceCount();
    std::cout << "Number of devices found: " << numDevices << std::endl;

    if (numDevices < 0){
        std::cout << "Error getting device count" << std::endl;
        exit(EXIT_FAILURE);
    } else if (numDevices == 0){
        std::cout << "No available audio devices found" << std::endl;
        exit(EXIT_SUCCESS);
    } 
    
    for (int i = 0; i < numDevices; i++){
        deviceInfo = Pa_GetDeviceInfo(i);
        std::cout << "Device " << i << ": " << deviceInfo->name << std::endl;
        std::cout << "  Name:" << deviceInfo->name << std::endl;
        std::cout << "  Max Input Channels: " << deviceInfo->maxInputChannels << std::endl;
        std::cout << "  Max Output Channels: " << deviceInfo->maxOutputChannels << std::endl;
        std::cout << "  Default Sample Rate: " << deviceInfo->defaultSampleRate << std::endl;
    }

    int device = 7;     // to-do: choose a device

    // Set input and output parameters of the chosen device
    memset(&inputParameters, 0, sizeof(inputParameters));
    inputParameters.channelCount = 2; // stereo (change if needed)
    inputParameters.device = device;
    inputParameters.hostApiSpecificStreamInfo = NULL; 
    inputParameters.sampleFormat = paFloat32; // 32-bit floating point (change if needed)
    inputParameters.suggestedLatency = Pa_GetDeviceInfo(device)->defaultLowInputLatency;

    memset(&outputParameters, 0, sizeof(outputParameters));
    outputParameters.channelCount = 2; // stereo (change if needed)
    outputParameters.device = device;
    outputParameters.hostApiSpecificStreamInfo = NULL; 
    outputParameters.sampleFormat = paFloat32; // 32-bit floating point (change if needed)
    outputParameters.suggestedLatency = Pa_GetDeviceInfo(device)->defaultLowInputLatency;
}

std::vector<pa::complex> pa::getAudioBufferLeft(){
    return audioBufferLeft;
}

std::vector<pa::complex> pa::getAudioBufferRight(){
    return audioBufferRight;
}

float pa::calculateCoefficients() {
    float RC = 1 / cutoff * 2 * M_PI;
    float dt = 1 / SAMPLE_RATE;

    return dt / (RC + dt);
}

void pa::lowpassFilter(){}  // to-do

void pa::FFT(std::vector<complex> &v, bool invert){
    int n = v.size();
    if (n <= 1) return;

    std::vector<complex> even(n / 2), odd(n / 2); // divide a recursão em duas metades (com valores pares e ímpares de índice)
    for (int i = 0; 2*i < n; i++){
        even[i] = v[2*i];
        odd[i] = v[2*i + 1];
    }

    FFT(even, invert);
    FFT(odd, invert);
    
    double theta = 2 * pi / n * (invert ? -1 : 1);  // define o ângulo de rotação (invert troca a direção da rotação)
    complex w(1), wn(cos(theta), sin(theta));       // define os vetores de rotação

    // combina os resultados das subtransformadas
    for (int i = 0; 2 * i < n; i++){
        v[i] = even[i] + w * odd[i];
        v[i + n/2] = even[i] - w * odd[i];
        if (invert) {
            v[i] /= 2;
            v[i + n/2] /= 2;
        }
        w *= wn;
    }
}

void pa::normalizeFFT(std::vector<complex>& fftData) {
    // Find maximum magnitude
    float maxMagnitude = 0.0;
    for (size_t i = 0; i < fftData.size(); ++i) {
        float magnitude = std::abs(fftData[i]);
        if (magnitude > maxMagnitude) {
            maxMagnitude = magnitude;
        }
    }

    // Normalize amplitudes to range [0, 1]
    if (maxMagnitude > 0.0) {
        for (size_t i = 0; i < fftData.size(); ++i) {
            fftData[i] = (fftData[i] / maxMagnitude) * 10.0f;
        }
    }
}
