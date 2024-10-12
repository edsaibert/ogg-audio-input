#ifndef DRAWABLE_H
#define DRAWABLE_H

/*
    Arquivo com a definição da classe Drawable
*/

#include "../includes.h"

class Drawable
{
public:
    using complex = std::complex<float>; // definição de tipo para números complexos
    std::vector<complex> buffer;

    // Construtor
    Drawable(size_t bufferSize, GLuint shaderProgram) : bufferSize(bufferSize), shaderProgram(shaderProgram)
    {
        verticesSize = bufferSize * 2;
        vertices = new GLfloat[verticesSize];
    }

    // Método virtual que desenha os vértices
    virtual void draw()
    {
        clearScreen();
        bindBuffers();
        setVertices();
        updateShaderProgram();
        performDraw();
        unbindBuffers();
    }

protected:
    virtual void clearScreen()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    };

    virtual void bindBuffers()
    {
        /*  Função que inicializa os objectos VAO e VBO   */
        glGenVertexArrays(1, &VAO); // Gera um array
        glGenBuffers(1, &VBO);      //  Gera um buffer

        glBindVertexArray(VAO);                                                          // Faz com que VAO seja o array vertex atual
        glBindBuffer(GL_ARRAY_BUFFER, VBO);                                              // Faz com que VBO seja o buffer atual
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void *)0); // Define as propriedades do buffer
        glEnableVertexAttribArray(0);                                                    // Habilita o buffer
    };

    virtual void updateShaderProgram() {
        glUseProgram(shaderProgram);
    };

    virtual void setVertices() = 0;
    virtual void performDraw() = 0;

    virtual void unbindBuffers() {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    };

    GLuint VAO, VBO, shaderProgram;
    const size_t samplesPerChannel = SAMPLE_RATE;
    size_t bufferSize, verticesSize;
    GLfloat *vertices;
};

#endif