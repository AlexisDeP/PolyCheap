

#include <SDL2/SDL.h>

#include "masa.h"
#include "color.h"

typedef struct resorte resorte_t;

typedef enum color Color;

resorte_t *crear_resorte(masa_t *m1, masa_t *m2, Color color);

void _borrar_resorte(resorte_t *resorte);

void planear_resorte(masa_t *masa, int x, int y, Color color_construccion, SDL_Renderer *renderer);