#include <stdbool.h>

#include "masa.h"
#include "resorte.h"
#include "calculo.h"
#include "lista.h"
#include "malla.h"

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

void insertar_masa(malla_t *malla, masa_t *masa) {
    masa->id = obtener_cantidad_masas(malla);
    lista_insertar_ultimo(malla->masas, masa);

}

static bool verificar_longitud_resortes(malla_t *malla, masa_t *masa, float longitud_maxima) {
    lista_iter_t *iter = lista_iter_crear(malla->resortes);
    if (iter == NULL) return false;
    
    while (!lista_iter_al_final(iter)) {
        resorte_t *resorte = lista_iter_ver_actual(iter);
        masa_t *masa1 = resorte->masa1;
        masa_t *masa2 = resorte->masa2;
        
        if (masa1 == masa || masa2 == masa) {
            float longitud = distancia_puntos(masa1->x, masa1->y, masa2->x, masa2->y);
            
            if (longitud > longitud_maxima) {
                lista_iter_destruir(iter);
                return false; // La longitud de un resorte se excede, retornar false
            }
        }
        
        lista_iter_avanzar(iter);
    }
    
    lista_iter_destruir(iter);
    return true; // La longitud de todos los resortes es válida, retornar true
}

static bool insertar_resorte(malla_t *malla, resorte_t *resorte) {
    resorte->id = obtener_cantidad_resortes(malla);
    return lista_insertar_ultimo(malla->resortes, resorte);
}   

void borrar_masa(malla_t *malla, masa_t *masa) {
    lista_iter_t *iter = lista_iter_crear(malla->masas);
    while (!lista_iter_al_final(iter)) {
        masa_t *masa_actual = lista_iter_ver_actual(iter);
        if (masa_actual == masa) {
            _borrar_masa(masa_actual);
            lista_iter_borrar(iter);
            lista_iter_destruir(iter);
            return;
        }
        lista_iter_avanzar(iter);
    }
    lista_iter_destruir(iter);
} 

masa_t *nueva_masa(malla_t *malla, float x, float y, float tam, Color color){
    masa_t *masa = crear_masa(x, y, tam, color);
    insertar_masa(malla, masa);
    return masa;
}

static masa_t *obtener_masa_id(const malla_t *malla, size_t id_masa) {
    lista_iter_t *iter_masas = lista_iter_crear(malla->masas);
    while(!lista_iter_al_final(iter_masas)) {
        masa_t *masa = lista_iter_ver_actual(iter_masas);
        if(masa->id == id_masa) {
            return masa;
        }

        lista_iter_avanzar(iter_masas);
    }
    return NULL;
}

masa_t *nueva_masa_fija(malla_t *malla, float x, float y, float tam, Color color){
    masa_t *masa = crear_masa_fija(x, y, tam, color);
    insertar_masa(malla, masa);
    return masa;
}

masa_t *detectar_masa(malla_t *malla, float x, float y, float tolerancia) {
    lista_iter_t *iter = lista_iter_crear(malla->masas);
    while (!lista_iter_al_final(iter)) {
        masa_t *masa_actual = lista_iter_ver_actual(iter);
        float distancia_x = masa_actual->x - x;
        float distancia_y = masa_actual->y - y;

        if (distancia_x < 0) {
            distancia_x *= -1;
        }
        if (distancia_y < 0) {
            distancia_y *= -1;
        }
        
        if (distancia_x <= tolerancia && distancia_y <= tolerancia) {
            lista_iter_destruir(iter);
            return masa_actual;
        }
        lista_iter_avanzar(iter);
    }
    lista_iter_destruir(iter);
    return NULL;
}

masa_t *obtener_masa(malla_t *malla, float x, float y) {
    lista_iter_t *iter = lista_iter_crear(malla->masas);
    while (!lista_iter_al_final(iter)){
        masa_t *masa_actual = lista_iter_ver_actual(iter);
        if(x == masa_actual->x && y == masa_actual->y){
            lista_iter_destruir(iter);
            return masa_actual;
        }
        lista_iter_avanzar(iter);
    }
    lista_iter_destruir(iter);
    return NULL;
}

void mover_masa(malla_t *malla, masa_t *masa, float x, float y, float longitud_maxima) {
    lista_iter_t *iter = lista_iter_crear(malla->masas);
    if (iter == NULL) return;
    
    while (!lista_iter_al_final(iter)) {
        masa_t *masa_actual = lista_iter_ver_actual(iter);
        
        if (masa_actual->id == masa->id) {
            // Guardar la posición anterior de la masa
            float x_anterior = masa_actual->x;
            float y_anterior = masa_actual->y;
            
            // Actualizar la posición de la masa
            masa_actual->x = x;
            masa_actual->y = y;
            
            if (!verificar_longitud_resortes(malla, masa_actual, longitud_maxima)) {
                // Restaurar la posición anterior de la masa si la longitud de algún resorte se excede
                masa_actual->x = x_anterior;
                masa_actual->y = y_anterior;
            }
            
            break;
        }
        
        lista_iter_avanzar(iter);
    }
    
    lista_iter_destruir(iter);
}

