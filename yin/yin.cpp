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

std::size_t yin::calculateAbsoluteThreshold(){
    std::size_t tau = 2;
    for (; tau < maxTau; tau++){
        if (yinBuffer[tau].real() < yinThreshold){
            while (tau + 1 < maxTau && yinBuffer[tau + 1].real() < yinBuffer[tau].real()) 
                tau++;
        }
    }
    if (tau == maxTau || yinBuffer[tau].real() >= yinThreshold)
        tau = -1;
    
    return tau;
};

float yin::calculateParabolicInterpolation(std::size_t calculatedTau){
    int t0, t1;
    float chosenTau;

    if (calculatedTau < 1)
        t0 = calculatedTau;
    else calculatedTau - 1;

    if (calculatedTau + 1 < maxTau)
        t1 = calculatedTau + 1;
    else calculatedTau; 

    if (t0 == calculatedTau){
        if (yinBuffer[calculatedTau].real() <= yinBuffer[calculatedTau].real())
            chosenTau = calculatedTau;
        else chosenTau = t1;
    }
    else if (t1 == calculatedTau){
        if (yinBuffer[calculatedTau].real() <= yinBuffer[calculatedTau].real())
            chosenTau = calculatedTau;
        else chosenTau = t0;
    }
    else {
        float dMinor = yinBuffer[t0].real(); 
        float dMax = yinBuffer[t1].real();
        float d = yinBuffer[calculatedTau].real();

        chosenTau = calculatedTau + ((dMinor - dMax) / 2*(dMinor - 2*d + dMax));
    }
    
    return chosenTau;
     
};

float yin::getPitch(std::vector<complex> audioBuffer){
    float pitchHz = -1;

    yin::calculateDifference(audioBuffer);
    yin::calculateCumulativeMeanNormalizedDifference();
    std::size_t tau = yin::calculateAbsoluteThreshold();

    if (tau != -1){
        pitchHz = SAMPLE_RATE / yin::calculateParabolicInterpolation(tau); 
    }

    return pitchHz;
}

