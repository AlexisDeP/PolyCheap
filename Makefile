PROGRAM=programa
CC=gcc
<<<<<<< HEAD
CFLAGS=-DTTF -pedantic -std=c99 -g
=======
CFLAGS=-DTTF -pedantic -std=c99
>>>>>>> 1f999196917812af0c82b6b1ee73828fe78df08a
LDFLAGS=-lSDL2 -lSDL2_ttf -lm
OBJS = $(SRCS:.c=.o)

all: $(PROGRAM)

$(PROGRAM): color.o resorte.o masa.o malla.o dibujo.o calculo.o lista.o simulacion.o main.o
	$(CC) color.o resorte.o masa.o malla.o dibujo.o calculo.o lista.o simulacion.o main.o -o $(PROGRAM) $(LDFLAGS)

lista.o: lista.c lista.h
	$(CC) $(CFLAGS) -c lista.c

color.o: color.c color.h
	$(CC) $(CFLAGS) -c color.c

masa.o: masa.c masa.h color.h
	$(CC) $(CFLAGS) -c masa.c

resorte.o: resorte.c resorte.h color.h
	$(CC) $(CFLAGS) -c resorte.c

malla.o: malla.c malla.h masa.h resorte.h color.h calculo.h lista.h
	$(CC) $(CFLAGS) -c malla.c

dibujo.o: dibujo.c dibujo.h color.h malla.h calculo.h lista.h
	$(CC) $(CFLAGS) -c dibujo.c

juego.o: juego.c juego.h malla.h
	$(CC) $(CFLAGS) -c juego.c

calculo.o: calculo.c calculo.h
	$(CC) $(CFLAGS) -c calculo.c

simulacion.o: simulacion.c simulacion.h lista.h malla.h calculo.h
	$(CC) $(CFLAGS) -c simulacion.c

main.o: main.c dibujo.h malla.h config.h calculo.h simulacion.h
	$(CC) $(CFLAGS) -c main.c

clean:
	rm -vf *.o $(PROGRAM)