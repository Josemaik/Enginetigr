#pragma once

//CollisionSystem - processes all entities with physics component

class Engine;

class PhysicsSystem
{
public:
	void update(Engine& engine, float& delta);
};

