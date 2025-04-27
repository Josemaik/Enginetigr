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

		//Player Movement
		if (!engine.isPaused())
		{
			if (engine.KeyDown('A') || engine.KeyDown(TK_LEFT)) //meter flecha izquierda, click izquierdo
			{
				pos.first -= vel.first * delta;
			}
			if (engine.KeyDown('D') || engine.KeyDown(TK_RIGHT)) //meter flecha derecha, click derecho
			{
				pos.first += vel.first * delta;
			}
			if (engine.KeyDown(TK_ESCAPE) || engine.KeyDown('P')) //Pause Menu
			{
				engine.SetPaused(true);
			}
		}
		else
		{
			if (engine.KeyDown(TK_DOWN) || engine.KeyDown('S'))
			{
				engine.current_option++;
				if (engine.current_option >= engine.pause_options)
					engine.current_option = engine.current_option - 1;
			}
			if (engine.KeyDown(TK_UP) || engine.KeyDown('W'))
			{
				engine.current_option--;
				if (engine.current_option < 0)
					engine.current_option = 0;
			}
			if (engine.KeyDown(TK_RETURN) && engine.current_option == 0)
			{
				engine.SetPaused(false);
			}
			if (engine.KeyDown(TK_RETURN) && engine.current_option == 1)
			{
				engine.StopRunning();
			}
		}
	}
}
