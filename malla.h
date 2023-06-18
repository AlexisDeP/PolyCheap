#include <stdbool.h>

#include "masa.h"
#include "resorte.h"

typedef struct malla malla_t;

//malla_t *_crear_malla();

bool borrar_masa(malla_t *malla, masa_t *masa);

bool insertar_masa(malla_t *malla, masa_t *masa);

masa_t *nueva_masa(malla_t *malla, int x, int y, int tam, Color color);

masa_t *nueva_masa_fija(malla_t *malla, int x, int y, int tam, Color color);

masa_t *detectar_masa(malla_t *malla, int x, int y, int tolerancia);

bool verificar_longitud_resortes(malla_t *malla, masa_t *masa, float longitud_maxima);

void mover_masa(malla_t *malla, masa_t *masa, int x, int y, float longitud_maxima);

resorte_t *detectar_resorte(malla_t *malla, int x, int y, float tolerancia);

bool borrar_resorte(malla_t *malla, resorte_t *resorte);

void eliminar_resortes_conectados(malla_t *malla, masa_t *masa);

bool insertar_resorte(malla_t *malla, resorte_t *resorte);

resorte_t *nuevo_resorte(malla_t *malla, masa_t *m1, masa_t *m2, Color color);

bool masas_conectadas(malla_t *malla, masa_t *m1, masa_t *m2);

bool excede_max_longitud(malla_t *malla, masa_t *masa, int x, int y, float maxima_longitud);



malla_t *crear_malla();