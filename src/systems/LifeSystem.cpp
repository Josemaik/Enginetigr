#include "LifeSystem.h"
#include "../managers/Engine.h"

void LifeSystem::update(Engine& engine, float& delta)
{
	//life
	for (auto& id : join<sprite, life>()) {
		auto& lifec = get<life>(id);
		lifec.current_time += delta;
		if (lifec.current_time >= lifec.Lifetime) {
			engine.MoveEnemies();
		}
	}
}
