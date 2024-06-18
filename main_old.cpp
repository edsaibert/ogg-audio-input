// ------------------- Libs --------------------
#include "includes.h"

// -------------------- Own --------------------
#include "shaders/shader.h"     // Shader, responsável pela criação de shaders
#include "audio/audio.h"        // Audio, responsável pela captura e reprodução de áudio

using namespace std;
const size_t bufferSize = 4096;
const size_t samplesPerChannel = 4096;

GLfloat* vertices = new GLfloat[bufferSize * 2];
std::vector<GLfloat*> verticesHistory;

int openAudioFile(FILE* ogg, OggVorbis_File* stream) {
    if (ov_open(ogg, stream, NULL, 0) < 0) {
        std::cout << "Failed to open audio file (function OpenAudioFile)" << std::endl;
        return 0;
    }
    std::cout << "Audio file opened (function OpenAudioFile)" << std::endl;
    return 1;
}

// Atualiza os vértices do array de vértices com os valores do buffer, por meio da transformação de coordenadas
void updateVertices(GLfloat vertices[], size_t verticesSize, short buffer[], size_t bufferSize){
    for (size_t i = 0; i < bufferSize && i < verticesSize / 2; i++){
        vertices[i * 2] = (i / (float)bufferSize) * 2.0f - 1.0f; // x-coordinate
        vertices[i * 2 + 1] = (buffer[i] / 32768.0f); // y-coordinate with scaling
    }
}

int readAudioChunk(FILE* ogg, OggVorbis_File* stream, GLfloat vertices[], size_t verticesSize){
    short buffer[samplesPerChannel]; // Buffer de leitura (4096 samples per channel)
    long ret = ov_read(stream, (char*)buffer, sizeof(buffer), 0, 2, 1, NULL);
    if (ret < 0){
        std::cout << "Failed to read audio file" << std::endl;
        return 0;
    }
    else if (ret == 0){
        return -1; // End of file
    }

    size_t samplesRead = ret / sizeof(short);
    updateVertices(vertices, verticesSize, buffer, samplesRead);
    return 1;
}

int main(int argc, char* argv[]) {
    OggVorbis_File stream;      // Arquivo que inicia a bitstream de dados
    FILE* ogg;                  // Arquivo onde será 'escrito' o áudio
    short int sucess;

    ogg = fopen("audio.ogg", "r+");

    if (ogg != NULL) {
        portAudioInicialize();  // Inicializa o PortAudio
        getDevices();           // Mostra os dispositivos de áudio disponíveis

        glfwInit();     // Inicializa a biblioteca GLFW
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);      // Define a versão do OpenGL(4.6)
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);     // Define o perfil do OpenGL (CORE)

        sucess = openAudioFile(ogg, &stream);

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

        size_t verticesSize = bufferSize * 2;

        GLuint VAO, VBO; // Cria ponteiros para o array e para o buffer do objeto shader
        initializeAndAttachObjects(VAO, VBO); // Inicializa e conecta os objetos criados


        int readStatus = readAudioChunk(ogg, &stream, vertices, verticesSize); // Read initial chunk

        while (!glfwWindowShouldClose(window) && readStatus != -1) {     // Enquanto a janela não for fechada e não for fim de arquivo
            glClearColor(0.07f, 0.13f, 0.17f, 1.0f);                     // Define a cor de fundo
            glfwSwapBuffers(window); // Troca os buffers de cor

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Limpa o buffer de cor
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, verticesSize * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            glUseProgram(shaderProgram);    // Usa o programa de shader
            glBindVertexArray(VAO);         // Faz com que VAO seja o array vertex atual
            glDrawArrays(GL_LINE_STRIP, 0, bufferSize);  // Desenha o array de vértices por meio de uma linha

            glfwPollEvents();    // Verifica se há eventos
            readStatus = readAudioChunk(ogg, &stream, vertices, verticesSize); // Read next chunk
        }

        // Deleta os objetos criados e finaliza o GLFW
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteProgram(shaderProgram); 

        delete[] vertices;
        
        ov_clear(&stream);
        glfwDestroyWindow(window);
        glfwTerminate();

        portAudioTerminate();   // Finaliza o PortAudio
    } else {
        std::cout << "Failed to open audio file" << std::endl;
        return 0;
    }
    return 1;
}