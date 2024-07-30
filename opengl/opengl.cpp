#include "opengl.h"

/*  Código fonte dos shaders (to-do)   */
const char *vertexShaderSource = "#version 460 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *fragmentShaderSource = "#version 460 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
    "}\n\0";

/*  Checa erros no openGL   */
int gl::checkGLError() {
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL Error: " << error << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

/*
    Construtor da classe OpenGL
*/
gl::gl(size_t bufferSize) : bufferSize(bufferSize) {
    /*
        O vetor de vértices necessita ter um tamanho duplicado do buffer de áudio.
        Isso ocorre pois cada vértice é composto por duas coordenadas (x, y). 
    */
    verticesSize = bufferSize * 2;
    vertices = new GLfloat[verticesSize];
}

/*
    Função que define os vértices a serem desenhados na tela
    O parâmetro é um vetor de floats, que contém o buffer de áudio
*/
void gl::setVertices(std::vector<gl::complex> audioBuffer){
    // Caso o tamanho do buffer de áudio seja diferente do tamanho do vetor de vértices
    // if(audioBuffer.size() * 2 != verticesSize) {
    //     std::cout << "Size mismatch between audio buffer and vertex data" << std::endl;
    //     return;

    float maxMagnitude = 0.0;

    /* 
        Encontra a amplitude máxima conforme a fórmula |a + bi| = sqrt(a^2 + b^2)
        onde a é a parte real e b é a parte imaginária
    */
    for (const auto& c : audioBuffer) {
        float magnitude = std::abs(c);
        if (magnitude > maxMagnitude) {
            maxMagnitude = magnitude;
        }
    }

        size_t n = audioBuffer.size();
        for (size_t i = 0; i < n; i++){
            float frequency = (i * SAMPLE_RATE) / n;  // i agora corresponde a frequencias
            float logFrequency = std::log10(frequency + 1);  // escola logarítmica
            float normalizedFrequency = (logFrequency - std::log10(20)) / (std::log10(20000) - std::log10(20));  // Normaliza entre 20 Hz e 20 kHz

            vertices[i * 2] = normalizedFrequency * 2 - 1;  // X coordinate entre [-1, 1]
            vertices[i * 2 + 1] = (std::abs(audioBuffer[i]) / maxMagnitude) - 0.5f;  // Y coordinate entre [0, 1]
        }
}


GLfloat* gl::getVertices(){ 
    return vertices;
}

/*
    Função que desenha os vértices na tela
    e atualiza o buffer de vértices
*/
void gl::draw(){
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f); // Define a cor de fundo

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Limpa o buffer de cor
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verticesSize * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glUseProgram(shaderProgram);                // Usa o programa de shader
    glBindVertexArray(VAO);                     // Faz com que VAO seja o array vertex atual
    glDrawArrays(GL_LINE_STRIP, 0, bufferSize / 2); // Desenha os vértices
}

/*
    Função que compila os shaders
*/
int gl::compileShaders() {
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);     // Cria um objeto -> vertex shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // Cria um objeto -> fragment shader

    // Conecta o caminho dos shaders para seus objetos
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); 
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);

    // Compila os shaders
    glCompileShader(vertexShader); 
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        return EXIT_FAILURE;
    }

    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        return EXIT_FAILURE;
    }

    shaderProgram = glCreateProgram();    // cria o programa do shader através do objeto

    // Anexa os shaders ao programa
    glAttachShader(shaderProgram, vertexShader); 
    glAttachShader(shaderProgram, fragmentShader);

    glLinkProgram(shaderProgram);                // Junta todas as partes descritas

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        return EXIT_FAILURE;
    }

    // Deleta os shaders, pois eles já estão anexados ao programa
    glDeleteShader(vertexShader); 
    glDeleteShader(fragmentShader);

    return EXIT_SUCCESS; // Retorna o programa do shader
}


/*
    Função que inicializa os objectos VAO e VBO
*/
void gl::inicializeObjects() {
    /*  Função que inicializa os objectos VAO e VBO   */
    glGenVertexArrays(1, &VAO); // Gera um array
    glGenBuffers(1, &VBO);      //  Gera um buffer

    glBindVertexArray(VAO);                                                          // Faz com que VAO seja o array vertex atual
    glBindBuffer(GL_ARRAY_BUFFER, VBO);                                              // Faz com que VBO seja o buffer atual
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void *)0); // Define as propriedades do buffer
    glEnableVertexAttribArray(0);                                                    // Habilita o buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);                                                // Desconecta o buffer
    glBindVertexArray(0);                                                            // Desconecta o array
}

/*
    Função que destrói os objetos criados
*/
void gl::destroyGL(){
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    delete[] vertices;
}