#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

float xTheta, yTheta, zTheta;

char *buffer;
float *zBuffer;
int height = 75, width = 150;

float rotateX(int  x, int y, int z){
	return y * sin(xTheta) * sin(yTheta) * cos(zTheta) - z * cos(xTheta) * sin(yTheta) * cos(zTheta) + 
		y * cos(xTheta) * sin(zTheta) + z * sin(xTheta) * sin(zTheta) + x * cos(yTheta) * cos(zTheta);
}
float rotateY(int x, int y, int z) {
  return y * cos(xTheta) * cos(zTheta) + z * sin(xTheta) * cos(zTheta) -
         y * sin(xTheta) * sin(yTheta) * sin(zTheta) + z * cos(xTheta) * sin(yTheta) * sin(zTheta) -
         x * cos(yTheta) * sin(zTheta);
}
float rotateZ(int x, int y, int z) {
  return z * cos(xTheta) * cos(yTheta) - y * sin(xTheta) * cos(yTheta) + x * sin(yTheta);
}

void createFace(float x, float y, float z, char stamp){
	float xr = rotateX(x, y, z);
	float yr = rotateY(x, y, z);
	float zr = rotateZ(x, y, z) + 100;

	float zInverse = 1/zr;

	int k = 50;

	int xp = (int) (width/2 + k * xr * zInverse*2);
	int yp = (int)(height/2 + k * yr * zInverse);
	
	int index = xp+yp*width;
	if ( index >= 0 && index < (width*height) ){
		if ( *(zBuffer+index) < zInverse ){
			*(zBuffer+index) = zInverse;
			*(buffer+index) = stamp;
		}
	}
	

}

int main(){

	int i;
	char background = ' ';

	while(1){

		buffer = malloc(height*width);
		zBuffer = malloc(height*width*sizeof(float));

		printf("\x1b[2J");
		memset(buffer, background, width*height);
		memset(zBuffer, 0, width*height*sizeof(float));
		
		float v1, v2, v3 = 25, cubeDimension = 5;
		for( v1 = -25; v1 < v3; v1++){
			for( v2 = -25; v2 < v3; v2++){
				createFace(v1, v2, -v3, ';');	// f1
				createFace(v1, v2, v3, ':');	// f3
				createFace(-v3, v1, v2, '~');	//f4
				createFace(v3, v1, v2, '-');	//f2
				createFace(v1, -v3, v2, '+');	//f6
				createFace(v1, v3, v2, '=');	//f5
			}
		}

		for(i = 0; i < (height*width); i++)
			putchar( i % width ? *(buffer+i) : '\n');

		xTheta += 0.01;
		yTheta += 0.01;
		zTheta += 0.01;

		usleep(8000*2);
		free(buffer);
		free(zBuffer);
	}

	
	return 0;
}
