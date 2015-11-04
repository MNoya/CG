#include "framework/canvas.h"
#include "framework/mm.h"
#include "GL/gl.h"
#include "time.h"
#include "obj.h"
#include "lights.h"

int main(int argc, char* argv[])
{
#ifdef WIN32
    freopen( "CON", "w", stdout );
    freopen( "CON", "w", stderr );
#endif
    srand(time(NULL));

    int cw = 700;
    int ch = 700;
    cg_init(cw, ch, NULL);

    int done = 0;
    float pitch=0.0;
    float yaw=0.0;
    float roll=0.0;
    int left=0;
    int right=0;
    int up=0;
    int down=0;
    int fill=0;

    printf("version: %s\n", glGetString(GL_VERSION));
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glMatrixMode(GL_PROJECTION);
    glViewport(0,0,cw,ch);
    glFrustum(-1,1,-1,1,1,1000);

    loadLightning();
    
    Vec3f* veColor = (Vec3f*) cg_malloc(sizeof(Vec3f));
    float color1 = (float)rand()/(float)RAND_MAX;
    float color2 = (float)rand()/(float)RAND_MAX;
    float color3 = (float)rand()/(float)RAND_MAX;
    veColor->x=color1;
    veColor->y=color2;
    veColor->z=color3;

    char* fileName = "Models/knight_normales.obj";
    Obj* model= ParseObj(fileName);
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

    printf("\nDone parsing %s\n",fileName);

    while (!done)
    {
        if(fill){  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL); }
        else { glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); }

        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(0.0,0.0,-50.0);

        // Rotations
        glRotatef(pitch,1.0,1.0,0.0);
        glRotatef(yaw,0.0,1.0,0.0);
        glRotatef(roll,0.0,0.0,1.0);

        glBegin(GL_TRIANGLES);
            DrawObj(model, veColor);
        glEnd();

        cg_repaint();

        if(up){ pitch+=0.1; }
        if(down){ pitch-=0.1; }
        if(left){ yaw-=0.1; } 
        if(right){ yaw+=0.1; }

        //------------------------------
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                // On Key Press
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym ==SDLK_LEFT){
                        left=1;
                        right=0;
                        up=0;
                        down=0;
                    }
                    if (event.key.keysym.sym ==SDLK_RIGHT){
                        left=0;
                        right=1;
                        up=0;
                        down=0;
                    }
                    if (event.key.keysym.sym ==SDLK_UP){
                        left=0;
                        right=0;
                        up=1;
                        down=0;
                    }
                    if (event.key.keysym.sym ==SDLK_DOWN){
                        left=0;
                        right=0;
                        up=0;
                        down=1;
                    }
                    if (event.key.keysym.sym ==SDLK_SPACE){
                        left=0;
                        right=0;
                        up=0;
                        down=0;
                        fill=abs(fill-1);
                    }
                    if (event.key.keysym.sym != SDLK_ESCAPE){
                        break;
                    }

                case SDL_QUIT : done = 1;

                // On Key Release
                case SDL_KEYUP:
                    if (event.key.keysym.sym != SDLK_ESCAPE){
                        left=0;
                        right=0;
                        up=0;
                        down=0;
                        break;
                    }
            }
        }
        //------------------------------
    }

    cg_free(veColor);
    cg_close(); 
    // Ejemplo del modulo de Manejo de Memoria (MM):
   /* int* pint = (int *)cg_malloc(10*sizeof(int));
    printf("pint is a pointer: %p\n", pint);

    cg_free(pint); // olvidarse de liberar este objeto produce un mensaje
	*/

    return 0;
}