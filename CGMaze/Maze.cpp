#include "Maze.h"
#include "ppm.h"
´/**
 This file contains the main game board and game logic
 */



	public Maze::Maze(ppmImage floorplan)
{

}
	Maze::~Maze()
{
		
}

void Maze::roll(int speed)
{
	prevRoll = roll;
	rollFlag = speed*rollRate;

}

void Maze::pitch(int speed)
{

	prevPitch = pitch;
	pitchFlag = speed * pitchRate;
}

void Maze::resetMaze()
{
	pitch = 0;
	roll = 0;
	prevPitch = 0;
	prevRoll = 0;
	rollFlag = 0;
	pitchFlag = 0;
	ball_x = ballStart_x;
	ball_y = ballStart_y;

	ball_z = ballRadius; 
	ballVelocity_x = ballVelocity_y = ballVelocity_z = 0;
}

void Maze::advance(time_t deltaTime)
{
	rotateField(deltaTime);
	moveBall(deltaTime);
	if (checkCollision)
	{
		handleCollision();
	}
}

bool Maze::checkCollision()
{
	return false;
}
