#include "obj.h"

Obj* obj_load(char* path)
{
    const char space[2] = " ";
    ObjData* contVec = CountLines(path);
    int nVertices = contVec->nVertices;
    int nCaras = contVec->nCaras;
    int nNormales = contVec->nNormales;
    int nTexturas = contVec->nTexturas;

    printf("\nParsing %s...",path);

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
    if( file == NULL ){
        printf("File %s error!\n",path);
        return 0;
    }

    int res;
    do
    {
        char line [ 128 ];
        res = fscanf(file, "%s", line);

        if (strcmp(line,"v")==0)
        {
            Vec3f vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
          
            listaVertices[o->nVertices] = vertex;
            //printf("\nVertice %d: ",o->nVertices);
            //printVector(vertex); 
            o->nVertices++;
        }

        else if (strcmp(line,"f")==0)
        {   
            char c;
            int vertice1, vertice2, vertice3;
            int normal1, normal2, normal3;
            int text1, text2, text3;

            int matches = fscanf(file, "%c %d/%d/%d %d/%d/%d %d/%d/%d\n", &c, &vertice1, &normal1, &text1, &vertice2, &normal2, &text2, &vertice3, &normal3, &text3);
            
            //printf("%d/%d/%d %d/%d/%d %d/%d/%d\n", vertice1, normal1, text1, vertice2, normal2, text2, vertice3, normal3, text3);

            Vec3i f = {vertice1-1,vertice2-1,vertice3-1};
            Vec3i n = {normal1-1,normal2-1,normal3-1};
            Vec3i t = {text1-1,text2-1,text3-1};
            
            listaCaras[o->nCaras] = f;
            listaNormalCara[o->nCaras]=n;
            listaTexturaCara[o->nCaras]=t;

            o->nCaras++;
        }
        else if (strcmp(line,"vn")==0)
        {
            Vec3f normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z ); 
            //printf("\nNormal %d: ",o->nNormales);
            //printVector(normal); 
            listaNormales[o->nNormales] = normal;  
            o->nNormales++; 
        }
        else if (strcmp(line,"vt")==0)
        {
            Vec2f vt;
            fscanf(file, "%f %f\n", &vt.x, &vt.y );  

            listaTexturas[o->nTexturas] = vt;  
            o->nTexturas++; 
        }
    } while (res != EOF);

    fclose(file);

    o->listaVertices = listaVertices;
    o->listaCaras = listaCaras;
    o->listaNormales = listaNormales;
    o->listaNormalCara = listaNormalCara;
    o->listaTexturas = listaTexturas;
    o->listaTexturaCara = listaTexturaCara;

    printf("\nCaras: %d\nVertices: %d\nNormales: %d\nTexturas: %d",o->nCaras,o->nVertices,o->nNormales,o->nTexturas);

    cg_free(contVec);

    return o;
}

void obj_render(Obj* o)
{
    glBegin(GL_TRIANGLES);
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
    glEnd();
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
    if( file == NULL ){
        printf("File %s error!\n",path);
        return 0;
    }

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