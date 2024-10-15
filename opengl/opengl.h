#ifndef OPENGL_H
#define OPENGL_H

/*
    Arquivo com a definição da classe OpenGL
*/

#include "../includes.h"
#include "../audio/audio.h"

class gl {
public:
    // using complex = std::complex<float>; // definição de tipo para números complexos
    const std::size_t samplesPerChannel = SAMPLE_RATE; // Número de amostras por canal

    GLfloat *vertices;   // Array de vértices, que será desenhado na tela
    std::size_t verticesSize; // Tamanho do array de vértices
    std::size_t bufferSize;   // Tamanho do buffer de vértices

    gl(std::size_t bufferSize); // Construtor

    GLuint VAO, VBO, shaderProgram;

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