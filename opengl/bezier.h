#ifndef BEZIER_H
#define BEZIER_H

/*
    Arquivo com a definição da classe Bezier (curva de Bézier)
*/

#include "../includes.h"
#include "drawable.h"

class Bezier : public Drawable {
public:
    Bezier(size_t bufferSize, GLuint shaderProgram) : Drawable(bufferSize, shaderProgram) {}

protected:
    void setVertices() override;
    void performDraw() override;
};

#endif 