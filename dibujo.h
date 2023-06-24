#ifndef DIBUJO_H
#define DIBUJO_H

#include "malla.h"

//Dibuja una malla en pantalla
//Pre: La malla existe
void renderizar_malla(malla_t *malla, SDL_Renderer *renderer);

//Dibuja un trazo desde la posicion de una masa a un punto (x, y)
//Pre: La masa existe
void planear_resorte(masa_t *masa, float x, float y, Color color_construccion, SDL_Renderer *renderer);

#endif