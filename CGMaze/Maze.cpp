#include "Maze.h"
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
	 delete mazeGrid;
}

 vector<Mesh> Maze::getMeshes()
 {
	 return meshes;
 }

 VertexList Maze::getVertexList()
 {
	 //puts("getting list");
	 VertexList V(vertexCoordinates | UVCoordinates,3);

	 int  meshcount = 0;
	 vector<Mesh> Meshes = getMeshes();
	 for each(Mesh t in Meshes)
	 {
		 //printf("Mesh no: %d with %d\n", ++meshcount, t.getVertices().size());
		 vector<Vertex> mverts = t.getVertices();
		 for (int i = 0; i < mverts.size(); i += 3)
		 {
			 printf("Adding triangle no %d:  %f %f %f\n",i, mverts.at(i).x, mverts.at(i + 1).x, mverts.at(i + 2).x);
			 V.addTriangle(mverts.at(i), mverts.at(i + 1), mverts.at(i + 2));
		 }
		 

	 }
	 return V;
 }

 VertexList Maze::getBallVertices()
 {
	 VertexList V(vertexCoordinates|UVCoordinates,8);

	 Mesh BallMesh = getBall();
	 vector<Vertex> bverts = BallMesh.getVertices();
	 for (int i = 0; i < bverts.size(); i+=3)
	 {
		 V.addTriangle(bverts.at(i), bverts.at(i + 1), bverts.at(i + 2));
	 }

	 return V;
 }

 Mesh Maze::getBall()
 {
	 return Mesh();
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

void Maze::advance(int deltaTime)
{
	rotateField(deltaTime);
	moveBall(deltaTime);
	if (checkCollision())
	{
		handleCollision();
	}
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
			status = status | (1 << area[h][w]); //every bit represents a bool of a floor type
		}

	}



	return false;
}

