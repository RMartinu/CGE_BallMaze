#include "VertexList.h"
#include <vector>
#include "Vertex.h"
#include "defines.h"
using namespace std;

VertexList::VertexList(int formatDescriptor)
{
	VertexList(formatDescriptor, 8);
}

VertexList::VertexList(int formatDescriptor, int numberOfEntries)
{
	//printf("My format: %d \n", formatDescriptor);
	vertexData = nullptr;
	currEntries = 0;
	indizes = nullptr;
	currEdges = 0;
	this->containsCoordinates = (formatDescriptor & (vertexCoordinates)) != 0;
	/*if (this->containsCoordinates);
	{
		puts("Contains Coords");
	}*/
	this->containsVertexColor = (formatDescriptor & (vertexColor)) != 0;
	//if (this->containsVertexColor) { puts("containes VColor"); }
	this->containsUVCoordinates = (formatDescriptor & (UVCoordinates)) != 0;
	//if (this->containsUVCoordinates) { puts("contains UVs"); }
	this->containsNormals = (formatDescriptor & (normals)) != 0;
	maxEntries = numberOfEntries;
	currEntries = 0;
	currEdges = 0;
	maxEdges = maxEntries * 1.5;
	indizes = new unsigned int[maxEdges];
	stride = 0;
	if (this->containsCoordinates == true)
	{
		stride += 3;
	}
	if (this->containsVertexColor == true)
	{
		stride += 3;
	}

	if (this->containsUVCoordinates == true)
	{
		stride += 2;
	}
	if (this->containsNormals == true)
	{
		stride += 3;
	}
	//printf("striding: %d", stride);
	//!!Change Back!!
	//stride = 3;
	//printf("allocate: %d\n", maxEntries*stride);
	vertexData = new float[maxEntries*stride];
}

VertexList::~VertexList()
{
	//TODO: fix crash on cleanUp!!!
	//if(indizes!=nullptr){ delete []indizes; }


	//
	if (vertexData != nullptr) {
		/*delete  []vertexData;*/
	}
}

//bool VertexList::addVertex(float x, float y, float z)
//{
//	//printf("we add a vertex %f %f %f\n", x, y, z);
//	if (!containsCoordinates || containsVertexColor || containsUVCoordinates || containsNormals)
//	{
//		puts("Wrong data format");
//		return false;
//	}
//	if (currEntries >= maxEntries)
//	{
//		extendVertexData();
//	}
//	vertexData[currEntries*stride] = x;
//	vertexData[currEntries*stride+1] = y;
//	vertexData[currEntries*stride+2] = z;
//	++currEntries;
//	
//	//printf("\n### new vcount: %d\n", numberOfVertices);
//	
//
//	return true;
//}

bool VertexList::addVertex(float x, float y, float z, float r, float g, float b, float nx, float ny, float nz)
{
	if (!containsCoordinates || !containsVertexColor || containsUVCoordinates || !containsNormals)
	{
		//puts("\n##refusing");
		return false;
	}
	if (currEntries + 3 > maxEntries)
	{
		//puts("extend vertex");
		extendVertexData();
	}
	vertexData[currEntries*stride] = x;
	vertexData[currEntries*stride + 1] = y;
	vertexData[currEntries*stride + 2] = z;
	vertexData[currEntries*stride + 3] = r;
	vertexData[currEntries*stride + 4] = g;
	vertexData[currEntries*stride + 5] = b;
	vertexData[currEntries*stride + 6] = nx;
	vertexData[currEntries*stride + 7] = ny;
	vertexData[currEntries*stride + 8] = nz;
	//printf("indizes pre entry: %d\n", currEntries);
	++currEntries;


	return true;
}

bool VertexList::addVertex(float x, float y, float z, float r, float g, float b, float u, float v, float nx, float ny, float nz)
{
	if (!containsCoordinates || !containsVertexColor || !containsUVCoordinates || !containsNormals)
	{
		return false;
	}
	if (currEntries >= maxEntries)
	{
		extendVertexData();
	}
	vertexData[currEntries*stride] = x;
	vertexData[currEntries*stride + 1] = y;
	vertexData[currEntries*stride + 2] = z;
	vertexData[currEntries*stride + 3] = r;
	vertexData[currEntries*stride + 4] = g;
	vertexData[currEntries*stride + 5] = b;
	vertexData[currEntries*stride + 6] = u;
	vertexData[currEntries*stride + 7] = v;
	vertexData[currEntries*stride + 8] = nx;
	vertexData[currEntries*stride + 9] = ny;
	vertexData[currEntries*stride + 10] = nz;
	++currEntries;
	return true;
}

