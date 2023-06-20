#include "malla.h"
#include "calculo.h"
#include "lista.h"

struct malla {
    lista_t* resortes;
    lista_t* masas;
};

typedef struct posicion {
    int x;
    int y;
} posicion_t;

typedef struct longitud {
    float l0;
    float l;
}   longitud_t;

typedef struct instante {
    posicion_t* posiciones;
    longitud_t* longitudes;
    size_t cantidad_masas;
    size_t cantidad_resortes;
} instante_t;

typedef struct simulacion {
    malla_t *malla;
    lista_t *instantes;
} simulacion_t;


static posicion_t *posicion_actual(simulacion_t *simulacion);

static float Aj(float m, float dt, float b) {
    return ((m/(dt*dt))+(b/dt));
}

static float Bj(float bj, float sumatoria_bj) {
    return (bj + sumatoria_bj);
}

static float bj(float m, float dt, float b, float g, int pos, int pos1, int pos2) {
    return (((m/(dt*dt))*((2*pos1) - pos2))+((b/dt)*pos1) + (m*g));
}

static float k(float kb, float l0, float pk) {
    return (kb/pow(l0, pk));
}

static float _sumatoria_bj(float kb, float pk, float pj1, float pk1, float rl0, float rl1) {
    return (k(kb, rl0, pk) * ((rl0 - rl1)/rl1) * (pj1 - pk1));
}

static float calcular_pos_act(simulacion_t *simu, float m, float dt, float b, float g, float kb, float pk) {

    lista_iter_t *iter = lista_iter_crear(malla->resortes);
    masa_t *mj, *mk;
    instante_t *ant, *ant2;
    size_t idk;
    float xj, yj, xk, yk, aux_x, aux_y;

    while (!lista_iter_al_final(iter)) {
        for(size_t i = 0; i < instante->cantidad_resortes; i++){
            resorte_t *resorte_actual = lista_iter_ver_actual(iter);
            obtener_masas_resorte(resorte_actual, &mj, &mk);
            xj = mj->x;
            yj = mj->y;
            xk = mk->x;
            yk = mk->y;
            ant = simu_inst_ant(simu);
            ant2 = simu_inst_ant2(simu);
            idk = obtener_idk(mj, resorte_actual);
            idr = obtener_
            aux_x = _sumatoria_bj(kb, pk, ant->posiciones[i].x, ant->posiciones[idk].x, ant->longitudes%%%%%%%, ant->longitudes%%%%%%%);
        }

        lista_iter_avanzar(iter);
    }
    lista_iter_destruir(iter);
}




simulacion_t* simu_crear() {
    simulacion_t* simulacion = malloc(sizeof(simulacion_t));
    if (simulacion == NULL) return NULL;

    simulacion->instantes = lista_crear();
    if (simulacion->instantes == NULL) {
        free(simulacion);
        return NULL;
    }

    return simulacion;
}

void simu_destruir(simulacion_t* simulacion, void (*destruir_dato)(void*)) {
    if (simulacion == NULL) return;

    lista_destruir(simulacion->instantes, destruir_dato);
    free(simulacion);
}

void simu_remover_inst(simulacion_t* simulacion) {      // Cuidado con la memoria !!!!!!!!!!!!
    if (simulacion == NULL || lista_esta_vacia(simulacion->instantes)) return;

    lista_borrar_primero(simulacion->instantes);
}

bool simu_agregar_inst(simulacion_t* simulacion, posicion_t* posiciones, size_t cantidad_masas, longitud_t* longitudes, size_t cantidad_resortes) {
    if (simulacion == NULL || posiciones == NULL) return false;

    instante_t* instante = malloc(sizeof(instante_t));
    if (instante == NULL) return false;

    instante->posiciones = posiciones;
    instante->cantidad_masas = cantidad_masas;
    instante->longitudes = longitudes;
    instante->cantidad_resortes = cantidad_resortes;

    // Insertar el instante al final de la lista
    if (!lista_insertar_ultimo(simulacion->instantes, instante)) {
        free(instante);
        return false;
    }

    // Si hay más de dos instantes, eliminar el más antiguo
    if (lista_largo(simulacion->instantes) > 2) {
        simu_remover_inst(simulacion);
    }

    return true;
}

instante_t* simu_inst_ant(simulacion_t* simulacion) {
    if (simulacion == NULL || lista_esta_vacia(simulacion->instantes)) {
        return NULL;
    }

    return lista_iter_ver_actual(lista_iter_crear(simulacion->instantes));
}

instante_t* simu_inst_ant2(simulacion_t* simulacion) {
    if (simulacion == NULL || lista_largo(simulacion->instantes) < 2) return NULL;

    lista_iter_t* iter = lista_iter_crear(simulacion->instantes);
    lista_iter_avanzar(iter);
    instante_t* instante_anterior = lista_iter_ver_actual(iter);
    lista_iter_destruir(iter);

    return instante_anterior;
}

