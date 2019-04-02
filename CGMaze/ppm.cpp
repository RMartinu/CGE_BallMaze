#include "ppm.h"


ppmImage::ppmImage(String name)
{

}
ppmImage::~ppmImage()
{
	delete pixelField;
}

int ppmImage::getHeight()
{
	return height;
}

int ppmImage::getWidth()
{
	return width;
}

pixel ppmImage::getPixel(int x, int y)
{
	if (x > width || y > height)
	{
		//Throw exception
	}

	int arrayIndex = y * width + x;

	return pixelField[arrayIndex];
}