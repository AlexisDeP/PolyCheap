#ifndef CALCULO_H
#define CALCULO_H

//Compara la distancia entre dos puntos (x, y)
//Post: Devuelve true en caso de que la distancia entre p1 y p2 este de un rango de toleracia, false en caso contrario.
bool comparar_puntos(float x1, float y1, float x2, float y2, float tolerancia);

// Devuelve la distancia entre dos puntos (x, y)
float distancia_puntos(float x1, float y1, float x2, float y2);

// Devuelve la distancia mas cercana de un punto (x, y) a una recta (creada por dos puntos)
float distancia_punto_a_recta(float xp1, float yp1, float xp2, float yp2, float x_punto, float y_punto);

float Aj(float m, float dt, float b);

float Bj(float bj, float sumatoria_bj);

float bj(float m, float dt, float b, float g, float pos1, float pos2);

float k(float kb, float l0, float pk);

float _sumatoria_bj(float kb, float pk, float pj1, float pk1, float rl0, float rl1);

#endif