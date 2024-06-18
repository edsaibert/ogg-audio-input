#ifndef OPENGL_H
#define OPENGL_H

/*
    Arquivo com a definição da classe OpenGL
*/

#include "../includes.h"

class gl {
public:
    const size_t bufferSize = 4096;
    const size_t samplesPerChannel = 4096;

    GLfloat* vertices = new GLfloat[bufferSize * 2];
    size_t verticesSize = bufferSize * 2;

    GLuint VAO, VBO, shaderProgram;

    int inicializeGL(); // Inicializa o OpenGL

    GLfloat* getVertices(); // Retorna o array de vértices

    void setVertices(); // Define os vértices

    void draw(); // Desenha os vértices

    void destroyGL(); // Destrói os objetos criados

    int compileShaders(); // Compila os shaders

    void inicializeObjects(); // Inicializa os objetos

private:

    int checkGLError(); // Verifica se houve erro no OpenGL

};

#endif