//bool VertexList::addVertex(float x, float y, float z, float u, float v)
//{
//	if (!containsCoordinates || containsVertexColor || !containsUVCoordinates || containsNormals)
//	{
//		return false;
//	}
//	if (currEntries >= maxEntries)
//	{
//		extendVertexData();
//	}
//	vertexData[currEntries*stride] = x;
//	vertexData[currEntries*stride + 1] = y;
//	vertexData[currEntries*stride + 2] = z;
//	vertexData[currEntries*stride + 3] = u;
//	vertexData[currEntries*stride + 4] = v;
//	++currEntries;
//	return true;
//}

bool VertexList::addVertex(Vertex v)
{
	/*if (containsCoordinates && !containsVertexColor && !containsUVCoordinates && !containsNormals)
	{
		return addVertex(v.x, v.y, v.z);
	}*/
	if (containsCoordinates && containsVertexColor && !containsUVCoordinates && containsNormals)
	{
		//printf("inserting: %f, %f, %f\n", v.x,v.y,v.z);
		bool t = addVertex(v.x, v.y, v.z, v.r, v.g, v.b, v.nx, v.ny, v.nz);
		if (!t)
		{
			puts("insert failed");
		}
		return t;
	}
	/*if (containsCoordinates && !containsVertexColor && containsUVCoordinates && !containsNormals)
	{
		return addVertex(v.x, v.y, v.z, v.u, v.v);
	}*/
	if (containsCoordinates && containsVertexColor && containsUVCoordinates && containsNormals)
	{
		return addVertex(v.x, v.y, v.z, v.r, v.g, v.b, v.u, v.v, v.nx, v.ny, v.nz);
	}
	return false;
}
bool VertexList::addIndex(int vertex1, int vertex2, int vertex3)
{
	if (currEdges + 3 > maxEdges)
	{
		if (!extendIndizes())
		{
			puts("Extending indizes failed");
			return false;
		}
	}
	//printf("triangle: %d, %d, %d", vertex1, vertex2, vertex3);
	indizes[currEdges] = vertex1;
	indizes[currEdges + 1] = vertex2;
	indizes[currEdges + 2] = vertex3;
	currEdges += 3;
	return true;
}

bool VertexList::extendVertexData()
{

	//puts("####Extending VBuffer");
	//printf("the vertex pointer before: %p\n", vertexData);
	float* newArray;
	//printf("Vertbuffer data: %d %d\n", maxEntries, stride);
	newArray = new float[maxEntries * stride + 3 * stride];

	if (newArray == nullptr)
	{
		printf("Error: allocation failed.");
		return false;
	}
	//printf("the actual pointer: %p\n", vertexData);
	//puts("copy now");
	//printf("processing %d * %d entries\n", currEntries, stride);
	for (int i = 0; currEntries*stride > i; ++i)
	{
		//printf("copying index %d \n", i);
		//printf("the value to copy: %f", vertexData[i]);
		newArray[i] = vertexData[i];
	}

	delete[] vertexData;
	vertexData = nullptr;
	vertexData = newArray;
	//printf("the vertexpointer after: %p\n", vertexData);
	maxEntries += 3;
	extendIndizes();
	return true;
}

bool VertexList::extendIndizes()
{
	//puts("extend index list");
	//printf("index pointer: %p\n", indizes);
	if (currEdges + 10 < maxEdges)
	{
		//printf("%d values are sufficient for %d entries\n", maxEdges, currEdges);
		return true;
	}

	unsigned int* newArray;
	newArray = new unsigned int[maxEdges + 10];
	if (newArray == nullptr)
	{
		printf("Error: allocation failed.");
		return false;
	}
	for (int i = 0; currEdges > i; ++i)
	{
		newArray[i] = indizes[i];
	}
	//puts("copy successful");
	delete[]indizes;
	//printf("new array for index pointer: %p\n", newArray);
	indizes = nullptr;
	indizes = newArray;
	//printf("new index pointer: %p\n", indizes);
	maxEdges += 10;
	return true;
}

bool VertexList::addTriangle(Vertex v1, Vertex v2, Vertex v3)
{
	int vi1 = findVertex(v1);
	int vi2 = findVertex(v2);
	int vi3 = findVertex(v3);

	//printf("in Verts at hand: %d, %d, %d", vi1, vi2,vi3);

	if (vi1 == -1)
	{
		addVertex(v1);
		vi1 = findVertex(v1);
	}
	if (vi2 == -1)
	{
		addVertex(v2);
		vi2 = findVertex(v2);
	}
	if (vi3 == -1)
	{
		addVertex(v3);
		vi3 = findVertex(v3);
	}
	//printf("out Verts at hand: %d, %d, %d\n", vi1, vi2, vi3);
	if (vi1 == -1 || vi2 == -1 || vi3 == -1)
	{
		//printf("vertex %d, %d, %d not inserted\n", vi1, vi2, vi3);
		return false;
	}

	return addIndex(vi1, vi2, vi3);

}

