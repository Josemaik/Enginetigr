#include "InputSystem.h"
#include "../managers/Engine.h"

void InputSystem::update(Engine& engine,float& delta)
{
	//input
	for (auto& id : join<input, physics>()) {
		auto& phy = get<physics>(id);
		auto& pos = phy.position;
		auto& vel = phy.velocity;

		phy.lastposition = phy.position;

		if (engine.KeyDown('A') || engine.KeyDown(TK_LEFT)) //meter flecha izquierda, click izquierdo
		{
			pos.first -= vel.first * delta;
		}
		if (engine.KeyDown('D') || engine.KeyDown(TK_RIGHT)) //meter flecha derecha, click derecho
		{
			pos.first += vel.first * delta;
		}
	}
}
