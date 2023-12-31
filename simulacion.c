#include <SDL2/SDL.h>

#include "simulacion.h"
#include "malla.h"
#include "dibujo.h"
#include "calculo.h"
#include "lista.h"
#include "config.h"

typedef struct posicion {
    float x;
    float y;
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

static instante_t* simu_inst_ant(simulacion_t* simulacion) {
    lista_iter_t *iter = lista_iter_crear(simulacion->instantes);
    instante_t *instante1 = lista_iter_ver_actual(iter);

    lista_iter_destruir(iter);
    return instante1;
}

static instante_t* simu_inst_ant2(simulacion_t* simulacion) {
    lista_iter_t* iter = lista_iter_crear(simulacion->instantes);
    lista_iter_avanzar(iter);
    instante_t *instante2 = lista_iter_ver_actual(iter);
;
    lista_iter_destruir(iter);
    return instante2;
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

void destruir_instante(void *instante) {
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
    
    lista_iter_t* iter_masas = lista_iter_crear(obtener_lista_masas_malla(malla));
    while (!lista_iter_al_final(iter_masas)) {
        masa_t* masa = lista_iter_ver_actual(iter_masas);
        instante->posiciones[i].x = obtener_x_masa(masa);
        instante->posiciones[i].y = obtener_y_masa(masa);

        lista_iter_avanzar(iter_masas);
        i++;
    }

    lista_iter_t* iter2 = lista_iter_crear(obtener_lista_resortes_malla(malla));
    while((resorte_actual = lista_iter_ver_actual(iter2))){
        
        instante->longitudes[j].l0 = distancia_entre_masas(obtener_masa1_resorte(resorte_actual), obtener_masa2_resorte(resorte_actual));
        instante->longitudes[j].l = distancia_entre_masas(obtener_masa1_resorte(resorte_actual), obtener_masa2_resorte(resorte_actual));
        
        lista_iter_avanzar(iter2);
        j++;
    }

    lista_iter_destruir(iter_masas);
    lista_iter_destruir(iter2);

    return instante;
}

static void agregar_instante(simulacion_t* simulacion, instante_t* instante) {         
    lista_iter_t *iter_instantes = lista_iter_crear(simulacion->instantes);
    lista_iter_avanzar(iter_instantes);
    destruir_instante(lista_iter_borrar(iter_instantes));
    lista_iter_destruir(iter_instantes);
    lista_insertar_primero(simulacion->instantes, instante);

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

simulacion_t* inicializar_simulacion(malla_t* malla) {

    simulacion_t* simulacion = simu_crear();
    simulacion->malla = malla;

    instante_t *primer_instante = inicializar_instante_desde_malla(malla);
    if (primer_instante == NULL) {
        lista_destruir(simulacion->instantes, (void (*)(void*))destruir_instante);
        free(simulacion);
        return NULL;
    }

    lista_insertar_ultimo(simulacion->instantes, primer_instante);

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
    posicion_t *posicion_masa_j = malloc(sizeof(posicion_t));
    if(posicion_masa_j == NULL) return NULL;

    instante_t *instante1 = simu_inst_ant(simu);
    instante_t *instante2 = simu_inst_ant2(simu);

    if(es_fija(masa)){
        posicion_masa_j->x = obtener_x_masa(masa);
        posicion_masa_j->y = obtener_y_masa(masa);
        return posicion_masa_j;
    }

    float sumatoria_x = 0;
    float sumatoria_y = 0;

    masa_t *m_j, *m_k;
    lista_iter_t *iter = lista_iter_crear(obtener_lista_resortes_malla(simu->malla));
    while (!lista_iter_al_final(iter)) {

        resorte_t *resorte = lista_iter_ver_actual(iter);
        obtener_masas_resorte(resorte, &m_j, &m_k);

        size_t id_resorte = obtener_id_resorte(resorte);
        size_t id_m_j = obtener_id_masa(m_j);
        size_t id_m_k = obtener_id_masa(m_k);

        if(id_m_j == obtener_id_masa(masa)) {
            
            sumatoria_x += _sumatoria_bj(kb, pk, instante1->posiciones[id_m_j].x, instante1->posiciones[id_m_k].x, instante1->longitudes[id_resorte].l0, instante1->longitudes[id_resorte].l);
            sumatoria_y += _sumatoria_bj(kb, pk, instante1->posiciones[id_m_j].y, instante1->posiciones[id_m_k].y, instante1->longitudes[id_resorte].l0, instante1->longitudes[id_resorte].l);
               
        
        } else if (id_m_k == obtener_id_masa(masa)) {
            
            sumatoria_x += _sumatoria_bj(kb, pk, instante1->posiciones[id_m_k].x, instante1->posiciones[id_m_j].x, instante1->longitudes[id_resorte].l0, instante1->longitudes[id_resorte].l);
            sumatoria_y += _sumatoria_bj(kb, pk, instante1->posiciones[id_m_k].y, instante1->posiciones[id_m_j].y, instante1->longitudes[id_resorte].l0, instante1->longitudes[id_resorte].l);

        } 

        lista_iter_avanzar(iter);

    }

    float bj_x = bj(m, dt, b, 0, instante1->posiciones[obtener_id_masa(masa)].x, instante2->posiciones[obtener_id_masa(masa)].x);
    
    float bj_y;
    if (es_fija(masa)) {
        bj_y = bj(m, dt, b, 0, instante1->posiciones[obtener_id_masa(masa)].y, instante2->posiciones[obtener_id_masa(masa)].y);
    } else {
        bj_y = bj(m, dt, b, g, instante1->posiciones[obtener_id_masa(masa)].y, instante2->posiciones[obtener_id_masa(masa)].y);
    }

    float pos_x = Bj(bj_x, sumatoria_x) / Aj(m, dt, b);
    float pos_y = Bj(bj_y, sumatoria_y) / Aj(m, dt, b);


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

static bool insertar_posicion_instante(instante_t* instante, posicion_t* posicion, size_t id) {
    
    instante->posiciones[id].x = posicion->x;
    instante->posiciones[id].y = posicion->y;
 
    return true;
}

static bool insertar_longitud_instante(instante_t* instante, longitud_t* longitud, size_t id) {

    instante->longitudes[id].l0 = longitud->l0;
    instante->longitudes[id].l = longitud->l;

    return true;

}

static instante_t* calcular_instante(simulacion_t *simu, float m, float dt, float b, float g, float kb, float pk) {
    
    instante_t *nuevo_instante = crear_instante(obtener_cantidad_masas(simu->malla), obtener_cantidad_resortes(simu->malla));
    
    instante_t *instante_1 = simu_inst_ant(simu);

    lista_iter_t *iter_masas = lista_iter_crear(obtener_lista_masas_malla(simu->malla));
    

    
    while(!lista_iter_al_final(iter_masas)){
        masa_t *masa_j = lista_iter_ver_actual(iter_masas);
        posicion_t *posicion_masa = calcular_pos_act_m(simu, masa_j, m, dt, b, g, kb, pk);
        insertar_posicion_instante(nuevo_instante, posicion_masa, obtener_id_masa(masa_j));
        lista_iter_avanzar(iter_masas);
    }

    masa_t *mj, *mk;
    lista_iter_t *iter_resortes = lista_iter_crear(obtener_lista_resortes_malla(simu->malla));
    while(!lista_iter_al_final(iter_resortes)){
        resorte_t *resorte = lista_iter_ver_actual(iter_resortes);
        size_t id_resorte = obtener_id_resorte(resorte);

        obtener_masas_resorte(lista_iter_ver_actual(iter_resortes), &mj, &mk);
        size_t id_masa_j = obtener_id_masa(mj);
        size_t id_masa_k = obtener_id_masa(mk);
        float l = distancia_puntos(instante_1->posiciones[id_masa_j].x, instante_1->posiciones[id_masa_j].y, 
                                    instante_1->posiciones[id_masa_k].x, instante_1->posiciones[id_masa_k].y);
        longitud_t *longitudes = crear_longitudes(instante_1->longitudes[id_resorte].l0, l);
        insertar_longitud_instante(nuevo_instante, longitudes, id_resorte);
        lista_iter_avanzar(iter_resortes);
    }

    lista_iter_destruir(iter_masas);
    lista_iter_destruir(iter_resortes);

    return nuevo_instante;
}

static void mover_masa_simulacion(masa_t *masa, float x, float y) {
    cambiar_x_masa(masa, x);
    cambiar_y_masa(masa, y);
}

static void convertir_instante_a_malla(instante_t *instante, malla_t *malla) {
    lista_iter_t *iter_masas = lista_iter_crear(obtener_lista_masas_malla(malla));
    lista_iter_t *iter_resortes = lista_iter_crear(obtener_lista_resortes_malla(malla));

    while(!lista_iter_al_final(iter_masas)) {
        masa_t *masa = lista_iter_ver_actual(iter_masas);
        size_t id_masa = obtener_id_masa(masa);
        mover_masa_simulacion(masa, instante->posiciones[id_masa].x, instante->posiciones[id_masa].y);
        lista_iter_avanzar(iter_masas);
    }

    while(!lista_iter_al_final(iter_resortes)) {
        resorte_t *resorte = lista_iter_ver_actual(iter_resortes);
        size_t id_resorte = obtener_id_resorte(resorte);
        cambiar_longitud_resorte(resorte, instante->longitudes[id_resorte].l);
        if(obtener_longitud_resorte(resorte) > LO_MAX/FACTOR_ESCALA){
            cambiar_color_resorte(resorte, COLOR_ROJO_FUERTE);
        }else{
            cambiar_color_resorte(resorte, COLOR_CELESTE);
        }
        lista_iter_avanzar(iter_resortes);
    }

    lista_iter_destruir(iter_masas);
    lista_iter_destruir(iter_resortes);
}

void simular_malla(malla_t *malla, simulacion_t *simu, float tiempo, float m, float dt, float b, float g, float kb, float pk) {
    float mi = m / obtener_cantidad_masas(malla);
    instante_t *instante_nuevo = calcular_instante(simu, mi, dt, b, g, kb, pk);
    agregar_instante(simu, instante_nuevo); 

    convertir_instante_a_malla(instante_nuevo, malla);
}

bool cumple_porcentaje_estiramiento(simulacion_t *simulacion, float porcentaje_maximo) {
    size_t num_resortes = obtener_cantidad_resortes(simulacion->malla);
    instante_t *instante = simu_inst_ant(simulacion);
    for(size_t i = 0; i < num_resortes; i++) {
        float porcentaje_estiramiento = (instante->longitudes[i].l - instante->longitudes[i].l0) / instante->longitudes[i].l0;
        if (porcentaje_estiramiento > porcentaje_maximo) return false;
    }
    return true;
}