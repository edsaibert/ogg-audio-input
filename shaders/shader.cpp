/*
    Arquivo que contém as funções de inicialização e compilação dos shaders
*/

#include "shader.h"

/*  Código fonte dos shaders    */
const char *vertexShaderSourceOld = "#version 460 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *fragmentShaderSourceOld = "#version 460 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
    "}\n\0";

/*  Função que inicializa, compila e conecta os Shaders */
GLuint compileAndAttachShaders() {
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);     // Cria um objeto -> vertex shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // Cria um objeto -> fragment shader

    // Conecta o caminho dos shaders para seus objetos
    glShaderSource(vertexShader, 1, &vertexShaderSourceOld, NULL); 
    glShaderSource(fragmentShader, 1, &fragmentShaderSourceOld, NULL);

    // Compila os shaders
    glCompileShader(vertexShader); 
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    GLuint shaderProgram = glCreateProgram();    // cria o programa do shader através do objeto

    // Anexa os shaders ao programa
    glAttachShader(shaderProgram, vertexShader); 
    glAttachShader(shaderProgram, fragmentShader);

    glLinkProgram(shaderProgram);                // Junta todas as partes descritas

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // Deleta os shaders, pois eles já estão anexados ao programa
    glDeleteShader(vertexShader); 
    glDeleteShader(fragmentShader);

    return shaderProgram; // Retorna o programa do shader
}

/*  Função que inicializa os objectos VAO e VBO   */
void initializeAndAttachObjects(GLuint& VAO, GLuint& VBO) {
    glGenVertexArrays(1, &VAO); // Gera um array 
    glGenBuffers(1, &VBO); //  Gera um buffer

    glBindVertexArray(VAO); // Faz com que VAO seja o array vertex atual
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // Faz com que VBO seja o buffer atual
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)0); // Define as propriedades do buffer
    glEnableVertexAttribArray(0); // Habilita o buffer 
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Desconecta o buffer
    glBindVertexArray(0); // Desconecta o array
}

void checkGLError() {
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL error: " << error << std::endl;
    }
}

