#ifndef COLOR_H
#define COLOR_H

#include <SDL2/SDL.h>

typedef enum color {

    COLOR_ROJO = 0x9B4545,
    COLOR_ROJO_FUERTE = 0xFF0000,    
    COLOR_CELESTE = 0x34878F,    
    COLOR_GRIS = 0x151515,
    COLOR_BLANCO = 0xFFFFFF,
    COLOR_VIOLETA = 0x70459B,
    COLOR_VERDE = 0x709B45,
    COLOR_NARANJA = 0xB9770E, 
    COLOR_DEFAULT = 0xBBBBBB,

} Color;

void definir_color(SDL_Renderer *renderer, Color color, uint8_t opacidad);

#endif