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



void _borrar_resorte(resorte_t *resorte){
    free(resorte);
}