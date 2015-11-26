#ifndef TEXTURE_H
#define TEXTURE_H

#include "framework/canvas.h"
#include "framework/mm.h"

#ifdef WIN32
#include "glew.h"
#else
#include <GL/gl.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <SDL/SDL_image.h>

GLuint texture_load(char* path);

#endif //TEXTURE_H