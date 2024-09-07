#include "yin.h"

yin::yin(size_t bufferSize) : yinBuffer(bufferSize, 0){}

void yin::calculateDifference(size_t maxTau, std::vector<complex> audioBuffer){
    size_t T = audioBuffer.size();        

    for (size_t tau = 1; tau < maxTau; tau++){
        float sum = 0.0f;

        // autocorrelation
        for (size_t i = 0; i + tau < T; i++){
            float diff = audioBuffer[i] - audioBuffer[i+tau];
            sum += diff * diff;
        }
        yinBuffer[tau] = sum;
    }
}

void yin::calculateCumulativeMeanNormalizedDifference(){};

long int yin::calculateAbsoluteThreshold(){};

float yin::calculateParabolicInterpolation(){};

