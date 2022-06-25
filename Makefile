CC = gcc
CFLAGS = -lm -lSDL2

Visualizer.exe: main.c
	$(CC) main.c $(CFLAGS) -o Visualizer.exe

clean:
	rm Visualizer.exe
