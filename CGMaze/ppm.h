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
	private int height, width;
	privateint colorDepth;
	private pixel pixelField[];

	public ppmImage(string name);
	public pixel getPixel(int x, int y);
	public int getWidth();
	public int getHeight();

	public ~ppmImage();


};

