#pragma once
class inputState
{

	
public:
	double camera_x=0, camera_y=80, camera_zoom=0.3; //y-min: 35, y-max: 80, x-min: -45, x-max: 45, camera_x=5, camera_y=50, camera_zoom=0.2
	int update_roll=0, update_pitch = 0;
	bool quitGame = false;
	bool resetGame = true;
	bool lmbPressed = false;
	bool turbo = false;
	float brightness = 1.0f;

	inputState();
	~inputState();
};

