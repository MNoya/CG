#ifndef OBJ_H
#define OBJ_H

#include "framework/canvas.h"
#include "framework/mm.h"
#include "GL/gl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct{
	float x;
	float y;
}Vec2f;

typedef struct {
    int x;
    int y;
    int z;
}Vec3i;

typedef struct {
    int nVertices;
    int nCaras;
    int nNormales;
    int nTexturas;
}ObjData;

typedef struct {
    float x;
    float y;
    float z;
} Vec3f;

typedef struct {
    float r;
    float g;
    float b;
    float a;
} RGBA;

typedef struct {
    Vec3f* listaVertices;
    Vec3i* listaCaras;
    Vec3f* listaNormales;
    Vec3i* listaNormalCara;
    Vec3i* listaTexturaCara;
    Vec2f* listaTexturas;
    int nVertices;
    int nCaras;
    int nNormales;
    int nTexturas;
} Obj;

Obj* obj_load(char* path);
void obj_render(Obj* unObj);
void obj_free(Obj* o);

ObjData* CountLines(char* path);
int GetVertice(char* verticeANormal);
int GetNormal(char* verticeANormal);
int GetTexture(char* verticeANormal);
void DrawTriangle(Obj* unObj, Vec3f v1, Vec3f v2, Vec3f v3, Vec3i normalcara, Vec3i texturacara);
void printVector(Vec3f v);
void printVectorIntegers(Vec3i v);

#endif //OBJ_H