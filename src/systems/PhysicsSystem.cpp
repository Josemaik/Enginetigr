#include "PhysicsSystem.h"
#include "../managers/Engine.h"

void PhysicsSystem::update(Engine& engine, float& delta)
{
	for (auto& id : join<physics, sprite, IA>())
	{
		auto& phy = get<physics>(id);
		auto& bh = get<IA>(id).behaviour;

		phy.lastposition = phy.position;

		if (bh == Behaviours::SquashStretch)
		{
			// Apply gravity
			phy.velocity.second += phy.gravity * delta;
		}

		//update position
		phy.position.first += phy.velocity.first * delta;
		phy.position.second += phy.velocity.second * delta;
	}
}
