#pragma once

class Engine;

enum States{Menu, Gameplay};

class Game
{
public:
	Game(Engine& engine_) : 
		engine(engine_)
	{}

	void Run();
private:
	Engine& engine;
	float GlobalTimer = 0.f;
	States CurrentState = Menu;
};

