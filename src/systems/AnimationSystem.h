#pragma once

//AnimationSystem - processes all entities with animation component

class Engine;

class AnimationSystem
{
public:
	void update(Engine& engine, float& delta);
};

