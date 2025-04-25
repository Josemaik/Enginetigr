#pragma once

class Engine;
class GameData;

class CollisionSystem
{
public:
	void update(Engine& engine, float& delta,GameData& gd);
};

