#pragma once
/**
Header file for Maze.cpp
*/
#include "ppm.h"
#include <vector>


#define epsilon_F 0.001

#define pitchRate 10
#define rollRate 10
#define gravity 9.81
#define ballRadius 1
#define ballMass 1
#define maxPitch 30
#define maxRoll 30

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

using namespace std;

class Vertex
{
public:
	double x, y, z, u, v, r, g, b;
	Vertex(float ix, float iy, float iz);
	Vertex();
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

public:	

	VertexList(int formatDescriptor);
	VertexList(int formatDescriptor, int numberOfEntries);
	~VertexList();
	bool addVertex(float x, float y, float z);
	bool addVertex(float x, float y, float z, float r, float g, float b);
	bool addVertex(float x, float y, float z, float r, float g, float b, float u, float v);
	bool addVertex(float x, float y, float z, float u, float v);
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
	 void advance(int deltaTime);
	 int checkCollision();
	 void handleCollision();
	 void rotateField(int deltaTime);
	 void moveBall(int deltaTime);
	 //Mesh getBall();
	 void getRotations(); //returns struct with rotation infos
	 int** getTouchedGround();
	 int getFloorAt(double width, double height);
};



