#ifndef SCENE_H
#define SCENE_H

#include "GL/gl.h"
#include "framework/canvas.h"
#include "framework/mm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "obj.h"

typedef struct node
{
    int type;
    Vec3f position;
    Vec3f scale;
    Vec3f rotation;
    struct node *child;
    struct node *next;
    char *string;
} node;

int parse_scene(char* path);
void parse_node(FILE* file, char* line, int depth);

#endif //SCENE_H