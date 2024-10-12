// ------------------- Libs --------------------
#include "includes.h"

// -------------------- Own --------------------
#include "opengl/opengl.h"     // OpenGL, responsável pela criação de objetos e funções do OpenGL
#include "audio/audio.h"        // Audio, responsável pela captura e reprodução de áudio
#include "yin/yin.h"
// #include "shaders/shader.h"     // Shader, responsável pela criação de shaders

using namespace std;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS){
        glfwSetWindowShouldClose(window, GLFW_TRUE); // enter encerra o programa
    }
};

int main(int argc, char* argv[]) {
    gl gl(FRAMES_PER_BUFFER); // Cria um objeto da classe OpenGL
    pa pa(FRAMES_PER_BUFFER); // Cria um objeto da classe PortAudio e define o tamanho do buffer de áudio
    yin yin(FRAMES_PER_BUFFER); // Cria um objeto da classe YIN

    pa.portAudioInicialize();  // Inicializa o PortAudio
    pa.getDevices();           // Mostra os dispositivos de áudio disponíveis
    pa.openStream();           // Abre o stream de áudio
    pa.startStream();          // Inicia o stream de áudio

    glfwInit();                                    // Inicializa a biblioteca GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // Define a versão do OpenGL(4.6)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Define o perfil do OpenGL (CORE)

    // sucess = openAudioFile(ogg, &stream);

    // Cria uma janela 800x800 pixels, com o título "OggVorbis.Player()"
    GLFWwindow *window = glfwCreateWindow(800, 800, "OggVorbis.Player()", NULL, NULL);
    if (window == NULL){
        std::cout << "Failed to create GLFW window" << std::endl;
        return 0;
    }

    glfwMakeContextCurrent(window); // Define a janela atual

    /*  Carrega o GLAD, responsável por gerenciar os ponteiros do OpenGL */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD" << std::endl;
    }
    glViewport(0, 0, 800, 800);

    gl.compileShaders();
    gl.inicializeObjects(); // Inicia

    // int readStatus = readAudioChunk(ogg, &stream, vertices, verticesSize); // Read initial chunk

    while (!glfwWindowShouldClose(window)){       // Enquanto a janela não for fechada e não for fim de arquivo
        glfwSwapBuffers(window); // Troca os buffers de cor

        std::vector<std::complex<float>> audioBufferLeft = pa.getAudioBufferLeft(); // Obtém o buffer de áudio do canal esquerdo
        yin.getPitch(audioBufferLeft);
        pa.FFT(audioBufferLeft, false); // Aplica a transformada rápida de Fourier
        // pa.normalizeFFT(audioBufferLeft); // Normaliza a transformada rápida de Fourier

        gl.setVertices(audioBufferLeft);
        gl.draw(); // Desenha os vértices

        glfwPollEvents();                         // Verifica se há eventos
        glfwSetKeyCallback(window, key_callback); // Define a função de callback para teclas (to-do)
        // readStatus = readAudioChunk(ogg, &stream, vertices, verticesSize); // Read next chunk
    }

    pa.stopStream(); // Para o stream de áudio
    gl.destroyGL(); // Deleta os objetos criados

    // ov_clear(&stream);
    glfwDestroyWindow(window);
    glfwTerminate();

    pa.closeStream();           // Fecha o stream de áudio
    pa.portAudioTerminate();   // Finaliza o PortAudio
    return 1;
}