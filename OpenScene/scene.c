#include "scene.h"

int parse_scene(char* path)
{
    printf("Parsing Scene\n");
    FILE *file = fopen ( path, "r" );
    if( file == NULL ){
        printf("File %s error!\n",path);
        return 0;
    }

    int res;
    int depth = 0;
    do
    {
        char line [ 128 ];
        res = fscanf(file, "%s", line);

        if (strcmp(line,"node")==0)
        {
            parse_node(file, line, depth);
        }
        else if (strcmp(line,"end")==0)
        {
            printf("End top level node\n");
            depth = 0;
        }

    } while (res != EOF);

    fclose(file);

    printf("End parse!\n");

    return 0;
}

void parse_node(FILE* file, char* line, int depth)
{
    depth++;
    printf("-- NODE DEPTH %d --\n", depth);
    fscanf(file, "%s", line);
    
    while (strcmp(line,"end")!=0)
    {
        if (strcmp(line,"model")==0)
        {   
            fscanf(file, "%s\n", line);
            printf("-- MODEL: %s\n", line);

            fscanf(file, "texture %s", line);
            printf("Texture: %s\n",line);

            Vec3f position;
            fscanf(file, "%s\n", line);
            fscanf(file, "%f %f %f\n", &position.x, &position.y, &position.z);
            printf("Position: "); printVector(position);

            Vec3f scale;
            fscanf(file, "%s", line);
            fscanf(file, "%f %f %f\n", &scale.x, &scale.y, &scale.z);
            printf("\nScale: "); printVector(scale);

            Vec3f rotation;
            fscanf(file, "%s", line);
            fscanf(file, "%f %f %f\n", &rotation.x, &rotation.y, &rotation.z);
            printf("\nRotation: "); printVector(rotation); printf("\n");
        }

        else if (strcmp(line,"light")==0)
        {
            printf("-- LIGHT\n");

            Vec3f position;
            fscanf(file, "%s\n", line);
            fscanf(file, "%f %f %f\n", &position.x, &position.y, &position.z);
            printf("Position: "); printVector(position);

            Vec3f scale;
            fscanf(file, "%s\n", line);
            fscanf(file, "%f %f %f\n", &scale.x, &scale.y, &scale.z);
            printf("\nScale: "); printVector(scale);

            Vec3f rotation;
            fscanf(file, "%s\n", line);
            fscanf(file, "%f %f %f\n", &rotation.x, &rotation.y, &rotation.z);
            printf("\nRotation: "); printVector(rotation);

            Vec3f color;
            fscanf(file, "%s\n", line);
            fscanf(file, "%f %f %f\n", &color.x, &color.y, &color.z);
            printf("\nColor: "); printVector(color); printf("\n");
        }

        else if (strcmp(line,"node")==0)
        {
           parse_node(file, line, depth); 
        }

        fscanf(file, "%s", line);
    }
}