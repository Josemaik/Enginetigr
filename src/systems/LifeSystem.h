#pragma once

//CollisionSystem - processes all entities with life component

class Engine;

class LifeSystem
{
public:
	void update(Engine& engine, float& delta);
};

