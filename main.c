#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

typedef struct {
	uint8_t r;
	uint8_t g;
	uint8_t b;
} Pixel;

struct Node {
	Pixel val;
	struct Node* next;
};

typedef struct Node Node;

Node* getEndNode(Node* n) {
	while (n->next) { n = n->next; }
	return n;
}

void printNodes(Node* n) {
	while (n) {
		printf("(%i, %i, %i)\n", n->val.r, n->val.g, n->val.b);
		n = n->next;
	}
}

void sonify(Node* curveStart, int length) {

}

// Width is constant
Node* getHilbertCurve(Node* nodes, int size, int width) {
	if (size == 1) { return nodes; }

	int n1_start_index = 0;
	int n2_start_index = size/2;
	int n3_start_index = (width * (size/2));
	int n4_start_index = (width * (size/2)) + size/2;

	Node* n1 = getHilbertCurve(nodes + n1_start_index, size/2, width);
	Node* n2 = getHilbertCurve(nodes + n2_start_index, size/2, width);
	Node* n3 = getHilbertCurve(nodes + n3_start_index, size/2, width);
	Node* n4 = getHilbertCurve(nodes + n4_start_index, size/2, width);

	(getEndNode(n1))->next = n2;
	(getEndNode(n2))->next = n4;
	(getEndNode(n4))->next = n3;

	return n1;
}

int main(int argc, char *argv[]) {
	// Check arguments
	if (argc > 2) {
		printf("USAGE: ./Visualizer.exe {IMAGE_PATH}");
		return -1;
	}

	char *fileName;
	if (argc == 1) fileName = (char *) "Mandelbrot.bmp";
	else fileName = argv[1];

	int width, height, channels;
	unsigned char* img = stbi_load(fileName, &width, &height, &channels, 0);

	if (width != height) {
		printf("ERROR: image isn't square :// try again");
		return -1;
	}

	Pixel *pixels = (Pixel *) malloc(width * width * sizeof(Pixel));
	Node* nodes = (Node *) malloc(width * width * sizeof(Node));

	int i = 0;
	for (unsigned char* p = img, i = 0; p != img + (width * width * channels); p += channels, i++) {
		Pixel pix = {*p, *(p + 1), *(p + 2)};
		pixels[i] = pix;
		nodes[i].val = pix;
		nodes[i].next = (Node *) NULL;
	}

	// Visualize the image
	Node* hilbertCurve = getHilbertCurve(nodes, width, width);
	
	// Debugging
	printNodes(hilbertCurve);

	// Sonification! :D
	sonify(hilbertCurve, width * width);

	// Free dynamic memory
	free(pixels);
	free(nodes);

	return 0;
}
