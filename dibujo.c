#include <SDL2/SDL.h>

#include "dibujo.h"
#include "masa.h"
#include "resorte.h"
#include "malla.h"
#include "lista.h"
#include "config.h"

static void __dibujar_masa(masa_t *masa, SDL_Renderer *renderer) {
    definir_color(renderer, obtener_color_masa(masa), 0x00);
    SDL_Rect r1 = {(obtener_x_masa(masa) - obtener_tam_masa(masa)/2)*FACTOR_ESCALA, (obtener_y_masa(masa) - obtener_tam_masa(masa)/2)*FACTOR_ESCALA, obtener_tam_masa(masa)*FACTOR_ESCALA, obtener_tam_masa(masa)*FACTOR_ESCALA};
    SDL_RenderDrawRect(renderer, &r1);
}

static void __dibujar_resorte(resorte_t *resorte, SDL_Renderer *renderer) {
    masa_t *masa1 = obtener_masa1_resorte(resorte);
    masa_t *masa2 = obtener_masa2_resorte(resorte);
    
    definir_color(renderer, obtener_color_resorte(resorte), 0x00);
    SDL_RenderDrawLine(renderer, obtener_x_masa(masa1)*FACTOR_ESCALA, obtener_y_masa(masa1)*FACTOR_ESCALA, obtener_x_masa(masa2)*FACTOR_ESCALA, obtener_y_masa(masa2)*FACTOR_ESCALA);
}

static void _dibujar_lista_masas(malla_t *malla, SDL_Renderer *renderer) {
    lista_iter_t *iter = lista_iter_crear(obtener_lista_masas_malla(malla));
    while (!lista_iter_al_final(iter)) {
        masa_t *masa = lista_iter_ver_actual(iter);
        __dibujar_masa(masa, renderer);
        lista_iter_avanzar(iter);
    }
    lista_iter_destruir(iter);
}

static void _dibujar_lista_resortes(malla_t *malla, SDL_Renderer *renderer) {
    lista_iter_t *iter = lista_iter_crear(obtener_lista_resortes_malla(malla));
    while (!lista_iter_al_final(iter)) {
        resorte_t *resorte = lista_iter_ver_actual(iter);
        __dibujar_resorte(resorte, renderer);
        lista_iter_avanzar(iter);
    }
    lista_iter_destruir(iter);
}

void renderizar_malla(malla_t *malla, SDL_Renderer *renderer) {
    _dibujar_lista_masas(malla, renderer);
    _dibujar_lista_resortes(malla, renderer);
}

void planear_resorte(masa_t *masa, float x, float y, Color color_construccion, SDL_Renderer *renderer) {
    definir_color(renderer, color_construccion, 0x00);
    SDL_RenderDrawLine(renderer, obtener_x_masa(masa)*FACTOR_ESCALA, obtener_y_masa(masa)*FACTOR_ESCALA, x*FACTOR_ESCALA, y*FACTOR_ESCALA);
}