int VertexList::findVertex(Vertex v)
{
	for (int i = 0; i < maxEntries*stride; i += stride)
	{
		if (abs(vertexData[i] - v.x) < epsilon_F && abs(vertexData[i + 1] - v.y) < epsilon_F && abs(vertexData[i + 2] - v.z) < epsilon_F)
		{
			if (containsUVCoordinates && containsVertexColor)
			{
				if (abs(vertexData[i + 3] - v.r) < epsilon_F && abs(vertexData[i + 4] - v.g) < epsilon_F && abs(vertexData[i + 5] - v.b) < epsilon_F && abs(vertexData[i + 6] - v.u) < epsilon_F && abs(vertexData[i + 7] - v.v) < epsilon_F)
				{
					return i / stride;
				}
				else
				{
					continue;
				}
			}
			else if (containsUVCoordinates && !containsVertexColor)
			{
				if (abs(vertexData[i + 3] - v.u) < epsilon_F && abs(vertexData[i + 4] - v.v) < epsilon_F)
				{
					return i / stride;
				}
				else
				{
					continue;
				}
			}
			else if (!containsUVCoordinates && containsVertexColor)
			{
				if (abs(vertexData[i + 3] - v.r) < epsilon_F && abs(vertexData[i + 4] - v.g) < epsilon_F && abs(vertexData[i + 5] - v.b) < epsilon_F)
				{
					return i / stride;
				}
				else
				{
					continue;
				}
			}
			else
			{
				return i / stride;
			}
		}
	}
	return -1;
}

unsigned int * VertexList::getIndizes()
{

	/*puts("Im returning the following indizes: \n");
	for (int i = 0; i < currEdges; ++i)
	{
		printf("%d ", this->indizes[i]);
	}
	puts("\n\n");*/
	return indizes;
}

int VertexList::getIndexCount()
{
	return currEdges;
}

int VertexList::getStride()
{
	return stride;
}

float * VertexList::getVertexData()
{
	return vertexData;
}

int VertexList::getVertexCount()
{
	return currEntries;
}

bool VertexList::getContainsCoordinates()
{
	return containsCoordinates;
}

bool VertexList::getContainsVertexColor()
{
	return containsVertexColor;
}

