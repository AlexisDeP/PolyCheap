#include <stdbool.h>
#include <math.h>
#include <stdlib.h>

#include "calculo.h"


bool comparar_puntos(int x1, int y1, int x2, int y2, int tolerancia){

    int diferencia_x = abs(x1 - x2);
    int diferencia_y = abs(y1 - y2);

    if (diferencia_x <= tolerancia && diferencia_y <= tolerancia) return true;

    return false;
}

float distancia_puntos(int x1, int y1, int x2, int y2) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    return sqrt(dx * dx + dy * dy);
}                                                       //Quizas reemplazable con:

float norma(float x, float y) {
    return sqrt(x * x + y * y);
}   

float distancia_punto_a_recta(int xp1, int yp1, int xp2, int yp2, int x_punto, int y_punto){
    
    float numerador = abs((yp2 - yp1) * x_punto - (xp2 - xp1) * y_punto + xp2 * yp1 - yp2 * xp1);
    float denominador = sqrt((yp2 - yp1) * (yp2 - yp1) + (xp2 - xp1) * (xp2 - xp1));

    return numerador / denominador;
}
