#pragma once

class Engine;
struct GameData;

class RenderSystem
{
public:
	void update(Engine& engine,GameData& gamedata,float fps);
};

