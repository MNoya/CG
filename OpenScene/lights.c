#include "lights.h"

void loadLightning()
{
    //Habilito la iluminación del pipeline estático de OpenGL.
    glEnable(GL_LIGHTING);

    //Creo un Vec4 para representar el color (RGBA) y la Intensidad de la luz.
    //P.e.: (1,1,1,1) = Luz Blanca intensa, (0.5,0.5,0.5,1) = Luz blanca tenue, (0.5,0,0,1) = Luz roja tenue.
    float l0[] = {1.0f,1.0f,1.0f,1.0f};
    //Creo un Vec4 para representar el color (RGBA) y la intensidad de la iluminación ambiente de la luz
    float la[] = {0.10f,0.10f,0.10f,1.0f};
    //Creo un Vec4 para representar la posición de la luz. El cuarto elemento representa el tipo de luz: 1=puntual, 0=direccional
    float l0p[]= {1.0f,1.0f,1.0f,1.0f};
    //Creo un Vec4 para representar el color (RGBA) y la intensidad especular de la luz
    float ls[] = {1.0f,1.0f,1.0f,1.0f};
    //Cargo la intesidad ambiente de la Luz Nro 0 del pipline estático.
    glLightfv(GL_LIGHT0, GL_AMBIENT, la);
    //Cargo la intesidad difusa de la Luz Nro 0 del pipline estático.
    glLightfv(GL_LIGHT0, GL_DIFFUSE, l0);
    //Cargo la posición de la Luz Nro 0 del pipline estático.
    glLightfv(GL_LIGHT0, GL_POSITION, l0p);
    //Cargo la intesidad especular de la Luz Nro 0 del pipline estático.
    glLightfv(GL_LIGHT0, GL_SPECULAR, ls);
    //Prendo la Luz nro 0 del pipline estático. Hay 8 luces, representadas por las constantes GL_LIGHT0 a GL_LIGHT7
    //Por defecto está todas apagadas al inicio.
    glEnable(GL_LIGHT0);
    
    //Creo un Vec4 para representar el color difuso(RGBA) del material del objeto a dibujar.
    float cyan[] = {1.0f, 0.0f, 1.0f, 1.f};
    //Cargo el color difuso del la cara de adelante del objeto a dibujar.
    glMaterialfv(GL_FRONT, GL_DIFFUSE, cyan);
    //Cargo el color especular del la cara de adelante del objeto a dibujar.
    glMaterialfv(GL_FRONT, GL_SPECULAR, ls);
    //Cargo el coeficiente especular de la cara de adelante del objeto a dibujar.
    glMateriali(GL_FRONT, GL_SHININESS, 32);
}