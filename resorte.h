#ifndef RESORTE_H
#define RESORTE_H

#include "color.h"
#include "masa.h"

typedef struct resorte resorte_t;

//Crea un resorte (con un color) que une dos masas.
//Pre: Las masas tienen que existir.
<<<<<<< HEAD
resorte_t *crear_resorte(masa_t *masa1, masa_t *masa2, Color color);
=======
resorte_t *crear_resorte(void *masa1, void *masa2, Color color);
>>>>>>> 1f999196917812af0c82b6b1ee73828fe78df08a

resorte_t *_copiar_resorte(const resorte_t *resorte);

//Borra un resorte.
//Pre: el resorte tiene que existir.
//Post: El resorte no existe.
void _borrar_resorte(resorte_t *resorte);

#endif