#include "canvas.h"
#include "mm.h"
#include <stdlib.h>

SDL_Surface* pSurface = NULL;
int surface_locked = 0;

void cg_init(int w, int h, char* title)
{
	assert (SDL_Init(SDL_INIT_VIDEO) == 0);
	pSurface = SDL_SetVideoMode(w, h, 0, SDL_OPENGL|SDL_DOUBLEBUF);
	assert (pSurface != NULL);

	if (title)
	{
		SDL_WM_SetCaption(title, NULL);
	}
	else
	{
		SDL_WM_SetCaption("CG Framework v3.0", NULL);
	}

	atexit(cg_memcheck);
}

void cg_repaint(void)
{
	assert (pSurface != NULL);
	SDL_GL_SwapBuffers();
}

void cg_close(void)
{
	SDL_Quit();
}

