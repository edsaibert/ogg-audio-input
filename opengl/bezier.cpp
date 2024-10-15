#include "bezier.h"

void Bezier::setVertices()
{
    float maxMagnitude = 0.0;

    /*
        Encontra a amplitude máxima conforme a fórmula |a + bi| = sqrt(a^2 + b^2)
        onde a é a parte real e b é a parte imaginária
    */
    for (const auto &c : buffer)
    {
        float magnitude = std::abs(c);
        if (magnitude > maxMagnitude)
        {
            maxMagnitude = magnitude;
        }
    }

    std::size_t n = buffer.size();
    for (std::size_t i = 0; i < n; i++)
    {
        float frequency = (i * SAMPLE_RATE) / n;                                                            // i agora corresponde a frequencias
        float logFrequency = std::log10(frequency + 1);                                                     // escola logarítmica
        float normalizedFrequency = (logFrequency - std::log10(20)) / (std::log10(20000) - std::log10(20)); // Normaliza entre 20 Hz e 20 kHz

        vertices[i * 2] = normalizedFrequency * 2 - 1;                     // X coordinate entre [-1, 1]
        vertices[i * 2 + 1] = (std::abs(buffer[i]) / maxMagnitude) - 0.5f; // Y coordinate entre [0, 1]
    }
}

void Bezier::performDraw()
{
    // draw the curve
    // will be necessary to create a shader for this

    shader->vertexShaderPath = "vertexBezier.vert";
    shader->fragmentShaderPath = "fragmentShader.frag";
    shader->compileShaders();
}