CC = gcc
CFLAGS = -lm -lSDL2

Visualizer.exe: synth.o main.c
	$(CC) main.c $(CFLAGS) -o Visualizer.exe

synth.o: synth.c
	$(CC) synth.c $(CFLAGS) -o synth.o

clean:
	rm Visualizer.exe synth.o
