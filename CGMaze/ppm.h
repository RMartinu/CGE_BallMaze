#pragma once
/**This represents a Image file*/

#include <string>

#define readBuffer 10

class pixel
{
	public char r, g, b;
	pixel(int ir, int ig, int ib);

};
class ppmImage
{
	int height, width;
	int colorDepth;
	pixel pixelField[];
public:
	ppmImage(string name);
	pixel getPixel(int x, int y);
	int getWidth();
	int getHeight();

	~ppmImage();


};

