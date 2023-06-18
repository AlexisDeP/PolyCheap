#include "malla.h"

//Inicializa un nivel.
//Pre: La malla existe
//     El nivel debe ser entre 1 y 10
//Post: Se crean dos masas fijas dentro de la malla.
void inicializar_nivel(malla_t *malla, int nivel);

//Guarda en un archivo binario la malla dibujada.
//Pre: La malla existe.
//Post: Crea un archivo "*.bin".
bool guardar_nivel(malla_t *malla);

//Abre un nivel guardado en un archivo "*.bin" y lo guarda en la malla.
//Pre: El archivo existe.
//     La malla existe.
//Post: El nivel se copia en la malla.
bool abrir_nivel(FILE *f, malla_t *malla);