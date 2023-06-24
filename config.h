#ifndef CONFIG_H
#define CONFIG_H

#define VENTANA_ANCHO 1080
#define VENTANA_ALTO 720

#define JUEGO_FPS 60
#define DT 0.0005
#define DURACION_SIMULACION 10.0

#define G 9.81                      // Gravedad
#define G_JUPITER 24.79
#define G_MARTE 3.71
#define G_LUNA 1.62
#define B 1.0                       // Coeficiente de amortiguación

#define TAM 22                      // Tamano masa

#define COLOR_CONSTRUCCION COLOR_ROJO
#define COLOR_MASA COLOR_VIOLETA
#define COLOR_MASA_FIJA COLOR_VERDE
#define COLOR_RESORTE COLOR_CELESTE

#define TOL_MASAS TAM/2         // Tolerancia de error al clickear un objeto
#define TOL_RESORTES 8.0

#define TOL_CLIC 3

#define LO_MAX 150                  // Máxima longitud de un resorte
#define FACTOR_ESCALA 50.            // Conversión entre coordenadas de pantalla y mundo

#define MAXIMO_ESTIRAMIENTO 0.13    // Porcentaje de estiramiento de un resorte para perder
#define MASA_TOTAL 10.0             // Suma de todas las masas de la malla
#define K_BASE 200                  // k_resorte = K_BASE / pow(l0, POTENCIA_K)
#define POTENCIA_K 1.5

#endif
