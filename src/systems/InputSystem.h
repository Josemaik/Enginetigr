#pragma once

//CollisionSystem - processes all entities with input component

class Engine;

class InputSystem
{
public:
	void update(Engine& engine, float& delta);
};

