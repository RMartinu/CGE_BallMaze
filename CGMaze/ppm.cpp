#include "ppm.h"


ppmImage::ppmImage(String name)
{}
ppmImage::~ppmImage()
{}

int ppmImage::getHeight()
{}

int ppmImage::getWidth()
{}

pixel ppmImage::getPixel(int x, int y)
{
	if (x > width || y > height)
	{
		//Throw exception
	}

	int arrayIndex = y * width + x;

	return pixelField[arrayIndex];
}