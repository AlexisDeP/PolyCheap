#include <SDL2/SDL.h>

#include "dibujo.h"
#include "masa.h"
#include "resorte.h"
#include "malla.h"
#include "lista.h"

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

struct malla {
    lista_t* resortes;
    lista_t* masas;
};

static void __dibujar_nodo(masa_t *masa, SDL_Renderer *renderer) {
    _color_render(renderer, masa->color, 0x00);
    SDL_Rect r1 = {masa->x - masa->tam/2, masa->y - masa->tam/2, masa->tam, masa->tam};
    SDL_RenderDrawRect(renderer, &r1);
}

static void __dibujar_resorte(resorte_t *resorte, SDL_Renderer *renderer) {
    masa_t *masa1 = resorte->masa1;
    masa_t *masa2 = resorte->masa2;
    
    _color_render(renderer, resorte->color, 0x00);
    SDL_RenderDrawLine(renderer, masa1->x, masa1->y, masa2->x, masa2->y);
}

static void _dibujar_lista_nodos(malla_t *malla, SDL_Renderer *renderer) {
    lista_iter_t *iter = lista_iter_crear(malla->masas);
    while (!lista_iter_al_final(iter)) {
        masa_t *masa = lista_iter_ver_actual(iter);
        __dibujar_nodo(masa, renderer);
        lista_iter_avanzar(iter);
    }
    lista_iter_destruir(iter);
}

static void _dibujar_lista_resortes(malla_t *malla, SDL_Renderer *renderer) {
    lista_iter_t *iter = lista_iter_crear(malla->resortes);
    while (!lista_iter_al_final(iter)) {
        resorte_t *resorte = lista_iter_ver_actual(iter);
        __dibujar_resorte(resorte, renderer);
        lista_iter_avanzar(iter);
    }
    lista_iter_destruir(iter);
}

void renderizar_malla(malla_t *malla, SDL_Renderer *renderer) {
    _dibujar_lista_nodos(malla, renderer);
    _dibujar_lista_resortes(malla, renderer);
}