void destruir_posicion(void* dato) {
    if (dato != NULL) free(dato);
}

instante_t* crear_instante(malla_t *malla) {
    instante_t* instante = malloc(sizeof(instante_t));
    if (instante == NULL) return NULL;

    instante->cantidad_masas = obtener_cantidad_masas(malla);
    instante->posiciones = malloc(sizeof(posicion_t) * instante->cantidad_masas);
    if (instante->posiciones == NULL) {
        free(instante);
        return NULL;
    }

    instante->cantidad_resortes = obtener_cantidad_resortes(malla);
    instante->longitudes = malloc(sizeof(longitud_t) * instante->cantidad_resortes);
    if (instante->longitudes == NULL){
        free(instante->posiciones);
        free(instante);
        return NULL;
    }

    return instante;
}

instante_t* inicializar_instante_desde_malla(malla_t* malla) {
    instante_t* instante = crear_instante();
    if(instante == NULL) return NULL;

    lista_iter_t* iter = lista_iter_crear(malla->masas);
    if (iter == NULL) {
        free(instante->posiciones);
        free(instante->longitudes);
        free(instante);
        return NULL;
    }

    lista_iter_t* iter2 = lista_iter_crear(malla->resortes);

    size_t i = 0;
    size_t j = 0;
    masa_t* masa_actual;
    resorte_t* resorte_actual;

    while ((masa_actual = lista_iter_ver_actual(iter))) {
        instante->posiciones[i].x = masa_actual->x;
        instante->posiciones[i].y = masa_actual->y;

        lista_iter_avanzar(iter);
        i++;
    }
    while((resorte_actual = lista_iter_ver_actual(iter2))){
        instante->longitudes[j].l0 = resorte_actual->longitud;
        instante->longitudes[j].l = resorte_actual->longitud;

        lista_iter_avanzar(iter2);
        j++;
    }

    lista_iter_destruir(iter);
    lista_iter_destruir(iter2);

    return instante;
}

void agregar_instante(simulacion_t* simulacion, instante_t* instante) {         // Cuidado con la memoria !!!!!!!!!
    // Verificar si la simulación tiene más de dos instantes
    if (lista_largo(simulacion->instantes) >= 2) {
        // Eliminar el instante más antiguo (el primero de la lista)
        lista_borrar_primero(simulacion->instantes);
    }
    
    // Agregar el nuevo instante al final de la lista
    lista_insertar_ultimo(simulacion->instantes, instante);
}

instante_t* copiar_instante(const instante_t* instante) {
    instante_t* copia = malloc(sizeof(instante_t));
    if (copia == NULL) return NULL;

    copia->cantidad_masas = instante->cantidad_masas;

    copia->posiciones = malloc(sizeof(posicion_t) * copia->cantidad_masas);
    if (copia->posiciones == NULL) {
        free(copia);
        return NULL;
    }

    for (size_t i = 0; i < copia->cantidad_masas; i++) {
        copia->posiciones[i].x = instante->posiciones[i].x;
        copia->posiciones[i].y = instante->posiciones[i].y;
    }

    return copia;
}

simulacion_t* inicializar_simulacion(malla_t* malla) {

    simulacion_t* simulacion = simu_crear();

    instante_t* primer_instante = inicializar_instante_desde_malla(malla);
    if (primer_instante == NULL) {
        lista_destruir(simulacion->instantes, destruir_instante);
        free(simulacion);
        return NULL;
    }

    lista_insertar_primero(simulacion->instantes, primer_instante);

    // Crear y agregar el segundo instante igual al primero
    instante_t* segundo_instante = copiar_instante(primer_instante);
    if (segundo_instante == NULL) {
        lista_borrar_primero(simulacion->instantes);
        lista_destruir(simulacion->instantes, destruir_instante);
        free(simulacion);
        return NULL;
    }

    lista_insertar_primero(simulacion->instantes, segundo_instante);

    return simulacion;
}

void simular(malla_t *malla, float tiempo, float dt, float mt, float b, float g) {
    simulacion_t* simulacion = inicializar_simulacion(malla);
    instante_t* nuevo_instante = crear_instante(malla);

    size_t ciclos = (size_t)floor(tiempo/dt);

    for(size_t i = 0; i < ciclos; i++){
        for(size_t j = 0; j < nuevo_instante->cant_masas; j++){

            int x = calcular_x();
            int y = calcular_y();

            nuevo_instante->posiciones[j].x = x;
            nuevo_instante->posiciones[j].y = y; 
        }
        agregar_instante(simulacion, nuevo_instante);
    }
}