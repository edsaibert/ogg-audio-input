#include "yin.h"

yin::yin(std::size_t bufferSize) : yinBuffer(bufferSize, 0){}

void yin::calculateDifference(std::vector<complex> audioBuffer){

    for (std::size_t tau = 1; tau < maxTau; tau++){
        complex sum = 0.0f;

        // autocorrelation
        for (std::size_t i = 0; i + tau < audioBuffer.size(); i++){
            complex diff = audioBuffer[i] - audioBuffer[i+tau];
            sum += diff * diff;
        }
        yinBuffer[tau] = sum;
    }
}

void yin::calculateCumulativeMeanNormalizedDifference(){
    complex sum = 0.0f;
    yinBuffer[0] = 1;

    for (std::size_t tau = 1; tau < maxTau; tau++){
        sum += yinBuffer[tau];
        yinBuffer[tau] *= (complex) tau/sum;
    }

};

long int yin::calculateAbsoluteThreshold(){};

float yin::calculateParabolicInterpolation(){};

