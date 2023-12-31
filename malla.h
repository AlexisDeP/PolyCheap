#ifndef MALLA_H
#define MALLA_H

#include <stdbool.h>

#include "masa.h"
#include "resorte.h"
#include "lista.h"

typedef struct malla malla_t;


//Borra la masa dentro de una malla.
//Pre: La malla y la masa deben existir.
//     La masa tiene que estar dentro de esa malla.
//Post: La masa no existe.
void borrar_masa(malla_t *malla, masa_t *masa);

//Crea una masa dentro de una malla en una posicion R2, con un tamaño y un color
//Post: La malla contiene a la masa
//      Devuelve la masa creada
masa_t *nueva_masa(malla_t *malla, float x, float y, float tam, Color color);

lista_t* obtener_lista_masas_malla(struct malla* malla);

lista_t* obtener_lista_resortes_malla(struct malla* malla);

//Crea una masa dentro una malla en una posicion fija (x, y) con un tamaño y un color.
//Pre: La malla existe.
//Post: La malla contiene la masa fija.
//      Devuelve la masa fija creada.
masa_t *nueva_masa_fija(malla_t *malla, float x, float y, float tam, Color color);

//Busca una masa dentro de una malla en un rango de tolerancia a un punto (x, y).
//Pre: La malla existe.
//Post: Devuelve la masa en caso de encontrarlo, caso contrario devuelve NULL.
masa_t *detectar_masa(malla_t *malla, float x, float y, float tolerancia);

masa_t *obtener_masa(malla_t *malla, float x, float y);

//Mueve una masa dentro de una malla a una punto (x, y) solo si esta dentro de una longitud maxima permitida.
//Pre: La malla y la masa existen.
//     La malla contiene a la masa.
//Post: La masa se mueve si esta dentro de la longitud maxima, caso contrario no se mueve.
void mover_masa(malla_t *malla, masa_t *masa, float x, float y, float longitud_maxima);

//Detecta si existe un resorte dentro de una malla en una punto (x, y) a una distancia inferior a la tolerancia.
//Pre: La malla existe.
//Post: Devuelve el resorte en caso de encontrarlo, caso contrario devuelve NULL.
resorte_t *detectar_resorte(malla_t *malla, float x, float y, float tolerancia);

void obtener_masas_resorte(resorte_t *resorte, masa_t **mj, masa_t **mk);

//Borra un resorte dentro de una malla.
//Pre: La malla y el resorte existen.
//     La malla contiene al resorte.
//Post: El resorte ya no existe.
void borrar_resorte(malla_t *malla, resorte_t *resorte);

//Borra los resortes conectados a una masa dentro de una malla.
//Pre: La malla y la masa existen.
//     La malla contiene a la masa.
//Post: Los resortes conectados a la masa ya no existen.
void borrar_resortes_conectados(malla_t *malla, masa_t *masa);

//Crea un resorte (con un color) que conecta dos masas dentro de una malla.
//Pre: La malla y las masas existen.
//Post: El resorte existe.
void nuevo_resorte(malla_t *malla, masa_t *m1, masa_t *m2, Color color);

//Verifica si dos masas estan conectadas por algun resorte.
//Pre: La malla y las masas existen.
//     La malla contiene a las masas.
//Post: Devuelve true en caso de que este conectados, false en caso contrario.
bool masas_conectadas(malla_t *malla, masa_t *m1, masa_t *m2);

//Compara la posicion de una masa a un punto (x, y).
//Pre: La malla y la masa existen.
//Post: Devuelve true si excede una longitud maxima, false en caso contrario.
bool excede_max_longitud(malla_t *malla, masa_t *masa, float x, float y, float maxima_longitud);

void insertar_masa(malla_t *malla, masa_t *masa);

size_t obtener_cantidad_masas(const malla_t* malla);

size_t obtener_cantidad_resortes(const malla_t* malla);

//Crea una malla.
//Post: Devuelve la malla creada.
malla_t *crear_malla();

void copiar_malla(const malla_t *malla, malla_t *malla_copia);

void reordenar_malla(const malla_t *malla);

void reordenar_id(const malla_t *malla);

masa_t *buscar_masa_id(malla_t *malla, size_t id_masa);

void destruir_malla(malla_t *malla);

#endif