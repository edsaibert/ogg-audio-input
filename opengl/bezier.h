#ifndef BEZIER_H
#define BEZIER_H

/*
    Arquivo com a definição da classe Bezier (curva de Bézier)
*/

#include "../includes.h"
#include "drawable.h"

class Bezier : public Drawable
{
public:
    Bezier(std::size_t bufferSize) : Drawable(bufferSize) {}

    ~Bezier() {}

    void loadShader() override
    {
        shader->vertexShaderPath = strdup("vertexBezier.vert");
        shader->fragmentShaderPath = strdup("fragmentShader.frag");
        shader->compileShaders();
    };

protected:
    void setVertices(std::vector<float> buffer) override;
    void performDraw() override;


    // GLfloat *vertices;
};

#endif