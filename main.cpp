// ------------------- Libs --------------------
#include "includes.h"

// -------------------- Own --------------------
#include "shaders/shader.h"     // Shader, responsável pela criação de shaders

using namespace std;

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

/*  Função que inicializa, compila e conecta os Shaders */
GLuint compileAndAttachShaders() {
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

void initializeAndAttachObjects(GLuint& VAO, GLuint& VBO, GLfloat vertices[], size_t verticesSize) {
    glGenVertexArrays(1, &VAO);     // Gera um array de vértices
    glGenBuffers(1, &VBO);          // Gera um buffer de vértices

    glBindVertexArray(VAO);         // Faz com que VAO seja o array vertex atual
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // Faz com que VBO seja o buffer atual

    glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW); // Coloca os vértices no buffer

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0); // Define os atributos do vértice
    glEnableVertexAttribArray(0); // Habilita o atributo do vértice

    glBindBuffer(GL_ARRAY_BUFFER, 0); // Desconecta o buffer
    glBindVertexArray(0); // Desconecta o array
}

void checkGLError() {
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL error: " << error << std::endl;
    }
}

int main(int argc, char* argv[]) {
    OggVorbis_File stream;      // Arquivo que inicia a bitstream de dados
    FILE* ogg;                  // Arquivo onde será 'escrito' o áudio
    oggpack_buffer* buffer;               // Buffer FILE

    ogg = fopen("audio.ogg", "r+");

    if (ogg != NULL) {
        glfwInit();     // Inicializa a biblioteca GLFW
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);      // Define a versão do OpenGL(4.6)
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);     // Define o perfil do OpenGL (CORE)

        GLfloat vertices[] = {
            0.5f, 0.5f, 0.0f,  // ponto 1 
            0.5f, -0.5f, 0.0f, // ponto 2 
        };

        // Cria uma janela 800x800 pixels, com o título "OggVorbis.Player()"
        GLFWwindow* window = glfwCreateWindow(800, 800, "OggVorbis.Player()", NULL, NULL);
        if (window == NULL) {
            std::cout << "Failed to create GLFW window" << std::endl;
            return 0;
        }

        glfwMakeContextCurrent(window);    // Define a janela atual

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cout << "Failed to initialize GLAD" << std::endl;
            return -1;
        }

        /* Define a área de visualização do OpenGL
           Nesse caso, a área de visualização é x=0, y=0 até x=800, y=800 */
        glViewport(0, 0, 800, 800);     

        GLuint shaderProgram = compileAndAttachShaders();

        size_t verticesSize = sizeof(vertices);

        GLuint VAO, VBO; // Cria ponteiros para o array e para o buffer do objeto shader
        initializeAndAttachObjects(VAO, VBO, vertices, verticesSize); // Inicializa e conecta os objetos criados

        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);    // Define a cor de fundo

        while (!glfwWindowShouldClose(window)) {     // Enquanto a janela não for fechada
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Limpa o buffer de cor

            glUseProgram(shaderProgram);    // Usa o programa de shader
            glBindVertexArray(VAO);         // Faz com que VAO seja o array vertex atual
            glDrawArrays(GL_LINES, 0, 2);  // Desenha o array de vértices

            glfwSwapBuffers(window); // Troca os buffers de cor
            glfwPollEvents();    // Verifica se há eventos
        }

        ov_open(ogg, &stream, NULL, 0);
        std::cout << "Audio file opened" << std::endl;

        // Deleta os objetos criados e finaliza o GLFW
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteProgram(shaderProgram); 
        
        ov_clear(&stream);
        glfwDestroyWindow(window);
        glfwTerminate();
    } else {
        std::cout << "Failed to open audio file" << std::endl;
        return 0;
    }
    return 1;
}
