#pragma once

class Engine;
struct GameData;

class CollisionSystem
{
public:
	void update(Engine& engine, float& delta,GameData& gd);
};

