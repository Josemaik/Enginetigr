#include "CollisionSystem.h"
#include "../managers/Engine.h"
#include <iostream>

void CollisionSystem::update(Engine& engine, float& delta, GameData& gd)
{
	//collisions
	for (auto& id : join<physics, sprite>()) {
		auto& phy = get<physics>(id);
		vec2f& laspos = phy.lastposition;
		vec2f& pos = phy.position;
		Sprite* spr = get<sprite>(id);
		if (spr->image == nullptr)
		{
			std::cerr << "Error: Sprite image is nullptr!" << std::endl;
			exit(1);
		}

		int imagewidth = spr->image->w;
		if (has<anim>(id))
		{
			auto& animc = get<anim>(id); //get anim frame width to calculate collision
			imagewidth = animc.framewidth;
		}

		//collision with screen bounds
		bool leftWall = pos.first < 0;
		bool rightWall = (pos.first + imagewidth) > ScreenWidth;
		bool buttomWall = (pos.second + spr->image->h) > ScreenHeight;
		bool upWall = pos.second < 0;

		//update always
		if (has<input>(id))
		{
			for (int i = 1; i < engine.GetMaxEntities(); i++) //Collisions Player-Enemies
			{
				Sprite* sprenemy = get<sprite>(i);
				if (sprenemy)
				{
					auto& phyenemy = get<physics>(i);
					auto& animc = get<anim>(i);
					// Calculate radius
					vec2f circleCenter = {
						phyenemy.position.first + animc.framewidth / 2.f,
						phyenemy.position.second + sprenemy->image->h / 2.f
					};
					float radius = std::min(animc.framewidth, sprenemy->image->h) / 2.f;

					
					if (engine.checkCircleRect(circleCenter, radius, pos, vec2f{ spr->image->w,spr->image->h }))
					{
						printf("Colision con player\n");
						gd.CurrentState = States::Dead;
					}
					/*if (engine.checkCircleCircle(phyenemy.position, sprenemy->image->w / 2.f, pos, spr->image->w / 2.f))
					{
						printf("Colision con player\n");
						gd.CurrentState = States::Dead;
					}*/
				}
			}
		}

		//update only if position has changed
		if (pos != laspos)
		{
			//check if is player
			if (has<input>(id) && (leftWall || rightWall))
			{
				pos.first = laspos.first;
				pos.second = laspos.second;
			}
			//check if is enemie
			if (has<IA>(id))
			{
				auto& bh = get<IA>(id).behaviour;
				if (rightWall || leftWall)
				{
					if (bh == Behaviours::BounceSimple)
					{
						phy.velocity.first *= -1;
					}
					else
					{
						phy.velocity.first *= -1.f;
						phy.bounciness -= 0.1f;
					}
				}
				if (buttomWall || upWall)
				{
					if (bh == Behaviours::BounceSimple)
					{
						phy.velocity.second *= -1;
					}
					else
					{
						phy.position.second = laspos.second;
						phy.velocity.second *= -phy.bounciness;
					}
				}
			}
		}
	}
}
