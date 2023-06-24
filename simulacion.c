#include <SDL2/SDL.h>

#include "simulacion.h"
#include "malla.h"
#include "dibujo.h"
#include "calculo.h"
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

typedef struct posicion {
    int x;
    int y;
} posicion_t;

typedef struct longitud {
    float l0;
    float l;
} longitud_t;

struct instante {
    posicion_t* posiciones;
    longitud_t* longitudes;
    size_t cantidad_masas;
    size_t cantidad_resortes;
};

struct simulacion {
    malla_t *malla;
    lista_t *instantes;
};




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

static bool simu_agregar_inst(simulacion_t* simulacion, posicion_t* posiciones, size_t cantidad_masas, longitud_t* longitudes, size_t cantidad_resortes) {
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

static instante_t* simu_inst_ant(simulacion_t* simulacion) {
    if (simulacion == NULL || lista_esta_vacia(simulacion->instantes)) {
        return NULL;
    }

    return lista_iter_ver_actual(lista_iter_crear(simulacion->instantes));
}

static instante_t* simu_inst_ant2(simulacion_t* simulacion) {
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

static instante_t* crear_instante(size_t cantidad_masas, size_t cantidad_resortes) {
    instante_t* instante = malloc(sizeof(instante_t));
    if (instante == NULL) return NULL;

    instante->posiciones = malloc(cantidad_masas * sizeof(posicion_t));
    if (instante->posiciones == NULL) {
        free(instante);
        return NULL;
    }

    instante->longitudes = malloc(cantidad_resortes * sizeof(longitud_t));
    if (instante->longitudes == NULL) {
        free(instante->posiciones);
        free(instante);
        return NULL;
    }

    instante->cantidad_masas = cantidad_masas;
    instante->cantidad_resortes = cantidad_resortes;

    return instante;
}

void destruir_instante(instante_t *instante) {
    if(instante == NULL) return;

    instante_t* inst = (instante_t*)instante;
    free(inst->longitudes);
    free(inst->posiciones);
    free(inst);
}

static instante_t* inicializar_instante_desde_malla(malla_t* malla) {
    instante_t* instante = crear_instante(obtener_cantidad_masas(malla), obtener_cantidad_resortes(malla));
    if(instante == NULL) return NULL;

    size_t i = 0;
    size_t j = 0;
    
    resorte_t* resorte_actual;
    
    lista_iter_t* iter_masas = lista_iter_crear(malla->masas);
    while (!lista_iter_al_final(iter_masas)) {
        masa_t* masa = lista_iter_ver_actual(iter_masas);
        instante->posiciones[i].x = masa->x;
        instante->posiciones[i].y = masa->y;

        lista_iter_avanzar(iter_masas);
        i++;
    }

    lista_iter_t* iter2 = lista_iter_crear(malla->resortes);
    while((resorte_actual = lista_iter_ver_actual(iter2))){
        
        instante->longitudes[j].l0 = distancia_entre_masas(resorte_actual->masa1, resorte_actual->masa2);
        instante->longitudes[j].l = distancia_entre_masas(resorte_actual->masa1, resorte_actual->masa2);
        
        lista_iter_avanzar(iter2);
        j++;
    }

    lista_iter_destruir(iter_masas);
    lista_iter_destruir(iter2);

    return instante;
}

static void agregar_instante(simulacion_t* simulacion, instante_t* instante) {         // Cuidado con la memoria !!!!!!!!!
    // Verificar si la simulación tiene más de dos instantes
    if (lista_largo(simulacion->instantes) >= 2) {
        // Eliminar el instante más antiguo (el primero de la lista)
        lista_borrar_primero(simulacion->instantes);
    }
    
    // Agregar el nuevo instante al final de la lista
    lista_insertar_ultimo(simulacion->instantes, instante);
}

static instante_t* copiar_instante(const instante_t* instante) {
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

    copia->cantidad_resortes = instante->cantidad_resortes;
    copia->longitudes = malloc(sizeof(posicion_t) * copia->cantidad_resortes);
    for (size_t i = 0; i < copia->cantidad_resortes; i++) {
        copia->longitudes[i].l0 = instante->longitudes[i].l0;
        copia->longitudes[i].l = instante->longitudes[i].l;
    }

    return copia;
}

static simulacion_t* inicializar_simulacion(malla_t* malla) {

    simulacion_t* simulacion = simu_crear();
    simulacion->malla = malla;

    instante_t *primer_instante = inicializar_instante_desde_malla(malla);
    if (primer_instante == NULL) {
        lista_destruir(simulacion->instantes, (void (*)(void*))destruir_instante);
        free(simulacion);
        return NULL;
    }

    lista_insertar_primero(simulacion->instantes, primer_instante);

    // Crear y agregar el segundo instante igual al primero
    instante_t* segundo_instante = copiar_instante(primer_instante);
    if (segundo_instante == NULL) {
        lista_borrar_primero(simulacion->instantes);
        lista_destruir(simulacion->instantes, (void (*)(void*))destruir_instante);
        free(simulacion);
        return NULL;
    }

    lista_insertar_ultimo(simulacion->instantes, segundo_instante);

    

    simulacion->malla = malla;

    return simulacion;
}

static posicion_t *calcular_pos_act_m(simulacion_t *simu, masa_t *masa, float m, float dt, float b, float g, float kb, float pk) {

    masa_t *masa_j, *masa_k;

    size_t sumatoria_x = 0;
    size_t sumatoria_y = 0;
<<<<<<< HEAD

    instante_t *instante1 = simu_inst_ant(simu);
    instante_t *instante2 = simu_inst_ant2(simu);

    lista_iter_t *iter = lista_iter_crear(simu->malla->resortes);
    while (!lista_iter_al_final(iter)) {
        resorte_t *resorte = lista_iter_ver_actual(iter);
        obtener_masas_resorte(resorte, &masa_j, &masa_k);

        size_t id_masa_j = buscar_id_masa(masa_j);
        size_t id_masa_k = buscar_id_masa(masa_k);
        

        if(id_masa_j == masa->id) {
            
            size_t id_resorte = buscar_id_resorte(resorte);
            
=======
    size_t bj_x = 0;
    size_t bj_y = 0;

    lista_iter_t *iter = lista_iter_crear(simu->malla->resortes);
    while (!lista_iter_al_final(iter)) {
        resorte_t *resorte = lista_iter_ver_actual(iter);
        
        obtener_masas_resorte(resorte, &masa_j, &masa_k);

        if(masa_j == masa) {
            size_t id_masa_j = buscar_id_masa(masa_j);
            size_t id_masa_k = buscar_id_masa(masa_k);
            size_t id_resorte = buscar_id_resorte(resorte);

            instante_t *instante1 = simu_inst_ant(simu);
            instante_t *instante2 = simu_inst_ant2(simu);

            bj_x = bj(m, dt, b, g, instante1->posiciones[id_masa_j].x, instante2->posiciones[id_masa_j].x);
            bj_y = bj(m, dt, b, g, instante1->posiciones[id_masa_j].y, instante2->posiciones[id_masa_j].y);

>>>>>>> 1f999196917812af0c82b6b1ee73828fe78df08a
            sumatoria_x += _sumatoria_bj(kb, pk, instante1->posiciones[id_masa_j].x, instante1->posiciones[id_masa_k].x, instante1->longitudes[id_resorte].l0, instante1->longitudes[id_resorte].l);
            sumatoria_y += _sumatoria_bj(kb, pk, instante1->posiciones[id_masa_j].y, instante1->posiciones[id_masa_k].y, instante1->longitudes[id_resorte].l0, instante1->longitudes[id_resorte].l);
        }

        lista_iter_avanzar(iter);
    }

<<<<<<< HEAD
    float bj_x = bj(m, dt, b, 0, instante1->posiciones[masa->id].x, instante2->posiciones[masa->id].x);
    float bj_y = bj(m, dt, b, g, instante1->posiciones[masa->id].y, instante2->posiciones[masa->id].y);

=======
>>>>>>> 1f999196917812af0c82b6b1ee73828fe78df08a
    size_t pos_x = Bj(bj_x, sumatoria_x) / Aj(m, dt, b);
    size_t pos_y = Bj(bj_y, sumatoria_y) / Aj(m, dt, b);

    posicion_t *posicion_masa_j = malloc(sizeof(posicion_t));
    if(posicion_masa_j == NULL) return NULL;

    posicion_masa_j->x = pos_x;
    posicion_masa_j->y = pos_y;

    lista_iter_destruir(iter);

    return posicion_masa_j;
}

static longitud_t *crear_longitudes(float l0, float l) {
    longitud_t *longitud = malloc(sizeof(longitud_t));
    if(longitud == NULL) return NULL;
    longitud->l0 = l0;
    longitud->l = l;
    return longitud;
}

static bool insertar_posicion_instante(instante_t* instante, posicion_t* posicion) {
    size_t nueva_cantidad_masas = instante->cantidad_masas + 1;

    // Redimensionar el arreglo de posiciones
    posicion_t* aux = realloc(instante->posiciones, nueva_cantidad_masas * sizeof(posicion_t));
    if(aux == NULL) return false;
    posicion_t* nuevas_posiciones = aux;

    instante->posiciones = nuevas_posiciones;
    instante->posiciones[nueva_cantidad_masas - 1] = *posicion;

    instante->cantidad_masas = nueva_cantidad_masas;

    return true;
}

static bool insertar_longitud_instante(instante_t* instante, longitud_t* longitud) {
    size_t nueva_cantidad_resortes = instante->cantidad_resortes + 1;

    // Redimensionar el arreglo de longitudes
    longitud_t* aux = realloc(instante->longitudes, nueva_cantidad_resortes * sizeof(longitud_t));
    if(aux == NULL) return false;
    longitud_t* nuevas_longitudes = aux;

    instante->longitudes = nuevas_longitudes;
    instante->longitudes[nueva_cantidad_resortes - 1] = *longitud;

    instante->cantidad_resortes = nueva_cantidad_resortes;

    return true;
}

static instante_t* calcular_instante(simulacion_t *simu, float m, float dt, float b, float g, float kb, float pk) {
<<<<<<< HEAD
    
    instante_t *nuevo_instante = crear_instante(obtener_cantidad_masas(simu->malla), obtener_cantidad_resortes(simu->malla));
    
    instante_t *instante_1 = simu_inst_ant(simu);

=======
    instante_t *nuevo_instante = crear_instante(obtener_cantidad_masas(simu->malla), obtener_cantidad_resortes(simu->malla));
    instante_t *instante_ant = simu_inst_ant(simu);
    instante_t *instante_ant2 = simu_inst_ant2(simu);
>>>>>>> 1f999196917812af0c82b6b1ee73828fe78df08a
    lista_iter_t *iter_masas = lista_iter_crear(simu->malla->masas);
    lista_iter_t *iter_resortes = lista_iter_crear(simu->malla->resortes);

    masa_t *mj, *mk;

    while(!lista_iter_al_final(iter_masas)){
        masa_t *masa_j = lista_iter_ver_actual(iter_masas);
        posicion_t *posicion_masa = calcular_pos_act_m(simu, masa_j, m, dt, b, g, kb, pk);
<<<<<<< HEAD

=======
>>>>>>> 1f999196917812af0c82b6b1ee73828fe78df08a
        if(!insertar_posicion_instante(nuevo_instante, posicion_masa)) return NULL;
        lista_iter_avanzar(iter_masas);
    }
    while(!lista_iter_al_final(iter_resortes)){
        size_t id_resorte = buscar_id_resorte(lista_iter_ver_actual(iter_resortes));
        obtener_masas_resorte(lista_iter_ver_actual(iter_resortes), &mj, &mk);
        size_t id_masa_j = buscar_id_masa(mj);
        size_t id_masa_k = buscar_id_masa(mk);
<<<<<<< HEAD
        size_t l = distancia_puntos(instante_1->posiciones[id_masa_j].x, instante_1->posiciones[id_masa_j].y, 
                                    instante_1->posiciones[id_masa_k].x, instante_1->posiciones[id_masa_k].y);
        longitud_t *longitudes = crear_longitudes(instante_1->longitudes[id_resorte].l0, l);
=======
        size_t l = distancia_puntos(instante_ant->posiciones[id_masa_j].x, instante_ant->posiciones[id_masa_j].y, 
                                    instante_ant->posiciones[id_masa_k].x, instante_ant->posiciones[id_masa_k].y);
        longitud_t *longitudes = crear_longitudes(instante_ant->longitudes[id_resorte].l0, l);
>>>>>>> 1f999196917812af0c82b6b1ee73828fe78df08a
        if(!insertar_longitud_instante(nuevo_instante, longitudes)) return NULL;
        lista_iter_avanzar(iter_resortes);
    }

    lista_iter_borrar(iter_masas);
    lista_iter_borrar(iter_resortes);

    return nuevo_instante;
}

static void actualizar_longitud_resorte(resorte_t *resorte, float longitud) {
    resorte->longitud = longitud;
}

static void convertir_instante_a_malla(instante_t *instante, malla_t *malla, float maxima_longitud) {
    lista_iter_t *iter_masas = lista_iter_crear(malla->masas);
    lista_iter_t *iter_resortes = lista_iter_crear(malla->resortes);

    while(!lista_iter_al_final(iter_masas)) {
        size_t id_masa = buscar_id_masa(lista_iter_ver_actual(iter_masas));
        mover_masa(malla, lista_iter_ver_actual(iter_masas), instante->posiciones[id_masa].x, instante->posiciones[id_masa].y, maxima_longitud);
        lista_iter_avanzar(iter_masas);
    }

    while(!lista_iter_al_final(iter_resortes)) {
        size_t id_resorte = buscar_id_resorte(lista_iter_ver_actual(iter_resortes));
        actualizar_longitud_resorte(lista_iter_ver_actual(iter_resortes), instante->longitudes[id_resorte].l);
        lista_iter_avanzar(iter_resortes);
    }

    lista_iter_destruir(iter_masas);
    lista_iter_destruir(iter_resortes);
}

void simular_malla(malla_t *malla, SDL_Renderer *renderer, float tiempo, float m, float dt, float b, float g, float kb, float pk, float longitud_maxima) {

    reordenar_id(malla);
<<<<<<< HEAD
    simulacion_t *simulacion = inicializar_simulacion(malla);
    if(simulacion == NULL) return;
    

    size_t iteraciones = (size_t)floor(tiempo/dt);
    malla_t *malla_graficadora = crear_malla(); 
    if(!copiar_malla(malla, malla_graficadora)) return;
    
    for(size_t i = 0; i < iteraciones; i++) {
        instante_t *instante_nuevo = calcular_instante(simulacion, m, dt, b, g, kb, pk);
        agregar_instante(simulacion, instante_nuevo);
=======
    simulacion_t *simu = inicializar_simulacion(malla);
    if(simu == NULL) return;

    size_t iteraciones = (size_t)floor(tiempo/dt);
    malla_t *malla_graficadora = crear_malla(); 
    
    if(!copiar_malla(malla, malla_graficadora)) return;
    
    for(size_t i = 0; i < iteraciones; i++) {
        instante_t *instante_nuevo = calcular_instante(simu, m, dt, b, g, kb, pk);
        agregar_instante(simu, instante_nuevo);
>>>>>>> 1f999196917812af0c82b6b1ee73828fe78df08a
        convertir_instante_a_malla(instante_nuevo, malla_graficadora, longitud_maxima);
        renderizar_malla(malla_graficadora, renderer);
    }
}