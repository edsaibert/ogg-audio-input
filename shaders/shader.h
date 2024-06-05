#ifndef SHADER_H
#define SHADER_H

// ------------------- Libs --------------------
#include "../includes.h"

// ------------------- Shader -------------------

GLuint compileAndAttachShaders();

void initializeAndAttachObjects(GLuint& VAO, GLuint& VBO);

#endif