resorte_t *detectar_resorte(malla_t *malla, float x, float y, float tolerancia) {
    lista_iter_t *iter = lista_iter_crear(malla->resortes);
    while (!lista_iter_al_final(iter)) {
        resorte_t *resorte_actual = lista_iter_ver_actual(iter);
        float xp1 = resorte_actual->masa1->x;
        float yp1 = resorte_actual->masa1->y;
        float xp2 = resorte_actual->masa2->x;
        float yp2 = resorte_actual->masa2->y;
        
        // Calcular la distancia del punto a la recta
        float distancia = distancia_punto_a_recta(xp1, yp1, xp2, yp2, x, y);

        // Si la distancia es menor a una tolerancia, se considera que el punto está cerca del resorte
        if (distancia < tolerancia) {
            // Verificar si el punto se encuentra entre las masas
            float x_min = xp1 < xp2 ? xp1 : xp2;
            float x_max = xp1 > xp2 ? xp1 : xp2;
            float y_min = yp1 < yp2 ? yp1 : yp2;
            float y_max = yp1 > yp2 ? yp1 : yp2;

            if (x >= x_min && x <= x_max && y >= y_min && y <= y_max) {
                lista_iter_destruir(iter);
                return resorte_actual;
            }
        }

        lista_iter_avanzar(iter);
    }

    lista_iter_destruir(iter);
    return NULL;
}

void obtener_masas_resorte(resorte_t *resorte, masa_t **mj, masa_t **mk){
    *mj = resorte->masa1;
    *mk = resorte->masa2;
}

void borrar_resorte(malla_t *malla, resorte_t *resorte) {
    lista_iter_t *iter = lista_iter_crear(malla->resortes);
    while (!lista_iter_al_final(iter)) {
        resorte_t *resorte_actual = lista_iter_ver_actual(iter);
        if (resorte_actual == resorte) {
            _borrar_resorte(resorte_actual);
            lista_iter_borrar(iter);
            lista_iter_destruir(iter);
            return;
        }
        lista_iter_avanzar(iter);
    }
    lista_iter_destruir(iter);
}                           

void borrar_resortes_conectados(malla_t *malla, masa_t *masa) {
    lista_iter_t *iter_resortes = lista_iter_crear(malla->resortes);
    if (iter_resortes == NULL) return;

    while (!lista_iter_al_final(iter_resortes)) {
        resorte_t *resorte = lista_iter_ver_actual(iter_resortes);
        
        if (resorte->masa1 == masa || resorte->masa2 == masa) {
            _borrar_resorte(resorte);
            lista_iter_borrar(iter_resortes);
        } else {
            lista_iter_avanzar(iter_resortes);
        }
    }
    
    lista_iter_destruir(iter_resortes);
}                          

void nuevo_resorte(malla_t *malla, masa_t *m1, masa_t *m2, Color color){
    resorte_t *resorte = crear_resorte(m1, m2, color);
    insertar_resorte(malla, resorte);
}

static resorte_t *_copiar_resorte(const malla_t *malla_copia, resorte_t *resorte, size_t id_masa1_copia, size_t id_masa2_copia) {
    
    resorte_t *resorte_copia = crear_resorte(resorte->masa1, resorte->masa2, resorte->color);
    resorte_copia->id = resorte->id;
    resorte_copia->longitud = resorte->longitud;
    masa_t *masa1_copia = obtener_masa_id(malla_copia, id_masa1_copia);
    resorte_copia->masa1 = masa1_copia;
    masa_t *masa2_copia = obtener_masa_id(malla_copia, id_masa2_copia);
    resorte_copia->masa2 = masa2_copia;
    resorte_copia->k_resorte = resorte->k_resorte;

    return resorte_copia;
}

bool masas_conectadas(malla_t *malla, masa_t *m1, masa_t *m2) {
    lista_iter_t *iter = lista_iter_crear(malla->resortes);
    if (iter == NULL) return false;

    while (!lista_iter_al_final(iter)) {
        resorte_t *resorte = lista_iter_ver_actual(iter);
        masa_t *masa_a = resorte->masa1;
        masa_t *masa_b = resorte->masa2;

        if ((masa_a == m1 && masa_b == m2) || (masa_a == m2 && masa_b == m1)) {
            lista_iter_destruir(iter);
            return true; // Las masas están conectadas por un resorte
        }

        lista_iter_avanzar(iter);
    }

    lista_iter_destruir(iter);
    return false; // Las masas no están conectadas por un resorte
}

