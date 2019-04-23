#include "Maze.h"
#include <glm\ext\scalar_constants.hpp>
#include "ppm.h"

#include "OBJLoad.h"
#include "defines.h"
/**
 This file contains the main game board and game logic
 */





Maze::Maze(ppmImage &floorplan)
{

	//theBall =  new OBJLoad("Resource\\UV-sphere.obj.txt");
	if (floorplan.getWidth() == 0 || floorplan.getHeight() == 0)
	{

		this->width = 300;
		this->height = 300;
	}
	else
	{

		this->width = floorplan.getWidth();
		this->height = floorplan.getHeight();

	}
	mazeGrid = new int[width*height];
	
	Mesh floor(0, 0, this->width, this->height, -1);
	meshes.push_back(floor);
	//printf("Vertexcount in mazefloor %d while the mesh has %d vertices \n", floor.getVertices().size(), floor.getVertexCount());

	
	
	if ((floorplan.getWidth() != 0 || floorplan.getHeight() != 0))
	{
		for (int h = 0; h < height; ++h)
		{
			for (int w = 0; w < width; ++w)
			{

				if (floorplan.getPixel(w, h).r == 255 && floorplan.getPixel(w, h).g == 255 && floorplan.getPixel(w, h).b == 255)
				{
					mazeGrid[h*width + w] = way;
					//printf("x: %d, y: %d, type: way\n", w,h);
					continue;
				}
				if (floorplan.getPixel(w, h).r == 0 && floorplan.getPixel(w, h).g == 0 && floorplan.getPixel(w, h).b == 0)
				{
					mazeGrid[h*width + w] = wall;
					//printf("############Wall %d %d\n", w, h);
					Mesh newWall(((double)w), ((double)h));
					//printf("############Wall %d %d\n", w, h);

					newWall.beepMe(w,h);
					//printf("new mesh actual: %d, %d, %d \n", newWall.getVertices().at(0).x, newWall.getVertices().at(1).x, newWall.getVertices().at(2).x);
					//meshes.push_back(newWall);
					//printf("x: %d, y: %d, type: wall\n", w, h);
					continue;
				}
				if (floorplan.getPixel(w, h).r == 0 && floorplan.getPixel(w, h).g == 255 && floorplan.getPixel(w, h).b == 0)
				{
					mazeGrid[h*width + w] = ballstart;
					ballStart_x = w;
					ballStart_y = h;
					//printf("x: %d, y: %d, type: start\n", w, h);
					continue;
				}
				if (floorplan.getPixel(w, h).r == 255 && floorplan.getPixel(w, h).g == 0 && floorplan.getPixel(w, h).b == 0)
				{
					mazeGrid[h*width + w] = finish;
					Mesh newFinish(w, h, 1, 1, 0.1);
					//meshes.push_back(newFinish);
					//printf("x: %d, y: %d, type: finish\n", w, h);
					continue;
				}
				if (floorplan.getPixel(w, h).r == 0 && floorplan.getPixel(w, h).g == 0 && floorplan.getPixel(w, h).b == 255)
				{
					mazeGrid[h*width + w] = hole;
					//printf("x: %d, y: %d, type: pit\n", w, h);
					continue;
				}


			}
		}
	}
	//floorplan.getPixel();
	
}

 Maze::~Maze()
{
	 delete[] mazeGrid;
	 //delete theBall;
}

 vector<Mesh> Maze::getMeshes()
 {
	 return meshes;
 }

 VertexList Maze::getVertexList()
 {
	 //puts("getting list");
	 VertexList V(vertexCoordinates|vertexColor|UVCoordinates|normals,3); //####normals

	 int  meshcount = 0;
	 vector<Mesh> Meshes = getMeshes();
	 for each(Mesh t in Meshes)
	 {
		 //printf("Mesh no: %d with %d\n", ++meshcount, t.getVertices().size());
		 vector<Vertex> mverts = t.getVertices();
		 for (int i = 0; i < mverts.size(); i += 3)
		 {
			 //printf("Adding triangle no %d:  %f %f %f\n",i, mverts.at(i).x, mverts.at(i + 1).x, mverts.at(i + 2).x);

			 Vertex a, b, c;
			 a = mverts.at(i);
			 b = mverts.at(i + 1);
			 c = mverts.at(i + 2);
			 a.r = b.g = c.b = 1;

			 V.addTriangle(a,b,c);
		 }
		 

	 }
	 return V;
 }

 VertexList Maze::getBallVertices()
 {
	 VertexList V(vertexCoordinates|vertexColor|normals,8);

	 //Mesh BallMesh = getBall();

	 //vector<Vertex> mverts = BallMesh.getVertices();
	 //for (int i = 0; i < mverts.size(); i += 3)
	 //{
		// //printf("Adding triangle no %d:  %f %f %f\n",i, mverts.at(i).x, mverts.at(i + 1).x, mverts.at(i + 2).x);

		// Vertex a, b, c;
		// a = mverts.at(i);
		// b = mverts.at(i + 1);
		// c = mverts.at(i + 2);
		// a.r = b.g = c.b = 0.5;
		// V.addTriangle(a, b, c);
	 //}
	 

	 //VertexList v = theBall->getVertexList();
	 V.shiftAllVertices(ball_x, ball_y,3);
	 return V;
 }

 Mesh Maze::getBall()
 {

	 //Mesh theGlobe(ball_x, ball_y, true);
	 Mesh theGlobe(ball_x, ball_y);
	 return theGlobe;
 }

 void Maze::updateRoll(int speed)
{
	prevRoll = roll;
	rollFlag = speed*rollRate;
	

}

