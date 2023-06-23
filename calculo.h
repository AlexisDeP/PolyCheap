#ifndef CALCULO_H
#define CALCULO_H

//Compara la distancia entre dos puntos (x, y)
//Post: Devuelve true en caso de que la distancia entre p1 y p2 este de un rango de toleracia, false en caso contrario.
bool comparar_puntos(int x1, int y1, int x2, int y2, int tolerancia);

// Devuelve la distancia entre dos puntos (x, y)
float distancia_puntos(int x1, int y1, int x2, int y2);

// Devuelve la norma de un vector R2
float norma(float x, float y);

// Devuelve la distancia mas cercana de un punto (x, y) a una recta (creada por dos puntos)
float distancia_punto_a_recta(int xp1, int yp1, int xp2, int yp2, int x_punto, int y_punto);

float Aj(float m, float dt, float b);

float Bj(float bj, float sumatoria_bj);

float bj(float m, float dt, float b, float g, int pos1, int pos2);

float k(float kb, float l0, float pk);

float _sumatoria_bj(float kb, float pk, float pj1, float pk1, float rl0, float rl1);

#endif