#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef struct {
	char version[3]; 
	int width;
	int height;
	int maxGrayLevel;
	int **imageData;
} pgm;

void init_out_image( pgm* out, pgm image){
	int i, j;
	strcpy(out->version, image.version);
	out->width = image.width;
	out->height = image.height;
	out->maxGrayLevel = image.maxGrayLevel;
	
	out->imageData = (int**) calloc(out->width, sizeof(int*));
	for(i = 0; i < out->width; i++) {
		out->imageData[i] = calloc(out->height, sizeof(int));
	}
	
	for(i = 0; i < out->width; i++) {
		for(j = 0; j < out->height; j++) {
			out->imageData[i][j] = image.imageData[i][j];
		};
	}
}

void read_comments(FILE *input_image) {
	char ch;
	char line[100];

	while ((ch = fgetc(input_image)) != EOF && ch == '\n') {
		;
    }
	if (ch == '#') {
        fgets(line, sizeof(line), input_image);
    } 
	else {
		fseek(input_image, -2L, SEEK_CUR);
	}
}

void read_pgm_file(char* dir, pgm* image) {
	FILE* input_image; 
	int ch;
	int i, j;

	input_image = fopen(dir, "r");
	if (input_image == NULL) {
		printf("File could not opened!");
		return;
	} 
	
	fgets(image->version, sizeof(image->version), input_image);
	read_comments(input_image);

	fscanf(input_image, "%d %d %d", &image->width, &image->height, &image->maxGrayLevel);
	
	image->imageData = (int**) calloc(image->width, sizeof(int*));
	for(i = 0; i < image->width; i++) {
		image->imageData[i] = calloc(image->height, sizeof(int));
	}
	
	for (i = 0; i < image->height; i++) {
		for (j = 0; j < image->width; j++) {
			fscanf(input_image, "%d", &ch);
			image->imageData[j][i] = ch;
		}
	}
	
	printf("Version: %s \nWidth: %d \nHeight: %d \nMaximum Gray Level: %d \n", image->version, image->width, image->height, image->maxGrayLevel);

}

int convolution(pgm* image, int kernel[3][3], int row, int col) {
	int i, j, sum = 0;
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			sum += image->imageData[i + row][j + col] * kernel[i][j];
		}
	}
	return sum;
}

void sobel_edge_detector(pgm* image, pgm* out_image) {
	int i, j, x, y;
	int mx[3][3] = {
		{-1, 0, 1},
		{-2, 0, 2},
		{-1, 0, 1}
	};
	int my[3][3] = {
		{-1, -2, -1},
		{0, 0, 0},
		{1, 2, 1}
	};
	
	for (i = 1; i < image->width - 2; i++) {
		for (j = 1; j < image->height - 2; j++) {
			x = convolution(image, mx, i, j);
			y = convolution(image, my, i, j);
			out_image->imageData[i][j] = sqrt(x*x + y*y);
		}
	}
	
}

void min_max_normalization(pgm* image) {
	int min = 0, max = 255, i, j;
	
	for(i = 0; i < image->width; i++) {
		for(j = 0; j < image->height; j++) {
			image->imageData[i][j] = (image->imageData[i][j] - min) / (max - min);
		}
	}
}

void write_pgm_file(pgm* image) { //char dir[50]
	FILE* out_image;
	int i, j, count = 0;
	/*char* token = strtok(dir, ".");
	if (token != NULL) {
		strcat(token, "_output.pgm");
		out_image = fopen(token, "wb");

	}*/
	
	out_image = fopen("C:/Users/fzehr/Desktop/input ve outputlar/casablanca_output.pgm", "wb");
	fprintf(out_image, "%s\n", image->version);
	fprintf(out_image, "%d %d\n", image->width, image->height);
	fprintf(out_image, "%d\n", image->maxGrayLevel);
	for(i = 0; i < image->height; i++) {
		for(j = 0; j < image->width; j++) {
			fprintf(out_image,"%d", image->imageData[j][i]);
			if (count % 17 == 0) 
				fprintf(out_image,"\n");
			else 
				fprintf(out_image," ");
			count ++;

		}
	}
}

int main(int argc, char **argv)
{
	char dir[100] = "C:/Users/fzehr/Desktop/input ve outputlar/casablanca.ascii.pgm";
	pgm image, out_image;
	
	/*printf("Enter the file name: ");
	scanf("%s", dir);*/

	read_pgm_file(dir, &image);
	init_out_image(&out_image, image);
	sobel_edge_detector(&image, &out_image);
	//min_max_normalization(&out_image);
	
	write_pgm_file(&out_image);
	//write_pgm_file(&out_image, dir);

	
	
	return 0;
}
