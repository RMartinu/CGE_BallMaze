#include "ppm.h"
#include<string>
#include<cstdio>

ppmImage::ppmImage(string fileName)
{
	FILE *fp;

	//open a file
	fp = fopen(fileName.c_str(), "r");
	//check if successfull
	if (fp == nullptr)
	{
		puts("Error opening file");
	}
	char buffer[readBuffer];
	int bytesRead = 0;

	//read the magic number
	bytesRead = fscanf(fp, "%8s", buffer);
	if (buffer[0] == 'P' && buffer[1] == '6' && (0 != isblank(buffer[2])))
	{
		//the magic number is ok, lets continue

		//reading width
		bytesRead = fscanf(fp, "%d", &width);
		if (bytesRead == 0)
		{
			//throw, break
		}
		bytesRead = fscanf(fp, "%d", &height);
		if (bytesRead == 0)
		{
			//throw, break
		}
		bytesRead = fscanf(fp, "%d", &colorDepth);
		if (bytesRead == 0)
		{
			//throw, break
		}

		//We have the basic metadata; construct the pixel buffer

		int totalPixels = width * height;
		pixelField = new pixel[totalPixels];

		int r, g, b;
		for (int i = 0; i < totalPixels; ++i)
		{
			bytesRead = fscanf(fp, "%d", &r);
			if (bytesRead == 0)
			{
				//throw, break
			}
			bytesRead = fscanf(fp, "%d", &g);
			if (bytesRead == 0)
			{
				//throw, break
			}
			bytesRead = fscanf(fp, "%d", &b);
			if (bytesRead == 0)
			{
				//throw, break
			}

			pixel p(r,g,b);
			pixelField[i] = p;

		}
	}
	else
	{
		//throw if so inclined
	}


	fclose(fp);

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