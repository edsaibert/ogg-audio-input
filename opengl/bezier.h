#ifndef BEZIER_H
#define BEZIER_H

/*
    Arquivo com a definição da classe Bezier (curva de Bézier)
*/

#include "../includes.h"
#include "drawable.h"


class Bezier : public Drawable {
public:
    Bezier(std::size_t bufferSize, GLuint shaderProgram, std::vector<float> buffer) : Drawable(bufferSize, shaderProgram, buffer) {
        vertices = new GLfloat[bufferSize * 2];
    } 

    ~Bezier() {}

protected:
    void setVertices() override;
    void performDraw() override;

    GLfloat* vertices;
};

#endif 