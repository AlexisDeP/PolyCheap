#include <stdbool.h>

#include "masa.h"

struct masa {
    size_t id;
    int x, y, tam;           // FLOAT O INT???
    bool es_fijo;
    float masa;
    Color color;
};

static masa_t *_crear_masa(){
    masa_t *masa = malloc(sizeof(masa_t));
    if(masa == NULL) return NULL;
    return masa;
}

masa_t *crear_masa(int x, int y, int tam, Color color){
    masa_t *nueva_masa = _crear_masa();
    if (nueva_masa == NULL) return NULL;
    
    nueva_masa->id = 0;
    nueva_masa->x = x;
    nueva_masa->y = y;
    nueva_masa->tam = tam;
    nueva_masa->es_fijo = false;
    nueva_masa->masa = 0.0;
    nueva_masa->color = color;
    
    return nueva_masa;
}

void _borrar_masa(masa_t *masa){
    free(masa);
}

masa_t *crear_masa_fija(int x, int y, int tam, Color color){
    masa_t *nueva_masa = _crear_masa();
    if (nueva_masa == NULL) return NULL;
    
    nueva_masa->id = 0;
    nueva_masa->x = x;
    nueva_masa->y = y;
    nueva_masa->tam = tam;
    nueva_masa->es_fijo = true;
    nueva_masa->masa = 0.0;
    nueva_masa->color = color;
    
    return nueva_masa;
}

bool es_fija(masa_t *masa){
    return masa->es_fijo;
}

void cambiar_color_masa(masa_t *masa, Color color_masa, Color color_masa_fija) {
    masa->color = masa->es_fijo ? color_masa_fija : color_masa;
}

float distancia_entre_masas(masa_t *m1, masa_t *m2) {
    int dx = m2->x - m1->x;
    int dy = m2->y - m1->y;
    return sqrt(dx*dx + dy*dy);
}

