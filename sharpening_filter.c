// C program to sharpen a grey-scale BMP image

# include <stdio.h>
# include <time.h>
# include <sys/types.h>
# include <stdlib.h>
# include <unistd.h>
int main()
{
	// input file name
	FILE *fIn = fopen("boats.bmp", "r");	

	//output file name			
	FILE *fOut = fopen("boats_sharp_fltr.bmp", "w+");		

	int i, j, x, y, temp, pixel;
	unsigned char byte[54], colorTable[1024];
	// filter log 7*7
	int filter[7][7] = {{0, 0, -1, -1, -1, 0, 0}, {0, -1, -3, -3, -3, -1, 0}, {-1, -3, 0, 7, 0, -3, -1}, {-1, -3, 7, 24, 7, -3, -1}, {-1, -3, 0, 7, 0, -3, -1}, {0, -1, -3, -3, -3, -1, 0}, { 0, 0, -1, -1, -1, 0, 0}};

	// read the header from the input file
	for(i = 0; i < 54; i++) {
		byte[i] = getc(fIn);
	}

	// write the header into the output file
	fwrite(byte, sizeof(unsigned char), 54, fOut);				

	// extract image height, width and bitDepth from imageHeader
	int height = *(int*)&byte[18];
	int width = *(int*)&byte[22];
	int bitDepth = *(int*)&byte[28];

	printf("width: %d\n",width);
	printf("height: %d\n",height );

	// calculate the size of the image
	int size = height * width;							

	//if ColorTable present, extract it
	if(bitDepth <= 8) {
		fread(colorTable, sizeof(unsigned char), 1024, fIn);
		fwrite(colorTable, sizeof(unsigned char), 1024, fOut);
	}

	// input_buffer matrix to store the image data
	unsigned char input_buffer[width][height];					
	unsigned char output_buffer[width][height];

	// read the input image data into buffer
	fread(input_buffer, sizeof(unsigned char), size, fIn);			

	clock_t start,end;
	double cpu;
	start=clock();
	for(i = 3; i < width - 3; i++) {
		for(j = 3; j < height - 3; j++) {
			temp = 0, pixel = 0;
			for(x = -3; x <= 3; x++) {
				for(y = -3; y <= 3; y++) {
					pixel = input_buffer[i + x][j + y] * filter[x + 3][y + 3];
					temp += pixel;
				}
			}
			output_buffer[i][j] = temp/48;	
		}
	}
	end=clock();

	cpu=((double)(end-start))/CLOCKS_PER_SEC;
	printf("Time taken = %lf\n", cpu);	
	// write output buffer to the output image
	fwrite(output_buffer, sizeof(unsigned char), size, fOut);	
	// close the files
	fclose(fIn);
	fclose(fOut);

	return 0;
}