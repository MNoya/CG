#include "texture.h"

GLuint texture_load(char* path)
{
    //Cargo la imagen de disco usando SDL_image
    SDL_Surface* surface = IMG_Load(path);
    if (surface==NULL) { //Si falla la carga de la imagen, despliego el mensaje de error correspondiente y termino el programa.
        printf("Error: \"%s\"\n",SDL_GetError());
        return 1;
    }

    GLuint texture = 0;
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