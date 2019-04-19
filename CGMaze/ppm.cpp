#include "ppm.h"
#include<string>
#include<cstdio>

ppmImage::ppmImage(string fileName)
{
	t = nullptr;
	FILE *fp;
	puts("Reading File");
	//open a file
	fp = fopen(fileName.c_str(), "r");
	//check if successfull
	if (fp == nullptr)
	{
		puts("Error opening file");
		this->colorDepth = 0;
		this->height = 0;
		this->width = 0;
		this->pixelField = nullptr;
		return;
	}
	char buffer[readBuffer];
	int bytesRead = 0;
	//read the magic number
	bytesRead = fscanf(fp, "%8s", buffer);
	while (fgetc(fp) != '\n');
	while (fgetc(fp) != '\n');

	
	if (buffer[0] == 'P' && buffer[1] == '3' )
	{
		//the magic number is ok, lets continue
		//printf("First line length: %d \nMagic number: %3s", bytesRead, buffer );
		//reading width
		bytesRead = fscanf(fp, "%7s", buffer);
		width = strtol(buffer, nullptr,10);
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
//		printf("Color: %d, w: %d, h%d", this->colorDepth, this->width, this->height);
		//We have the basic metadata; construct the pixel buffer

		pixelField = nullptr;
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
	//		printf("r: %d, g: %d, b: %d\t", p.r, p.g, p.b);
			pixelField[i] = p;

		}
	}
	else
	{
		//throw if so inclined
	}
	//puts("should be done");

	fclose(fp);

}
ppmImage::~ppmImage()
{
	if (pixelField != nullptr) {
		delete[] pixelField;
	}
	if (t != nullptr)
	{
		delete[] t;
	}
}

int ppmImage::getHeight()
{
	return height;
}

unsigned char * ppmImage::imageDataAsCharArray()
{
	if (t != nullptr) {
		delete []t;
		t = nullptr;
	}
	 t= new unsigned char[width*height*3];

	 for (int i = 0; i < width*height; i += 3)
	 {
		 t[i + 0] = pixelField->r;
		 t[i + 1] = pixelField->g;
		 t[i + 2] = pixelField->b;
	 }
	return t;
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
		printf("Access to %d %d illegal\n", width, height);
		return pixel();
	}

	int arrayIndex = y * width + x;

	return pixelField[arrayIndex];
}

pixel::pixel(int ir, int ig, int ib)
{
	r = ir;
	g = ig;
	b = ib;
}

pixel::pixel()
{
	r = g = b = 0;
}
