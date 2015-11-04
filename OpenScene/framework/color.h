#ifndef COLOR_H
#define COLOR_H

#ifndef APPLEMAC
#include <SDL/SDL.h>
#else
#include <SDL.h>
#endif //APPLEMAC

typedef unsigned char byte;

typedef struct
{
	byte r, g, b;
} Color;

/** 
 * Crear un color con los valores r, g, b. 
 * r,g,b deben pertenecer al rango 0-255.
 **/
Color cg_color_new(byte r, byte g, byte b);

#endif //COLOR_H

