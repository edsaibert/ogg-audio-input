#ifndef OPENGL_H
#define OPENGL_H

/*
    Arquivo com a definição da classe OpenGL
*/

#include "../includes.h"
#include "../audio/audio.h"

class gl {
public:
    const size_t samplesPerChannel = SAMPLE_RATE; // Número de amostras por canal

    GLfloat *vertices;   // Array de vértices, que será desenhado na tela
    size_t verticesSize; // Tamanho do array de vértices
    size_t bufferSize;   // Tamanho do buffer de vértices

    gl(size_t bufferSize); // Construtor

    GLuint VAO, VBO, shaderProgram;

    int inicializeGL(); // Inicializa o OpenGL

    GLfloat* getVertices(); // Retorna o array de vértices

    void setVertices(std::vector<float> audioBuffer); // Define os vértices

    void draw(); // Desenha os vértices

    void destroyGL(); // Destrói os objetos criados

    int compileShaders(); // Compila os shaders

    void inicializeObjects(); // Inicializa os objetos

private:

    int checkGLError(); // Verifica se houve erro no OpenGL

};

#endif