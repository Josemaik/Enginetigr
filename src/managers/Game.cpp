#include "Game.h"
#include "../src/managers/Engine.h"
#include <iomanip>
#include <sstream>
#include <iostream>



void Game::Run()
{
	//Load de un xml
	Sprite spr2;
	spr2.Load(spr2, "<?xml version=\"1.0\"?><sprite x = \"123\" y = \"456\" texture_file = \"../data/squinkle.png\" />");
	spr2.x = (ScreenWidth / 2.f) - (spr2.image->w / 2.f);
	spr2.y = ScreenHeight - spr2.image->h;
	
	//Creo player
	int player = 0;
	add<sprite>(player) = &spr2;
	//add<position>(player) = ;
	//add<speedX>(player) = 60.f;
	add<physics>(player) = PhysicsComponent{ .position = vec2f((ScreenWidth / 2.f) - (spr2.image->w / 2.f), ScreenHeight - spr2.image->h),
																					 .velocity = vec2f{60.f,0.f} };
	add<input>(player) = true;
	//add<lastposition>(player) = vec2f{};
	//Enemigo que rebota y pierde energía
	/*Sprite spritenemie("../data/ball.png");
	int enemie = 1;
	add<sprite>(enemie) = &spritenemie;
	add<speedY>(enemie) = 30.f;
	add<speedX>(enemie) = 30.f;
	add<position>(enemie) = vec2f((ScreenWidth / 2.5f), 0.f);
	add<lastposition>(enemie) = vec2f{};
	add<IA>(enemie) = AIComponent{.behaviour = Behaviours::SquashStretch};*/
	//Enemigo que rebota y pierde energía
	//Sprite spritenemie1("../data/ball_mid.png");
	//int enemiemid = 2;
	//add<sprite>(enemiemid) = spritenemie1;
	//add<speedY>(enemiemid) = 60.f;
	//add<speedX>(enemiemid) = -60.f;
	//add<position>(enemiemid) = vec2f(30.f, 0.f);
	//add<lastposition>(enemiemid) = vec2f{};
	//add<IA>(enemiemid) = AIComponent{ .behaviour = Behaviours::SquashStretch };

	//Creo enemie simple - rebotan normal y cambian de sentido al colisionar con cualquier objeto
	Sprite spritenemie1("../data/hexagonball_frame1.png");
	//sprite spritenemie2("../data/hexagonball_frame2.png");
	//sprite spritenemie3("../data/hexagonball_frame3.png");

	int enemie = 1;
	add<sprite>(enemie) = &spritenemie1;
	add<physics>(enemie) = PhysicsComponent{.position = vec2f((ScreenWidth / 2.5f), 0.f) ,
																						       .velocity = vec2f{30.f,30.f} };
	add<IA>(enemie) = AIComponent{.behaviour = Behaviours::BounceSimple};
	//add<Anim>(enemie) = AnimationComponent{ .spritesheet{spritenemie1,spritenemie2,spritenemie3}, .framerate = 5.f,.currentframe = 0 };
	
	//Loop
	while (engine.isRunning())
	{
		//Get Deltatime
		static double elapsed = 0;
		double now = engine.getTime();
		float delta = now - elapsed;
		elapsed = now;
		//fps
		float fps = 1.f / (delta + !delta);

		//Wait
		engine.Wait(timeStep);

		engine.Clear();

		switch (CurrentState)
		{
		case Menu:
		{
			engine.Print("Press ENTER to Start GAME!",70.f, ScreenHeight / 2.5f);

			if (engine.KeyDown(TK_SPACE))
			{
				CurrentState = States::Gameplay;
			}
		}
			break;
		case Gameplay:
		{
			for (auto& id : join<physics,sprite>()) {
				auto& phy = get<physics>(id);
				phy.lastposition = phy.position;
			}

			//input
			for (auto& id : join<input,physics>()) {
				auto& phy = get<physics>(id);
				auto& pos = phy.position;
				auto& vel = phy.velocity;
				if (engine.KeyDown('A'))
				{
					pos.first -= vel.first * delta;
				}
				if (engine.KeyDown('D'))
				{
					pos.first += vel.first * delta;
				}
			}
			//physics
			for (auto& id : join<physics, sprite,IA>())
			{
				auto& phy = get<physics>(id);
				/*auto& pos = phy.position;
				auto& vel = phy.velocity;*/
				/*float& speedx = get<speedX>(id);
				float& speedy = get<speedY>(id);
				vec2f& pos = get<position>(id);*/
				phy.position.first +=  phy.velocity.first * delta;
				phy.position.second += phy.velocity.second * delta;
			}
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

				//update only if position has changed
				if (pos != laspos)
				{
					//check if is player
					if (has<input>(id) && leftWall || rightWall)
					{
						pos.first = laspos.first;
						pos.second = laspos.second;
					}
					//check if is enemie
					if (has<IA>(id))
					{
						if (rightWall || leftWall)
						{
							//lateral derecho
							//auto& speed = phy.velocity.first;
							phy.velocity.first *= -1;
						}
						if (buttomWall || upWall)
						{
							//auto& speed = ;
							phy.velocity.second *= -1;
							//colisiona abajo
						}
						//colision con otra esfera
						if (id == 1)
						{
							//es el 2
							auto& phy2 = get<physics>(2);
							Sprite* spr2 = get<sprite>(2);
							vec2f& pos2 = phy2.position;

							if (spr2 != nullptr)
							{
								if (engine.checkCircleCircle(pos, spr->image->w / 2.5f, pos2, spr2->image->w / 2.5f))
								{
									printf("Colision entre circulos\n");
									auto& speedy2 = phy2.velocity.second;
									auto& speedx2 = phy2.velocity.first;
									speedy2 += 5;
									speedx2 += 5;
									speedy2 *= -1;
									speedx2 *= -1;
									auto& speedy = phy.velocity.second;
									auto& speedx = phy.velocity.first;
									speedy *= -1;
									speedx *= -1;
								}
							}
						}
					}
				}
			}

			//Animation
			//for (auto& id : join<sprite, Anim>())
			//{
				//Sprite& spr = get<sprite>(id);
				/*float& time = get<Anim>(id).currentTime;
				time += delta;
				printf("currenttime: %f", time);*/
				
			//}

			//IA
			//for (auto& id : join<position,IA>())
			//{
			//	Behaviours bh = get<IA>(id).behaviour;
			//	switch (bh)
			//	{
			//	case Behaviours::BounceSimple:
			//	{
			//		//printf("Soy simple\n");

			//	}
			//		break;
			//	case Behaviours::SquashStretch:
			//	{
			//		//printf("Me aplasto\n");

			//	}
			//		break;
			//	default:
			//		break;
			//	}
			//}

			//render
			for (auto& id : join<sprite,physics>()) {
				auto& phy = get<physics>(id);
				auto* spr = get<sprite>(id);
				spr->x = phy.position.first;
				spr->y = phy.position.second;
				get<sprite>(id)->Draw(engine.getScreen());
			}

			//HUD
			engine.Print((engine.toString(fps) + " fps").c_str(), 0, 5);

			std::ostringstream ss;
			ss << std::fixed << std::setprecision(2) << GlobalTimer;
			engine.Print(("Current Time: " + engine.toString(ss.str()) + "s").c_str(), 160, 5);

			GlobalTimer += delta;
		}
			break;
		default:
			break;
		}

		engine.DoSystemEventsAndUpdateScreen();
	}
}
