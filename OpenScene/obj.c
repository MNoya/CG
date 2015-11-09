#include "GL/gl.h"
#include "framework/canvas.h"
#include "framework/mm.h"
#include "obj.h"

Obj* obj_load(char* path)
{
    const char space[2] = " ";
    Vec3i* contVec = CountLines(path);
    int vecX = contVec->x;//cant vertices
    int vecY = contVec->y;//cant caras
    int vecZ = contVec->z;//cant normales
    cg_free(contVec); //Necessary?

    Vec3f* listaVertices = (Vec3f*) cg_malloc(sizeof(Vec3f) * vecX);
    Vec3i* listaCaras = (Vec3i*) cg_malloc(sizeof(Vec3i) * vecY);
    Vec3f* listaNormales = (Vec3f*) cg_malloc(sizeof(Vec3f) * vecZ);
    Vec3i* listaNormalCara = (Vec3i*) cg_malloc(sizeof(Vec3i)*vecY);

    Obj* o = cg_malloc(sizeof(Obj));
    o->nVertices = 0;
    o->nCaras = 0;
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
                    o->nVertices++;
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
                }

                else if (strcmp(token,"f")==0)
                {   
                    o->nCaras++;
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
                    //int tex1 = GetTexture(stringsVerticeNormal[0]);

                    int vertice2 = GetVertice(stringsVerticeNormal[1]);
                    int normal2 = GetNormal(stringsVerticeNormal[1]);
                    //int tex2 = GetTexture(stringsVerticeNormal[1]);

                    int vertice3 = GetVertice(stringsVerticeNormal[2]);
                    int normal3 = GetNormal(stringsVerticeNormal[2]);
                    //int tex3 = GetTexture(stringsVerticeNormal[2]);

                    //printf("\nVertices: %i %i %i",vertice1,vertice2,vertice3);
                    //printf("\nNormales: %i %i %i",normal1,normal2,normal3);

                    Vec3i f = {vertice1,vertice2,vertice3};
                    Vec3i n = {normal1,normal2,normal3};
                    
                    /*
                    printf("Vertices de %d: ",o->nCaras);
                    printVectorIntegers(f);
                    printf("Normales de %d: ",o->nCaras);
                    printVectorIntegers(n);
                    */
                    
                    listaCaras[o->nCaras] = f;
                    listaNormalCara[o->nCaras]=n;
                }
                else if (strcmp(token,"vn")==0)
                {
                    o->nNormales++;
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

void obj_render(Obj* unObj)
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
            unObj->listaNormalCara[i]);
    }
}

void obj_free(Obj* o)
{
    printf("Freeing Object\n");
    int nVertices = o->nVertices;
    int nCaras = o->nCaras;
    int nNormales = o->nNormales;

    // Free vertex array
    /*printf("Freeing %d elements from listaVertices\n",nVertices);
    for (int i = 1; i <= nVertices; ++i)
    {
        printf("Freeing listaVertices[%d]\n", i);
        cg_free(&o->listaVertices);
    }
    
    // Free face array
    printf("Freeing %d elements from listaCaras\n",nCaras);
    for (int i = 1; i <= nCaras; ++i)
    {
        printf("Freeing listaCaras[%d] and listaNormalCara[%d]\n", i,i);
        cg_free(&o->listaCaras[i]);
        cg_free(&o->listaNormalCara[i]);
    }

    // Free normal array
    printf("Freeing %d elements from listaNormales\n",nNormales);
    for (int i = 1; i < nNormales; ++i)
    {
        printf("Freeing listaNormales[%d]\n", i);
        cg_free(&o->listaNormales[i]);
    }*/

    // Free struct
    printf("Free listaVertices: %p\n",o->listaVertices);
    cg_free(o->listaVertices);

    printf("Free listaCaras: %p\n",o->listaCaras);
    cg_free(o->listaCaras);
    
    printf("Free listaNormales: %p\n",o->listaNormales);
    cg_free(o->listaNormales);

    printf("Free listaNormalCara: %p\n",o->listaNormalCara);
    cg_free(o->listaNormalCara);  

    printf("Free obj: %p\n",o);
    cg_free(o);
}

//-----------------------------------

Vec3i* CountLines(char* path)
{
	FILE *file = fopen ( path, "r" );
	Vec3i* result = cg_malloc(sizeof(Vec3i));
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

int GetVertice(char* s)
{
    char *token1 = cg_malloc(sizeof(char) * 254);
    token1 = strcpy(token1, s);
    token1 = strtok(token1, "/");
    int res = atoi(token1);
    cg_free(token1);

    //printf("\nGetVertice %s, returns: %d",s,res);
    return res;
}

int GetNormal(char* s)
{
    char *token2 = cg_malloc(sizeof(char) * 254);
    token2 = strcpy(token2, s);
    token2 = strtok(token2, "/");
    token2 = strtok(NULL, "/");
    int res = atoi(token2);
    cg_free(token2);

    //printf("\nGetNormal %s, returns: %d",s,res);
    return res;
}

int GetTexture(char* s)
{
    char *token3 = cg_malloc(sizeof(char) * 254);
    token3 = strcpy(token3, s);
    token3 = strtok(token3, "/");
    token3 = strtok(NULL, "/");
    token3 = strtok(NULL, "/");
    int res = atoi(token3);
    cg_free(token3);

    printf("\nGetTexture %s Returns: %d",s,res);
    return res;
}

void DrawTriangle(Obj* o, Vec3f v1, Vec3f v2, Vec3f v3, Vec3i normalcara)
{       
    //glColor3f(color->x,color->y,color->z);
    glNormal3f(o->listaNormales[normalcara.x].x, o->listaNormales[normalcara.x].y, o->listaNormales[normalcara.x].z);
    glVertex3f(v1.x, v1.y, v1.z);
    
    //glColor3f(color->x,color->y,color->z);
    glNormal3f(o->listaNormales[normalcara.y].x, o->listaNormales[normalcara.y].y, o->listaNormales[normalcara.y].z);
    glVertex3f(v2.x, v2.y, v2.z);

    //glColor3f(color->x,color->y,color->z);
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