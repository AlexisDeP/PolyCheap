#include <SDL2/SDL.h>
#include <stdbool.h>

#include "malla.h"
#include "dibujo.h"
#include "calculo.h"
#include "config.h"
#include "simulacion.h"
<<<<<<< HEAD
#include "masa.h"
#include "resorte.h"

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
=======
>>>>>>> 1f999196917812af0c82b6b1ee73828fe78df08a

#ifdef TTF
#include <SDL2/SDL_ttf.h>




void escribir_texto(SDL_Renderer *renderer, TTF_Font *font, const char *s, int x, int y) {
    SDL_Color color = {255, 255, 255};  // Estaría bueno si la función recibe un enumerativo con el color, ¿no?
    SDL_Surface *surface = TTF_RenderText_Solid(font, s, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = surface->w;
    rect.h = surface->h;

    SDL_RenderCopy(renderer, texture, NULL, &rect);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}
#endif


int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

#ifdef TTF
    TTF_Init();
    TTF_Font* font = TTF_OpenFont("FreeSansBold.ttf", 24);
    if(font == NULL) {
        fprintf(stderr, "No pudo cargarse la tipografía\n");
        return 1;
    }
#endif

    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;

    SDL_CreateWindowAndRenderer(VENTANA_ANCHO, VENTANA_ALTO, 0, &window, &renderer);
    SDL_SetWindowTitle(window, "PolyCheapBridge - Herrera/Spadachini");

    int dormir = 0;

    // BEGIN código del alumno
    //bool estoy_dibujando = false;
    int coordx = 0, coordy = 0;
    int iniciox, inicioy;

    malla_t *malla_principal = crear_malla();
    masa_t *masa, *masa_desplazamiento, *masa_detectada, *masa_aux;
    resorte_t *resorte;

    //masa_t *masa_fija = nueva_masa_fija(malla_principal, 100, 100, TAM, COLOR_MASA_FIJA);   // Solo para pruebas

    bool simulando = false;
    bool desplazando = false;
    bool dibujando = false;
    bool en_rango = false;
    // END código del alumno

    unsigned int ticks = SDL_GetTicks();
    while(1) {
        if(SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                break;

            // BEGIN código del alumno
            if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                
                iniciox = event.motion.x;
                inicioy = event.motion.y;
                if(!simulando){
                    masa_desplazamiento = detectar_masa(malla_principal, iniciox, inicioy, TOL_MASAS);
                    if(masa_desplazamiento != NULL && !es_fija(masa_desplazamiento)){
                        desplazando = true;
                    }
                }
            }
            else if(event.type == SDL_MOUSEMOTION) {
                coordx = event.motion.x;
                coordy = event.motion.y;

                if(dibujando){
                    en_rango = !excede_max_longitud(malla_principal, masa_aux, coordx, coordy, LO_MAX);
                    if(excede_max_longitud(malla_principal, masa_aux, coordx, coordy, LO_MAX)){
                        cambiar_color_masa(masa_aux, COLOR_MASA, COLOR_MASA_FIJA);
                        en_rango = false;

                    }else{
                        cambiar_color_masa(masa_aux, COLOR_CONSTRUCCION, COLOR_CONSTRUCCION);
                        en_rango = true;
                    }
                }

                if(desplazando){
                    mover_masa(malla_principal, masa_desplazamiento, event.motion.x, event.motion.y, LO_MAX);
                }
            }
            else if(event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT) {
                
                if(!simulando){ //Bool 
                    desplazando = false;

                    if(comparar_puntos(iniciox, inicioy, event.motion.x, event.motion.y, TOL_CLIC)){

                        masa_detectada = detectar_masa(malla_principal, event.motion.x, event.motion.y, TOL_MASAS);

                        if(!dibujando){
                            if(masa_detectada == NULL){
                                nueva_masa(malla_principal, iniciox, inicioy, TAM, COLOR_MASA);

                            }else{
                                masa_aux = masa_detectada;
                                dibujando = true;
                            }
                        }else if(!en_rango){
                            dibujando = false;

                        }else if(masa_detectada == NULL){
                            masa = nueva_masa(malla_principal, iniciox, inicioy, TAM, COLOR_MASA);
                            nuevo_resorte(malla_principal, masa_aux, masa, COLOR_RESORTE);
<<<<<<< HEAD
                            
                            
=======
>>>>>>> 1f999196917812af0c82b6b1ee73828fe78df08a
                            cambiar_color_masa(masa_aux, COLOR_MASA, COLOR_MASA_FIJA);
                            dibujando = false;

                        }else if(masa_aux == masa_detectada){
                            cambiar_color_masa(masa_aux, COLOR_MASA, COLOR_MASA_FIJA);
                            dibujando = false;
                            
                        }else if(masa_detectada != NULL && !masas_conectadas(malla_principal, masa_aux, masa_detectada)){
                            nuevo_resorte(malla_principal, masa_aux, masa_detectada, COLOR_RESORTE);
                            
                            cambiar_color_masa(masa_aux, COLOR_MASA, COLOR_MASA_FIJA);
                            dibujando = false;

                        }else{
                            cambiar_color_masa(masa_aux, COLOR_MASA, COLOR_MASA_FIJA);
                            dibujando = false;
                        }
                    }
                }
            }
            if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_RIGHT) {
                iniciox = event.motion.x;
                inicioy = event.motion.y;
            }
            else if(event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_RIGHT) {

                masa = detectar_masa(malla_principal, iniciox, inicioy, TOL_MASAS);
                resorte = detectar_resorte(malla_principal, iniciox, inicioy, TOL_RESORTES);

                if(comparar_puntos(iniciox, inicioy, event.motion.x, event.motion.y, TOL_CLIC)){
                    if(!simulando){
                        if(!dibujando){
                            if(masa != NULL && !es_fija(masa)){
                                borrar_masa(malla_principal, masa);
                                borrar_resortes_conectados(malla_principal, masa);
                            }else if(resorte != NULL){
                                borrar_resorte(malla_principal, resorte);
                            }else{
                                simulando = true;
                            }
                        }else{
                            cambiar_color_masa(masa_aux, COLOR_MASA, COLOR_MASA_FIJA);
                            dibujando = false;
                        }
                    }else{
                        simulando = false;
                    }
                }
            }
            // END código del alumno

            continue;
        }
        SDL_SetRenderDrawColor(renderer, 0x07, 0x06, 0x06, 0x00);
        
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0x00);

        // BEGIN código del alumno
