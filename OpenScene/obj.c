#include "GL/gl.h"
#include "framework/canvas.h"
#include "framework/mm.h"
#include "obj.h"

Vec3i* CountLines(char* path){
	FILE *file = fopen ( path, "r" );
	Vec3i* result=malloc(sizeof(Vec3i));
    const char space[2] = " ";
    if (file)
    {
    	int vectores=0;
    	int caras=0;
        int normales=0;
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
            }
        }
        result->x=vectores;
        result->y=caras;
        result->z=normales;
        fclose(file);
    }
  	return result;
}


Obj* ParseObj(char* path){
    const char space[2] = " ";
    Vec3i* contVec = CountLines(path);
    int vecX = contVec->x;//cant vertices
    int vecY = contVec->y;//cant caras
    int vecZ = contVec->z;//cant normales

    Vec3f* listaVertices = (Vec3f*) malloc(sizeof(Vec3f) * vecX);
    Vec3i* listaCaras = (Vec3i*) malloc(sizeof(Vec3i) * vecY);
    Vec3f* listaNormales = (Vec3f*) malloc(sizeof(Vec3f) * vecZ);
    Vec3i* listaNormalCara = (Vec3i*) malloc(sizeof(Vec3i)*vecY);

    Obj* o = malloc(sizeof(Obj));
    o->nVertices = 1;
    o->nCaras = 1;
    o->nNormales = 0;

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
                //printf("Analizando token: %s ",token);
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
                    //printf("Vertice %d\n: ",o->nVertices);
                    //printVector(v);
                    o->nVertices++;
                }

                else if (strcmp(token,"f")==0)
                {   
                    //printf("\nLinea de Cara %d: %s\n",o->nCaras,token);

                    token = strtok(NULL, space);

                    int j = 0;
                    char* stringsVerticeNormal[3];

                    while( token != NULL ) 
                    {
                        stringsVerticeNormal[j] = token;
                        j++;
                        token = strtok(NULL, space);
                    }

                    //printf("%s %s %s",stringsVerticeNormal[0],stringsVerticeNormal[1],stringsVerticeNormal[2]);

                    int vertice1 = GetVertice(stringsVerticeNormal[0]);
                    int normal1 = GetNormal(stringsVerticeNormal[0]);
                    int vertice2 = GetVertice(stringsVerticeNormal[1]);
                    int normal2 = GetNormal(stringsVerticeNormal[1]);
                    int vertice3 = GetVertice(stringsVerticeNormal[2]);
                    int normal3 = GetNormal(stringsVerticeNormal[2]);

                    //printf("\nVertices: %i %i %i",vertice1,vertice2,vertice3);
                    //printf("\nNormales: %i %i %i",normal1,normal2,normal3);

                    Vec3i f = {vertice1,vertice2,vertice3};
                    Vec3i n = {normal1,normal2,normal3};
                    //printf("Vertices de %d: ",o->nCaras);
                    //printVectorIntegers(f);
                    //printf("Normales de %d: ",o->nCaras);
                    //printVectorIntegers(n);
                    listaCaras[o->nCaras] = f;
                    listaNormalCara[o->nCaras]=n;
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
            }
        }
        fclose(file);
    }
    o->listaVertices = listaVertices;
    o->listaCaras = listaCaras;
    o->listaNormales = listaNormales;
    o->listaNormalCara = listaNormalCara;

    return o;
}

int GetVertice(char* verticeANormal)
{
    char *token1 = malloc(sizeof(char) * 254);
    token1 = strcpy(token1, verticeANormal);
    token1 = strtok(token1, "/");
    int res = atoi(token1);

    //printf("\nGetVertice %s, returns: %d",verticeANormal,res);
    return res;
}

int GetNormal(char* verticeANormal)
{
    char *token2 = malloc(sizeof(char) * 254);
    token2 = strcpy(token2, verticeANormal);
    token2 = strtok(token2, "/");
    token2 = strtok(NULL, "/");
    int res = atoi(token2);

    //printf("\nGetNormal  %s, returns: %d",verticeANormal,res);
    return res;
}

void DrawObj(Obj* unObj, Vec3f* color)
{
    int nCaras = unObj->nCaras;
    //printf("Drawing OBJ of %d faces\n", nCaras);

    for (int i = 1; i < nCaras; ++i)
    {
        int vx = unObj->listaCaras[i].x;
        int vy = unObj->listaCaras[i].y;
        int vz = unObj->listaCaras[i].z;
    
        //printf("Face %d: %d %d %d\n", i, vx, vy, vz);   
        //printf("DrawTriangle between %d  %d  %d\n",unObj->listaVertices[vy],unObj->listaVertices[vx], unObj->listaVertices[vz]); 

        DrawTriangle(unObj,
            unObj->listaVertices[vx],
            unObj->listaVertices[vy],
            unObj->listaVertices[vz], 
            color,
            unObj->listaNormalCara[i]);
    }
}

void DrawTriangle(Obj* o, Vec3f v1, Vec3f v2, Vec3f v3, Vec3f* color, Vec3i normalcara)
{       
    glColor3f(color->x,color->y,color->z);
    glNormal3f(o->listaNormales[normalcara.x].x, o->listaNormales[normalcara.x].y, o->listaNormales[normalcara.x].z);
    glVertex3f(v1.x, v1.y, v1.z);
    
    glColor3f(color->x,color->y,color->z);
    glNormal3f(o->listaNormales[normalcara.y].x, o->listaNormales[normalcara.y].y, o->listaNormales[normalcara.y].z);
    glVertex3f(v2.x, v2.y, v2.z);

    glColor3f(color->x,color->y,color->z);
    glNormal3f(o->listaNormales[normalcara.z].x, o->listaNormales[normalcara.z].y, o->listaNormales[normalcara.z].z);
    glVertex3f(v3.x, v3.y, v3.z);
}

void printVector(Vec3f v)
{
    printf("(%2f,%2f,%2f)\n",v.x,v.y,v.z);
}

void printVectorIntegers(Vec3i v)
{
    printf("(%d,%d,%d)\n",v.x,v.y,v.z);
}