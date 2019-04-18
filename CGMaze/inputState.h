#pragma once
class inputState
{

	
public:
	double camera_x=0, camera_y=0, camera_zoom=1; //TODO: insert fitting values later
	int update_roll=0, update_pitch = 0;
	bool quitGame = false;
	bool resetGame = true;

	inputState();
	~inputState();
};

