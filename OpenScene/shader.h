#ifndef SHADER_H_INCLUDED
#define SHADER_H_INCLUDED
#ifdef WIN32
#include "glew.h"
#else
#include <GL/gl.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef GLuint ObjectProgram;
typedef GLuint Shader;

ObjectProgram shader_compile(char* source, GLenum type);
Shader shader_new(char* vert_file, char* frag_file);

void shader_free(Shader shader);
void shader_use(Shader shader);
void shader_stop(Shader shader);
GLint shader_get_att_loc(Shader shader, char* att);
GLint shader_get_unif_loc(Shader shader, char* unif);

#endif // SHADER_H_INCLUDED
