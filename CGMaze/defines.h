#pragma once

#define epsilon_F 0.001

#define pitchRate 2
#define rollRate 2
#define gravity 9.81
#define ballRadius 0.9
#define ballMass 1
#define maxPitch 30
#define maxRoll 30
#define maxVelocity 5

#define way 0
#define ballstart 1
#define wall 2
#define hole 3
#define finish 4

#define untouching -1

#define gameFinished 1
#define gameRunning 0
#define gameLost 2

#define vertexCoordinates (1<<0)
#define vertexColor (1<<1)
#define UVCoordinates (1<<2)
#define normals (1<<3)