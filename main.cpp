
// ------------------- Natives -------------------
#include <iostream>
#include <stdio.h>

// ----------------- OGG Vorbis -----------------
#include <ogg/ogg.h>
#include <vorbis/vorbisfile.h>
#include <vorbis/vorbisenc.h>

// ------------------- OpenGL -------------------
// #include <GL/glew.h>            // GLEW, que permite o uso de funções OpenGL
#include "glad.h"
#include <GL/freeglut.h>        // freeglut, que fornece funções para a criação de janelas e renderização de objetos 3D
#include <glm/glm.hpp>          // GLM, que fornece funções matemáticas e vetoriais
#include <GLFW/glfw3.h>         // GLFW, que permite a criação de janelas


// #include <fstream>

using namespace std;

int main(int agrc, char* argc[]) {
    OggVorbis_File stream;      // Arquivo que inicia a bitstream de dados
    FILE* ogg;                  // Arquivo onde será 'escrito' o áudio
    oggpack_buffer* buffer;               // Buffer FILE

    ogg = fopen("audio.ogg", "r+");

    if (ogg != NULL){
        glfwInit();     // Inicializa a biblioteca GLFW
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);      // Define a versão do OpenGL(3.3)
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);     // Define o perfil do OpenGL

        GLFWwindow* window = glfwCreateWindow(800, 800, "OggVorbis.Player()", NULL, NULL);
        if (window == NULL){
            std::cout << "Failed to create GLFW window" << std::endl;
            return 0;
        }
        glfwMakeContextCurrent(window);
        while (!glfwWindowShouldClose(window)){
            glfwPollEvents();
        }

        gladLoadGL();

        ov_open(ogg, &stream, NULL, 0);
        printf("Open file");
        
        
        ov_clear(&stream);
        glfwDestroyWindow(window);
        glfwTerminate();
    }
    else {
        std::cout << "Failed to open audio file" << std::endl;
        return 0;
    }
    return 1;

}


