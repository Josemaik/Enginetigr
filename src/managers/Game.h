#pragma once

#include "../systems/InputSystem.h"
#include "../systems/PhysicsSystem.h"
#include "../systems/CollisionSystem.h"
#include "../systems/RenderSystem.h"
#include "../systems/LifeSystem.h"

class Engine;

class Game
{
public:
	Game(Engine& engine_) : 
		engine(engine_)
	{}

	void Run();
private:
	Engine& engine;
	//Systems
	InputSystem inputsystem;
	PhysicsSystem physicssystem;
	CollisionSystem collisionsystem;
	RenderSystem rendersystem;
	LifeSystem lifesystem;
};

