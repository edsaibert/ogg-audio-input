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

gl::gl(size_t bufferSize) : bufferSize(bufferSize) {
    vertices = new GLfloat[bufferSize];
    verticesSize = bufferSize;
}


int gl::inicializeGL(){

    /* Define a área de visualização do OpenGL
       Nesse caso, a área de visualização é x=0, y=0 até x=800, y=800 */
    glViewport(0, 0, 800, 800);

    compileShaders();

    if (shaderProgram == 0) {
        return EXIT_FAILURE;
    }

    inicializeObjects(); // Inicializa e conecta os objetos criados
}

void gl::setVertices(std::vector<float> audioBuffer){
    std::cout << "Setting vertices" << std::endl;

    if(audioBuffer.size() != verticesSize) {
        // Handle size mismatch
        std::cout << "Size mismatch between audio buffer and vertex data" << std::endl;
        return;
    }

    // // Normalize the audio data and apply it to the vertex data
    for (size_t i = 0; i < audioBuffer.size(); i++) {
    //     // Normalize the sample to the range -1.0 to 1.0
        float normalizedSample = audioBuffer[i];

    //     // Apply the normalized sample to the vertex data
        vertices[i] = normalizedSample;
        std::cout << "Vertex: " << vertices[i] << std::endl;
    }
}


GLfloat* gl::getVertices(){ 
    return vertices;
}

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

void gl::destroyGL(){
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    delete[] vertices;
}