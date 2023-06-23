#include <stdbool.h>

#include "resorte.h"
#include "color.h"

struct resorte {
    size_t id;
    void *objeto1, *objeto2;
    float longitud, k_resorte;
    Color color;
};

static resorte_t *_crear_resorte(){
    
    resorte_t *resorte = malloc(sizeof(resorte_t));
    if(resorte == NULL) return NULL;
    return resorte;
}

resorte_t *crear_resorte(void *o1, void *o2, Color color) {
    resorte_t *resorte = _crear_resorte();
    if (resorte == NULL) return NULL;
    
    resorte->id = 0;  // Asigna un ID adecuado según tus necesidades
    resorte->objeto1 = o1;
    resorte->objeto2 = o2;
    resorte->longitud = 0; //distancia_entre_masas(o1, o2) // Implementa una función para calcular la distancia entre masas, me rehuso
    resorte->k_resorte = 0;
    resorte->color = color;  // Define el color por defecto
    
    return resorte;
}  

resorte_t *_copiar_resorte(const resorte_t *resorte) {

    resorte_t *resorte_copia = crear_resorte(resorte->objeto1, resorte->objeto2, resorte->color);
    resorte_copia->id = resorte->id;
    resorte_copia->longitud = resorte->longitud;

    resorte_copia->k_resorte = resorte->k_resorte;

    return resorte_copia;
}

void _borrar_resorte(resorte_t *resorte){
    free(resorte);
}