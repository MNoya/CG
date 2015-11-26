#ifndef SCENE_H
#define SCENE_H

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
#include "obj.h"
#include "texture.h"

typedef struct node
{
    int type;
    Vec3f position;
    Vec3f scale;
    Vec3f rotation;

    // Model
    Obj* object;
    GLuint texture;

    // Light
    int nLight;
    Vec3f color;

    int nChilds;
    struct node* childs;
} scene_node;

int parse_scene(char* path);
scene_node* parse_node(FILE* file, char* line, int depth);
void render_node(scene_node* n);

#endif //SCENE_H