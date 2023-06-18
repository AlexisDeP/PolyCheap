#include <SDL2/SDL.h>

#include "color.h"

void _color_render(SDL_Renderer *renderer, Color color, uint8_t opacidad){

    uint8_t r = (color >> 16) & 0xFF;
    uint8_t g = (color >> 8 ) & 0xFF;
    uint8_t b = color & 0xFF;

    SDL_SetRenderDrawColor(renderer, r, g, b, opacidad);
}
