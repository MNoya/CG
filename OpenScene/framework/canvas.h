#ifndef CANVAS_H
#define CANVAS_H

#ifndef APPLEMAC
#include <SDL/SDL.h>
#else
#include <SDL.h>
#endif //APPLEMAC

#include "color.h"

#include <assert.h>

void cg_init(int, int, char*);
void cg_repaint(void);
void cg_close(void);

#endif //CANVAS_H