void Maze::handleCollision()
{
	int **candidates=getTouchedGround();
	
	
	for (int h = ball_y-2; h < ball_y+2; ++h)
	{
		for (int w = ball_x-2; w < ball_x+2; ++w)
		{
			switch (mazeGrid[h*width+w])
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
				if (ball_y < h && (ball_y + ballRadius) > h)
				{
					//below and colliding
					ballVelocity_y *= -1;
				}
				if (ball_x <w && ball_x + ballRadius>w)
				{
					//left and coliding
					ballVelocity_x *= -1;
				}
				if (ball_y > (h + 1) && ball_y < (-ballRadius) < (h + 1))
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

void Maze::rotateField(int deltaTime)
{
	prevPitch = pitch;
	prevRoll = roll;
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


void Maze::moveBall(int deltaTime)
{
	ball_x += ballVelocity_x * (deltaTime/(float)1000);
	ball_y += ballVelocity_y * (deltaTime / (float)1000);
}

void Maze::getRotations()
{

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
	return mazeGrid[(int)h*width + width];

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
	Vertex v[8];
	v[0].x = pos_x; v[0].y = pos_y; v[0].z = 0;
	v[1].x = pos_x + width; v[1].y = pos_y; v[1].z = 0;
	v[2].x = pos_x; v[2].y = pos_y + depth; v[2].z = 0;
	v[3].x = pos_x; v[3].y = pos_y; v[3].z = height;
	v[4].x = pos_x + width; v[4].y = pos_y + depth; v[4].z = 0;
	v[5].x = pos_x + width; v[5].y = pos_y; v[5].z = height;
	v[6].x = pos_x; v[6].y = pos_y + depth; v[6].z = height;
	v[7].x = pos_x + width; v[7].y = pos_y + depth; v[7].z = height;
	printf("%d %d %d %d %d", pos_x, pos_y, width, depth, height);
	printf("%d %d %d %d %d %d %d", v[0].y, v[1].y, v[2].y, v[3].y, v[4].y, v[5].y, v[6].y);
	//first triangle, front lower
	vertList.push_back(v[0]);
	vertList.push_back(v[1]);
	vertList.push_back(v[2]);

	//second triangle, front upper
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
	vertList.push_back(v[0]);


	//fifth triangle right lower
	vertList.push_back(v[1]);
	vertList.push_back(v[4]);
	vertList.push_back(v[5]);

	//sixt triagle right upper
	vertList.push_back(v[4]);
	vertList.push_back(v[5]);
	vertList.push_back(v[7]);

	//seventh triangle top uppper
	vertList.push_back(v[3]);
	vertList.push_back(v[5]);
	vertList.push_back(v[6]);


	//eigth triangle top lower
	vertList.push_back(v[5]);
	vertList.push_back(v[6]);
	vertList.push_back(v[7]);

	//nineth tri front lower
	vertList.push_back(v[0]);
	vertList.push_back(v[1]);
	vertList.push_back(v[3]);

	//tenth tri front upper
	vertList.push_back(v[1]);
	vertList.push_back(v[3]);
	vertList.push_back(v[6]);

	//eleventh back upper
	vertList.push_back(v[4]);
	vertList.push_back(v[6]);
	vertList.push_back(v[7]);

	//twelfth back lower
	vertList.push_back(v[2]);
	vertList.push_back(v[4]);
	vertList.push_back(v[6]);

	//vertList.push_back(v[4]);
	//vertList.push_back(v[5]);
	//vertList.push_back(v[6]);
	//vertList.push_back(v[7]);


	printf("Vertex data: %d %d %d", v[0].x, v[0].y, v[0].z);
	printf("This mesh consists of %d vertices\n", vertList.size());

}

Mesh::Mesh(double pos_x, double pos_y)
{
	printf("Building at: %f %f\n", pos_x, pos_y);
	//creates a unity cube at the specified xy-coordinates and at height zero
	Vertex v[8];
	v[0].x = pos_x; v[0].y = pos_y; v[0].z = 0;
	v[1].x = pos_x+1; v[1].y = pos_y; v[1].z = 0;
	v[2].x = pos_x; v[2].y = pos_y+1; v[2].z = 0;
	v[3].x = pos_x; v[3].y = pos_y; v[3].z = 3;
	v[4].x = pos_x+1; v[4].y = pos_y+1; v[4].z = 0;
	v[5].x = pos_x+1; v[5].y = pos_y; v[5].z = 3;
	v[6].x = pos_x; v[6].y = pos_y+1; v[6].z = 3;
	v[7].x = pos_x+1; v[7].y = pos_y+1; v[7].z = 3;

	//first triangle, front lower
	vertList.push_back(v[0]);
	vertList.push_back(v[1]);
	vertList.push_back(v[2]);

	//second triangle, front upper
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
	vertList.push_back(v[0]);


	//fifth triangle right lower
	vertList.push_back(v[1]);
	vertList.push_back(v[4]);
	vertList.push_back(v[5]);

	//sixt triagle right upper
	vertList.push_back(v[4]);
	vertList.push_back(v[5]);
	vertList.push_back(v[7]);

	//seventh triangle top uppper
	vertList.push_back(v[3]);
	vertList.push_back(v[5]);
	vertList.push_back(v[6]);


	//eigth triangle top lower
	vertList.push_back(v[5]);
	vertList.push_back(v[6]);
	vertList.push_back(v[7]);

	//nineth tri front lower
	vertList.push_back(v[0]);
	vertList.push_back(v[1]);
	vertList.push_back(v[3]);

	//tenth tri front upper
	vertList.push_back(v[1]);
	vertList.push_back(v[3]);
	vertList.push_back(v[6]);

	//eleventh back upper
	vertList.push_back(v[4]);
	vertList.push_back(v[6]);
	vertList.push_back(v[7]);

	//twelfth back lower
	vertList.push_back(v[2]);
	vertList.push_back(v[4]);
	vertList.push_back(v[6]);


	printf("first vertex: %f %f %f", vertList.at(0).x, vertList.at(0).y, vertList.at(0).z);
}

void Mesh::beepMe(int pos_x, int pos_y)
{
	printf("reached me: %d %d", pos_x, pos_y);
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
	this->containsCoordinates = (formatDescriptor & (vertexCoordinates))!=0;
	/*if (this->containsCoordinates);
	{
		puts("Contains Coords");
	}*/
	this->containsVertexColor = (formatDescriptor & (vertexColor)) != 0;
	//if (this->containsVertexColor) { puts("containes VColor"); }
	this->containsUVCoordinates = (formatDescriptor & (UVCoordinates)) != 0;
	//if (this->containsUVCoordinates) { puts("contains UVs"); }
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
	
	//printf("striding: %d", stride);
	//!!Change Back!!
	//stride = 3;
	//printf("allocate: %d\n", maxEntries*stride);
	vertexData = new float[maxEntries*stride];
}

VertexList::~VertexList()
{
	if(indizes!=nullptr){ delete indizes; }
	//
	if (vertexData != nullptr) {
		delete vertexData;
	}
}

bool VertexList::addVertex(float x, float y, float z)
{
	//printf("we add a vertex %f %f %f\n", x, y, z);
	if (!containsCoordinates || containsVertexColor || containsUVCoordinates)
	{
		puts("Wrong data format");
		return false;
	}
	if (currEntries >= maxEntries)
	{
		extendVertexData();
	}
	vertexData[currEntries*stride] = x;
	vertexData[currEntries*stride+1] = y;
	vertexData[currEntries*stride+2] = z;
	++currEntries;
	
	//printf("\n### new vcount: %d\n", numberOfVertices);
	

	return true;
}

bool VertexList::addVertex(float x, float y, float z, float r, float g, float b)
{
	if (!containsCoordinates || !containsVertexColor || containsUVCoordinates)
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
	//printf("indizes pre entry: %d\n", currEntries);
	++currEntries;


	return true;
}

bool VertexList::addVertex(float x, float y, float z, float r, float g, float b, float u, float v)
{
	if (!containsCoordinates || !containsVertexColor || !containsUVCoordinates)
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
	++currEntries;
	return true;
}

bool VertexList::addVertex(float x, float y, float z, float u, float v)
{
	if (!containsCoordinates || containsVertexColor || !containsUVCoordinates)
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
	vertexData[currEntries*stride + 3] = u;
	vertexData[currEntries*stride + 4] = v;
	++currEntries;
	return true;
}

bool VertexList::addVertex(Vertex v)
{
	if (containsCoordinates && !containsVertexColor && !containsUVCoordinates)
	{
		return addVertex(v.x, v.y, v.z);
	}
	if (containsCoordinates && containsVertexColor && !containsUVCoordinates)
	{
		return addVertex(v.x, v.y, v.z, v.r, v.g, v.b);
	}
	if (containsCoordinates && !containsVertexColor && containsUVCoordinates)
	{
		return addVertex(v.x, v.y, v.z, v.u, v.v);
	}
	if (containsCoordinates && containsVertexColor && containsUVCoordinates)
	{
		return addVertex(v.x, v.y, v.z, v.r, v.g, v.b, v.u, v.v);
	}
	return false;
}
bool VertexList::addIndex(int vertex1, int vertex2, int vertex3)
{
	if (currEdges+3>maxEdges)
	{
		if (!extendIndizes())
		{
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

	puts("####Extending VBuffer");
	printf("the vertex pointer before: %p\n", vertexData);
	float* newArray;
	printf("Vertbuffer data: %d %d\n", maxEntries, stride);
	newArray = new float[maxEntries * stride + 3 * stride];

	if (newArray == nullptr)
	{
		printf("Error: allocation failed.");
		return false;
	}
	//printf("the actual pointer: %p\n", vertexData);
	//puts("copy now");
	//printf("processing %d * %d entries\n", currEntries, stride);
	for (int i = 0; currEntries*stride>i; ++i)
	{
		//printf("copying index %d \n", i);
		//printf("the value to copy: %f", vertexData[i]);
		newArray[i] = vertexData[i];
	}

	delete vertexData;
	vertexData = nullptr;
	vertexData = newArray;
	printf("the vertexpointer after: %p\n", vertexData);
	maxEntries += 3;
	extendIndizes();
	return true;
}

bool VertexList::extendIndizes()
{
	puts("extend index list");
	printf("index pointer: %p\n", indizes);
	if (currEdges+10 < maxEdges)
	{
		printf("%d values are sufficient for %d entries\n", maxEdges, currEdges);
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
	puts("copy successful");
	delete indizes;
	printf("new array for index pointer: %p\n", newArray);
	indizes = nullptr;
	indizes = newArray;
	printf("new index pointer: %p\n", indizes);
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
	//printf("out Verts at hand: %d, %d, %d", vi1, vi2, vi3);
	if (vi1 == -1 || vi2 == -1 || vi3 == -1)
	{
		return false;
	}

	return addIndex(vi1, vi2, vi3);
	
}

int VertexList::findVertex(Vertex v)
{
	for (int i = 0; i < maxEntries*stride; i+=stride)
	{
		if (vertexData[i]==v.x && vertexData[i+1] == v.y && vertexData[i + 2] == v.z)
		{
			if (containsUVCoordinates && containsVertexColor)
			{
				if (vertexData[i+3] == v.r && vertexData[i + 4] == v.g && vertexData[i + 5] == v.b && vertexData[i + 6] == v.u && vertexData[i + 7] == v.v)
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
				if (vertexData[i + 3] == v.u && vertexData[i + 4] == v.v)
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
				if (vertexData[i + 3] == v.r && vertexData[i + 4] == v.g && vertexData[i + 5] == v.b)
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

Vertex::Vertex(float ix, float iy, float iz)
{
	x = ix;
	y = iy;
	z = iz;
	u = 0;v = 0;r = g = b = 0;
}

Vertex::Vertex()
{
	x = y = z = u = v = r = g = b = 0;
}
