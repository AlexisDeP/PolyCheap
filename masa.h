#ifndef MASA_H
#define MASA_H

#include "color.h"

typedef struct masa masa_t;

masa_t *crear_masa(int x, int y, int tam, Color color);

masa_t *crear_masa_fija(int x, int y, int tam, Color color);

void _borrar_masa(masa_t *masa);

bool es_fija(masa_t *masa);

void cambiar_color_masa(masa_t *masa, Color color_masa, Color color_masa_fija);

float distancia_entre_masas(masa_t *m1, masa_t *m2);

#endif