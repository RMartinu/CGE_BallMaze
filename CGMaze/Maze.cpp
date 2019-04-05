#include "Maze.h"
#include "ppm.h"
/**
 This file contains the main game board and game logic
 */





Maze::Maze(ppmImage floorplan)
{
	this->width = floorplan.getWidth();
	this->height = floorplan.getHeight();
	mazeGrid = new int[width*height];
	for(int h = 0; h<height; ++h)
	{
		for (int w = 0; w < width; ++w)
		{
			
			if (floorplan.getPixel(w, h).r == 255 && floorplan.getPixel(w, h).g == 255 && floorplan.getPixel(w, h).b == 255)
			{
				mazeGrid[h*width + w] = way;
				continue;
			}
			if (floorplan.getPixel(w, h).r == 0 && floorplan.getPixel(w, h).g == 0 && floorplan.getPixel(w, h).b == 0)
			{
				mazeGrid[h*width + w] = wall;
				continue;
			}
			if (floorplan.getPixel(w, h).r == 0 && floorplan.getPixel(w, h).g == 255 && floorplan.getPixel(w, h).b == 0)
			{
				mazeGrid[h*width + w] = start;
				ballStart_x = w;
				ballStart_y = h;
				continue;
			}
			if (floorplan.getPixel(w, h).r == 255 && floorplan.getPixel(w, h).g == 0 && floorplan.getPixel(w, h).b == 0)
			{
				mazeGrid[h*width + w] = finish;
				continue;
			}
			if (floorplan.getPixel(w, h).r == 0 && floorplan.getPixel(w, h).g == 0 && floorplan.getPixel(w, h).b == 255)
			{
				mazeGrid[h*width + w] = hole;
				continue;
			}


		}
	}
	floorplan.getPixel()
}

 Maze::~Maze()
{
	 delete mazeGrid;
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

int Maze::checkCollision()
{
	int status = 0;
	int[4][4] area = getTouchedGround();
	for (int h = 0; h<4; ++h)
	{
		for (int w = 0; w < 4; ++w)
		{
			/*if (status < area[h][w])
			{
				status = area[h][w];
			}
*/
			status = status | (1 << area[h][w]); //every bit represents a bool of a floor type
		}

	}



	return false;
}

void Maze::handleCollision()
{
	int candidates[4][4] = getTouchedGround();
	
	
	for (int h = ball_y-2; h < ball_y+2; ++h)
	{
		for (int w = ball_x-2; w < ball_x+2; ++w)
		{
			switch (mazeGrid[h][w])
			{
			case finish:
			{

				if (ball_x<w + 1 && ball_x>w && ball_y<h + 1 && ball_y>h)
				{
					gameState = gameFinished;
				}
				
				break; }
			case wall:
			{

				if (ball_x > (w + 1) && ball_x - ballRadius < (w + 1))
				{
					//On the rights side and colliding

					ballVelocity_x *= -1;
				}
				if (ball_y < (h && ball_y + ballRadius > h))
				{
					//below and colliding
					ballVelocity_y *= -1;
				}
				if (ball_x <w && ball_x + ballRadius>w)
				{
					//left and coliding
					ballVelocity_x *= -1;
				}
				if (ball_y > h + 1 && ball_ < -ballRadius < h + 1)
				{
					//above and colliding
					ballVelocity_y *= -1;
				}
				
				break; }
			case hole:
			{
				if (ball_x<w + 1 && ball_x>w && ball_y<h + 1 && ball_y>h)
				{
					gameState = gameLost;
				}
				break; }
			default: {break; }

			}
		}
	}

}

void Maze::rotateField(time_t deltaTime)
{
	prev_Pitch = pitch;
	prev_Roll = roll;
	pitch += (pitchFlag * pitchRate) * (deltaTime / (float)1000);
	roll += (rollFlag * rollRate) * (deltaTime / (float)1000);
	if (pitch > maxPitch)
	{
		pitch = maxPitch;
	}
	if (pitch < -maxPitch)
	{
		pitch = -maxPitch;
	}
	if (roll > maxRoll)
	{
		roll = maxRoll;
	}
	if (roll < -maxRoll)
	{
		roll = -maxRoll;
	}
}


void Maze::moveBall(time_t deltaTime)
{
	ball_x += ballVelocity_x * (deltaTime/(float)1000);
	ball_y += ballVelocity_y * (deltaTime / (float)1000);
}

int[4][4] Maze::getTouchedGround()
{
	int[4][4] fp;
	fp[0][0] = getFloorAt(ball_x - 2, ball_y - 2);
	fp[0][1] = getFloorAt(ball_x - 1, ball_y - 2);
	fp[0][2] = getFloorAt(ball_x, ball_y - 2);
	fp[0][3] = getFloorAt(ball_x + 1, ball_y - 2);
	fp[1][0] = getFloorAt(ball_x - 2, ball_y - 1);
	fp[1][1] = getFloorAt(ball_x - 1, ball_y - 1);
	fp[1][2] = getFloorAt(ball_x, ball_y - 1);
	fp[1][3] = getFloorAt(ball_x + 1, ball_y - 1);
	fp[2][0] = getFloorAt(ball_x - 2, ball_y);
	fp[2][1] = getFloorAt(ball_x - 1, ball_y);
	fp[2][2] = getFloorAt(ball_x, ball_y);
	fp[2][3] = getFloorAt(ball_x + 1, ball_y);
	fp[3][0] = getFloorAt(ball_x - 2, ball_y + 1);
	fp[3][1] = getFloorAt(ball_x - 1, ball_y + 1);
	fp[3][2] = getFloorAt(ball_x, ball_y + 1);
	fp[3][3] = getFloorAt(ball_x + 1, ball_y + 1);
	return fp;
}
int Maze::getFloorAt(double w, double h)
{
	return mazeGrid[h*width + width];

}
