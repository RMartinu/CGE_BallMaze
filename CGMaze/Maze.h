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

using namespace std;
class Maze
{
	int width, height;
	double ball_x, ball_y,ball_z;
	int ballStart_x, ballStart_y;
	double pitch, roll, prevPitch, prevRoll;
	double rollFlag,pitchFlag;
	double ballVelocity_x, ballVelocity_y, ballVelocity_z;
	vector<Mesh> meshes;
	int[] mazeGrid;

	public Maze(ppmImage floorplan);
	public ~Maze();
	public vector<Mesh> getMeshes();
	public void roll(int speed);
	public void pitch(int speed);
	public void resetMaze();
	public void advance(time_t deltaTime);
	private bool checkCollision();
	private void handleCollision();
	private void rotateField(time_t deltaTime);
	private void moveBall(time_t deltaTime);
	public void getRotations(); //returns struct with rotation infos
	private 
};

class Mesh
{
	double pos_x, pos_y, pos_z;
	double width, height, depth;
	vector<Vertex> vertexList;

	public bool intersectsWithSphere(double pos_x, double pos_y, double pos_z, double radius);
	public vector<Vertex> getVertices();


};
class Vertex
{
	public double x, y, z, u, v;
};
