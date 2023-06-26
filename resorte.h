#ifndef RESORTE_H
#define RESORTE_H

#include "color.h"
#include "masa.h"

typedef struct resorte resorte_t;

//Crea un resorte (con un color) que une dos masas.
//Pre: Las masas tienen que existir.
resorte_t *crear_resorte(masa_t *masa1, masa_t *masa2, Color color);

masa_t *obtener_masa1_resorte(resorte_t *resorte);

masa_t *obtener_masa2_resorte(resorte_t *resorte);

Color obtener_color_resorte(resorte_t *resorte);

float obtener_k_resorte(resorte_t *resorte);

void cambiar_id_resorte(resorte_t *resorte, size_t id);

size_t obtener_id_resorte(resorte_t *resorte);

void cambiar_longitud_resorte(resorte_t *resorte, float longitud);

void cambiar_masa1_resorte(resorte_t *resorte, masa_t *masa1);

void cambiar_masa2_resorte(resorte_t *resorte, masa_t *masa2);

void cambiar_k_resorte(resorte_t *resorte, float k_resorte);

void cambiar_color_resorte(resorte_t *resorte, Color color);

float obtener_longitud_resorte(resorte_t *resorte);

//Borra un resorte.
//Pre: el resorte tiene que existir.
//Post: El resorte no existe.
void _borrar_resorte(void *resorte);

#endif