bool excede_max_longitud(malla_t *malla, masa_t *masa, float x, float y, float maxima_longitud) {
    float distancia = distancia_puntos(masa->x, masa->y, x, y);
    return (distancia > maxima_longitud);
}



size_t obtener_cantidad_masas(const malla_t* malla) {
    return lista_largo(malla->masas);
}

size_t obtener_cantidad_resortes(const malla_t* malla){
    return lista_largo(malla->resortes);
}

malla_t *crear_malla() {
    malla_t *nueva_malla = malloc(sizeof(malla_t));
    if (nueva_malla == NULL) {
        return NULL;
    }

    nueva_malla->resortes = lista_crear();
    if (nueva_malla->resortes == NULL) {
        free(nueva_malla);
        return NULL;
    }

    nueva_malla->masas = lista_crear();
    if (nueva_malla->masas == NULL) {
        lista_destruir(nueva_malla->resortes, NULL);
        free(nueva_malla);
        return NULL;
    }

    return nueva_malla;
}

bool copiar_malla(const malla_t *malla, malla_t *malla_copia) {

    lista_iter_t *iter_masas = lista_iter_crear(malla->masas);
    while (!lista_iter_al_final(iter_masas)) {
        masa_t *masa = lista_iter_ver_actual(iter_masas);
        masa_t *masa_copia = _copiar_masa(masa);
        lista_insertar_ultimo(malla_copia->masas, masa_copia);
        lista_iter_avanzar(iter_masas);
    }

    masa_t *mj, *mk;
    lista_iter_t *iter_resortes = lista_iter_crear(malla->resortes);
    while (!lista_iter_al_final(iter_resortes)) {
        resorte_t *resorte = lista_iter_ver_actual(iter_resortes);
        obtener_masas_resorte(resorte, &mj, &mk);   
        
        resorte_t *resorte_copia = _copiar_resorte(malla_copia, resorte, mj->id, mk->id);
        lista_insertar_ultimo(malla_copia->resortes, resorte_copia);

        lista_iter_avanzar(iter_resortes);
    }

    lista_iter_destruir(iter_masas);
    lista_iter_destruir(iter_resortes);

    return true;
}

void reordenar_malla(const malla_t *malla) {
    lista_iter_t *iter = lista_iter_crear(malla->masas);
    size_t cant_masas = obtener_cantidad_masas(malla);
    for(size_t i = 0; i < cant_masas; i++) {
        masa_t *masa = lista_iter_ver_actual(iter);
        masa->id = i;
        lista_iter_avanzar(iter);
    }
    lista_iter_destruir(iter);
}

void reordenar_id(const malla_t *malla) {
    lista_iter_t *iter_masas = lista_iter_crear(malla->masas);
    size_t cant_masas = obtener_cantidad_masas(malla);
    for(size_t i = 0; i < cant_masas; i++) {
        masa_t *masa = lista_iter_ver_actual(iter_masas);
        masa->id = i;
        
        lista_iter_avanzar(iter_masas);
    }
    lista_iter_destruir(iter_masas);

    lista_iter_t *iter_resortes = lista_iter_crear(malla->resortes);
    size_t cant_resortes = obtener_cantidad_resortes(malla);
    
    for(size_t i = 0; i < cant_resortes; i++) {
        resorte_t *resorte = lista_iter_ver_actual(iter_resortes);
        resorte->id = i;
        
        lista_iter_avanzar(iter_resortes);
    }
    lista_iter_destruir(iter_resortes);
}

size_t buscar_id_resorte(resorte_t *resorte) {
    return resorte->id;
}

size_t buscar_id_masa(masa_t *masa) {
    return masa->id;
}

masa_t *buscar_masa_id(malla_t *malla, size_t id_masa) {
    lista_iter_t *iter = lista_iter_crear(malla->masas);
    while(!lista_iter_al_final(iter)) {
        masa_t *masa = lista_iter_ver_actual(iter);
        if(masa->id == id_masa) {
            lista_iter_destruir(iter);
            return masa;
        }
        lista_iter_avanzar(iter);
    }

    lista_iter_destruir(iter);
    return NULL;
}

void destruir_malla(malla_t *malla) {
    lista_destruir(malla->masas, _borrar_masa);
    lista_destruir(malla->resortes, _borrar_resorte);
    free(malla);
}