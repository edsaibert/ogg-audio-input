#include "./shader.h"

std::string Shader::loadShaderSource(const char *filePath)
{
    std::ifstream shaderFile;
    shaderFile.open(filePath);

    if (!shaderFile.is_open())
    {
        std::cerr << "Failed to open shader file: " << filePath << std::endl;
        return "";
    }

    std::stringstream shaderStream;
    shaderStream << shaderFile.rdbuf();
    shaderFile.close();

    return shaderStream.str();
}

int Shader::compileShaders()
{
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);     // Cria um objeto -> vertex shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // Cria um objeto -> fragment shader

    std::string vertexShaderStr = loadShaderSource(vertexShaderPath);
    std::string fragmentShaderStr = loadShaderSource(fragmentShaderPath);

    std::cout << "vertex shader: " << vertexShaderStr << "fragment shader: " << fragmentShaderStr << std::endl;
    const char *VShaderConst = vertexShaderStr.c_str();
    const char *FShaderConst = fragmentShaderStr.c_str();

    // Conecta o caminho dos shaders para seus objetos
    glShaderSource(vertexShader, 1, &VShaderConst, NULL);
    glShaderSource(fragmentShader, 1, &FShaderConst, NULL);

    // Compila os shaders
    glCompileShader(vertexShader);
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
        return EXIT_FAILURE;
    }

    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
        return EXIT_FAILURE;
    }

    shaderProgram = glCreateProgram(); // cria o programa do shader através do objeto

    // Anexa os shaders ao programa
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    glLinkProgram(shaderProgram); // Junta todas as partes descritas

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
        return EXIT_FAILURE;
    }

    // Deleta os shaders, pois eles já estão anexados ao programa
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    std::cout << "Sucess on compiling shaders" << std::endl;
    return EXIT_SUCCESS; // Retorna o programa do shader
}