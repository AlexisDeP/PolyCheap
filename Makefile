PROGRAM=programa
CC=gcc
CFLAGS=-DTTF
LDFLAGS=-lSDL2 -lSDL2_ttf -lm

all: $(PROGRAM)

$(PROGRAM): color.o resorte.o masa.o malla.o dibujo.o config.o juego.o calculo.o main.o lista.o
	$(CC) color.o resorte.o masa.o malla.o dibujo.o config.o juego.o calculo.o main.o lista.o -o $(PROGRAM) $(LDFLAGS)

lista.o: lista.c lista.h
	$(CC) $(CFLAGS) -c lista.c

color.o: color.c color.h
	$(CC) $(CFLAGS) -c color.c

masa.o: masa.c masa.h color.h
	$(CC) $(CFLAGS) -c masa.c

resorte.o: resorte.c resorte.h masa.h color.h
	$(CC) $(CFLAGS) -c resorte.c

malla.o: malla.c malla.h masa.h resorte.h color.h calculo.h lista.h
	$(CC) $(CFLAGS) -c malla.c

dibujo.o: dibujo.c dibujo.h color.h masa.h resorte.h malla.h calculo.h lista.h
	$(CC) $(CFLAGS) -c dibujo.c

juego.o: juego.c juego.h
	$(CC) $(CFLAGS) -c juego.c

calculo.o: calculo.c calculo.h
	$(CC) $(CFLAGS) -c calculo.c

config.o: config.c config.h
	$(CC) $(CFLAGS) -c config.c

main.o: main.c color.h dibujo.h masa.h resorte.h malla.h config.h juego.h calculo.h lista.h
	$(CC) $(CFLAGS) -c main.c

clean:
	rm -vf *.o $(PROGRAM)