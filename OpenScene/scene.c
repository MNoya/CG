#include "scene.h"

int LIGHT_COUNT = 0;

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
            
            // strdup
            char *dup = cg_malloc((strlen(line)+1) * sizeof(char));
            n->name = strcpy(dup, line);
            
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
            n->nLight = LIGHT_COUNT;
            n->lightOn = 0;
            LIGHT_COUNT++;

            printf("-- LIGHT\n");

            float light_type;
            fscanf(file, "%s\n", line);
            fscanf(file, "%f\n", &light_type);
            light_type ? printf("Positional\n") : printf("Directional\n");
            n->light_type = light_type;

            Vec3f position;
            fscanf(file, "%s\n", line);
            fscanf(file, "%f %f %f\n", &position.x, &position.y, &position.z);
            printf("Position: "); printVector(position);
            n->position = position;

            Vec3f scale;
            fscanf(file, "%s\n", line);
            fscanf(file, "%f %f %f\n", &scale.x, &scale.y, &scale.z);
            printf("\nScale: "); printVector(scale);
            n->scale = scale;

            Vec3f rotation;
            fscanf(file, "%s\n", line);
            fscanf(file, "%f %f %f\n", &rotation.x, &rotation.y, &rotation.z);
            printf("\nRotation: "); printVector(rotation);
            n->rotation = rotation;

            RGBA ambient;
            fscanf(file, "%s\n", line);
            fscanf(file, "%f %f %f %f\n", &ambient.r, &ambient.g, &ambient.b, &ambient.a);
            printf("\nAmbient: %f %f %f %f", ambient.r, ambient.g, ambient.b, ambient.a);
            n->ambient = ambient;

            RGBA diffuse;
            fscanf(file, "%s\n", line);
            fscanf(file, "%f %f %f %f\n", &diffuse.r, &diffuse.g, &diffuse.b, &diffuse.a);
            printf("\nDiffuse: %f %f %f %f", diffuse.r, diffuse.g, diffuse.b, diffuse.a); 
            n->diffuse = diffuse;

            RGBA specular;
            fscanf(file, "%s\n", line);
            fscanf(file, "%f %f %f %f\n", &specular.r, &specular.g, &specular.b, &specular.a);
            printf("\nSpecular: %f %f %f %f", specular.r, specular.g, specular.b, specular.a);
            n->specular = specular;
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

void render_node(scene_node* node, int level, Vec3f translation, Vec3f rotation, float scale,
                int use_shader, int specular, Shader shader, GLuint uniform_especular, GLuint uniform_tex)
{
    glPushMatrix();

    if ( node->type == MODEL )
    {
        //printf("Rendering %s\n",node->name);

        glTranslatef(node->position.x + translation.x, 
                     node->position.y + translation.y,
                     node->position.z + translation.z);
        glRotatef(node->rotation.x + rotation.x, 1.0f, 0.0f, 0.0f);
        glRotatef(node->rotation.y + rotation.y, 0.0f, 1.0f, 0.0f);
        glRotatef(node->rotation.z + rotation.z, 0.0f, 0.0f, 1.0f);
        glScalef(node->scale.x+scale,node->scale.y+scale,node->scale.z+scale);  

        if (use_shader)
        {
            shader_use(shader);
                glUniform1i(uniform_especular, specular);
                glUniform1i(uniform_tex, 0);
                glBindTexture(GL_TEXTURE_2D,node->texture);
                obj_render(node->object);
            shader_stop(shader);
        }
        else
        {
            glBindTexture(GL_TEXTURE_2D,node->texture);
            obj_render( node->object );
        }
    }
    else if ( node->type == LIGHT)
    {
        int lightN = node->nLight+16384; //Adjust unsigned int
        if (node->lightOn == 0 )
        {
            printf("Turning Light %d ON!\n",node->nLight);

            float ambient[] = {node->ambient.r,node->ambient.g,node->ambient.b,node->ambient.a};
            float diffuse[] = {node->diffuse.r,node->diffuse.g,node->diffuse.b,node->diffuse.a};
            float specular[] = {node->specular.r,node->specular.g,node->specular.b,node->specular.a};
            float pos[]= {node->position.x,node->position.y,node->position.z, node->light_type};
            
            glLightfv(lightN, GL_AMBIENT, ambient);
            glLightfv(lightN, GL_DIFFUSE, diffuse);
            glLightfv(lightN, GL_SPECULAR, specular);
            glLightfv(lightN, GL_POSITION, pos);

            glEnable(lightN);
            
            float color[] = {1.0f, 0.0f, 1.0f, 1.f};

            glMaterialfv(GL_FRONT, GL_DIFFUSE, color);
            glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
            glMateriali(GL_FRONT, GL_SHININESS, 32);           

            node->lightOn = 1;
        }     
    }

    int nChilds = node->nChilds;
    if (nChilds > 0)
    {
        for (int i = 0; i < nChilds; ++i)
        {
            render_node( node->childs[i], level+1, translation, rotation, scale, 
                        use_shader, specular, shader, uniform_especular, uniform_tex );
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
        cg_free(node->name);
        if (node->texture)
            glDeleteTextures(1, &node->texture);
    }
        
    else if (node-> type == LIGHT)
        printf("   There was a light at this node, nothing to free here\n");

    cg_free(node);
    printf("Finished scene_free call\n");
}