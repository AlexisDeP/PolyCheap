#ifndef MASA_H
#define MASA_H

#include <stdbool.h>

#include "color.h"

typedef struct masa masa_t;

masa_t *crear_masa(float x, float y, float tam, Color color);

masa_t *crear_masa_fija(float x, float y, float tam, Color color);

void _borrar_masa(void *masa);

bool es_fija(masa_t *masa);

void cambiar_color_masa(masa_t *masa, Color color_masa, Color color_masa_fija);

void cambiar_id_masa(masa_t *masa, size_t id);

float obtener_x_masa(masa_t *masa);

float obtener_y_masa(masa_t *masa);

float obtener_tam_masa(masa_t *masa);

size_t obtener_id_masa(masa_t *masa);

Color obtener_color_masa(masa_t *masa);

bool obtener_es_fijo_masa(masa_t *masa);

float obtener_masa_masa(masa_t *masa);

void cambiar_x_masa(masa_t *masa, float x);

void cambiar_y_masa(masa_t *masa, float y);

void cambiar_es_fijo_masa(masa_t *masa, bool es_fijo);

void cambiar_masa_masa(masa_t *masa, float m);

masa_t *_copiar_masa(const masa_t *masa);

float distancia_entre_masas(masa_t *m1, masa_t *m2);

#endif