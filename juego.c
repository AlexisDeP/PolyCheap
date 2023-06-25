#include "malla.h"
#include "juego.h"
#include "color.h"
#include "lista.h"

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


void inicializar_nivel(malla_t *malla, int nivel) {
    
    masa_t *masa_A = crear_masa_fija(100 / 50, 400 / 50, 22 / 50.0, COLOR_BLANCO);
    masa_t *masa_B = crear_masa_fija((nivel * 100) / 50, 400 / 50, 22 / 50.0, COLOR_BLANCO);

    insertar_masa(malla, masa_A);
    insertar_masa(malla, masa_B);
    
}

static bool leer_numero_de_datos(FILE *f, size_t *datos) {

	if (fread(datos, sizeof(size_t), 1, f) != 1) return false;

	return true;

}

static bool leer_masa(FILE *f, size_t *id, float *x, float *y, float *tam, bool *es_fijo, float *masa, Color *color) {

	if (fread(id, sizeof(size_t), 1, f) != 1 
		|| fread(x, sizeof(float), 1, f) != 1
		|| fread(y, sizeof(float), 1, f) != 1 
		|| fread(tam, sizeof(float), 1, f) != 1
        || fread(es_fijo, sizeof(bool), 1, f) != 1
        || fread(masa, sizeof(float), 1, f) != 1
        || fread(color, sizeof(Color), 1, f) != 1) 
        return false;

	return true;

}

static bool leer_resorte(FILE *f, size_t *id, size_t *id_masa_1, size_t *id_masa_2, float *longitud, Color *color) {

	if (fread(id, sizeof(size_t), 1, f) != 1 
		|| fread(id_masa_1, sizeof(size_t), 1, f) != 1
		|| fread(id_masa_2, sizeof(size_t), 1, f) != 1 
		|| fread(longitud, sizeof(float), 1, f) != 1
        || fread(color, sizeof(Color), 1, f) != 1) 
        return false;

	return true;

}

static bool escribir_numero_de_datos(FILE *f, size_t datos) {
	
	if (fwrite(&datos, sizeof(size_t), 1, f) != 1) return false;

	return true;

}

static bool escribir_masa(FILE *f, size_t id, float x, float y, float tam, bool es_fijo, float masa, Color color) {

	if (fwrite(&id, sizeof(size_t), 1, f) != 1 
		|| fwrite(&x, sizeof(float), 1, f) != 1 
		|| fwrite(&y, sizeof(float), 1, f) != 1 
        || fwrite(&tam, sizeof(float), 1, f) != 1
		|| fwrite(&es_fijo, sizeof(bool), 1, f) != 1
        || fwrite(&masa, sizeof(float), 1, f) != 1
        || fwrite(&color, sizeof(Color), 1, f) != 1) 
        return false;
    
	return true;

}

static bool escribir_resorte(FILE *f, size_t id, size_t id_masa_1, size_t id_masa_2, float longitud, Color color) {

	if (fwrite(&id, sizeof(size_t), 1, f) != 1 
		|| fwrite(&id_masa_1, sizeof(size_t), 1, f) != 1
		|| fwrite(&id_masa_2, sizeof(size_t), 1, f) != 1 
		|| fwrite(&longitud, sizeof(float), 1, f) != 1
        || fwrite(&color, sizeof(Color), 1, f) != 1) 
        return false;

	return true;

}

bool guardar_nivel(FILE *f, malla_t *malla) {

    size_t cant_masas = obtener_cantidad_masas(malla);
	if(!escribir_numero_de_datos(f, cant_masas)) return false;
    printf("NM: %zd\n", cant_masas);

    lista_iter_t *iter_masas = lista_iter_crear(malla->masas);
    while(!lista_iter_al_final(iter_masas)) {

        masa_t *masa = lista_iter_ver_actual(iter_masas);
        if(!escribir_masa(f, masa->id, masa->x, masa->y, masa->tam, masa->es_fijo, masa->masa, masa->color)) {
            lista_iter_destruir(iter_masas);
            return false;
        }
        printf("\tID:%zd x: %f, y: %f, tam: %f, es_fijo: %d, masa: %f\n", masa->id, masa->x, masa->y, masa->tam, masa->es_fijo, masa->masa);
        lista_iter_avanzar(iter_masas);

    }
    lista_iter_destruir(iter_masas);

    size_t cant_resortes = obtener_cantidad_resortes(malla);
	if(!escribir_numero_de_datos(f, cant_resortes)) return false;
    printf("NM: %zd\n", cant_resortes);

    lista_iter_t *iter_resortes = lista_iter_crear(malla->resortes);
    while(!lista_iter_al_final(iter_resortes)) {

        resorte_t *resorte = lista_iter_ver_actual(iter_resortes);
        if(!escribir_resorte(f, resorte->id, resorte->masa1->id, resorte->masa2->id, resorte->longitud, resorte->color)) {
            lista_iter_destruir(iter_resortes);
            return false;
        }
        printf("\tID:%zd x: %zd, y: %zd, longitud: %f\n", resorte->id, resorte->masa1->id, resorte->masa2->id, resorte->longitud);
        lista_iter_avanzar(iter_resortes);

    }
    lista_iter_destruir(iter_resortes);

    return true;

}

bool abrir_nivel(FILE *f, malla_t *malla) {

    size_t cant_masas = 0;
	if (!leer_numero_de_datos(f, &cant_masas)) return false;

    size_t id = 0;
    float x = 0;
	float y = 0;
	float tam = 0;
	bool es_fijo = 0;
    float masa_m = 0;
    Color color = 0;  
    for(size_t i = 0; i < cant_masas; i++) {
        
        if (!leer_masa(f, &id, &x, &y, &tam, &es_fijo, &masa_m, &color)) return false;
        masa_t *masa = nueva_masa(malla, x, y, tam, color);
        masa->es_fijo = es_fijo;
        masa->id = id;
        masa->masa = masa_m;
    }

    size_t cant_resortes = 0;
	if (!leer_numero_de_datos(f, &cant_resortes)) return false;

    size_t id_resorte = 0;
    size_t id_masa_1 = 0;
	size_t id_masa_2 = 0;
	float longitud_resorte = 0;
    Color color_resorte = 0;  
    for (size_t i = 0; i < cant_resortes; i++) {
        
        if (!leer_resorte(f, &id_resorte, &id_masa_1, &id_masa_2, &longitud_resorte, &color_resorte)) return false;
        masa_t *masa_1 = buscar_masa_id(malla, id_masa_1);
        masa_t *masa_2 = buscar_masa_id(malla, id_masa_2);
        nuevo_resorte(malla, masa_1, masa_2, color_resorte);
    }

    return true;

}