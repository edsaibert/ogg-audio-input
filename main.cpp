// ------------------- Libs --------------------
#include "includes.h"

// -------------------- Own --------------------
#include "shaders/shader.h"     // Shader, responsável pela criação de shaders

using namespace std;

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
