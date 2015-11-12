#include "GL/gl.h"
#include "framework/canvas.h"
#include "framework/mm.h"
#include "obj.h"

Obj* obj_load(char* path)
{
    const char space[2] = " ";
    ObjData* contVec = CountLines(path);
    int nVertices = contVec->nVertices;
    int nCaras = contVec->nCaras;
    int nNormales = contVec->nNormales;
    int nTexturas = contVec->nTexturas;

    cg_free(contVec); //Necessary?

    Vec3f* listaVertices = (Vec3f*) cg_malloc(sizeof(Vec3f) * nVertices);
    
    Vec3i* listaCaras = (Vec3i*) cg_malloc(sizeof(Vec3i) * nCaras);
    Vec3i* listaNormalCara = (Vec3i*) cg_malloc(sizeof(Vec3i) * nCaras);
    Vec3i* listaTexturaCara = (Vec3i*) cg_malloc(sizeof(Vec3i)* nCaras);    
    Vec3f* listaNormales = (Vec3f*) cg_malloc(sizeof(Vec3f) * nNormales);
    Vec2f* listaTexturas = (Vec2f*) cg_malloc(sizeof(Vec2f)* nTexturas);

    Obj* o = (Obj*) cg_malloc(sizeof(Obj));
    o->nVertices = 0;
    o->nCaras = 0;
    o->nNormales = 0;
    o->nTexturas = 0;

    FILE *file = fopen ( path, "r" );
    if (file)
    {
        char line [ 128 ];
        while (fgets ( line, sizeof line, file ) != NULL ) /* read a line */
        {
            //printf("\nParseando linea %s", line);
            if( strcmp(line,"")!=0 )
            {
                char *token = strtok(line, space);
                //printf("\nAnalizando token: %s ",token);
                if (strcmp(token,"v")==0)
                {
                    token = strtok(NULL, space);
                    float Vertex[3];
                    int i = 0;
                    while( token != NULL ) 
                    {
                        Vertex[i] = atof(token);
                        i++;
                        token = strtok(NULL, space);
                    }
                  
                    Vec3f v = {Vertex[0],Vertex[1],Vertex[2]};
                    listaVertices[o->nVertices] = v;
                    //printf("\nVertice %d: ",o->nVertices);
                    //printVector(v); 
                    o->nVertices++;
                }

                else if (strcmp(token,"f")==0)
                {   
                    //printf("\nLinea de Cara %d: %s\n",o->nCaras,token);

                    token = strtok(NULL, space);

                    int j = 0;
                    char* stringFace[3];

                    /*char ch;
                    int vertexIndex[3], uvIndex[3], normalIndex[3];
                    int matches = fscanf(file, "%c %d/%d/%d %d/%d/%d %d/%d/%d\n", &ch, &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
                    if (matches != 10){
                        printf("File can't be read by our simple parser, got only %d matches\n",matches);
                    }*/

                    while( token != NULL ) 
                    {
                        stringFace[j] = token;
                        j++;
                        token = strtok(NULL, space);
                    }

                    int vertice1 = GetVertice(stringFace[0]);
                    int normal1 = GetNormal(stringFace[0]);
                    int text1 = GetTexture(stringFace[0]);

                    int vertice2 = GetVertice(stringFace[1]);
                    int normal2 = GetNormal(stringFace[1]);
                    int text2 = GetTexture(stringFace[1]);

                    int vertice3 = GetVertice(stringFace[2]);
                    int normal3 = GetNormal(stringFace[2]);
                    int text3 = GetTexture(stringFace[2]);



                    Vec3i f = {vertice1,vertice2,vertice3};
                    Vec3i n = {normal1,normal2,normal3};
                    Vec3i t = {text1,text2,text3};
                    
                    /*
                    printf("Vertices de %d: ",o->nCaras);
                    printVectorIntegers(f);
                    printf("Normales de %d: ",o->nCaras);
                    printVectorIntegers(n);
                    printf("Texturas de %d: ",o->nCaras);
                    printVectorIntegers(t);
                    */
                    
                    listaCaras[o->nCaras] = f;
                    listaNormalCara[o->nCaras]=n;
                    listaTexturaCara[o->nCaras]=t;

                    o->nCaras++;
                }
                else if (strcmp(token,"vn")==0)
                {
                    float normal[3];
                    token = strtok(NULL, space);
                    int k = 0;
                    while( token != NULL ) 
                    {
                        normal[k] = atof(token);
                        k++;
                        token = strtok(NULL, space);
                    }
                    Vec3f vn = {normal[0],normal[1],normal[2]};
                    listaNormales[o->nNormales] = vn;  
                    o->nNormales++; 
                }
                else if (strcmp(token,"vt")==0)
                {
                    float textura[2];
                    token = strtok(NULL, space);
                    int k = 0;
                    while( token != NULL ) 
                    {
                        textura[k] = atof(token);
                        k++;
                        token = strtok(NULL, space);
                    }

                    Vec2f vt = {textura[0],textura[1]};
                    listaTexturas[o->nTexturas] = vt;  
                    o->nTexturas++; 
                }
            }
        }
        fclose(file);
    }
    o->listaVertices = listaVertices;
    o->listaCaras = listaCaras;
    o->listaNormales = listaNormales;
    o->listaNormalCara = listaNormalCara;
    o->listaTexturas = listaTexturas;
    o->listaTexturaCara = listaTexturaCara;

    return o;
}

