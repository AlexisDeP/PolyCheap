#include <SDL2/SDL.h>
#include <stdbool.h>

#include "masa.h"
#include "resorte.h"
#include "color.h"

struct masa {
    size_t id;
    int x, y, tam;           // FLOAT O INT???
    bool es_fijo;
    float masa;
    Color color;
};

struct resorte {
    size_t id;
    struct masa *masa1, *masa2;
    float longitud, k_resorte;
    Color color;
};

static resorte_t *_crear_resorte(){
    resorte_t *resorte = malloc(sizeof(resorte_t));
    if(resorte == NULL) return NULL;
    return resorte;
}

resorte_t *crear_resorte(masa_t *m1, masa_t *m2, Color color) {
    resorte_t *nuevo_resorte = _crear_resorte();
    if (nuevo_resorte == NULL) return NULL;
    
    nuevo_resorte->id = 0;  // Asigna un ID adecuado según tus necesidades
    nuevo_resorte->masa1 = m1;
    nuevo_resorte->masa2 = m2;
    nuevo_resorte->longitud = distancia_entre_masas(m1, m2);  // Implementa una función para calcular la distancia entre masas
    nuevo_resorte->k_resorte = 0;
    nuevo_resorte->color = color;  // Define el color por defecto
    
    return nuevo_resorte;
}  

void _borrar_resorte(resorte_t *resorte){
    free(resorte);
}

void planear_resorte(masa_t *masa, int x, int y, Color color_construccion, SDL_Renderer *renderer) {
    _color_render(renderer, color_construccion, 0x00);
    SDL_RenderDrawLine(renderer, masa->x, masa->y, x, y);
}