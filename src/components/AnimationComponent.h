#pragma once

#include <vector>
#include "../managers/Sprite.hpp"

struct AnimationComponent
{
	std::vector<int> frames; //frames vector
	float framerate; //cada x segundos cambia de frame
	float currentTime; // tiempo actual que ha pasado
	int currentframe; //index del frame a dibujar
	int framewidth = 0; //ancho del frame a dibujar
	int numframes = 0; //número de frames de la animación
};