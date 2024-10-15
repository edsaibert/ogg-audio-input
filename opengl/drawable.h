#ifndef DRAWABLE_H
#define DRAWABLE_H

/*
    Arquivo com a definição da classe Drawable
*/

#include "../includes.h"
#include "./shader.h"
#include "./tbo.h"

class Drawable
{
public:
    // using complex = std::complex<float>; // definição de tipo para números complexos

    // Construtor
    Drawable(
        std::size_t bufferSize)
        : bufferSize(bufferSize) {
        shader = new Shader();
        // vertices = new GLfloat[bufferSize * 2];
    }

    ~Drawable()
    {
        glDeleteBuffers(1, &VBO);
        glDeleteVertexArrays(1, &VAO);
    }

    // Método virtual que desenha os vértices
    virtual void draw(std::vector<float> buffer)
    {
        clearScreen();
        bindBuffers();
        setVertices(buffer);
        updateShaderProgram();
        performDraw();
        unbindBuffers();
    }

    virtual void loadShader() = 0;
    virtual void bindTextureBuffer(std::vector<float> buffer) {
        tbo = new TextureBuffer(bufferSize, buffer);
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

    virtual void updateShaderProgram()
    {
        glUseProgram(shader->shaderProgram);
    };

    virtual void setVertices(std::vector<float> buffer) = 0;
    virtual void performDraw() = 0;

    virtual void unbindBuffers()
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    };

    GLuint VAO, VBO;
    GLfloat *vertices;
    const std::size_t samplesPerChannel = SAMPLE_RATE;
    std::size_t bufferSize, verticesSize;
    // std::vector<float> buffer;

    TextureBuffer *tbo;
    Shader *shader;
};

#endif