#ifndef MASA_H
#define MASA_H

#include <stdbool.h>

#include "color.h"

typedef struct masa masa_t;

masa_t *crear_masa(float x, float y, float tam, Color color);

masa_t *crear_masa_fija(float x, float y, float tam, Color color);

void _borrar_masa(masa_t *masa);

bool es_fija(masa_t *masa);

void cambiar_color_masa(masa_t *masa, Color color_masa, Color color_masa_fija);

masa_t *_copiar_masa(const masa_t *masa);

float distancia_entre_masas(masa_t *m1, masa_t *m2);

#endif