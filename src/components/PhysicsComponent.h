#pragma once

#include "../utils/Vector2.h"

//PhysicsComponent - Define physics that one entity can posses

struct PhysicsComponent
{
	vec2f position;
	vec2f lastposition;
	vec2f velocity;
	float gravity = 12.0; //gravity force
	float bounciness; // 0.0 = min, 1.0 = max
};