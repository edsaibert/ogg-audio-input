#ifndef INCLUDES_H
#define INCLUDES_H

// ------------------- Natives -------------------
#include <iostream>             // Bibliotecas padrão de entrada e saída
#include <vector>               // Biblioteca de vetores
#include <cstring>             // Biblioteca de strings

// ----------------- OGG Vorbis -----------------
#include <ogg/ogg.h>
#include <vorbis/vorbisfile.h>
#include <vorbis/vorbisenc.h>

// ------------------ PortAudio -----------------
#include <portaudio.h>          // PortAudio, que permite a captura e reprodução de áudio

// ------------------- OpenGL -------------------
// #include <GL/glew.h>            // GLEW, que permite o uso de funções OpenGL
#include "glad.h"
#include <GL/freeglut.h>        // freeglut, que fornece funções para a criação de janelas e renderização de objetos 3D
#include <glm/glm.hpp>          // GLM, que fornece funções matemáticas e vetoriais
#include <GLFW/glfw3.h>         // GLFW, que permite a criação de janelas

#define SAMPLE_RATE 44100       //  taxa de amostragem
#define FRAMES_PER_BUFFER 2048  // tamanho do buffer de áudio

#endif