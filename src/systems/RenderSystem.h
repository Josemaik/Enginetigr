#pragma once

class Engine;
class GameData;

class RenderSystem
{
public:
	void update(Engine& engine,GameData& gamedata,float fps);
};

