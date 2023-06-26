#include <stdbool.h>

#include "resorte.h"
#include "color.h"
#include "masa.h"

struct resorte {
    size_t id;
    masa_t *masa1, *masa2;
    float longitud, k_resorte;
    Color color;
};

static resorte_t *_crear_resorte(){
    
    resorte_t *resorte = malloc(sizeof(resorte_t));
    if(resorte == NULL) return NULL;
    return resorte;
}

masa_t *obtener_masa1_resorte(resorte_t *resorte) {
    return resorte->masa1;
}

masa_t *obtener_masa2_resorte(resorte_t *resorte) {
    return resorte->masa2;
}

Color obtener_color_resorte(resorte_t *resorte) {
    return resorte->color;
}

size_t obtener_id_resorte(resorte_t *resorte) {
    return resorte->id;
}

float obtener_longitud_resorte(resorte_t *resorte) {
    return resorte->longitud;
}

float obtener_k_resorte(resorte_t *resorte) {
    return resorte->k_resorte;
}

void cambiar_id_resorte(resorte_t *resorte, size_t id) {
    resorte->id = id;
}

void cambiar_longitud_resorte(resorte_t *resorte, float longitud) {
    resorte->longitud = longitud;
}

void cambiar_masa1_resorte(resorte_t *resorte, masa_t *masa1) {
    resorte->masa1 = masa1;
}

void cambiar_masa2_resorte(resorte_t *resorte, masa_t *masa2) {
    resorte->masa2 = masa2;
}

void cambiar_k_resorte(resorte_t *resorte, float k_resorte) {
    resorte->k_resorte = k_resorte;
}

void cambiar_color_resorte(resorte_t *resorte, Color color) {
    resorte->color = color;
}

resorte_t *crear_resorte(masa_t *masa1, masa_t *masa2, Color color) {
    resorte_t *resorte = _crear_resorte();
    if (resorte == NULL) return NULL;
    
    resorte->id = 0;  // Asigna un ID adecuado según tus necesidades
    resorte->masa1 = masa1;
    resorte->masa2 = masa2;
    resorte->longitud = distancia_entre_masas(masa1, masa2); 
    resorte->k_resorte = 0;
    resorte->color = color;  // Define el color por defecto
    
    return resorte;
}  



void _borrar_resorte(void *resorte){
    free(resorte);
}