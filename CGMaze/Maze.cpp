#include "Maze.h"
#include <glm\ext\scalar_constants.hpp>
#include "ppm.h"
/**
 This file contains the main game board and game logic
 */





Maze::Maze(ppmImage &floorplan)
{
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

	

	if (floorplan.getWidth() != 0 || floorplan.getHeight() != 0)
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
					meshes.push_back(newWall);
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
					meshes.push_back(newFinish);
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

	// OBJLoad b("Resource\\UV-sphere.obj.txt");
	 return V;
 }

 VertexList Maze::getBallVertices()
 {
	 VertexList V(vertexCoordinates|vertexColor|normals,8);

	 Mesh BallMesh = getBall();

	 vector<Vertex> mverts = BallMesh.getVertices();
	 for (int i = 0; i < mverts.size(); i += 3)
	 {
		 //printf("Adding triangle no %d:  %f %f %f\n",i, mverts.at(i).x, mverts.at(i + 1).x, mverts.at(i + 2).x);

		 Vertex a, b, c;
		 a = mverts.at(i);
		 b = mverts.at(i + 1);
		 c = mverts.at(i + 2);
		 a.r = b.g = c.b = 0.5;
		 V.addTriangle(a, b, c);
	 }

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

vector<Vertex> Mesh::getVertices()
{
	return this->vertList;
}

int Mesh::getVertexCount()
{
	return this->vertList.size();
}

Mesh::Mesh()
{
	//Creates a blank Mesh; used for arbitrary geometry, needs to set all parameters post construction
	
}

Mesh::Mesh(double pos_x, double pos_y, double width, double depth, double height)
{
	//Creates a cubical Mesh with specified dimensions; used to create the floor plate
	Vertex v[12];
	v[0].x = pos_x; v[0].y = pos_y; v[0].z = 0; v[0].u = 0.5; v[0].v = 0.5; //bottom, left, back
	v[1].x = pos_x + width; v[1].y = pos_y; v[1].z = 0; v[1].u = 0.5; v[1].v = 0; //bottom, right, back
	v[2].x = pos_x; v[2].y = pos_y + depth; v[2].z = 0; v[2].u = 0.5; v[2].v = 0; //bottom, left, front
	v[3].x = pos_x; v[3].y = pos_y; v[3].z = height; v[3].u = 1; v[3].v = 0.5; //top, left, back
	v[4].x = pos_x + width; v[4].y = pos_y + depth; v[4].z = 0; v[4].u = 0.5; v[4].v = 0.5; //bottom, right, front
	v[5].x = pos_x + width; v[5].y = pos_y; v[5].z = height; v[5].u = 1; v[5].v = 0; //top, right, back
	v[6].x = pos_x; v[6].y = pos_y + depth; v[6].z = height; v[6].u = 1; v[6].v = 0; //top, left, front
	v[7].x = pos_x + width; v[7].y = pos_y + depth; v[7].z = height; v[7].u = 1; v[7].v = 0.5; //top, right, front

	v[8].x = pos_x; v[8].y = pos_y; v[8].z = 0; v[8].u = 1; v[8].v = 0.5; //2 bottom, left, back
	v[9].x = pos_x + width; v[9].y = pos_y; v[9].z = 0; v[9].u = 1; v[9].v = 1; //2 bottom, right, back
	v[10].x = pos_x; v[10].y = pos_y + depth; v[10].z = 0; v[10].u = 0.5; v[10].v = 0.5; //2 bottom, left, front
	v[11].x = pos_x + width; v[11].y = pos_y + depth; v[11].z = 0; v[11].u = 0.5; v[11].v = 1; //2 bottom, right, front



	//printf("%d %d %d %d %d", pos_x, pos_y, width, depth, height);
	//printf("%d %d %d %d %d %d %d", v[0].y, v[1].y, v[2].y, v[3].y, v[4].y, v[5].y, v[6].y);
	//first triangle, bottom lower
	vertList.push_back(v[8]);
	vertList.push_back(v[9]);
	vertList.push_back(v[10]);

	//second triangle, bottom upper
	vertList.push_back(v[9]);
	vertList.push_back(v[10]);
	vertList.push_back(v[11]);

	//third triangle left lower
	vertList.push_back(v[0]);
	vertList.push_back(v[2]);
	vertList.push_back(v[3]);


	//forth triangle left upper
	vertList.push_back(v[2]);
	vertList.push_back(v[3]);
	vertList.push_back(v[6]);


	//fifth triangle right lower
	vertList.push_back(v[1]);
	vertList.push_back(v[4]);
	vertList.push_back(v[5]);

	//sixt triagle right upper
	vertList.push_back(v[4]);
	vertList.push_back(v[5]);
	vertList.push_back(v[7]);

	//seventh triangle top upper
	vertList.push_back(v[3]);
	vertList.push_back(v[5]);
	vertList.push_back(v[6]);


	//eigth triangle top lower
	vertList.push_back(v[5]);
	vertList.push_back(v[6]);
	vertList.push_back(v[7]);

	//nineth tri back lower
	vertList.push_back(v[0]);
	vertList.push_back(v[1]);
	vertList.push_back(v[3]);

	//tenth tri back upper
	vertList.push_back(v[1]);
	vertList.push_back(v[3]);
	vertList.push_back(v[5]);

	//eleventh front upper
	vertList.push_back(v[4]);
	vertList.push_back(v[6]);
	vertList.push_back(v[7]);

	//twelfth front lower
	vertList.push_back(v[2]);
	vertList.push_back(v[4]);
	vertList.push_back(v[6]);


	//printf("Vertex data: %d %d %d", v[0].x, v[0].y, v[0].z);
	//printf("This mesh consists of %d vertices\n", vertList.size());

}

Mesh::Mesh(double pos_x, double pos_y)
{
	//printf("Building at: %f %f\n", pos_x, pos_y);
	//creates a unity cube at the specified xy-coordinates and at height zero
	Vertex v[12];
	v[0].x = pos_x; v[0].y = pos_y; v[0].z = 0; v[0].u = 0; v[0].v = 0.5; //bottom, left, back
	v[1].x = pos_x+1; v[1].y = pos_y; v[1].z = 0; v[1].u = 0; v[1].v = 0; //bottom, right, back
	v[2].x = pos_x; v[2].y = pos_y+1; v[2].z = 0; v[2].u = 0; v[2].v = 0; //bottom, left, front
	v[3].x = pos_x; v[3].y = pos_y; v[3].z = 3; v[3].u = 0.5; v[3].v = 0.5; //top, left, back
	v[4].x = pos_x+1; v[4].y = pos_y+1; v[4].z = 0; v[4].u = 0; v[4].v =0.5; //bottom, right, front
	v[5].x = pos_x+1; v[5].y = pos_y; v[5].z = 3; v[5].u = 0.5; v[5].v = 0; //top, right, back
	v[6].x = pos_x; v[6].y = pos_y+1; v[6].z = 3; v[6].u = 0.5; v[6].v = 0; //top, left, front
	v[7].x = pos_x+1; v[7].y = pos_y+1; v[7].z = 3; v[7].u = 0.5; v[7].v =0.5; //top, right, front

	v[8].x = pos_x; v[8].y = pos_y; v[8].z = 3; v[8].u = 0.5; v[8].v = 0.5; //v[8].nx = 0; v[8].ny = 0; v[8].nz = 1; //2 top, left, back	
	v[9].x = pos_x+1; v[9].y = pos_y; v[9].z = 3; v[9].u = 0.5; v[9].v = 1; //v[9].nx = 0; v[9].ny = 0; v[9].nz = 1; //2 top, right, back
	v[10].x = pos_x; v[10].y = pos_y + 1; v[10].z = 3; v[10].u = 0; v[10].v = 0.5; //v[10].nx = 0; v[10].ny = 0; v[10].nz = 1;//2 top, left, front
	v[11].x = pos_x + 1; v[11].y = pos_y + 1; v[11].z = 3; v[11].u = 0; v[11].v = 1; //v[11].nx = 0; v[11].ny = 0; v[11].nz = 1;//2 top, right, front

	//first triangle, bottom lower
	vertList.push_back(v[0]);
	vertList.push_back(v[1]);
	vertList.push_back(v[2]);

	//second triangle, bottom upper
	vertList.push_back(v[1]);
	vertList.push_back(v[2]);
	vertList.push_back(v[4]);

	//third triangle left lower
	vertList.push_back(v[0]);
	vertList.push_back(v[2]);
	vertList.push_back(v[3]);


	//forth triangle left upper
	vertList.push_back(v[2]);
	vertList.push_back(v[3]);
	vertList.push_back(v[6]);


	//fifth triangle right lower
	vertList.push_back(v[1]);
	vertList.push_back(v[4]);
	vertList.push_back(v[5]);

	//sixt triagle right upper
	vertList.push_back(v[4]);
	vertList.push_back(v[5]);
	vertList.push_back(v[7]);

	//seventh triangle top upper
	vertList.push_back(v[8]);
	vertList.push_back(v[9]);
	vertList.push_back(v[10]);


	//eigth triangle top lower
	vertList.push_back(v[9]);
	vertList.push_back(v[10]);
	vertList.push_back(v[11]);

	//nineth tri back lower
	vertList.push_back(v[0]);
	vertList.push_back(v[1]);
	vertList.push_back(v[3]);

	//tenth tri back upper
	vertList.push_back(v[1]);
	vertList.push_back(v[3]);
	vertList.push_back(v[5]);

	//eleventh front upper
	vertList.push_back(v[4]);
	vertList.push_back(v[6]);
	vertList.push_back(v[7]);

	//twelfth front lower
	vertList.push_back(v[2]);
	vertList.push_back(v[4]);
	vertList.push_back(v[6]);


	//printf("first vertex: %f %f %f", vertList.at(0).x, vertList.at(0).y, vertList.at(0).z);


}

Mesh::Mesh(double pos_x, double pos_y, bool isSphere)
{
	const float r = 0;
	const float g = 132;
	const float b = 255;

	Vertex v[19];
	v[0].x = 0.000000, v[0].y = 0.707107, v[0].z = -0.707107, v[0].r = r, v[0].g = g, v[0].b = b;

}

void Mesh::beepMe(int pos_x, int pos_y)
{
	//printf("reached me: %d %d", pos_x, pos_y);
}

Mesh::~Mesh()
{
}

VertexList::VertexList(int formatDescriptor)
{
	VertexList(formatDescriptor, 8);
}

VertexList::VertexList(int formatDescriptor, int numberOfEntries)
{
	//printf("My format: %d \n", formatDescriptor);
	vertexData = nullptr;
	currEntries = 0;
	indizes = nullptr;
	currEdges = 0;
	this->containsCoordinates = (formatDescriptor & (vertexCoordinates)) != 0;
	/*if (this->containsCoordinates);
	{
		puts("Contains Coords");
	}*/
	this->containsVertexColor = (formatDescriptor & (vertexColor)) != 0;
	//if (this->containsVertexColor) { puts("containes VColor"); }
	this->containsUVCoordinates = (formatDescriptor & (UVCoordinates)) != 0;
	//if (this->containsUVCoordinates) { puts("contains UVs"); }
	this->containsNormals = (formatDescriptor & (normals)) != 0;
	maxEntries = numberOfEntries;
	currEntries = 0;
	currEdges = 0;
	maxEdges = maxEntries * 1.5;
	indizes = new unsigned int[maxEdges];
	stride = 0;
	if (this->containsCoordinates == true)
	{
		stride += 3;
	}
	if (this->containsVertexColor == true)
	{
		stride += 3;
	}

	if (this->containsUVCoordinates == true)
	{
		stride += 2;
	}
	if (this->containsNormals == true)
	{
		stride += 3;
	}
	//printf("striding: %d", stride);
	//!!Change Back!!
	//stride = 3;
	//printf("allocate: %d\n", maxEntries*stride);
	vertexData = new float[maxEntries*stride];
}

VertexList::VertexList(float * vData,int vCount, int * iData,int dCount, int formatDescriptor)
{
	//todo: adapt stride
	//this->stride = 11; // because thats what the loader puts out
	//this->maxEdges=this->currEdges = dCount;
	//this->maxEntries=this->currEntries = vCount;
	//this->vertexData = new float[vCount*stride];
	//this->indizes = new unsigned int[dCount];
	//this->containsCoordinates = true;
	//this->containsNormals = true;
	//this->containsUVCoordinates = true;
	//this->containsVertexColor = true;

	//for (int i = 0; i < vCount*stride; i++)
	//{
	//	vertexData[i] = vData[i];
	//}
	//for (int i = 0; i < dCount;i++) {
	//	indizes[i] = iData[i];
	//}
}

VertexList::~VertexList()
{
	//TODO: fix crash on cleanUp!!!
	//if(indizes!=nullptr){ delete []indizes; }


	//
	if (vertexData != nullptr) {
		/*delete  []vertexData;*/
	}
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

bool VertexList::addVertex(float x, float y, float z, float r, float g, float b, float nx, float ny, float nz)
{
	if (!containsCoordinates || !containsVertexColor || containsUVCoordinates || !containsNormals)
	{
		//puts("\n##refusing");
		return false;
	}
	if (currEntries + 3 > maxEntries)
	{
		//puts("extend vertex");
		extendVertexData();
	}
	vertexData[currEntries*stride] = x;
	vertexData[currEntries*stride + 1] = y;
	vertexData[currEntries*stride + 2] = z;
	vertexData[currEntries*stride + 3] = r;
	vertexData[currEntries*stride + 4] = g;
	vertexData[currEntries*stride + 5] = b;
	vertexData[currEntries*stride + 6] = nx;
	vertexData[currEntries*stride + 7] = ny;
	vertexData[currEntries*stride + 8] = nz;
	//printf("indizes pre entry: %d\n", currEntries);
	++currEntries;


	return true;
}

bool VertexList::addVertex(float x, float y, float z, float r, float g, float b, float u, float v, float nx, float ny, float nz)
{
	if (!containsCoordinates || !containsVertexColor || !containsUVCoordinates || !containsNormals)
	{
		return false;
	}
	if (currEntries >= maxEntries)
	{
		extendVertexData();
	}
	vertexData[currEntries*stride] = x;
	vertexData[currEntries*stride + 1] = y;
	vertexData[currEntries*stride + 2] = z;
	vertexData[currEntries*stride + 3] = r;
	vertexData[currEntries*stride + 4] = g;
	vertexData[currEntries*stride + 5] = b;
	vertexData[currEntries*stride + 6] = u;
	vertexData[currEntries*stride + 7] = v;
	vertexData[currEntries*stride + 8] = nx;
	vertexData[currEntries*stride + 9] = ny;
	vertexData[currEntries*stride + 10] = nz;
	++currEntries;
	return true;
}

//bool VertexList::addVertex(float x, float y, float z, float u, float v)
//{
//	if (!containsCoordinates || containsVertexColor || !containsUVCoordinates || containsNormals)
//	{
//		return false;
//	}
//	if (currEntries >= maxEntries)
//	{
//		extendVertexData();
//	}
//	vertexData[currEntries*stride] = x;
//	vertexData[currEntries*stride + 1] = y;
//	vertexData[currEntries*stride + 2] = z;
//	vertexData[currEntries*stride + 3] = u;
//	vertexData[currEntries*stride + 4] = v;
//	++currEntries;
//	return true;
//}

bool VertexList::addVertex(Vertex v)
{
	/*if (containsCoordinates && !containsVertexColor && !containsUVCoordinates && !containsNormals)
	{
		return addVertex(v.x, v.y, v.z);
	}*/
	if (containsCoordinates && containsVertexColor && !containsUVCoordinates && containsNormals)
	{
		//printf("inserting: %f, %f, %f\n", v.x,v.y,v.z);
		bool t = addVertex(v.x, v.y, v.z, v.r, v.g, v.b, v.nx, v.ny, v.nz);
		if (!t)
		{
			puts("insert failed");
		}
		return t;
	}
	/*if (containsCoordinates && !containsVertexColor && containsUVCoordinates && !containsNormals)
	{
		return addVertex(v.x, v.y, v.z, v.u, v.v);
	}*/
	if (containsCoordinates && containsVertexColor && containsUVCoordinates && containsNormals)
	{
		return addVertex(v.x, v.y, v.z, v.r, v.g, v.b, v.u, v.v, v.nx, v.ny, v.nz);
	}
	return false;
}
bool VertexList::addIndex(int vertex1, int vertex2, int vertex3)
{
	if (currEdges + 3 > maxEdges)
	{
		if (!extendIndizes())
		{
			puts("Extending indizes failed");
			return false;
		}
	}
	//printf("triangle: %d, %d, %d", vertex1, vertex2, vertex3);
	indizes[currEdges] = vertex1;
	indizes[currEdges + 1] = vertex2;
	indizes[currEdges + 2] = vertex3;
	currEdges += 3;
	return true;
}

bool VertexList::extendVertexData()
{

	//puts("####Extending VBuffer");
	//printf("the vertex pointer before: %p\n", vertexData);
	float* newArray;
	//printf("Vertbuffer data: %d %d\n", maxEntries, stride);
	newArray = new float[maxEntries * stride + 3 * stride];

	if (newArray == nullptr)
	{
		printf("Error: allocation failed.");
		return false;
	}
	//printf("the actual pointer: %p\n", vertexData);
	//puts("copy now");
	//printf("processing %d * %d entries\n", currEntries, stride);
	for (int i = 0; currEntries*stride > i; ++i)
	{
		//printf("copying index %d \n", i);
		//printf("the value to copy: %f", vertexData[i]);
		newArray[i] = vertexData[i];
	}

	delete[] vertexData;
	vertexData = nullptr;
	vertexData = newArray;
	//printf("the vertexpointer after: %p\n", vertexData);
	maxEntries += 3;
	extendIndizes();
	return true;
}

bool VertexList::extendIndizes()
{
	//puts("extend index list");
	//printf("index pointer: %p\n", indizes);
	if (currEdges + 10 < maxEdges)
	{
		//printf("%d values are sufficient for %d entries\n", maxEdges, currEdges);
		return true;
	}

	unsigned int* newArray;
	newArray = new unsigned int[maxEdges + 10];
	if (newArray == nullptr)
	{
		printf("Error: allocation failed.");
		return false;
	}
	for (int i = 0; currEdges > i; ++i)
	{
		newArray[i] = indizes[i];
	}
	//puts("copy successful");
	delete[]indizes;
	//printf("new array for index pointer: %p\n", newArray);
	indizes = nullptr;
	indizes = newArray;
	//printf("new index pointer: %p\n", indizes);
	maxEdges += 10;
	return true;
}

bool VertexList::addTriangle(Vertex v1, Vertex v2, Vertex v3)
{
	int vi1 = findVertex(v1);
	int vi2 = findVertex(v2);
	int vi3 = findVertex(v3);

	//printf("in Verts at hand: %d, %d, %d", vi1, vi2,vi3);

	if (vi1 == -1)
	{
		addVertex(v1);
		vi1 = findVertex(v1);
	}
	if (vi2 == -1)
	{
		addVertex(v2);
		vi2 = findVertex(v2);
	}
	if (vi3 == -1)
	{
		addVertex(v3);
		vi3 = findVertex(v3);
	}
	//printf("out Verts at hand: %d, %d, %d\n", vi1, vi2, vi3);
	if (vi1 == -1 || vi2 == -1 || vi3 == -1)
	{
		//printf("vertex %d, %d, %d not inserted\n", vi1, vi2, vi3);
		return false;
	}

	return addIndex(vi1, vi2, vi3);

}

int VertexList::findVertex(Vertex v)
{
	for (int i = 0; i < maxEntries*stride; i += stride)
	{
		if (abs(vertexData[i] - v.x) < epsilon_F && abs(vertexData[i + 1] - v.y) < epsilon_F && abs(vertexData[i + 2] - v.z) < epsilon_F)
		{
			if (containsUVCoordinates && containsVertexColor)
			{
				if (abs(vertexData[i + 3] - v.r) < epsilon_F && abs(vertexData[i + 4] - v.g) < epsilon_F && abs(vertexData[i + 5] - v.b) < epsilon_F && abs(vertexData[i + 6] - v.u) < epsilon_F && abs(vertexData[i + 7] - v.v) < epsilon_F)
				{
					return i / stride;
				}
				else
				{
					continue;
				}
			}
			else if (containsUVCoordinates && !containsVertexColor)
			{
				if (abs(vertexData[i + 3] - v.u) < epsilon_F && abs(vertexData[i + 4] - v.v) < epsilon_F)
				{
					return i / stride;
				}
				else
				{
					continue;
				}
			}
			else if (!containsUVCoordinates && containsVertexColor)
			{
				if (abs(vertexData[i + 3] - v.r) < epsilon_F && abs(vertexData[i + 4] - v.g) < epsilon_F && abs(vertexData[i + 5] - v.b) < epsilon_F)
				{
					return i / stride;
				}
				else
				{
					continue;
				}
			}
			else
			{
				return i / stride;
			}
		}
	}
	return -1;
}

unsigned int * VertexList::getIndizes()
{

	/*puts("Im returning the following indizes: \n");
	for (int i = 0; i < currEdges; ++i)
	{
		printf("%d ", this->indizes[i]);
	}
	puts("\n\n");*/
	return indizes;
}

int VertexList::getIndexCount()
{
	return currEdges;
}

int VertexList::getStride()
{
	return stride;
}

float * VertexList::getVertexData()
{
	return vertexData;
}

int VertexList::getVertexCount()
{
	return currEntries;
}

bool VertexList::getContainsCoordinates()
{
	return containsCoordinates;
}

bool VertexList::getContainsVertexColor()
{
	return containsVertexColor;
}

bool VertexList::getContainsUVCoordinates()
{
	return containsUVCoordinates;
}
bool VertexList::getContainsNormals()
{
	return containsNormals;
}
Vertex::Vertex(float ix, float iy, float iz)
{
	x = ix;
	y = iy;
	z = iz;
	u = 0;
	v = 0;
	r = g = b = 0;
	nx = ny = nz = 0;
}

//Vertex::Vertex()
//{
//	x = y = z = u = v = r = g = b = nx=ny=nz= 0;
//}


OBJLoad::OBJLoad()
{
}

OBJLoad::OBJLoad(string filename)
{

	this->indexList = nullptr;
	this->laced = nullptr;

	FILE * input=nullptr;
	char rBuffer[rdBuffer];
	input = fopen(filename.c_str(), "r");
	if (input == nullptr)
	{
		printf("###Error loading : %s\n", filename.c_str());
	}


	while (fgets(rBuffer, rdBuffer, input) != nullptr)
	{
		char id[rdBuffer];
		sscanf(rBuffer, "%s", id);

		printf("ID string: %s", id);

		if (id[0] == '#')
		{
			continue;
		}
		if (strncmp(id, "mtllib", 6) == 0)
		{
			continue;
		}

		if (strncmp(id, "o", 1) == 0)
		{
			continue;
		}

		if (strncmp(id, "v", 1) == 0 && strlen(id) == 1)
		{
			char in;
			double x, y, z;
			sscanf(rBuffer, "%c %lf %lf %lf", &in, &x, &y, &z);
			//printf("found VertexCoords: %f %f %f\n", x, y, z);
			vspace v;
			v.x = x;
			v.y = y;
			v.z = z;
			this->vertics.push_back(v);
			continue;
		}
		
		if (strncmp(id, "vt", 2) == 0)
		{
			char in[rdBuffer];
			double u, v;
			sscanf(rBuffer, "%*s %lf %lf", &u, &v);
			//printf("got UVs %lf %lf\n", u, v);
			UV uv;
			uv.u = u;
			uv.v = v;
			uvCoords.push_back(uv);
			continue;
		}
		if (strncmp(id, "vn", 2) == 0)
		{
			char in[rdBuffer];
			double x, y, z;
			sscanf(rBuffer, "%*s %lf %lf %lf", &x, &y, &z);
			//printf("Found normies: %lf %lf %lf\n", x, y, z);
			normal n;
			n.x = x;
			n.y = y;
			n.z = z;
			Normal.push_back(n);
			continue;
		}
		if (strncmp(id, "usemtl", 6) == 0) {
			continue;
		}
		if (strncmp(id, "s", 1) == 0) { continue; }
	
		if (strncmp(id, "f", 1 && true) == 0) {
			
			int vindex, uvindex, normindex;
			int counter = 0;
			fullVert *vcol[4];

			char *head, *tail;
			head = tail = rBuffer + 1;
			for (int i = 0; i < 4; i++)
			{
				vindex = strtol(head, &tail, 10);
				if (head == tail) { break; }
				head = tail + 1;
				uvindex = strtol(head, &tail, 10);
				head = tail + 1;
				normindex = strtol(head, &tail, 10);
				head = tail + 1;

				printf("Got indizes: %d %d %d\n", vindex, uvindex, normindex);
				vcol[i] = new fullVert(this, vindex, uvindex, normindex);
				vcol[i]->r = 0;
				vcol[i]->g = 1;
				vcol[i]->b = 0.7;
				counter++;
			}
			
			printf("Got %d verts for face\n", counter);

			if (counter == 3)
			{

				triangle t(*(vcol[0]), *(vcol[1]), *(vcol[2]));

				fullVertices.push_back(*(vcol[0]));
				indeX.push_back(fullVertices.size());
				fullVertices.push_back(*(vcol[1]));
				indeX.push_back(fullVertices.size());
				indeX.push_back(fullVertices.size());
				fullVertices.push_back(*(vcol[2]));
				

				/*tris.push_back(t);
				puts("Pushed a single tri");*/
				for (int i = 0; i < 3; ++i)
				{
					delete vcol[i];

				}
			}
			if (counter == 4)
			{
				triangle t(*(vcol[0]), *(vcol[1]), *(vcol[2]));
				//tris.push_back(t);
				fullVertices.push_back(*(vcol[0]));
				indeX.push_back(fullVertices.size());
				fullVertices.push_back(*(vcol[1]));
				indeX.push_back(fullVertices.size());
				indeX.push_back(fullVertices.size());
				fullVertices.push_back(*(vcol[2]));
				triangle t1(*(vcol[0]), *(vcol[1]), *(vcol[2]));
				//tris.push_back(t1);
				fullVertices.push_back(*(vcol[1]));
				indeX.push_back(fullVertices.size());
				fullVertices.push_back(*(vcol[2]));
				indeX.push_back(fullVertices.size());
				indeX.push_back(fullVertices.size());
				fullVertices.push_back(*(vcol[3]));

				for (int i = 0; i < 4; ++i)
				{
					delete vcol[i];

				}
			}

		}
	}

	fclose(input);


}


OBJLoad::~OBJLoad()
{
}

float * OBJLoad::getVSet()
{

	float * buffer = new float[3*sizeof(float)*this->getVSetSize()];
	for (int i = 0; i < getVSetSize(); i++)
	{
		buffer[i*3] = this->vertics.at(i).x;
		buffer[i * 3+1] = this->vertics.at(i).y;
		buffer[i * 3+2] = this->vertics.at(i).z;
	}
	return buffer;
}

float * OBJLoad::getUVSet()
{

	float *uv = new float[2 * sizeof(float)*this->getUVSetSize()];

	for (int i = 0; i < getUVSetSize(); i++)
	{
		uv[2 * i] = this->uvCoords.at(i).u;
		uv[2 * i+1] = this->uvCoords.at(i).v;
	}
	return uv;
}

float * OBJLoad::getNormalSet()
{

	float * norms = new float[3* sizeof(float)*getNormalSetSize()];
	for (int i =0; i<getNormalSetSize(); i++)
	{
		norms[3 * i] = this->Normal.at(i).x;
		norms[3 * i+1] = this->Normal.at(i).y;
		norms[3 * i+2] = this->Normal.at(i).z;
	}
	return norms;
}

float * OBJLoad::getInterlacedData()
{
	if (laced != nullptr)
	{
		return laced;
	}
	laced = new float(tris.size()*sizeof(float)*11*3);
	memset(laced, 0, tris.size() * sizeof(float) * 11 * 3);

	for (int i = 0; i < tris.size(); ++i)
	{
		triangle t = tris.at(i);
		fullVert t1=t.a, t2=t.b, t3=t.c;
		puts("Processing this:");
		t1.printMe(); t2.printMe();t3.printMe();
		puts("");
		laced[i * 33 + 0] = t1.vx;
		laced[i * 33 + 1] = t1.vy;
		laced[i * 33 + 2] = t1.vz;

		laced[i * 33 + 3] = t1.r;
		laced[i * 33 + 4] = t1.g;
		laced[i * 33 + 5] = t1.b;

		laced[i * 33 + 6] = t1.U;
		laced[i * 33 + 7] = t1.V;

		laced[i * 33 + 8] = t1.nx;
		laced[i * 33 + 9] = t1.ny;
		laced[i * 33 + 10] = t1.nz;


		laced[i * 33 +11] = t2.vx;
		laced[i * 33 + 12] = t2.vy;
		laced[i * 33 + 13] = t2.vz;

		laced[i * 33 + 14] = t2.r;
		laced[i * 33 +15] = t2.g;
		laced[i * 33 +16] = t2.b;

		laced[i * 33 + 17] = t2.U;
		laced[i * 33 + 18] = t2.V;

		laced[i * 33 + 19] = t2.nx;
		laced[i * 33 + 20] = t2.ny;
		laced[i * 33 + 21] = t2.nz;


		laced[i * 33 + 22] = t3.vx;
		laced[i * 33 + 23] = t3.vy;
		laced[i * 33 + 24] = t3.vz;

		laced[i * 33 + 25] = t3.r;
		laced[i * 33 + 26] = t3.g;
		laced[i * 33 + 27] = t3.b;

		laced[i * 33 + 28] = t3.U;
		laced[i * 33 + 29] = t3.V;

		laced[i * 33 + 30] = t3.nx;
		laced[i * 33 + 31] = t3.ny;
		laced[i * 33 + 32] = t3.nz;


	
	}
	return laced;
}

int * OBJLoad::getIndexList()
{
	if (indexList != nullptr) { return indexList; }
	 indexList= new int[tris.size()*3];
	memset(indexList, 0, tris.size()*3*sizeof(int));
	//gets more complex if a more space efficient representation of the loaded model is used
	for (int i = 0; i < tris.size()*3; ++i) {
		indexList[i] = i;
		printf("Added index %d ", i );
	}
	return indexList;
}

vspace OBJLoad::getVertex(int index)
{
	if (index - 1 < vertics.size())
		return this->vertics.at(index - 1);

	printf("### index %d over cap %d\d", index, vertics.size());
}

UV OBJLoad::getUv(int index)
{
	if (index - 1 < uvCoords.size())
		return this->uvCoords.at(index - 1);
	printf("### index %d over cap %d\d", index, uvCoords.size());

}

normal OBJLoad::getNormal(int index)
{
	if (index - 1 < Normal.size())
		return this->Normal.at(index - 1);
	printf("### index %d over cap %d\d", index, Normal.size());
}

VertexList OBJLoad::getVertexList( float x, float y, float z)
{
	printf("+++Object contains %d tris\n", tris.size());
	VertexList v(vertexCoordinates | vertexColor | UVCoordinates | normals,8);

	for (triangle t : tris)
	{
		Vertex a;// = t.a.get();
		Vertex b;//; = t.b.get();
		Vertex c;// = t.c.get();
		

		//v.addTriangle(a,b,c);
	}
	//Vertex a, b, c;
	//a.x=b.y=c.z = 1;
	//a.nx = b.ny = c.nz = 1;
	//v.addTriangle(a,b,c);
	printf("######### Vertexlist has %d indizes %d vertices\n", v.getIndexCount(),  v.getVertexCount());
	return v;
}

fullVert::fullVert(OBJLoad *res, int vspaceIndex, int uvIndex, int normalIndex)
{
	this->vx = res->getVertex(vspaceIndex).x;
	this->vy = res->getVertex(vspaceIndex).y;
	this->vz = res->getVertex(vspaceIndex).z;

	this->U = res->getUv(uvIndex).u;
	this->V = res->getUv(uvIndex).v;

	this->nx = res->getNormal(normalIndex).x;
	this->ny = res->getNormal(normalIndex).y;
	this->nz = res->getNormal(normalIndex).z;

	printf("Arise: %f %f %f \t %f %f \t %f %f %f\n", vx,vy,vz, U,V, nx,ny,nz);


}

fullVert::fullVert(const fullVert & in)
{

	this->b = in.b;
	this->g = in.g;
	this->nx = in.nx;
	this->ny = in.ny;
	this->nz = in.nz;
	this->r = in.r;
	this->U = in.U;
	this->V = in.V;
	this->vx = in.vx;
	this->vy = in.vy;
	this->vz = in.vz;
}

Vertex fullVert::get()
{
	Vertex t;
	t.b = 1;
	t.g = 1;
	t.r = 0;

	t.nx = this->nx;
	t.ny = this->ny;
	t.nz = this->nz;

	t.u = this->U;
	t.v = this->V;

	t.x = this->vx;
	t.y = this->vy;
	t.z = this->vz;
	return t;
}

triangle::triangle(fullVert a, fullVert b, fullVert c)
{
	this->a = a;
	this->b = b;
	this->c = c;
}
