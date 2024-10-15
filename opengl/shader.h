#include "../includes.h"

/*
    Classe que implementa os métodos de inicialização dos shaders

    Não esquecer:
    - De inicializar o objeto com os caminhos dos shaders (vertex e fragment)
*/

class Shader
{
public:
    Shader() {};
    ~Shader() {
        glDeleteProgram(shaderProgram);
    };

    std::string loadShaderSource(const char *filePath);
    int compileShaders();

    GLuint shaderProgram;
    char* vertexShaderPath; // Caminho do vertex shader
    char* fragmentShaderPath; // Caminho do fragment shader
};