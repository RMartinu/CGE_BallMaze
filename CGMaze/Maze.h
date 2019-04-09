#pragma once
/**
Header file for Maze.cpp
*/
#include "ppm.h"
#include <vector>

#define pitchRate 10
#define rollRate 10
#define gravity 9.81
#define ballRadius 1
#define ballMass 1
#define maxPitch 30
#define maxRoll 30

#define way 0
#define start 1
#define wall 2
#define hole 3
#define finish 4

#define untouching -1

#define gameFinished 1
#define gameRunning 0
#define gameLost 2

#define vertexCoordinates 0
#define vertexColor 1
#define UVCoordinates 2

using namespace std;

class Vertex
{
public:
	double x, y, z, u, v;
};



class Mesh
{
private:
	double pos_x, pos_y, pos_z;
	double width, height, depth;
	vector<Vertex> vertexList;


public:
	bool intersectsWithSphere(double pos_x, double pos_y, double pos_z, double radius);
	vector<Vertex> getVertices();
	Mesh();
	Mesh(double pos_x, double pos_y, double width, double depth, double height);
	Mesh(double pos_x, double pos_y);
	~Mesh();


};


class VertexList
{
private:
	int* indizes;
	int numberOfIndizes;
	float* vertexData;
	int numberOfVertices;
	int maxEntries;
	int maxEdges;
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
	int* getIndizes();
	int getIndexCount();
	float* getVertexData();
	int getVertexCount();


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

public: Maze(ppmImage floorplan);
	 ~Maze();
	 vector<Mesh> getMeshes();
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



