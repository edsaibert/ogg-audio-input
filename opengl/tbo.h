#include "../includes.h"

/*
    Implementa uma classe TBO OpenGL (Texture Buffer Object)
*/

class TextureBuffer {
public:
    TextureBuffer(std::size_t bufferSize, std::vector<float>buffer) : bufferSize(bufferSize), buffer(buffer) {
        genBuffers();
        genTextures();
    }

    ~TextureBuffer(){
        glDeleteBuffers(1, &TBO);
        glDeleteTextures(1, &TBO_tex);
    }

protected:
    void genTextures(){
        glGenTextures(1, &TBO_tex);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_BUFFER, TBO_tex);
        glTexBuffer(GL_TEXTURE_BUFFER, GL_R32F, TBO);
    }

    void genBuffers(){
        glGenBuffers(1, &TBO);
        glBindBuffer(GL_TEXTURE_BUFFER, TBO);
        glBufferData(GL_TEXTURE_BUFFER, bufferSize * sizeof(float), &buffer[0], GL_STATIC_DRAW);
        glBindBuffer(GL_TEXTURE_BUFFER, 0);
    }

    std::vector<float> buffer;
    std::size_t bufferSize;
    GLuint TBO, TBO_tex;
};