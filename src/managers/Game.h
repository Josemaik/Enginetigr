#pragma once

#include "../systems/InputSystem.h"
#include "../systems/PhysicsSystem.h"
#include "../systems/CollisionSystem.h"
#include "../systems/RenderSystem.h"
#include "../systems/LifeSystem.h"
#include "../systems/AnimationSystem.h"

//Game - Contains all game logic

class Engine;

class Game
{
public:
	//Constructor
	Game(Engine& engine_) : 
		engine(engine_)
	{}

	void Run();
private:
	//engine reference
	Engine& engine;
	//Systems
	InputSystem inputsystem;
	PhysicsSystem physicssystem;
	CollisionSystem collisionsystem;
	RenderSystem rendersystem;
	LifeSystem lifesystem;
	AnimationSystem animsystem;
};

