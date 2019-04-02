#pragma once
/**This represents a Image file*/


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

	ppmImage(String name);
	pixel getPixel(int x, int y);
	int getWidth();
	int getHeight();

	~ppmImage();


};

