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

    int dispSize = 100;
    printf("\r");

    fflush(stdout);
    float vol_l = 0;
    float vol_r = 0;

    // 512 samples per channel
    for (unsigned long i = 0; i < framesPerBuffer; i++){
        vol_l += max(vol_l, paAbs(in[i * 2]));
        vol_r += max(vol_r, paAbs(in[i * 2 + 1]));
    }

    return 0;
}

static void checkError(PaError err) {
    if (err != paNoError) {
        std::cout << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
        exit(EXIT_FAILURE);
    }
}

void portAudioInicialize() {
    PaError err;
    err = Pa_Initialize();

    checkError(err);
}

void portAudioTerminate() {
    PaError err;
    err = Pa_Terminate();

    checkError(err);
}

void getDevices() {
    int numDevices = Pa_GetDeviceCount();
    std::cout << "Number of devices found: " << numDevices << std::endl;

    if (numDevices < 0){
        std::cout << "Error getting device count" << std::endl;
        exit(EXIT_FAILURE);
    } else if (numDevices == 0){
        std::cout << "No available audio devices found" << std::endl;
        exit(EXIT_SUCCESS);
    } 
    
    const PaDeviceInfo* deviceInfo;
    for (int i = 0; i < numDevices; i++){
        deviceInfo = Pa_GetDeviceInfo(i);
        std::cout << "Device " << i << ": " << deviceInfo->name << std::endl;
        std::cout << "  Name:" << deviceInfo->name << std::endl;
        std::cout << "  Max Input Channels: " << deviceInfo->maxInputChannels << std::endl;
        std::cout << "  Max Output Channels: " << deviceInfo->maxOutputChannels << std::endl;
        std::cout << "  Default Sample Rate: " << deviceInfo->defaultSampleRate << std::endl;
    }

    int device = 7;     // to-do: choose a device

    PaStreamParameters inputParameters;
    PaStreamParameters outputParameters;

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

    PaStream* stream;
    PaError err = Pa_OpenStream(
        &stream,
        &inputParameters,
        &outputParameters,
        SAMPLE_RATE,
        FRAMES_PER_BUFFER,
        paNoFlag,
        paTestCallback,
        NULL
    );
    checkError(err);

    err = Pa_StartStream(stream);
    checkError(err);

    Pa_Sleep(10* 1000); // Sleep for 10 seconds

    err = Pa_StopStream(stream);
    checkError(err);

    err = Pa_CloseStream(stream);
    checkError(err);

}