#ifdef TTF
        //escribir_texto(renderer, font, "Mono Bridge", 100, 20);
        char aux[100];
        sprintf(aux, "%03d, %03d", coordx, coordy);
        escribir_texto(renderer, font, aux, VENTANA_ANCHO - 100, VENTANA_ALTO - 34);

        if(simulando){
            escribir_texto(renderer, font, "SIMULANDO", 100, 20);
            char aux[100];
            sprintf(aux, "%03d, %03d", coordx, coordy);
            escribir_texto(renderer, font, aux, VENTANA_ANCHO - 100, VENTANA_ALTO - 34);
        }
#endif



        if(dibujando && en_rango){
            planear_resorte(masa_detectada, coordx, coordy, COLOR_CONSTRUCCION, renderer);
        }

        if(simulando){
<<<<<<< HEAD
            simular_malla(malla_principal, renderer, DURACION_SIMULACION, MASA_TOTAL / 5, DT, B, G, K_BASE, POTENCIA_K, LO_MAX);
=======
            simular_malla(malla_principal, renderer, DURACION_SIMULACION, MASA_TOTAL, DT, B, G, K_BASE, POTENCIA_K, LO_MAX);
>>>>>>> 1f999196917812af0c82b6b1ee73828fe78df08a
        } else {
            renderizar_malla(malla_principal, renderer);
        }

        // END código del alumno

        

        SDL_RenderPresent(renderer);
        ticks = SDL_GetTicks() - ticks;
        if(dormir) {
            SDL_Delay(dormir);
            dormir = 0;
        }
        else if(ticks < 1000 / JUEGO_FPS)
            SDL_Delay(1000 / JUEGO_FPS - ticks);
        ticks = SDL_GetTicks();
    }

    // BEGIN código del alumno
    // END código del alumno

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

#ifdef TTF
    TTF_CloseFont(font);
    TTF_Quit();
#endif
    SDL_Quit();
    return 0;
}