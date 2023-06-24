#ifndef RESORTE_H
#define RESORTE_H

#include "color.h"
#include "masa.h"

typedef struct resorte resorte_t;

//Crea un resorte (con un color) que une dos masas.
//Pre: Las masas tienen que existir.
resorte_t *crear_resorte(masa_t *masa1, masa_t *masa2, Color color);


//Borra un resorte.
//Pre: el resorte tiene que existir.
//Post: El resorte no existe.
void _borrar_resorte(resorte_t *resorte);

#endif