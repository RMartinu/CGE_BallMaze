#pragma once
#include<vector>
#include "Vertex.h"
using namespace std;
class Mesh
{
private:
	//double pos_x, pos_y, pos_z;
	//double width, height, depth
	vector<Vertex> vertList;


public:
	bool intersectsWithSphere(double pos_x, double pos_y, double pos_z, double radius);
	vector<Vertex> getVertices();
	int getVertexCount();
	Mesh();
	Mesh(double pos_x, double pos_y, double width, double depth, double height);
	Mesh(double pos_x, double pos_y);
	Mesh(double pos_x, double pos_y, bool isSphere);
	void beepMe(int pos_x, int pos_y);
	~Mesh();


};