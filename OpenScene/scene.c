#include "scene.h"

int parse_scene(char* path)
{
    printf("Parsing Scene\n");
    FILE *file = fopen ( path, "r" );
    if( file == NULL ){
        printf("File %s error!\n",path);
        return 0;
    }

    char line [ 128 ];
    int res = fscanf(file, "%s", line);
    int depth = 0;
    scene_node* scene = (scene_node*) cg_malloc(sizeof(scene_node));
    do
    {
        if (strcmp(line,"node")==0)
        {
            scene_node* childs = parse_node(file, line, depth);
            scene->childs = childs;
        }
        else if (strcmp(line,"end")==0)
        {
            printf("End top level node\n");
            depth = 0;
        }

        res = fscanf(file, "%s", line);

    } while (res != EOF);

    fclose(file);

    printf("End parse!\n");

    cg_free(scene);

    return 0;
}

scene_node* parse_node(FILE* file, char* line, int depth)
{
    depth++;
    printf("-- NODE DEPTH %d --\n", depth);
    fscanf(file, "%s", line);

    scene_node* n = (scene_node*) cg_malloc(sizeof(scene_node));

    // Easier to define the childs on the file than parse it 2 times
    if (strcmp(line,"childs")==0)
    {
        int numChildren;
        fscanf(file, "%d", &numChildren);
        if (numChildren > 0)
        {
            printf("-- CHILDS: %d\n",numChildren);
            scene_node* childs = (scene_node*) cg_malloc(sizeof(scene_node) * numChildren);
            n->childs = childs;
        }
        n->nChilds = numChildren;
    }

    while (strcmp(line,"end")!=0)
    {
        int match = 0;
        if (strcmp(line,"model")==0)
        {   
            fscanf(file, "%s\n", line);
            printf("-- MODEL: %s\n", line);
            Obj* model = obj_load(line);
            n->object = model;
            
            fscanf(file, "texture %s\n", line);
            printf("Texture: %s\n",line);
            GLuint texture = texture_load(line);
            n->texture = texture;

            Vec3f position;
            fscanf(file, "position %f %f %f\n", &position.x, &position.y, &position.z);
            printf("Position: "); printVector(position);
            n->position = position;

            Vec3f scale;
            fscanf(file, "scale %f %f %f\n", &scale.x, &scale.y, &scale.z);
            printf("\nScale: "); printVector(scale);
            n->scale = scale;

            Vec3f rotation;
            fscanf(file, "rotation %f %f %f\n", &rotation.x, &rotation.y, &rotation.z);
            printf("\nRotation: "); printVector(rotation); printf("\n");
            n->rotation = rotation;
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

    return n;
}