#include "scene.h"

scene_node* parse_scene(char* path)
{
    printf("Parsing Scene\n");
    FILE *file = fopen ( path, "r" );
    if( file == NULL ){
        printf("File %s error!\n",path);
        return 0;
    }

    int depth = 0;
    scene_node* scene = (scene_node*) cg_malloc(sizeof(scene_node));
    scene->type = ROOT;
    scene->nChilds = 0;

    char line [ 128 ];
    int res = fscanf(file, "%s", line);
    do
    {
        if (strcmp(line,"node")==0)
        {
            printf("PARSING NODE [%d]\n",scene->nChilds);
            scene_node* node = parse_node(file, line, depth);
            printf("PARSED NODE AT %p ",node);
            scene->childs[scene->nChilds] = node;
            printf("SCENE.CHILDS[%d] = %p\n",scene->nChilds);
            scene->nChilds++;
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

    return scene;
}

scene_node* parse_node(FILE* file, char* line, int depth)
{
    depth++;
    printf("-- NODE DEPTH %d --\n", depth);
    fscanf(file, "%s", line);

    scene_node* n = (scene_node*) cg_malloc(sizeof(scene_node));

    int currentChild = 0;
    while(strcmp(line,"end")!=0)
    {
        printf("Line: %s\n",line);
        if (strcmp(line,"model")==0)
        {   
            n->type = MODEL;

            fscanf(file, "%s\n", line);
            printf("-- MODEL: %s\n", line);
            Obj* model = obj_load(line);
            n->object = model;
            printf("%s",line);
            n->name = strdup(line);
            
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
            n->type = LIGHT;

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
           n->childs[currentChild] = parse_node(file, line, depth);
           currentChild++;
        }

        fscanf(file, "%s\n", line);
    }

    printf("END\n");

    n->nChilds = currentChild;
    return n;
}

void render_node(scene_node* node)
{
    glPushMatrix();

    if ( node->type == MODEL )
    {
        //printf("Rendering %s\n",node->name);

        glTranslatef(node->position.x, node->position.y, node->position.z);
        glRotatef(node->rotation.x, 1.0f, 0.0f, 0.0f);
        glRotatef(node->rotation.y, 0.0f, 1.0f, 0.0f);
        glRotatef(node->rotation.z, 0.0f, 0.0f, 1.0f);
        glScalef(node->scale.x,node->scale.y,node->scale.z);     
        glBindTexture(GL_TEXTURE_2D,node->texture);
        obj_render( node->object );
    }

    int nChilds = node->nChilds;
    if (nChilds > 0)
    {
        for (int i = 0; i < nChilds; ++i)
        {
            render_node( node->childs[i] );
        }
    }
     
    glPopMatrix();
}

void scene_free(scene_node* node)
{
    int nChilds = node->nChilds;
    if (nChilds > 0)
    {
        printf("There are %d childs to free at %p\n",nChilds,node);
        for (int i = 0; i < nChilds; ++i)
        {
            printf(" Scene Freeing child %d\n",i);
            scene_free(node->childs[i]);
        }  
    }
   
    if (node->type == MODEL)
    {
        printf("  Freeing object %s at %p\n",node->name,node->object);
        obj_free(node->object);
        if (node->texture)
            glDeleteTextures(1, &node->texture);
    }
        
    else if (node-> type == LIGHT)
        printf("   There was a light at this node, nothing to free here\n");

    cg_free(node);
    printf("Finished scene_free call\n");
}