#pragma once

#include "../utils/Vector2.h"

struct PhysicsComponent
{
	vec2f position;
	vec2f lastposition;
	vec2f velocity;
	float gravity = 12.0;
	float radius;
	float bounciness; // 0.0 = sin rebote, 1.0 = rebote perfecto
};