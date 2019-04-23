#pragma once
/**
Header file for Maze.cpp
*/
#include "ppm.h"
#include <vector>


#define epsilon_F 0.001

#define pitchRate 2
#define rollRate 2
#define gravity 9.81
#define ballRadius 0.9
#define ballMass 1
#define maxPitch 30
#define maxRoll 30
#define maxVelocity 5

#define way 0
#define ballstart 1
#define wall 2
#define hole 3
#define finish 4

#define untouching -1

#define gameFinished 1
#define gameRunning 0
#define gameLost 2

#define vertexCoordinates (1<<0)
#define vertexColor (1<<1)
#define UVCoordinates (1<<2)
#define normals (1<<3)

using namespace std;

class Vertex
{
public:
	float x, y, z, u, v, r, g, b, nx,ny,nz;
	Vertex(float ix, float iy, float iz);
	Vertex()
	{
		x = y = z = u = v = r = g = b = nx = ny = nz = 0;
	};
};



class Mesh
{
private:
	//double pos_x, pos_y, pos_z;
	//double width, height, depth;
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

	VertexList(int formatDescriptor);
	VertexList(int formatDescriptor, int numberOfEntries);
	~VertexList();
	//bool addVertex(float x, float y, float z);
	bool addVertex(float x, float y, float z, float r, float g, float b, float nx, float ny, float nz);
	bool addVertex(float x, float y, float z, float r, float g, float b, float u, float v, float nx, float ny, float nz);
	VertexList transformVertexList(VertexList in);
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
	void printFullVertexList();
};


class Maze
{

private:
	int width, height;
	double ball_x, ball_y, ball_z;
	int ballStart_x, ballStart_y;
	double pitch, roll, prevPitch, prevRoll;
	double rollFlag,pitchFlag;
	double ballVelocity_x, ballVelocity_y, ballVelocity_z;
	vector<Mesh> meshes;
	int *mazeGrid;
	int gameState;

public: Maze(ppmImage &floorplan);
	 ~Maze();
	 vector<Mesh> getMeshes();

	 VertexList getVertexList();
	 VertexList getBallVertices();
	 Mesh getBall();
	 void updateRoll(int speed);
	 void updatePitch(int speed);
	 void resetMaze();
	 void advance(double deltaTime);
	 int checkCollision();
	 void handleCollision();
	 void rotateField(double deltaTime);
	 void moveBall(double deltaTime);
	 //Mesh getBall();
	 void getRotations(); //returns struct with rotation infos
	 double getRoll();
	 double getPitch();

	 int** getTouchedGround();
	 int getFloorAt(double width, double height);
	 int getWidth();
	 int getHeight();
};



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
	fullVert() {};
	fullVert(OBJLoad *res, int vspaceIndex, int uvIndex, int normalIndex);
	fullVert(const fullVert &in);
	Vertex get();
	void printMe() {
		printf("V\t cord: %f, %f, %f\tcolor: %f, %f, %f\t UV: %f, %f\t Normal: %f, %f, %f\n",this->vx, vy,vz, r,g,b,U,V,nx,ny,nz);
	};

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





class OBJLoad {


private:
	vector<vspace> vertics;
	vector<UV> uvCoords;
	vector<normal> Normal;
	vector<fullVert> fullVertices;
	vector<triangle> tris;
	float *vSet, *UVset, *NormalSet;
	int *indexList;
	float xoffset, yoffset, zoffset;


public:
	OBJLoad();
	OBJLoad(string filename);
	~OBJLoad();
	float * getVSet();
	float *getUVSet();
	float *getNormalSet();
	float *getInterlacedData();
	int getInterlacedDataSize() { return this->tris.size()*3; };
	int *getIndexList();
	int getVSetSize() {return this->vertics.size(); };
	int getUVSetSize() { return this->uvCoords.size(); };
	int getNormalSetSize() { return this->Normal.size(); };


	int getIndexListSize() { return this->tris.size(); };
	void setOffset(float x, float y, float z) { xoffset = x; yoffset = y; zoffset = z; };

	vspace getVertex(int index);
	UV getUv(int index);
	normal getNormal(int index);



	VertexList getVertexList(float, float, float);
};





