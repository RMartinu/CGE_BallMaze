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

using namespace std;
class Maze
{

	private int width, height;
	private double ball_x, ball_y, ball_z;
	private int ballStart_x, ballStart_y;
	private double pitch, roll, prevPitch, prevRoll;
	private double rollFlag,pitchFlag;
	private double ballVelocity_x, ballVelocity_y, ballVelocity_z;
	private vector<Mesh> meshes;
	private int mazeGrid[];
	private int gameState;

	public Maze(ppmImage floorplan);
	public ~Maze();
	public vector<Mesh> getMeshes();
	public Mesh getBall();
	public void roll(int speed);
	public void pitch(int speed);
	public void resetMaze();
	public void advance(time_t deltaTime);
	private int checkCollision();
	private void handleCollision();
	private void rotateField(time_t deltaTime);
	private void moveBall(time_t deltaTime);
	public void getRotations(); //returns struct with rotation infos
	private int** getTouchedGround();
	private int getFloorAt(double width, double height);
};

class Mesh
{
	double pos_x, pos_y, pos_z;
	double width, height, depth;
	vector<Vertex> vertexList;


	public bool intersectsWithSphere(double pos_x, double pos_y, double pos_z, double radius);
	public vector<Vertex> getVertices();
	public Mesh();
	public Mesh(double pos_x, double pos_y, double width, double depth,  double height);
	public Mesh(double pos_x, double pos_y);
	public ~Mesh();


};
class Vertex
{
	public double x, y, z, u, v;
};
