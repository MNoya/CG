#include "framework/canvas.h"
#include "framework/mm.h"
#include "GL/gl.h" //open gl magic
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct{
	int x;
	int y;
}Vec2;

typedef struct {
    int x;
    int y;
    int z;
}Vec3i;

typedef struct {
    float x;
    float y;
    float z;
} Vec3f;

typedef struct {
    Vec3f* listaVertices;
    Vec3i* listaCaras;
    Vec3f* listaNormales;
    Vec3i* listaNormalCara;
    int nVertices;
    int nCaras;
    int nNormales;
} Obj;

Obj* obj_load(char* path);
void obj_render(Obj* unObj);
void obj_free(Obj* o);

Vec3i* CountLines(char* path);
int GetVertice(char* verticeANormal);
int GetNormal(char* verticeANormal);
void DrawTriangle(Obj* unObj, Vec3f v1, Vec3f v2, Vec3f v3, Vec3i normalcara);
void printVector(Vec3f v);
void printVectorIntegers(Vec3i v);