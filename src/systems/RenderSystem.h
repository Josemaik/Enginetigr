#pragma once

//CollisionSystem - processes all entities with render component

class Engine;
class GameData;

class RenderSystem
{
public:
	void update(Engine& engine,GameData& gamedata,float fps);
};

