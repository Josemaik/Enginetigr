#pragma once

struct GameData
{
	//current 
	int CurrentLevel = 0;
	//time to change level
	int LevelTime = 20.f;
	//global timer
	int GlobalTime = 0.f;
	//time to spawn new enemie
	float spawnenemieTime = 5.f;
};