#include "Mesh.h"
#include "Vertex.h"
#include <vector>
#include "VertexList.h"
using namespace std;


vector<Vertex> Mesh::getVertices()
{
	return this->vertList;
}

int Mesh::getVertexCount()
{
	return this->vertList.size();
}

Mesh::Mesh()
{
	//Creates a blank Mesh; used for arbitrary geometry, needs to set all parameters post construction

}

Mesh::Mesh(double pos_x, double pos_y, double width, double depth, double height)
{
	//Creates a cubical Mesh with specified dimensions; used to create the floor plate
	Vertex v[12];
	v[0].x = pos_x; v[0].y = pos_y; v[0].z = 0; v[0].u = 0.5; v[0].v = 0.5; //bottom, left, back
	v[1].x = pos_x + width; v[1].y = pos_y; v[1].z = 0; v[1].u = 0.5; v[1].v = 0; //bottom, right, back
	v[2].x = pos_x; v[2].y = pos_y + depth; v[2].z = 0; v[2].u = 0.5; v[2].v = 0; //bottom, left, front
	v[3].x = pos_x; v[3].y = pos_y; v[3].z = height; v[3].u = 1; v[3].v = 0.5; //top, left, back
	v[4].x = pos_x + width; v[4].y = pos_y + depth; v[4].z = 0; v[4].u = 0.5; v[4].v = 0.5; //bottom, right, front
	v[5].x = pos_x + width; v[5].y = pos_y; v[5].z = height; v[5].u = 1; v[5].v = 0; //top, right, back
	v[6].x = pos_x; v[6].y = pos_y + depth; v[6].z = height; v[6].u = 1; v[6].v = 0; //top, left, front
	v[7].x = pos_x + width; v[7].y = pos_y + depth; v[7].z = height; v[7].u = 1; v[7].v = 0.5; //top, right, front

	v[8].x = pos_x; v[8].y = pos_y; v[8].z = 0; v[8].u = 1; v[8].v = 0.5; //2 bottom, left, back
	v[9].x = pos_x + width; v[9].y = pos_y; v[9].z = 0; v[9].u = 1; v[9].v = 1; //2 bottom, right, back
	v[10].x = pos_x; v[10].y = pos_y + depth; v[10].z = 0; v[10].u = 0.5; v[10].v = 0.5; //2 bottom, left, front
	v[11].x = pos_x + width; v[11].y = pos_y + depth; v[11].z = 0; v[11].u = 0.5; v[11].v = 1; //2 bottom, right, front


	//printf("%d %d %d %d %d", pos_x, pos_y, width, depth, height);
	//printf("%d %d %d %d %d %d %d", v[0].y, v[1].y, v[2].y, v[3].y, v[4].y, v[5].y, v[6].y);
	//first triangle, bottom lower
	vertList.push_back(v[8]);
	vertList.push_back(v[9]);
	vertList.push_back(v[10]);

	//second triangle, bottom upper
	vertList.push_back(v[9]);
	vertList.push_back(v[10]);
	vertList.push_back(v[11]);

	//third triangle left lower
	vertList.push_back(v[0]);
	vertList.push_back(v[2]);
	vertList.push_back(v[3]);


	//forth triangle left upper
	vertList.push_back(v[2]);
	vertList.push_back(v[3]);
	vertList.push_back(v[6]);


	//fifth triangle right lower
	vertList.push_back(v[1]);
	vertList.push_back(v[4]);
	vertList.push_back(v[5]);

	//sixt triagle right upper
	vertList.push_back(v[4]);
	vertList.push_back(v[5]);
	vertList.push_back(v[7]);

	//seventh triangle top upper
	vertList.push_back(v[3]);
	vertList.push_back(v[5]);
	vertList.push_back(v[6]);


	//eigth triangle top lower
	vertList.push_back(v[5]);
	vertList.push_back(v[6]);
	vertList.push_back(v[7]);

	//nineth tri back lower
	vertList.push_back(v[0]);
	vertList.push_back(v[1]);
	vertList.push_back(v[3]);

	//tenth tri back upper
	vertList.push_back(v[1]);
	vertList.push_back(v[3]);
	vertList.push_back(v[5]);

	//eleventh front upper
	vertList.push_back(v[4]);
	vertList.push_back(v[6]);
	vertList.push_back(v[7]);

	//twelfth front lower
	vertList.push_back(v[2]);
	vertList.push_back(v[4]);
	vertList.push_back(v[6]);


	//printf("Vertex data: %d %d %d", v[0].x, v[0].y, v[0].z);
	//printf("This mesh consists of %d vertices\n", vertList.size());

}

