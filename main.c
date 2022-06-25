#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

typedef struct {
	int* val;
	int* next;
	int* prev;
} Node;

void writeToFile(Node* n) {
	// TODO implement me
}

Node* getHilbertCurveHelper(int * s1, int * s2, int * s3, int * s4) {
	return (Node *) NULL;
}

Node* getHilbertCurve(int * pixels, int width, int height) {
	int * s1 = (int *) malloc(sizeof(int) * width * height / 4);
	int * s2 = (int *) malloc(sizeof(int) * width * height / 4);
	int * s3 = (int *) malloc(sizeof(int) * width * height / 4);
	int * s4 = (int *) malloc(sizeof(int) * width * height / 4);

	int s1counter = 0, s2counter = 0, s3counter = 0, s4counter = 0;

	for (int i = 0; i < width * height; i++) {
		bool top = (i < width * height / 2);
		bool left = (i % width < width / 2);

		if (top && left) {
			s1[s1counter] = pixels[i];
			s1counter++;
		}
		else if (top && !left) {
			s2[s2counter] = pixels[i];
			s2counter++;
		}
		else if (!top && left) {
			s3[s3counter] = pixels[i];
			s3counter++;
		}
		else if (!top && !left) {
			s4[s4counter] = pixels[i];
			s4counter++;
		}
	}

	for (int i = 0; i < s1counter; i++) {
		printf("%x\n",s1[i]);
	}

	Node * returnValue = getHilbertCurveHelper(s1, s2, s3, s4);

	free(s1);
	free(s2);
	free(s3);
	free(s4);

	return returnValue;
}

Node* visualize(SDL_Surface *image) {

	// Image width and height
	const int width = image->w;
	const int height = image->h;

	// Create our 1d pixel array on the heap
	int * pixel_array = (int *) malloc(width * height * sizeof(int));

	int pitch = image->pitch;

	// Write to the pixels
	SDL_LockSurface(image);

	// Iterate through pixels, store them in an array
	for (int y = 0; y < height; y++) {
		unsigned int* row = (unsigned int*)((char*)image->pixels + pitch * y);
		for (int x = 0; x < width; x++) {
			pixel_array[y * width + x] = row[x];
		}
	}

	// Create doubly-linked lists for each block of 4 px in the array
	Node* curve = getHilbertCurve(pixel_array, width, height);

	// Be free!
    free(pixel_array);

	// Stop writing
	SDL_UnlockSurface(image);

	// Return!
	return curve;

}

int main(int argc, char *argv[]) {
	// Check arguments
	if (argc > 2) {
		printf("USAGE: ./Visualizer.exe {IMAGE_PATH}");
		return -1;
	}

	char *fileName;
	if (argc == 1) fileName = (char *)"Mandelbrot.bmp";
	else fileName = argv[1];

	// Initialize SDL stuff
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Surface *image = SDL_LoadBMP(fileName);

	// Visualize the image
	Node* visualization = visualize(image);
	writeToFile(visualization);

	// Cleanup
	SDL_FreeSurface(image);
	SDL_Quit();

	return 0;
}
