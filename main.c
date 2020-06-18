#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <memory.h>

#include "function.h"



typedef struct {
	unsigned char padding;
} pad;



typedef struct {
    unsigned char blue;
    unsigned char green;
    unsigned char red;
	unsigned char alignment;
} rgba;



typedef struct {
    unsigned char blue;
    unsigned char green;
    unsigned char red;
} rgb;



typedef struct {
	int width, height, offset;
	short bitsPerPixel;
	rgba *pixelesA[100][100];
	rgb *pixeles[100][100];
} tBitmapData;



int main(int argc, char *argv[]) {
	FILE *f1, *f2;
	tBitmapData *bmpdata1 = (tBitmapData *) malloc(sizeof(tBitmapData));
	tBitmapData *bmpdata2 = (tBitmapData *) malloc(sizeof(tBitmapData));
	rgba *pixelA;
	rgb *pixel;
	pad *pd;
	int i, j, k, padding;
	f1 = fopen(argv[1], "r");
	f2 = fopen(argv[2], "r");

	printf("\nFirst file (input):\n");
	if (f1 == NULL)
		perror("Not found\n");
	else {
		fseek(f1, 18, SEEK_SET);
		fread(&(bmpdata1->width), sizeof((bmpdata1->width)), 1, f1);
		printf("Width: %d\n", bmpdata1->width);
		fseek(f1, 22, SEEK_SET);
		fread(&(bmpdata1->height), sizeof((bmpdata1->height)), 1, f1);
		printf("Height: %d\n", bmpdata1->height);
		fseek(f1, 10, SEEK_SET);
		fread(&(bmpdata1->offset), sizeof((bmpdata1->offset)), 1, f1);
		printf("Offset: %d\n", bmpdata1->offset);
		fseek(f1, 28, SEEK_SET);
		fread(&(bmpdata1->bitsPerPixel), sizeof(bmpdata1->bitsPerPixel), 1, f1);
		printf("Bits per pixel: %d\n", bmpdata1->bitsPerPixel);
	}

	printf("\nSecond file (output):\n");
	if (f2 == NULL)
		perror("Not found\n");
	else {
		fseek(f2, 18, SEEK_SET);
		fread(&(bmpdata2->width), sizeof((bmpdata2->width)), 1, f2);
		printf("Width: %d\n", bmpdata2->width);
		fseek(f2, 22, SEEK_SET);
		fread(&(bmpdata2->height), sizeof((bmpdata2->height)), 1, f2);
		printf("Height: %d\n", bmpdata2->height);
		fseek(f2, 10, SEEK_SET);
		fread(&(bmpdata2->offset), sizeof((bmpdata2->offset)), 1, f2);
		printf("Offset: %d\n", bmpdata2->offset);
		fseek(f2, 28, SEEK_SET);
		fread(&(bmpdata2->bitsPerPixel), sizeof(bmpdata2->bitsPerPixel), 1, f2);
		printf("Bits per pixel: %d\n", bmpdata2->bitsPerPixel);
	}
	printf("\n");



	padding = (bmpdata1->width) % 4;
	printf("Padding in first file: %d\n", padding);
	unsigned char bitmap1[bmpdata1->height * (3 * bmpdata1->width + padding)];

	if (bmpdata1->bitsPerPixel == 32) {
		fseek(f1, bmpdata1->offset, SEEK_SET);
		k=0;
		for (i=0; i<bmpdata1->width; ++i) {
			//printf("R   G   B Row %d\n", i+1);
			for (j=0; j<(bmpdata1->height); ++j) {
				pixelA = (rgba *) malloc(sizeof(rgba));		//free dorobić
				fread(pixelA, sizeof(rgba), 1, f1);
				//printf("Pixel %d: B: %3d G: %d R: %d A: %d \n", j+1, pixelA->blue, pixelA->green, pixelA->red, pixelA->alignment);
				bitmap1[k++] = pixelA->blue;
				bitmap1[k++] = pixelA->green;
				bitmap1[k++] = pixelA->red;
			}
			for (j=0; j<padding; ++j) {
				bitmap1[k++] = 0;
			}
		}
	}
	if (bmpdata1->bitsPerPixel == 24) {
		fseek(f1, bmpdata1->offset, SEEK_SET);
		k=0;
		for (i=0; i<bmpdata1->width; ++i) {
			//printf("R   G   B Row %d\n", i+1);
			for (j=0; j<(bmpdata1->height); ++j) {
				pixel = (rgb *) malloc(sizeof(rgb));		//free dorobić
				fread(pixel, sizeof(rgb), 1, f1);
				//printf("Pixel %d: B: %3d G: %d R: %d \n", j+1, pixel->blue, pixel->green, pixel->red);
				bitmap1[k++] = pixel->blue;
				bitmap1[k++] = pixel->green;
				bitmap1[k++] = pixel->red;
			}
			pd = (pad *) malloc(sizeof(pad));		//free dorobić
			for (j=0; j<padding; ++j) {
				bitmap1[k++] = 0;
				fread(pd, sizeof(pad), 1, f1);
			}
		}
	}
	free(pixelA);
	free(pixel);
	free(pd);

	for (i=0; i<bmpdata1->height * (3 * bmpdata1->width + padding); ++i) {
		printf("%d ", bitmap1[i]);
	}



	padding = (bmpdata2->width) % 4;
	printf("\n\nPadding in second file: %d\n", padding);
	unsigned char bitmap2[bmpdata2->height * (3 * bmpdata2->width + padding)];

	if (bmpdata2->bitsPerPixel == 32) {
		fseek(f2, bmpdata2->offset, SEEK_SET);
		k=0;
		for (i=0; i<bmpdata2->width; ++i) {
			//printf("R   G   B Row %d\n", i+1);
			for (j=0; j<(bmpdata2->height); ++j) {
				pixelA = (rgba *) malloc(sizeof(rgba));		//free dorobić
				fread(pixelA, sizeof(rgba), 1, f2);
				//printf("Pixel %d: B: %3d G: %d R: %d A: %d \n", j+1, pixelA->blue, pixelA->green, pixelA->red, pixelA->alignment);
				bitmap2[k++] = pixelA->blue;
				bitmap2[k++] = pixelA->green;
				bitmap2[k++] = pixelA->red;
			}
			for (j=0; j<padding; ++j) {
				bitmap2[k++] = 0;
			}
		}
	}
	if (bmpdata2->bitsPerPixel == 24) {
		fseek(f2, bmpdata2->offset, SEEK_SET);
		k=0;
		for (i=0; i<bmpdata2->width; ++i) {
			printf("R   G   B Row %d\n", i+1);
			for (j=0; j<(bmpdata2->height); ++j) {
				pixel = (rgb *) malloc(sizeof(rgb));		//free dorobić
				fread(pixel, sizeof(rgb), 1, f2);
				printf("Pixel %d: B: %3d G: %d R: %d \n", j+1, pixel->blue, pixel->green, pixel->red);
				bitmap2[k++] = pixel->blue;
				bitmap2[k++] = pixel->green;
				bitmap2[k++] = pixel->red;
			}
			pd = (pad *) malloc(sizeof(pad));		//free dorobić
			for (j=0; j<padding; ++j) {
				bitmap2[k++] = 0;
				fread(pd, sizeof(pad), 1, f2);
			}
		}
	}
	free(pixelA);
	free(pixel);
	free(pd);

	for (i=0; i<bmpdata2->height * (3 * bmpdata2->width + padding); ++i) {
		printf("%d ", bitmap2[i]);
	}


	//bmpdata1->width, bmpdata1->height, bmpdata2->width, bmpdata2->height,	
	printf("\n\nAddress of bitmap1: %p", bitmap1);
	printf("\nAddress of bitmap2: %p", bitmap2);
	printf("\nWidth of bitmap1: %d", bmpdata1->width);
	printf("\nHeight of bitmap1: %d", bmpdata1->height);
	printf("\nWidth of bitmap2: %d", bmpdata2->width);
	printf("\nHeight of bitmap2: %d", bmpdata2->height);
	printf("\nStart:");

	function(bitmap1, bitmap2, bmpdata1->width, bmpdata2->width, bmpdata1->height, bmpdata2->height);

	printf("\n\nFINAL RESULT: %d\n", bitmap2[0]);
	fclose(f1);
	fclose(f2);

    return 0;
}
