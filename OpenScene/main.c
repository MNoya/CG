#include "framework/canvas.h"
#include "framework/mm.h"

#ifdef WIN32
#include "glew.h"
#else
#include <GL/gl.h>
#endif

#include "obj.h"
#include "texture.h"
#include "shader.h"
#include "scene.h"

void toggleZBuff(char bEnabled);
void toggleWireframe(char bEnabled);
void toggleBackfaceCulling(char bEnabled);
void toggleLights(char bEnabled);
void tryCamera(int nodeID, scene_node* s);
void resetValues();

// Scene values
float pitch = 0.0f;
float yaw = 0.0f;
float roll = 0.0f;
float scale = 0.0f;
float ang_vel = 0.1f;
float posX = 0;
float posY = 0;
float posZ = 0;
int cw = 600;
int ch = 600;

int main(int argc, char* argv[])
{


    cg_init(cw, ch, NULL);

#ifdef WIN32
    freopen( "CON", "w", stdout );
    freopen( "CON", "w", stderr );

    GLenum err= glewInit();
    if(err!=GLEW_OK)
    {
        //problem: glewInit failed, something is seriously wrong
        printf("GLEW Error: %s\n", glewGetErrorString(err));
        return -1;
    }
#endif
    printf("GL Version: %s\n", glGetString(GL_VERSION));
    printf("GLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
    glClearColor(0,0,0,0.1); //Background color
    
    // Actualizar la pantalla:
    glMatrixMode(GL_MODELVIEW); //Applies subsequent matrix operations to the modelview matrix stack.
    glLoadIdentity();

    glMatrixMode(GL_PROJECTION); //Applies subsequent matrix operations to the projection matrix stack.
    glViewport(0,0, cw, ch);
    glFrustum(-1,1,-1,1,1,1000);

    char done = 0;
    char wireframe = 0;
    char bfc = 0;
    char zbuff = 1;
    glEnable(GL_DEPTH_TEST); // Z-Buffer
    char use_shader = 0;
    char specular = 0;
    glEnable(GL_LIGHTING); // Lights on by default
    char light = 1;

    unsigned char key_pressed[1024];
    memset(key_pressed, 0, 1024);

    printf("Loading shaders\n");
    Shader gouraud = shader_new("shaders/gouraud_vp.glsl", "shaders/gouraud_fp.glsl");
    GLint uniform_especular = shader_get_unif_loc(gouraud, "especular");
    GLint uniform_tex = shader_get_unif_loc(gouraud, "tex");

    // Scene chooser
    printf("Choose a scene: ");
    int scene_chosen = 0;
    char* scene_path;
    scene_node* Background = parse_scene("Scenes/background.txt");
    while (!scene_chosen)
    {
        SDL_Event event_choice;
        while(SDL_PollEvent(&event_choice))
        {
            switch (event_choice.type)
            {
                case SDL_KEYDOWN:
                    key_pressed[event_choice.key.keysym.sym] = 1;
                    int key = event_choice.key.keysym.sym;
                    if (key == SDLK_1) {scene_chosen=1; break; }
                    else if (key == SDLK_2) {scene_chosen=2; break; }
                    else if (key == SDLK_3) {scene_chosen=3; break; }
                    else if (key != SDLK_ESCAPE) break;
                case SDL_QUIT : scene_chosen = 1; done = 1; break;
                case SDL_KEYUP: key_pressed[event_choice.key.keysym.sym] = 0;
            }
        }

        // Draw background scene
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glEnable(GL_TEXTURE_2D);

        Vec3f translation = {0, 0, 0}; Vec3f rotation = {0, 0, 0};
        render_node(Background, 0, translation, rotation, scale, use_shader, specular, gouraud, uniform_especular, uniform_tex);
        
        cg_repaint();
    }
    //----------------------------------------------//
    scene_free(Background);

    switch(scene_chosen)
    {
        case 1: scene_path = "Scenes/scene1.txt"; break;
        case 2: scene_path = "Scenes/scene2.txt"; break;
        //case 3: scene_path = "Scenes/background.txt"; break;
    }
    printf("%d\n",scene_chosen);

    printf("Loading scene graph\n");
    scene_node* Scene = parse_scene(scene_path);

    printf("Done, drawing...\n");   

    // Camera Options: 1-9 nodes, 0 for scene root
    int camera_node = 0;

    while (!done)
    {
        /* Process Keypresses
            p: shader
            L: specular
            n: z-buffer
            m: wireframe
            b: b-culling
            t: lights
        */
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_KEYDOWN:
                    key_pressed[event.key.keysym.sym] = 1;
                    int key = event.key.keysym.sym;
                    if (key==SDLK_p)      { use_shader = !use_shader; printf("Using Shader: %d\n",use_shader); break; }
                    else if (key==SDLK_l) { specular = !specular; printf("Specular: %d\n",specular); break; }
                    else if (key==SDLK_z) { zbuff = !zbuff; printf("Z-Buffer: %d\n",zbuff); toggleZBuff(zbuff); break; }
                    else if (key==SDLK_m) { wireframe = !wireframe; printf("Wireframe: %d\n", wireframe); toggleWireframe(wireframe); break; }
                    else if (key==SDLK_b) { bfc = !bfc; printf("Backface Culling: %d\n",bfc); toggleBackfaceCulling(bfc); break; }
                    else if (key==SDLK_t) { light = !light; printf("Lights: %d\n",light); toggleLights(light); break; }
                    else if (key==SDLK_0) { camera_node = 0; tryCamera(camera_node, Scene); break; }
                    else if (key==SDLK_1) { camera_node = 1; tryCamera(camera_node, Scene); break; }
                    else if (key==SDLK_2) { camera_node = 2; tryCamera(camera_node, Scene); break; }
                    else if (key==SDLK_3) { camera_node = 3; tryCamera(camera_node, Scene); break; }
                    else if (key==SDLK_4) { camera_node = 4; tryCamera(camera_node, Scene); break; }
                    else if (key==SDLK_5) { camera_node = 5; tryCamera(camera_node, Scene); break; }
                    else if (key==SDLK_6) { camera_node = 6; tryCamera(camera_node, Scene); break; }
                    else if (key==SDLK_7) { camera_node = 7; tryCamera(camera_node, Scene); break; }
                    else if (key==SDLK_8) { camera_node = 8; tryCamera(camera_node, Scene); break; }
                    else if (key==SDLK_9) { camera_node = 9; tryCamera(camera_node, Scene); break; }
                    else if (key != SDLK_ESCAPE) break;

                case SDL_QUIT : done = 1; break;
                case SDL_KEYUP: key_pressed[event.key.keysym.sym] = 0;
            }
        }
        //----------------------------------------------//

        /* Camera control
            Rotate pitch:   UP/DOWN
            Rotate yaw:     SPACE
            Rotate roll:    LEFT/RIGHT
            Zoom In/Out:    +/-
            posX:           a/d
            posY:           w/s
            posZ:           q/e
        */

        if(key_pressed[SDLK_UP]) pitch += ang_vel;
        if(key_pressed[SDLK_DOWN]) pitch -= ang_vel;
        if(key_pressed[SDLK_SPACE]) yaw += ang_vel;
        if(key_pressed[SDLK_RIGHT]) roll += ang_vel;
        if(key_pressed[SDLK_LEFT]) roll -= ang_vel;
        if(key_pressed[SDLK_KP_PLUS]) scale += 0.01;
        if(key_pressed[SDLK_KP_MINUS]) scale -= 0.01;
        if(key_pressed[SDLK_a]) posX += 0.1;
        if(key_pressed[SDLK_d]) posX -= 0.1;
        if(key_pressed[SDLK_w]) posY += 0.1;
        if(key_pressed[SDLK_s]) posY -= 0.1;
        if(key_pressed[SDLK_q]) posZ += 0.01;
        if(key_pressed[SDLK_e]) posZ -= 0.01;

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glEnable(GL_TEXTURE_2D);

        Vec3f translation = {posX, posY, posZ};
        Vec3f rotation = {pitch, yaw, roll};
        render_node(Scene, camera_node, translation, rotation, scale, use_shader, specular, gouraud, uniform_especular, uniform_tex);

        // Print node values modified
        scene_node* node = get_node(camera_node, Scene);
        if (node != NULL)
        {
            if (translation.x != 0) printf("Node %d X %f\n",camera_node, node->position.x);
            if (translation.y != 0) printf("Node %d Y %f\n",camera_node, node->position.y);
            if (translation.z != 0) printf("Node %d Z %f\n",camera_node, node->position.z);

            if (rotation.x != 0) printf("Node %d Pitch %f\n",camera_node, node->rotation.x);
            if (rotation.y != 0) printf("Node %d Yaw %f\n",camera_node, node->rotation.y);
            if (rotation.z != 0) printf("Node %d Roll %f\n",camera_node, node->rotation.z);

            if (scale != 0) printf("Node %d scale: %f\n",camera_node, node->scale.x);
        }

        resetValues();
        cg_repaint();
    }

    printf("--------------------\n");
    shader_free(gouraud);
    scene_free(Scene);
    
    // Liberar recursos
    cg_memcheck();
    cg_close();

    return 0;
}

void toggleZBuff(char bEnabled)
{
    bEnabled ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
}

void toggleWireframe(char bEnabled)
{
    bEnabled ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void toggleBackfaceCulling(char bEnabled)
{
    if (bEnabled) { glEnable(GL_CULL_FACE); glCullFace(GL_BACK); glFrontFace(GL_CW); }
    else { glDisable(GL_CULL_FACE); }

}

void toggleLights(char bEnabled)
{
    bEnabled ? glEnable(GL_LIGHTING) : glDisable(GL_LIGHTING);
}

void tryCamera(int nodeID, scene_node* s)
{
    scene_node* node = get_node(nodeID, s);
    if (node != NULL)
    {
        if (node->type == MODEL) printf("Camera at Node %d, %s \n",nodeID,node->name);
        else if (node->type == LIGHT) printf("Camera at Node %d, Light Number %d \n",nodeID,node->nLight);
        else if (node->type == ROOT) printf("Camera at Scene Root\n");
    }
    else
    {
        printf("The scene doesn't have a Node %d\n",nodeID);
    }
}

void resetValues()
{
    pitch = 0.0f;
    yaw = 0.0f;
    roll = 0.0f;
    scale = 0.0f;
    posX = 0;
    posY = 0;
    posZ = 0;
}