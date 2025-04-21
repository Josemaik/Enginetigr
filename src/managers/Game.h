#pragma once

class Engine;

enum States{Menu, Gameplay, Dead};

class Game
{
public:
	Game(Engine& engine_) : 
		engine(engine_)
	{}

	void Run();
private:
	Engine& engine;
	States CurrentState = Menu;
};

