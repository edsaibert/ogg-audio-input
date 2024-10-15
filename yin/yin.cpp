#include "yin.h"

yin::yin(std::size_t bufferSize) : yinBuffer(bufferSize, 0){}

void yin::calculateDifference(std::vector<float> audioBuffer){
    yinBuffer.resize(maxTau);

    for (std::size_t tau = 1; tau < maxTau; tau++){
        float sum = 0.0f;

        // autocorrelation
        for (std::size_t i = 0; i + tau < audioBuffer.size(); i++){
            float diff = audioBuffer[i] - audioBuffer[i+tau];
            sum += diff * diff;
        }
        yinBuffer[tau] = sum;
    }
}

void yin::calculateCumulativeMeanNormalizedDifference(){
    float sum = 0.0f;
    yinBuffer[0] = 1.0f;

    for (std::size_t tau = 1; tau < maxTau; tau++){
        sum += yinBuffer[tau];
        yinBuffer[tau] = yinBuffer[tau] * tau / sum;
    }

};

std::size_t yin::calculateAbsoluteThreshold(){
    std::size_t tau = 2;
    for (; tau < maxTau; tau++){
        if (yinBuffer[tau] < yinThreshold){
            while (tau + 1 < maxTau && (yinBuffer[tau + 1] < yinBuffer[tau])) {
                tau++;
            }
            return tau;
        }
    }
    // if (tau == maxTau || yinBuffer[tau].real() >= yinThreshold)
    //     tau = -1;
    
    return -1;
};

float yin::calculateParabolicInterpolation(std::size_t calculatedTau) {
    int t0, t1;
    float chosenTau;

    if (calculatedTau < 1)
        t0 = calculatedTau;
    else 
        t0 = calculatedTau - 1;

    if (calculatedTau + 1 < maxTau)
        t1 = calculatedTau + 1;
    else 
        t1 = calculatedTau;

    // Parabolic interpolation
    if (t0 == calculatedTau || t1 == calculatedTau) {
        if (yinBuffer[t0] <= yinBuffer[t1])
            chosenTau = t0;
        else
            chosenTau = t1;
    } else {
        float dMinor = yinBuffer[t0];
        float dMax = yinBuffer[t1];
        float d = yinBuffer[calculatedTau];

        chosenTau = calculatedTau + ((dMinor - dMax) / (2 * (dMinor - 2 * d + dMax)));
    }

    return chosenTau;
};


float yin::getPitch(std::vector<float> audioBuffer){
    float pitchHz = -1;

    yin::calculateDifference(audioBuffer);
    yin::calculateCumulativeMeanNormalizedDifference();
    std::size_t tau = yin::calculateAbsoluteThreshold();

    if (tau != -1){
        pitchHz = SAMPLE_RATE / yin::calculateParabolicInterpolation(tau); 
    }
    std::cout << "picthHz: " << pitchHz << std::endl;
    return pitchHz;
}

