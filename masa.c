#include <stdbool.h>

#include "masa.h"

struct masa {
    size_t id;
    float x, y, tam;       
    bool es_fijo;
    float masa;
    Color color;
};

static masa_t *_crear_masa(){
    masa_t *masa = malloc(sizeof(masa_t));
    if(masa == NULL) return NULL;
    return masa;
}

masa_t *crear_masa(float x, float y, float tam, Color color){
    masa_t *masa = _crear_masa();
    if (masa == NULL) return NULL;
    
    masa->id = 0;
    masa->x = x;
    masa->y = y;
    masa->tam = tam;
    masa->es_fijo = false;
    masa->masa = 0.0;
    masa->color = color;
    
    return masa;
}

void _borrar_masa(masa_t *masa){
    free(masa);
}

masa_t *crear_masa_fija(float x, float y, float tam, Color color){
    masa_t *masa = _crear_masa();
    if (masa == NULL) return NULL;
    
    masa->id = 0;
    masa->x = x;
    masa->y = y;
    masa->tam = tam;
    masa->es_fijo = true;
    masa->masa = 0.0;
    masa->color = color;
    
    return masa;
}

bool es_fija(masa_t *masa){
    return masa->es_fijo;
}

void cambiar_color_masa(masa_t *masa, Color color_masa, Color color_masa_fija) {
    masa->color = masa->es_fijo ? color_masa_fija : color_masa;
}

masa_t *_copiar_masa(const masa_t *masa) {
    masa_t *masa_copia = crear_masa(masa->x, masa->y, masa->tam, masa->color);
    masa_copia->id = masa->id;
    masa_copia->es_fijo = masa->es_fijo;
    masa_copia->masa = masa->masa;

    return masa_copia;
}

float distancia_entre_masas(masa_t *m1, masa_t *m2) {
    float dx = m2->x - m1->x;
    float dy = m2->y - m1->y;
    return sqrt(dx*dx + dy*dy);
}

