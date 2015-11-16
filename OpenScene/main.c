#include "framework/canvas.h"
#include "framework/mm.h"

#ifdef WIN32
#include "glew.h"
#else
#include <GL/gl.h>
#endif

#include "obj.h"
#include "lights.h"
#include "shader.h"

#include <SDL/SDL_image.h>

GLuint texture_load(char* path);
void enableCullFace();

int main(int argc, char* argv[])
{
    int cw = 600;
    int ch = 600;

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
    glClearColor(0,0,1,0.1); //Background color
    
    // Actualizar la pantalla:
    glMatrixMode(GL_MODELVIEW); //Applies subsequent matrix operations to the modelview matrix stack.
    glLoadIdentity();

    glMatrixMode(GL_PROJECTION); //Applies subsequent matrix operations to the projection matrix stack.
    glViewport(0,0, cw, ch);
    glFrustum(-1,1,-1,1,1,1000);

    // Scene values
    float ang = 0.0f;
    float pitch = 0.0f;
    float ang_vel = 0.1f;
    int distance = -50;
    char done = 0;
    char wireframe = 0;
    char bfc = 0;
    char zbuff = 1;
    glEnable(GL_DEPTH_TEST); // Z-Buffer
    char light = 1;
    loadLightning();

    unsigned char key_pressed[1024];
    memset(key_pressed, 0, 1024);

    char* modelName = "Models/knight_texturas.obj";
    char* textureName = "Models/knight_good.png";
    Obj* model = obj_load(modelName);
    printf("\nDone parsing %s",modelName);
    printf("\nCaras: %d\nVertices: %d\nNormales: %d\nTexturas: %d",model->nCaras,model->nVertices,model->nNormales,model->nTexturas);

    printf("\nLoading shaders");
    char use_shader = 0;
    char specular = 0;
    Shader gouraud = shader_new("shaders/gouraud_vp.glsl", "shaders/gouraud_fp.glsl");
    
    GLint uniform_especular = shader_get_unif_loc(gouraud, "especular");
    GLint uniform_tex = shader_get_unif_loc(gouraud, "tex");

    printf("\nLoading texture %s ",textureName);
    GLuint texture = texture_load(textureName);

    printf("Done, drawing...");

    while (!done)
    {
        /* Process Keypresses
            p: shader
            s: specular
            z: z-buffer
            m: wireframe
        */
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_KEYDOWN:
                    key_pressed[event.key.keysym.sym] = 1;
                    if (event.key.keysym.sym == SDLK_p) { use_shader = !use_shader; break; }
                    else if (event.key.keysym.sym == SDLK_s) { specular = !specular; break; }
                    else if (event.key.keysym.sym == SDLK_z) { zbuff = !zbuff; zbuff ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST); break; }
                    else if (event.key.keysym.sym == SDLK_m) { wireframe = !wireframe; wireframe ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); break; }
                    else if (event.key.keysym.sym == SDLK_b) { bfc = !bfc; bfc ? enableCullFace() : glDisable(GL_CULL_FACE); }
                    else if (event.key.keysym.sym != SDLK_ESCAPE) break;

                case SDL_QUIT : done = 1; break;
                case SDL_KEYUP: key_pressed[event.key.keysym.sym] = 0;
            }
        }
        //----------------------------------------------//

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(0.0,0.0,distance);
        glRotatef(pitch, 1.0f, 0.0f, 0.0f);
        glRotatef(ang, 0.0f, 1.0f, 0.0f);
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);

        if(key_pressed[SDLK_RIGHT]) ang += ang_vel;
        if(key_pressed[SDLK_LEFT]) ang -= ang_vel;
        if(key_pressed[SDLK_UP]) pitch += ang_vel;
        if(key_pressed[SDLK_DOWN]) pitch -= ang_vel;

        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        //Informo a OpenGL que para todas las operaciones a continuación utilice las texturas 2D cargadas
        glEnable(GL_TEXTURE_2D);

        if(use_shader)
        {
            shader_use(gouraud);
            glUniform1i(uniform_especular, specular);
            //Le digo al shader que el sampler2D de nombre "tex" se corresponde con GL_TEXTURE0
            //que es donde cargué mi textura.
            glUniform1i(uniform_tex, 0);
            //Luego asocio la textura con el id "texture"
            glBindTexture(GL_TEXTURE_2D,texture);
            glBegin(GL_TRIANGLES);
                obj_render(model);
            glEnd();
            shader_stop(gouraud);
        }
        else
        {
            glBindTexture(GL_TEXTURE_2D,texture);
            glBegin(GL_TRIANGLES);
                obj_render(model);
            glEnd();
        }

        cg_repaint();
    }
    obj_free(model);
    //shader_free(gouraud);
    glDeleteTextures(1,&texture);
    
    // Liberar recursos:
    cg_close();

    return 0;
}

GLuint texture_load(char* path)
{
    //Cargo la imagen de disco usando SDL_image
    SDL_Surface* surface = IMG_Load(path);
    if (surface==NULL) { //Si falla la carga de la imagen, despliego el mensaje de error correspondiente y termino el programa.
        printf("Error: \"%s\"\n",SDL_GetError());
        return 1;
    }

    GLuint texture;
    //Creo un espacio para alojar una textura en memoria de video
    glGenTextures(1,&texture);
    //Activo la textura nro 0
    glActiveTexture(GL_TEXTURE0);
    //Habilito la carga para la textura recien creada
    glBindTexture(GL_TEXTURE_2D,texture);

    //Cargo los datos de la imagen en la textura.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);
    
    //Luego de copiada la imagen a memoria de video, puedo liberarla sin problemas
    SDL_FreeSurface(surface);

    //Seteo el filtro a usar cuando se mapea la textura a una superficie mas chica (GL_LINEAR = filtro bilineal)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    //Seteo el filtro a usar cuando se mapea la textura a una superficie mas grande (GL_LINEAR = filtro bilineal)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //Seteo el comportamiento cuando encuentro coordenadas de textura fuera del rango [0,1]
    //GL_REPEAT es el comportamiento por defecto.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    return texture;
}

void enableCullFace()
{
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);
}