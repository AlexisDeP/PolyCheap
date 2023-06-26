#include <stdbool.h>
#include <math.h>
#include <stdlib.h>

#include "calculo.h"

bool comparar_puntos(float x1, float y1, float x2, float y2, float tolerancia){

    float diferencia_x = abs(x1 - x2);
    float diferencia_y = abs(y1 - y2);

    if (diferencia_x <= tolerancia && diferencia_y <= tolerancia) return true;

    return false;
}

float distancia_puntos(float x1, float y1, float x2, float y2) {
    float dx = x2 - x1;
    float dy = y2 - y1;
    return sqrt(dx * dx + dy * dy);
}                                                       //Quizas reemplazable con:

float distancia_punto_a_recta(float xp1, float yp1, float xp2, float yp2, float x_punto, float y_punto){
    
    float numerador = abs((yp2 - yp1) * x_punto - (xp2 - xp1) * y_punto + xp2 * yp1 - yp2 * xp1);
    float denominador = sqrt((yp2 - yp1) * (yp2 - yp1) + (xp2 - xp1) * (xp2 - xp1));

    return numerador / denominador;
}

float Aj(float m, float dt, float b) {
    return ((m/(dt*dt))+(b/dt));
}

float Bj(float bj, float sumatoria_bj) {
    return (bj + sumatoria_bj);
}

float bj(float m, float dt, float b, float g, float pos1, float pos2) {
    return (((m/(dt*dt))*((2*pos1) - pos2))+((b/dt)*pos1) + (m*g));
}

float k(float kb, float l0, float pk) {
    return (kb/pow(l0, pk));
}

float _sumatoria_bj(float kb, float pk, float pj1, float pk1, float rl0, float rl1) {
    return k(kb, rl0, pk) * ((rl0 - rl1) / rl1) * (pj1 - pk1);
}