bool VertexList::getContainsUVCoordinates()
{
	return containsUVCoordinates;
}
bool VertexList::getContainsNormals()
{
	return containsNormals;
}
void VertexList::shiftAllVertices(double x, double y, double z)
{

	for (int i = 0; i < this->getVertexCount(); i += this->stride)
	{
		this->vertexData[i] += x;
		this->vertexData[i + 1] += y;
		this->vertexData[i + 2] += z;
	}
}

	/*
bool VertexList::addVertex(float x, float y, float z, float r, float g, float b, float nx, float ny, float nz)
{
	if (!containsCoordinates || !containsVertexColor || containsUVCoordinates || !containsNormals)
	{
		//puts("\n##refusing");
		return false;
	}
	if (currEntries + 3 > maxEntries)
	{
		//puts("extend vertex");
		extendVertexData();
	}
	vertexData[currEntries*stride] = x;
	vertexData[currEntries*stride + 1] = y;
	vertexData[currEntries*stride + 2] = z;
	vertexData[currEntries*stride + 3] = r;
	vertexData[currEntries*stride + 4] = g;
	vertexData[currEntries*stride + 5] = b;
	vertexData[currEntries*stride + 6] = nx;
	vertexData[currEntries*stride + 7] = ny;
	vertexData[currEntries*stride + 8] = nz;
	//printf("indizes pre entry: %d\n", currEntries);
	++currEntries;


	return true;
}

	
bool VertexList::addVertex(float x, float y, float z, float r, float g, float b, float u, float v, float nx, float ny, float nz)
{
	if (!containsCoordinates || !containsVertexColor || !containsUVCoordinates || !containsNormals)
	{
		return false;
	}
	if (currEntries >= maxEntries)
	{
		extendVertexData();
	}
	vertexData[currEntries*stride] = x;
	vertexData[currEntries*stride + 1] = y;
	vertexData[currEntries*stride + 2] = z;
	vertexData[currEntries*stride + 3] = r;
	vertexData[currEntries*stride + 4] = g;
	vertexData[currEntries*stride + 5] = b;
	vertexData[currEntries*stride + 6] = u;
	vertexData[currEntries*stride + 7] = v;
	vertexData[currEntries*stride + 8] = nx;
	vertexData[currEntries*stride + 9] = ny;
	vertexData[currEntries*stride + 10] = nz;
	++currEntries;
	return true;
}

//bool VertexList::addVertex(float x, float y, float z, float u, float v)
//{
//	if (!containsCoordinates || containsVertexColor || !containsUVCoordinates || containsNormals)
//	{
//		return false;
//	}
//	if (currEntries >= maxEntries)
//	{
//		extendVertexData();
//	}
//	vertexData[currEntries*stride] = x;
//	vertexData[currEntries*stride + 1] = y;
//	vertexData[currEntries*stride + 2] = z;
//	vertexData[currEntries*stride + 3] = u;
//	vertexData[currEntries*stride + 4] = v;
//	++currEntries;
//	return true;
//}

bool VertexList::addVertex(Vertex v)
{
	if (containsCoordinates && containsVertexColor && !containsUVCoordinates && containsNormals)
	{
		//printf("inserting: %f, %f, %f\n", v.x,v.y,v.z);
		bool t = addVertex(v.x, v.y, v.z, v.r, v.g, v.b, v.nx, v.ny, v.nz);
		if (!t)
		{
			puts("insert failed");
		}
		return t;
	}
	if (containsCoordinates && containsVertexColor && containsUVCoordinates && containsNormals)
	{
		return addVertex(v.x, v.y, v.z, v.r, v.g, v.b, v.u, v.v, v.nx, v.ny, v.nz);
	}
	return false;
}
bool VertexList::addIndex(int vertex1, int vertex2, int vertex3)
{
	if (currEdges + 3 > maxEdges)
	{
		if (!extendIndizes())
		{
			puts("Extending indizes failed");
			return false;
		}
	}
	//printf("triangle: %d, %d, %d", vertex1, vertex2, vertex3);
	indizes[currEdges] = vertex1;
	indizes[currEdges + 1] = vertex2;
	indizes[currEdges + 2] = vertex3;
	currEdges += 3;
	return true;
}

bool VertexList::extendVertexData()
{

	//puts("####Extending VBuffer");
	//printf("the vertex pointer before: %p\n", vertexData);
	float* newArray;
	//printf("Vertbuffer data: %d %d\n", maxEntries, stride);
	newArray = new float[maxEntries * stride + 3 * stride];

	if (newArray == nullptr)
	{
		printf("Error: allocation failed.");
		return false;
	}
	//printf("the actual pointer: %p\n", vertexData);
	//puts("copy now");
	//printf("processing %d * %d entries\n", currEntries, stride);
	for (int i = 0; currEntries*stride > i; ++i)
	{
		//printf("copying index %d \n", i);
		//printf("the value to copy: %f", vertexData[i]);
		newArray[i] = vertexData[i];
	}

	delete[] vertexData;
	vertexData = nullptr;
	vertexData = newArray;
	//printf("the vertexpointer after: %p\n", vertexData);
	maxEntries += 3;
	extendIndizes();
	return true;
}

bool VertexList::extendIndizes()
{
	//puts("extend index list");
	//printf("index pointer: %p\n", indizes);
	if (currEdges + 10 < maxEdges)
	{
		//printf("%d values are sufficient for %d entries\n", maxEdges, currEdges);
		return true;
	}

	unsigned int* newArray;
	newArray = new unsigned int[maxEdges + 10];
	if (newArray == nullptr)
	{
		printf("Error: allocation failed.");
		return false;
	}
	for (int i = 0; currEdges > i; ++i)
	{
		newArray[i] = indizes[i];
	}
	//puts("copy successful");
	delete[]indizes;
	//printf("new array for index pointer: %p\n", newArray);
	indizes = nullptr;
	indizes = newArray;
	//printf("new index pointer: %p\n", indizes);
	maxEdges += 10;
	return true;
}

bool VertexList::addTriangle(Vertex v1, Vertex v2, Vertex v3)
{
	int vi1 = findVertex(v1);
	int vi2 = findVertex(v2);
	int vi3 = findVertex(v3);

	//printf("in Verts at hand: %d, %d, %d", vi1, vi2,vi3);

	if (vi1 == -1)
	{
		addVertex(v1);
		vi1 = findVertex(v1);
	}
	if (vi2 == -1)
	{
		addVertex(v2);
		vi2 = findVertex(v2);
	}
	if (vi3 == -1)
	{
		addVertex(v3);
		vi3 = findVertex(v3);
	}
	//printf("out Verts at hand: %d, %d, %d\n", vi1, vi2, vi3);
	if (vi1 == -1 || vi2 == -1 || vi3 == -1)
	{
		//printf("vertex %d, %d, %d not inserted\n", vi1, vi2, vi3);
		return false;
	}

	return addIndex(vi1, vi2, vi3);

}

int VertexList::findVertex(Vertex v)
{
	for (int i = 0; i < maxEntries*stride; i += stride)
	{
		if (abs(vertexData[i] - v.x) < epsilon_F && abs(vertexData[i + 1] - v.y) < epsilon_F && abs(vertexData[i + 2] - v.z) < epsilon_F)
		{
			if (containsUVCoordinates && containsVertexColor)
			{
				if (abs(vertexData[i + 3] - v.r) < epsilon_F && abs(vertexData[i + 4] - v.g) < epsilon_F && abs(vertexData[i + 5] - v.b) < epsilon_F && abs(vertexData[i + 6] - v.u) < epsilon_F && abs(vertexData[i + 7] - v.v) < epsilon_F)
				{
					return i / stride;
				}
				else
				{
					continue;
				}
			}
			else if (containsUVCoordinates && !containsVertexColor)
			{
				if (abs(vertexData[i + 3] - v.u) < epsilon_F && abs(vertexData[i + 4] - v.v) < epsilon_F)
				{
					return i / stride;
				}
				else
				{
					continue;
				}
			}
			else if (!containsUVCoordinates && containsVertexColor)
			{
				if (abs(vertexData[i + 3] - v.r) < epsilon_F && abs(vertexData[i + 4] - v.g) < epsilon_F && abs(vertexData[i + 5] - v.b) < epsilon_F)
				{
					return i / stride;
				}
				else
				{
					continue;
				}
			}
			else
			{
				return i / stride;
			}
		}
	}
	return -1;
}

unsigned int * VertexList::getIndizes()
{

	return indizes;
}

int VertexList::getIndexCount()
{
	return currEdges;
}

int VertexList::getStride()
{
	return stride;
}

float * VertexList::getVertexData()
{
	return vertexData;
}

int VertexList::getVertexCount()
{
	return currEntries;
}

bool VertexList::getContainsCoordinates()
{
	return containsCoordinates;
}

bool VertexList::getContainsVertexColor()
{
	return containsVertexColor;
}

bool VertexList::getContainsUVCoordinates()
{
	return containsUVCoordinates;
}
bool VertexList::getContainsNormals()
{
	return containsNormals;
}
void VertexList::shiftAllVertices(double x, double y, double z)
{

	for (int i = 0; i < this->getVertexCount(); i += this->stride)
	{
		this->vertexData[i] += x;
		this->vertexData[i + 1] += y;
		this->vertexData[i + 2] += z;
	}
}*/

