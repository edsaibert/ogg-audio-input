#ifndef YIN_H
#define YIN_H

/*
    Implementação do algoritmo de detecção de pitch YIN, conforme artigo de 2002 escrito por
    Alain de Cheveigne e Hideki Kawahara: "YIN, a fundamental frequency estimator for speech and music" 
*/

#include "../includes.h"

class yin {
    public:
        // using complex = std::complex<float>; // definição de tipo para números complexos
        std::size_t maxTau = FRAMES_PER_BUFFER/2;
        float yinThreshold = 0.9f;

        std::vector<float> yinBuffer; // definição do buffer

        yin(std::size_t bufferSize); // construtor

        float getPitch(std::vector<float> audioBuffer); // função que irá calcular o pitch conforme o buffer de áudio passado 
    
    private:
        /*  Calcula o quadrado de diferença com uma versão deslocada (shifted) do sinal */
        void calculateDifference(std::vector<float> audioBuffer); 

        /*  Calcula a média da diferença calculada no passo anterior e a normaliza */
        void calculateCumulativeMeanNormalizedDifference();

        /*  Procura os valores que estão acima do limite (threshold) estabelecido 
            e retorna o deslocamento (shift) com melhor aproximação
        */
        std::size_t calculateAbsoluteThreshold();

        /*  Muitas vezes o período não é um múltiplo do período de amostragem.
            Nesse caso é necessário calcular a interpolação a fim de conseguir
            um número não inteiro que melhor represente o deslocamento (shift) do sinal
        */
        float calculateParabolicInterpolation(std::size_t calculatedTau); 

};

#endif