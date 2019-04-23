#pragma once

#include <string>
#include<vector>
//#include "Maze.h"
#include "Vertex.h"
#include "VertexList.h"

#define rdBuffer 4096
using namespace std;


class OBJLoad;
class vspace
{
public:
	double x, y, z;
};

class UV
{
public:
	double u, v;
};

class normal
{
public:
	double x, y, z;
};

class fullVert
{


public:
	fullVert() { r = 0; g = b = 1; };
	fullVert( OBJLoad *res, int vspaceIndex, int uvIndex, int normalIndex);
	fullVert(const fullVert &in);
	Vertex get();

	double vx, vy, vz;
	double nx, ny, nz;
	double U, V;
	double r, g, b;
};

class triangle
{

public: 
	triangle(fullVert a, fullVert b, fullVert c);
	fullVert a, b, c;
		
};





class OBJLoad{


	private:
		vector<vspace> vertics;
		vector<UV> uvCoords;
		vector<normal> normalList;
		vector<fullVert> fullVertices;
		vector<triangle> tris;


public:
	OBJLoad();
	OBJLoad(string filename);
	~OBJLoad();

	vspace getVertex(int index);
	UV getUv(int index);
	normal getNormal(int index);



	VertexList getVertexList();
};



