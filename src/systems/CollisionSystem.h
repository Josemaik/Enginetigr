#pragma once

//CollisionSystem - processes all entities with collision component

class Engine;
class GameData;

class CollisionSystem
{
public:
	void update(Engine& engine, float& delta,GameData& gd);
};