Mesh::Mesh(double pos_x, double pos_y)
{
	//printf("Building at: %f %f\n", pos_x, pos_y);
	//creates a unity cube at the specified xy-coordinates and at height zero
	Vertex v[12];
	v[0].x = pos_x; v[0].y = pos_y; v[0].z = 0; v[0].u = 0; v[0].v = 0.5; //bottom, left, back
	v[1].x = pos_x + 1; v[1].y = pos_y; v[1].z = 0; v[1].u = 0; v[1].v = 0; //bottom, right, back
	v[2].x = pos_x; v[2].y = pos_y + 1; v[2].z = 0; v[2].u = 0; v[2].v = 0; //bottom, left, front
	v[3].x = pos_x; v[3].y = pos_y; v[3].z = 3; v[3].u = 0.5; v[3].v = 0.5; //top, left, back
	v[4].x = pos_x + 1; v[4].y = pos_y + 1; v[4].z = 0; v[4].u = 0; v[4].v = 0.5; //bottom, right, front
	v[5].x = pos_x + 1; v[5].y = pos_y; v[5].z = 3; v[5].u = 0.5; v[5].v = 0; //top, right, back
	v[6].x = pos_x; v[6].y = pos_y + 1; v[6].z = 3; v[6].u = 0.5; v[6].v = 0; //top, left, front
	v[7].x = pos_x + 1; v[7].y = pos_y + 1; v[7].z = 3; v[7].u = 0.5; v[7].v = 0.5; //top, right, front

	v[8].x = pos_x; v[8].y = pos_y; v[8].z = 3; v[8].u = 0.5; v[8].v = 0.5; //v[8].nx = 0; v[8].ny = 0; v[8].nz = 1; //2 top, left, back	
	v[9].x = pos_x + 1; v[9].y = pos_y; v[9].z = 3; v[9].u = 0.5; v[9].v = 1; //v[9].nx = 0; v[9].ny = 0; v[9].nz = 1; //2 top, right, back
	v[10].x = pos_x; v[10].y = pos_y + 1; v[10].z = 3; v[10].u = 0; v[10].v = 0.5; //v[10].nx = 0; v[10].ny = 0; v[10].nz = 1;//2 top, left, front
	v[11].x = pos_x + 1; v[11].y = pos_y + 1; v[11].z = 3; v[11].u = 0; v[11].v = 1; //v[11].nx = 0; v[11].ny = 0; v[11].nz = 1;//2 top, right, front

	//first triangle, bottom lower
	vertList.push_back(v[0]);
	vertList.push_back(v[1]);
	vertList.push_back(v[2]);

	//second triangle, bottom upper
	vertList.push_back(v[1]);
	vertList.push_back(v[2]);
	vertList.push_back(v[4]);

	//third triangle left lower
	vertList.push_back(v[0]);
	vertList.push_back(v[2]);
	vertList.push_back(v[3]);


	//forth triangle left upper
	vertList.push_back(v[2]);
	vertList.push_back(v[3]);
	vertList.push_back(v[6]);


	//fifth triangle right lower
	vertList.push_back(v[1]);
	vertList.push_back(v[4]);
	vertList.push_back(v[5]);

	//sixt triagle right upper
	vertList.push_back(v[4]);
	vertList.push_back(v[5]);
	vertList.push_back(v[7]);

	//seventh triangle top upper
	vertList.push_back(v[8]);
	vertList.push_back(v[9]);
	vertList.push_back(v[10]);


	//eigth triangle top lower
	vertList.push_back(v[9]);
	vertList.push_back(v[10]);
	vertList.push_back(v[11]);

	//nineth tri back lower
	vertList.push_back(v[0]);
	vertList.push_back(v[1]);
	vertList.push_back(v[3]);

	//tenth tri back upper
	vertList.push_back(v[1]);
	vertList.push_back(v[3]);
	vertList.push_back(v[5]);

	//eleventh front upper
	vertList.push_back(v[4]);
	vertList.push_back(v[6]);
	vertList.push_back(v[7]);

	//twelfth front lower
	vertList.push_back(v[2]);
	vertList.push_back(v[4]);
	vertList.push_back(v[6]);


	//printf("first vertex: %f %f %f", vertList.at(0).x, vertList.at(0).y, vertList.at(0).z);


}

Mesh::Mesh(double pos_x, double pos_y, bool isSphere)
{
	const float r = 0;
	const float g = 132;
	const float b = 255;

	Vertex v[19];
	v[0].x = 0.000000, v[0].y = 0.707107, v[0].z = -0.707107, v[0].r = r, v[0].g = g, v[0].b = b;

}

void Mesh::beepMe(int pos_x, int pos_y)
{
	//printf("reached me: %d %d", pos_x, pos_y);
}

Mesh::~Mesh()
{
}

VertexList::VertexList(const VertexList & in)
{
	this->containsCoordinates = in.containsCoordinates;
	this->containsNormals = in.containsNormals;
	this->containsUVCoordinates = in.containsUVCoordinates;
	this->containsVertexColor = in.containsVertexColor;
	this->currEdges = in.currEdges;
	this->currEntries = in.currEntries;
	this->maxEdges = in.maxEdges;
	this->maxEntries = in.maxEntries;
	this->stride = in.stride;

	//deep copy indizes
	this->indizes = new unsigned int[currEdges];
	for (int i = 0; i < currEdges; ++i) { this->indizes[i] = in.indizes[i]; }

	//deep copy vertex data
	this->vertexData = new float[currEntries];
	for (int i = 0; i < currEntries; ++i)
	{
		this->vertexData[i] = in.vertexData[i];
	}
}
