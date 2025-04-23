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
			exit(1); // Opcional, o dejar que continúe mostrando algo.
		}
		//collision with screen bounds
		bool leftWall = pos.first < 0;
		bool rightWall = (pos.first + spr->image->w) > ScreenWidth;
		bool buttomWall = (pos.second + spr->image->h) > ScreenHeight;
		bool upWall = pos.second < 0;

		//update always
		if (has<input>(id))
		{
			for (int i = 1; i < engine.GetMaxEntities(); i++)
			{
				Sprite* sprenemy = get<sprite>(i);
				if (sprenemy)
				{
					auto& phyenemy = get<physics>(i);
					/*if (engine.checkCircleRect(phyenemy.position, sprenemy->image->w / 2.5f, pos, vec2f{ spr->image->w,spr->image->h }))
					{
						printf("Colision con player\n");
						CurrentState = States::Dead;
					}*/
					if (engine.checkCircleCircle(phyenemy.position, sprenemy->image->w / 2.f, pos, spr->image->w / 2.f))
					{
						printf("Colision con player\n");
						gd.CurrentState = States::Dead;
					}
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
						//phy.gravity += 5.f;
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
						//phy.position.second -= spr->image->w / 2.5f;
						phy.position.second = laspos.second;
						phy.velocity.second *= -phy.bounciness;
					}
				}
				//colision con otra esfera
				//if (id == 1)
				//{
				//	//es el 2
				//	auto& phy2 = get<physics>(2);
				//	Sprite* spr2 = get<sprite>(2);
				//	vec2f& pos2 = phy2.position;

				//	if (spr2 != nullptr)
				//	{
				//		if (engine.checkCircleCircle(pos, spr->image->w / 2.5f, pos2, spr2->image->w / 2.5f))
				//		{
				//			printf("Colision entre circulos\n");
				//			auto& speedy2 = phy2.velocity.second;
				//			auto& speedx2 = phy2.velocity.first;
				//			speedy2 += 5;
				//			speedx2 += 5;
				//			speedy2 *= -1;
				//			speedx2 *= -1;
				//			auto& speedy = phy.velocity.second;
				//			auto& speedx = phy.velocity.first;
				//			speedy *= -1;
				//			speedx *= -1;
				//		}
				//	}
				//}
			}
		}
	}
}
