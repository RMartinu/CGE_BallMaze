#pragma once

#include "Vertex.h"
class VertexList
{
private:
	unsigned int* indizes;
	float* vertexData;
	int stride;

	int maxEntries;
	int currEntries;
	int maxEdges;
	int currEdges;
	bool containsCoordinates;
	bool containsVertexColor;
	bool containsUVCoordinates;
	bool containsNormals;

public:

	VertexList(const VertexList &in);
	VertexList(int formatDescriptor);
	VertexList(int formatDescriptor, int numberOfEntries);
	~VertexList();
	//bool addVertex(float x, float y, float z);
	bool addVertex(float x, float y, float z, float r, float g, float b, float nx, float ny, float nz);
	bool addVertex(float x, float y, float z, float r, float g, float b, float u, float v, float nx, float ny, float nz);
	//bool addVertex(float x, float y, float z, float u, float v);
	bool addVertex(Vertex v);
	bool addIndex(int vertex1, int vertex2, int vertex3);
	bool extendVertexData();
	bool extendIndizes();
	bool addTriangle(Vertex v1, Vertex v2, Vertex v3);
	int findVertex(Vertex v);
	unsigned int* getIndizes();
	int getIndexCount();
	int getStride();
	float* getVertexData();
	int getVertexCount();
	bool getContainsCoordinates();
	bool getContainsVertexColor();
	bool getContainsUVCoordinates();
	bool VertexList::getContainsNormals();
	void shiftAllVertices(double x, double y, double z);
};