void Maze::updatePitch(int speed)
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

void Maze::advance(double deltaTime)
{
	rotateField(deltaTime);
	moveBall(deltaTime);
	if (checkCollision())
	{
		//printf("Collision imminent!!\n");
		//handleCollision();


	}
	//printf("Pitch roll updatestate up %f, ur %f\n", this->rollFlag, this->pitchFlag);
	//printf("this mazes current orientation: p: %f r: %f \n", this->pitch, this->roll);

}

int Maze::checkCollision()
{
	int status = 0;
	int **area = getTouchedGround();
	for (int h = 0; h<4; ++h)
	{
		for (int w = 0; w < 4; ++w)
		{
			/*if (status < area[h][w])
			{
				status = area[h][w];
			}
*/
			//printf("%d", area[h][w]);
			status = status | (1 << area[h][w]); //every bit represents a bool of a floor type
		}

	}
	
	for (int i = 0; i < 4; ++i)
	{
		delete area[i];
	}
	delete area;

	//printf("Status: %d\n", status);
	return status;
}

void Maze::handleCollision()
{
	int **candidates=getTouchedGround();

	bool collided_x = false;
	bool collided_y = false;
	
	for (int h = ball_y - 2; h < ball_y + 2; ++h)
	{
		for (int w = ball_x - 2; w < ball_x + 2; ++w)
		{
			switch (mazeGrid[h*width + w])
			{
			case finish:
			{
				puts("im done with this");
				if (ball_x<w + 1 && ball_x>w && ball_y<h + 1 && ball_y>h)
				{
					gameState = gameFinished;
				}

				break; }
			//case wall:
			//{

			//	
			//	if (ball_x > (w + 1) && ball_x - ballRadius < (w + 1))
			//	{
			//		
			//		//On the rights side and colliding
			//		if (!collided_x)
			//		{printf("Touching wall on the left\n");
			//			collided_x= true;

			//			ballVelocity_x *= -0.75;
			//		}
			//	}
			//	if (ball_y < h && (ball_y + ballRadius) > h)
			//	{
			//		//above and colliding
			//		if (!collided_y)
			//		{
			//			printf("Touching wall below\n");
			//			collided_y = true;
			//			printf("Velocity y: %f", ballVelocity_y);
			//			if (ballVelocity_y>1)
			//			{
			//				ballVelocity_y *= -0.75;
			//			}
			//			else
			//			{
			//				ball_y = (int)ball_y;
			//				ballVelocity_y *= 0;
			//			}

			//		
			//		}
			//	}
			//	if (ball_x <w && ball_x + ballRadius>w)
			//	{
			//		//left and coliding
			//		if (!collided_x)
			//		{
			//			printf("Touching wall on the right\n");
			//			collided_x = true;

			//			ballVelocity_x *= -0.75;
			//		}
			//	}
			//	if (ball_y > (h + 1) && (ball_y-ballRadius) < (h + 1))
			//	{
			//		//below and colliding
			//		if (!collided_y)
			//		{
			//			printf("Touching wall above\n");
			//			collided_y = true;

			//			ballVelocity_y *= -0.75;
			//		}
			//	}

			//	break; }
			case wall:
			{


				if (ball_x > (w) && (ball_x - ballRadius) < (w))
				{
					
					//On the rights side and colliding
					if (roll>0)
					{
					//puts("XXXXX");
					//if (!collided_x)
					{
						printf("Touching wall on the left\n");
						collided_x = true;
						ball_x = (int)ball_x+1;
						ballVelocity_x *= 0;
					}
					}
				}
				if (ball_y < h && (ball_y + ballRadius) > h)
				{
					//above and colliding
					if (pitch > 0)
					{
						//if (!collided_y)
						{
							printf("Touching wall below\n");
							collided_y = true;
							ball_y = (int)ball_y;
								ballVelocity_y *= 0;
							


						}
					}
				}
				if (ball_x <(w )&& ball_x + ballRadius>w)
				{
					//left and coliding
					if (roll < 0)
					{
						//if (!collided_x)
						{
							printf("Touching wall on the right\n");
							collided_x = true;
							ball_x = (int)ball_x;
							ballVelocity_x *= 0;
						}
					}
				}
				if (ball_y > (h ) && (ball_y - ballRadius) < (h ))
				{
					//below and colliding
					if (pitch < 0)
					{
						//if (!collided_y)
						{
							printf("Touching wall above\n");
							collided_y = true;

							ball_y = (int)ball_y+1;
							ballVelocity_y *= 0;
						}
					}
				}

				break; }
			case hole:
			{
				puts("down the rabbit hole");
				if (ball_x<w + 1 && ball_x>w && ball_y<h + 1 && ball_y>h)
				{
					gameState = gameLost;
				}
				break; }
			default: {break; }

			}
		}
	}

		for (int i = 0; i < 4; ++i)
		{
			delete candidates[i];
		}
		delete candidates;


}

