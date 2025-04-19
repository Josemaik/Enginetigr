#pragma once

#include <vector>
#include "../managers/SpriteManager.hpp"

struct AnimationComponent
{
	std::vector<Sprite> spritesheet; //frames vector
	float framerate; //cada x segundos cambia de frame
	float currentTime; // tiempo actual que ha pasado
	int currentframe; //index del frame a dibujar
};