void obj_render(Obj* o)
{
    int nCaras = o->nCaras;
    //printf("Drawing OBJ of %d faces\n", nCaras);

    for (int i = 0; i < nCaras; ++i)
    {
        int vx = o->listaCaras[i].x;
        int vy = o->listaCaras[i].y;
        int vz = o->listaCaras[i].z;

        DrawTriangle(o,
            o->listaVertices[vx],
            o->listaVertices[vy],
            o->listaVertices[vz],
            o->listaNormalCara[i],
            o->listaTexturaCara[i]);
    }
}

void obj_free(Obj* o)
{
    printf("Freeing Object\n");
    int nVertices = o->nVertices;
    int nCaras = o->nCaras;
    int nNormales = o->nNormales;

    // Free struct
    printf("Free listaVertices: %p\n",o->listaVertices);
    cg_free(o->listaVertices);

    printf("Free listaCaras: %p\n",o->listaCaras);
    cg_free(o->listaCaras);
    
    printf("Free listaNormales: %p\n",o->listaNormales);
    cg_free(o->listaNormales);

    printf("Free listaNormalCara: %p\n",o->listaNormalCara);
    cg_free(o->listaNormalCara); 

    printf("Free listaTexturaCara: %p\n",o->listaTexturaCara);
    cg_free(o->listaTexturaCara);

    printf("Free listaTexturas: %p\n",o->listaTexturas);
    cg_free(o->listaTexturas); 

    printf("Free obj: %p\n",o);
    cg_free(o);
}

//-----------------------------------

ObjData* CountLines(char* path)
{
	FILE *file = fopen ( path, "r" );
	ObjData* result = (ObjData*) cg_malloc(sizeof(ObjData));
    const char space[2] = " ";
    if (file)
    {
    	int vectores=0;
    	int caras=0;
        int normales=0;
        int texturas=0;
        char line [ 128 ];
        while ( fgets ( line, sizeof line, file ) != NULL ) 
        {
            if( strcmp(line,"")!=0 )
            {
                char *token = strtok(line, space);

                if (strcmp(token,"v")==0)
                {
                   vectores++;
                }
                else if (strcmp(token,"f")==0)
                {
                 	caras++;
                }
                else if (strcmp(token,"vn")==0)
                {
                    normales++;
                }
                else if (strcmp(token,"vt")==0)
                {
                    texturas++;
                }
            }
        }
        result->nVertices=vectores;
        result->nCaras=caras;
        result->nNormales=normales;
        result->nTexturas=texturas;
        fclose(file);

        //printf("%d %d %d %d",vectores,caras,normales,texturas);
    }
  	return result;
}

int GetVertice(char* str)
{
    char *token1 = cg_malloc(sizeof(char) * 254);
    token1 = strcpy(token1, str);
    token1 = strtok(token1, "/");
    int res = atoi(token1)-1;
    cg_free(token1);

    //printf("\nGetVertice %s, returns: %d",str,res);
    return res;
}

int GetNormal(char* str)
{
    char *token2 = cg_malloc(sizeof(char) * 254);
    token2 = strcpy(token2, str);
    token2 = strtok(token2, "/");
    token2 = strtok(NULL, "/");
    int res = atoi(token2)-1;
    cg_free(token2);

    //printf("\nGetNormal %s, returns: %d",str,res);
    return res;
}

int GetTexture(char* str)
{
    char *token3 = cg_malloc(sizeof(char) * 254);
    token3 = strcpy(token3, str);
    token3 = strtok(token3, "/");
    token3 = strtok(NULL, "/");
    token3 = strtok(NULL, "/");
    int res = atoi(token3)-1;
    cg_free(token3);

    //printf("\nGetTexture %s, returns: %d",str,res);
    return res;
}

void DrawTriangle(Obj* o, Vec3f v1, Vec3f v2, Vec3f v3, Vec3i normalcara, Vec3i texturacara)
{   

    /*printf("DrawTriangle between (%2f,%2f,%2f) (%2f,%2f,%2f) (%2f,%2f,%2f)\n",
        v1.x, v1.y, v1.z,
        v2.x, v2.y, v2.z,
        v3.x, v3.y, v3.z);*/

    //glColor3f(color->x,color->y,color->z);
    glNormal3f(o->listaNormales[normalcara.x].x, o->listaNormales[normalcara.x].y, o->listaNormales[normalcara.x].z);
    glTexCoord2f(o->listaTexturas[texturacara.x].x, o->listaTexturas[texturacara.x].y);
    glVertex3f(v1.x, v1.y, v1.z);
    
    //glColor3f(color->x,color->y,color->z);
    glNormal3f(o->listaNormales[normalcara.y].x, o->listaNormales[normalcara.y].y, o->listaNormales[normalcara.y].z);
    glTexCoord2f(o->listaTexturas[texturacara.y].x, o->listaTexturas[texturacara.y].y);
    glVertex3f(v2.x, v2.y, v2.z);

    //glColor3f(color->x,color->y,color->z);
    glNormal3f(o->listaNormales[normalcara.z].x, o->listaNormales[normalcara.z].y, o->listaNormales[normalcara.z].z);
    glTexCoord2f(o->listaTexturas[texturacara.z].x, o->listaTexturas[texturacara.z].y);
    glVertex3f(v3.x, v3.y, v3.z);
}

void printVector(Vec3f v)
{
    printf("(%2f,%2f,%2f)",v.x,v.y,v.z);
}

void printVectorIntegers(Vec3i v)
{
    printf("(%d,%d,%d)\n",v.x,v.y,v.z);
}