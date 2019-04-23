#pragma once
/**
Header file for Maze.cpp
*/
#include "ppm.h"
#include <vector>
#include "OBJLoad.h"
#include "Vertex.h"
#include "Mesh.h"
#include "VertexList.h"



using namespace std;









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
	//OBJLoad *theBall;

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



