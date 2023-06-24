#include <SDL2/SDL.h>

#include "dibujo.h"
#include "masa.h"
#include "resorte.h"
#include "malla.h"
#include "lista.h"
#include "config.h"

struct masa {
    size_t id;
    float x, y, tam;      
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
    SDL_Rect r1 = {(masa->x - masa->tam/2)*FACTOR_ESCALA, (masa->y - masa->tam/2)*FACTOR_ESCALA, masa->tam*FACTOR_ESCALA, masa->tam*FACTOR_ESCALA};
    SDL_RenderDrawRect(renderer, &r1);
}

static void __dibujar_resorte(resorte_t *resorte, SDL_Renderer *renderer) {
    masa_t *masa1 = resorte->masa1;
    masa_t *masa2 = resorte->masa2;
    
    _color_render(renderer, resorte->color, 0x00);
    SDL_RenderDrawLine(renderer, masa1->x*FACTOR_ESCALA, masa1->y*FACTOR_ESCALA, masa2->x*FACTOR_ESCALA, masa2->y*FACTOR_ESCALA);
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

void planear_resorte(masa_t *masa, float x, float y, Color color_construccion, SDL_Renderer *renderer) {
    _color_render(renderer, color_construccion, 0x00);
    SDL_RenderDrawLine(renderer, masa->x*FACTOR_ESCALA, masa->y*FACTOR_ESCALA, x*FACTOR_ESCALA, y*FACTOR_ESCALA);
}