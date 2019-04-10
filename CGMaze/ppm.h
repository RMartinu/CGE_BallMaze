#pragma once
/**This represents a Image file*/

#include <string>

#define readBuffer 10
using namespace std;
class pixel
{
public:
	unsigned char r, g, b;
	pixel(int ir, int ig, int ib);
	pixel();

};
class ppmImage
{
private:
		int height, width;
	 int colorDepth;
	 pixel *pixelField;
	 unsigned char *t;

public:
	ppmImage(string name);
	 pixel getPixel(int x, int y);
	 int getWidth();
	 int getHeight();

	 unsigned char* imageDataAsCharArray();

	 ~ppmImage();


};