/*
VertexList::VertexList(int formatDescriptor)
{
	VertexList(formatDescriptor, 8);
}

VertexList::VertexList(int formatDescriptor, int numberOfEntries)
{
	//printf("My format: %d \n", formatDescriptor);
	vertexData = nullptr;
	currEntries = 0;
	indizes = nullptr;
	currEdges = 0;
	this->containsCoordinates = (formatDescriptor & (vertexCoordinates)) != 0;

	this->containsVertexColor = (formatDescriptor & (vertexColor)) != 0;
	//if (this->containsVertexColor) { puts("containes VColor"); }
	this->containsUVCoordinates = (formatDescriptor & (UVCoordinates)) != 0;
	//if (this->containsUVCoordinates) { puts("contains UVs"); }
	this->containsNormals = (formatDescriptor & (normals)) != 0;
	maxEntries = numberOfEntries;
	currEntries = 0;
	currEdges = 0;
	maxEdges = maxEntries * 1.5;
	indizes = new unsigned int[maxEdges];
	stride = 0;
	if (this->containsCoordinates == true)
	{
		stride += 3;
	}
	if (this->containsVertexColor == true)
	{
		stride += 3;
	}

	if (this->containsUVCoordinates == true)
	{
		stride += 2;
	}
	if (this->containsNormals == true)
	{
		stride += 3;
	}
	//printf("striding: %d", stride);
	//!!Change Back!!
	//stride = 3;
	//printf("allocate: %d\n", maxEntries*stride);
	vertexData = new float[maxEntries*stride];
}

VertexList::~VertexList()
{
	//TODO: fix crash on cleanUp!!!
	//if(indizes!=nullptr){ delete []indizes; }


	//
	if (vertexData != nullptr) {
		/*delete  []vertexData;
	}
}
*/