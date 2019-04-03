#pragma once
/**
Header file for Maze.cpp
*/
#include "ppm.h"
#include <vector>

#define pitchRate 10
#define rollRate 10
#define gravity 9.81

using namespace std;
class Maze
{
	int width, height;
	double ball_x, ball_y,ball_z;
	double ballRadius, ballMass;
	double pitch, roll;
	double ballVelocity_x, ballVelocity_y, ballVelocity_z;
	vector<Mesh> meshes;

	public Maze(ppmImage floorplan);
	public ~Maze();
	public vector<Mesh> getMeshes();
	public void roll(int speed, time_t deltaTime);
	public void pitch(int speed, time_t deltaTime);
	public void reset();
	public void advance();
	private bool checkCollision();
	private void handleCollision();
	private void rotateField();
	private void moveBall();
	public void getRotations(); //returns struct with rotation infos
};

class Mesh
{
	double pos_x, pos_y, pos_z;
	double width, height, depth;

	public bool intersectsWithSphere(double pos_x, double pos_y, double pos_z, double radius);


};

