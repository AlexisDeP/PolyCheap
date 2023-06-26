#include <stdbool.h>

#include "masa.h"
#include "resorte.h"
#include "calculo.h"
#include "lista.h"
#include "malla.h"

struct malla {
    lista_t* resortes;
    lista_t* masas;
};

void insertar_masa(malla_t *malla, masa_t *masa) {

    cambiar_id_masa(masa, obtener_cantidad_masas(malla));
    lista_insertar_ultimo(malla->masas, masa);
}

lista_t* obtener_lista_masas_malla(struct malla* malla) {
    return malla->masas;
}

lista_t* obtener_lista_resortes_malla(struct malla* malla) {
    return malla->resortes;
}

static bool verificar_longitud_resortes(malla_t *malla, masa_t *masa, float longitud_maxima) {
    lista_iter_t *iter = lista_iter_crear(malla->resortes);
    if (iter == NULL) return false;
    
    while (!lista_iter_al_final(iter)) {
        resorte_t *resorte = lista_iter_ver_actual(iter);
        masa_t *masa1 = obtener_masa1_resorte(resorte);
        masa_t *masa2 = obtener_masa2_resorte(resorte);
        
        if (masa1 == masa || masa2 == masa) {
            float longitud = distancia_puntos(obtener_x_masa(masa1), obtener_y_masa(masa1), obtener_x_masa(masa2), obtener_y_masa(masa2));
            
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
    cambiar_id_resorte(resorte, obtener_cantidad_resortes(malla));
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
        if(obtener_id_masa(masa) == id_masa) {
            lista_iter_destruir(iter_masas);
            return masa;
        }

        lista_iter_avanzar(iter_masas);
    }
    lista_iter_destruir(iter_masas);
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
        float distancia_x = obtener_x_masa(masa_actual) - x;
        float distancia_y = obtener_y_masa(masa_actual) - y;

        if (distancia_x < 0) distancia_x *= -1;
        if (distancia_y < 0) distancia_y *= -1;
        
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
        if(x == obtener_x_masa(masa_actual) && y == obtener_y_masa(masa_actual)){
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
        
        if (obtener_id_masa(masa_actual) == obtener_id_masa(masa)) {
            // Guardar la posición anterior de la masa
            float x_anterior = obtener_x_masa(masa_actual);
            float y_anterior = obtener_y_masa(masa_actual);
            
            // Actualizar la posición de la masa
            cambiar_x_masa(masa_actual, x);
            cambiar_y_masa(masa_actual, y);
            
            if (!verificar_longitud_resortes(malla, masa_actual, longitud_maxima)) {
                // Restaurar la posición anterior de la masa si la longitud de algún resorte se excede
                cambiar_x_masa(masa_actual, x_anterior);
                cambiar_y_masa(masa_actual, y_anterior);
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
        float xp1 = obtener_x_masa(obtener_masa1_resorte(resorte_actual));
        float yp1 = obtener_y_masa(obtener_masa1_resorte(resorte_actual));
        float xp2 = obtener_x_masa(obtener_masa2_resorte(resorte_actual));
        float yp2 = obtener_y_masa(obtener_masa2_resorte(resorte_actual));
        
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
    *mj = obtener_masa1_resorte(resorte);
    *mk = obtener_masa2_resorte(resorte);
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

    while (!lista_iter_al_final(iter_resortes)) {
        resorte_t *resorte = lista_iter_ver_actual(iter_resortes);
        
        if (obtener_masa1_resorte(resorte) == masa || obtener_masa2_resorte(resorte) == masa) {
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
    
    resorte_t *resorte_copia = crear_resorte(obtener_masa1_resorte(resorte), obtener_masa2_resorte(resorte), obtener_color_resorte(resorte));
    cambiar_id_resorte(resorte_copia, obtener_id_resorte(resorte));
    cambiar_longitud_resorte(resorte_copia, obtener_longitud_resorte(resorte));
    masa_t *masa1_copia = obtener_masa_id(malla_copia, id_masa1_copia);
    cambiar_masa1_resorte(resorte_copia, masa1_copia);
    masa_t *masa2_copia = obtener_masa_id(malla_copia, id_masa2_copia);
    cambiar_masa2_resorte(resorte_copia, masa2_copia);
    cambiar_k_resorte(resorte_copia, obtener_k_resorte(resorte));

    return resorte_copia;
}

bool masas_conectadas(malla_t *malla, masa_t *m1, masa_t *m2) {
    lista_iter_t *iter = lista_iter_crear(malla->resortes);
    if (iter == NULL) return false;

    while (!lista_iter_al_final(iter)) {
        resorte_t *resorte = lista_iter_ver_actual(iter);
        masa_t *masa_a = obtener_masa1_resorte(resorte);
        masa_t *masa_b = obtener_masa2_resorte(resorte);

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
    float distancia = distancia_puntos(obtener_x_masa(masa), obtener_y_masa(masa), x, y);
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

void copiar_malla(const malla_t *malla, malla_t *malla_copia) {

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
        resorte_t *resorte_copia = _copiar_resorte(malla_copia, resorte, obtener_id_masa(mj), obtener_id_masa(mk));
        lista_insertar_ultimo(malla_copia->resortes, resorte_copia);

        lista_iter_avanzar(iter_resortes);
    }

    lista_iter_destruir(iter_masas);
    lista_iter_destruir(iter_resortes);
}

void reordenar_malla(const malla_t *malla) {
    lista_iter_t *iter = lista_iter_crear(malla->masas);
    size_t cant_masas = obtener_cantidad_masas(malla);
    for(size_t i = 0; i < cant_masas; i++) {
        masa_t *masa = lista_iter_ver_actual(iter);
        cambiar_id_masa(masa, i);
        lista_iter_avanzar(iter);
    }
    lista_iter_destruir(iter);
}

void reordenar_id(const malla_t *malla) {
    lista_iter_t *iter_masas = lista_iter_crear(malla->masas);
    size_t cant_masas = obtener_cantidad_masas(malla);
    for(size_t i = 0; i < cant_masas; i++) {
        masa_t *masa = lista_iter_ver_actual(iter_masas);
        cambiar_id_masa(masa, i);
        lista_iter_avanzar(iter_masas);
    }
    lista_iter_destruir(iter_masas);

    lista_iter_t *iter_resortes = lista_iter_crear(malla->resortes);
    size_t cant_resortes = obtener_cantidad_resortes(malla);
    for(size_t i = 0; i < cant_resortes; i++) {
        resorte_t *resorte = lista_iter_ver_actual(iter_resortes);
        cambiar_id_resorte(resorte, i);
        lista_iter_avanzar(iter_resortes);
    }
    lista_iter_destruir(iter_resortes);
}

masa_t *buscar_masa_id(malla_t *malla, size_t id_masa) {
    lista_iter_t *iter = lista_iter_crear(malla->masas);
    while(!lista_iter_al_final(iter)) {
        masa_t *masa = lista_iter_ver_actual(iter);
        if(obtener_id_masa(masa) == id_masa) {
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