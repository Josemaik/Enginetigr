#pragma once

struct SpawnerComponent
{
	float interval;            // Tiempo entre spawns
	float timer = 0.0f;        // Tiempo actual acumulado
	int nextEntityID = 1; //enemy id
};