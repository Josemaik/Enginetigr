#pragma once

struct GameData
{
	//spawn point player
	vec2f spawnpoint{ 0.f,0.f };
	//current 
	float bestScore = 0.f;
	//timer
	float GlobalTimer = 0.f;
	//int CurrentLevel = 0;
	////time to change level
	//int LevelTime = 20.f;
	////global timer
	//int GlobalTime = 0.f;
	////time to spawn new enemie
	//float spawnenemieTime = 5.f;
};