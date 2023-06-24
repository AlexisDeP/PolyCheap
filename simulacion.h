#ifndef SIMULACION_H
#define SIMULACION_H

#include "malla.h"

typedef struct simulacion simulacion_t;

typedef struct instante instante_t;

simulacion_t* simu_crear();

simulacion_t* inicializar_simulacion(malla_t* malla);

void simu_destruir(simulacion_t* simulacion, void (*destruir_dato)(void*));

void simu_remover_inst(simulacion_t* simulacion);

void destruir_posicion(void* dato);

void destruir_instante(instante_t *instante);

void simular_malla(malla_t *malla, simulacion_t *simu, float tiempo, float m, float dt, float b, float g, float kb, float pk, float longitud_maxima);

#endif