void Maze::rotateField(double deltaTime)
{
	prevPitch = pitch;
	prevRoll = roll;
	pitch =pitch +(pitchFlag * pitchRate) * (deltaTime);
	//printf("deltapitch %f %f %f \n", pitchFlag, pitchRate, deltaTime);
	roll += (rollFlag * rollRate) * (deltaTime );
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

	//printf("actual pitch %f, act roll %f\n", pitch, roll);
}


void Maze::moveBall(double deltaTime)
{
	//ballVelocity_x = fmin(fmax(ballVelocity_x -roll/5,-maxVelocity),maxVelocity);
	//ballVelocity_y = fmin(fmax(ballVelocity_y + pitch / 5, -maxVelocity), maxVelocity);
	
	//printf("veloX: %f, veloY %f\n", ballVelocity_x, ballVelocity_y);

	double fx, fy;
	fx = ball_x + ballVelocity_x * deltaTime;
	fy = ball_y + ballVelocity_y * deltaTime;

	double cogx=(int)fx+0.5, cogy=(int)fy+0.5;	
	

		// y=kx+d
	
		//k=y/x

		double kVel = ballVelocity_y / ballVelocity_x;
		double kImp = (ball_y - cogy) / (ball_x - cogx);


		int xoffset, yoffset;
		if (ballVelocity_x > 0)
		{
			xoffset = 1;
		}
		else
		{
			xoffset = 0;
		}
		if (ballVelocity_y > 0)
		{
			yoffset = 1;
		}
		else
		{
			yoffset = 0;
		}

	if (this->getFloorAt(fx+xoffset, fy+yoffset) != wall)
	{//move	
	ball_x += ballVelocity_x * (deltaTime);
	ball_y += ballVelocity_y * (deltaTime);
	//ballVelocity_x = -roll;
	//ballVelocity_y = pitch;

	ballVelocity_x = fmin(fmax(ballVelocity_x -roll/5,-maxVelocity),maxVelocity);
	ballVelocity_y = fmin(fmax(ballVelocity_y + pitch / 5, -maxVelocity), maxVelocity);

	}
	else
	{//plan b
		//if (kVel <= kImp)
		//{
		//	ballVelocity_x *= -1;
		//}
		//if (kVel >= kImp)
		//{
		//	ballVelocity_y *= -1;
		//}
		//printf("Vect: %f\n", kVel);
		//double px=ball_x, py=ball_y;
		//px += ballVelocity_x * (deltaTime);
		//py += ballVelocity_y * (deltaTime);
		//if (getFloorAt(px, py) != wall)
		//{
		//	ball_x = px;
		//	ball_y = py;

		//	puts("bounce");
		//}
		//else
		//{
		//	puts("redisplace");

		//	//ballVelocity_x = ballVelocity_y = 0;
		//
		//	int sx, sy;
		//	double minDist = 999;
		//	for (int i=(int)ball_x-1; i<(int)ball_x+2;++i)
		//	{
		//		for (int j = (int)ball_y - 1; j < (int)ball_y + 2; ++j)
		//		{
		//			if (getFloorAt(i, j) != wall)
		//			{
		//				//calc distanace
		//				double d = abs(i - ball_x) + abs(j - ball_y);
		//				if (d < minDist)
		//				{
		//					minDist = d;
		//					sx = i; sy = j;
		//				}
		//			}
		//		}
		//	}

		//	if ((ball_x > sx && ballVelocity_x > 0)|| (ball_x < sx && ballVelocity_x < 0))
		//	{
		//		ballVelocity_x = 0;
		//	}

		//	if ((ball_y > sy && ballVelocity_y > 0) || (ball_y < sy && ballVelocity_y < 0))
		//	{
		//		ballVelocity_y = 0;
		//	}
		//	ball_x = sx; ball_y = sy;

		/*}*/
		
	//	ballVelocity_x = ballVelocity_y = 0;

		bool lateral = false;
		bool ventral = false;
		if (getFloorAt(ball_x+1, ball_y) == wall || getFloorAt(ball_x-1, ball_y) == wall)
		{
			puts("lateral collision");
			ball_x = (int)(ball_x + 0.5);
			ballVelocity_x *= -1;
			lateral = true;
		}
		if (getFloorAt(ball_x, ball_y+1) == wall || getFloorAt(ball_x, ball_y-1) == wall)
		{
			puts("dorsal");
			ball_y = (int)(ball_y + 0.5);
			ballVelocity_y *= -1;
			ventral = true;
		}
		 

		if (lateral && ventral)
		{
			ballVelocity_x = ballVelocity_y = 0;
		}
		ball_x += ballVelocity_x * (deltaTime);
		ball_y += ballVelocity_y * (deltaTime);

		printf("blocked at %f %f \n", fx,fy);
	}

	//printf("Ballposition: x: %f, y: %f\n", ball_x, ball_y);
}

void Maze::getRotations()
{

}

double Maze::getRoll()
{
	return this->roll;
}

double Maze::getPitch()
{
	return this->pitch;
}

int** Maze::getTouchedGround()
{
	int** fp = new int*[4];
	for (int i = 0; i < 4; ++i)
	{
		fp[i] = new int[4];
	}
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
	return mazeGrid[(int)h*width + (int)w];

}

int Maze::getWidth()
{
	return this->width;
}

int Maze::getHeight()
{
	return this->height;
}


//bool VertexList::addVertex(float x, float y, float z)
//{
//	//printf("we add a vertex %f %f %f\n", x, y, z);
//	if (!containsCoordinates || containsVertexColor || containsUVCoordinates || containsNormals)
//	{
//		puts("Wrong data format");
//		return false;
//	}
//	if (currEntries >= maxEntries)
//	{
//		extendVertexData();
//	}
//	vertexData[currEntries*stride] = x;
//	vertexData[currEntries*stride+1] = y;
//	vertexData[currEntries*stride+2] = z;
//	++currEntries;
//	
//	//printf("\n### new vcount: %d\n", numberOfVertices);
//	
//
//	return true;
//}

