#include "Game.h"
#include "../src/managers/Engine.h"
#include "../utils/sngtn/GameData.h"
#include <iomanip>
#include <sstream>
#include <iostream>



void Game::Run()
{
	//Load Player from xml
	//Sprite spriteplayer;
	//spriteplayer.Load(spriteplayer, "<?xml version=\"1.0\"?><sprite x = \"123\" y = \"456\" texture_file = \"../data/squinkle.png\" />");
	GameData gamedata;
	engine.LoadSprites("../data/entities.txt");
	engine.CreatePlayer(gamedata);
	//Load Record
	engine.LoadRecord(gamedata);

	Spawner spawner{ 5.f };
	////Creo player
	//int player = 0;
	//add<sprite>(player) = &spriteplayer;
	////add<position>(player) = ;
	////add<speedX>(player) = 60.f;
	//Sprite* spr = get<sprite>(player);
	//add<physics>(player) = PhysicsComponent{ .position = vec2f((ScreenWidth / 2.f) - (spr->image->w / 2.f), ScreenHeight - spr->image->h),
	//																				 .velocity = vec2f{60.f,0.f} };
	//add<input>(player) = true;
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
	//int enemiemid = 1;
	//add<sprite>(enemiemid) = &spritenemie1;
	//add<physics>(enemiemid) = PhysicsComponent{ .position = vec2f(60.f, 0.f) ,
	//																						.velocity = vec2f{30.f,0.f} , .gravity = 20.f,.bounciness = 0.8f};
	//add<IA>(enemiemid) = AIComponent{ .behaviour = Behaviours::SquashStretch };

	//Creo enemie simple - rebotan normal y cambian de sentido al colisionar con cualquier objeto
	//Sprite spritenemie1("../data/hexagonball_frame1.png");
	//sprite spritenemie2("../data/hexagonball_frame2.png");
	//sprite spritenemie3("../data/hexagonball_frame3.png");

	/*int enemie = 1;
	add<sprite>(enemie) = &spritenemie1;
	add<physics>(enemie) = PhysicsComponent{.position = vec2f((ScreenWidth / 2.5f), 0.f) ,
																						       .velocity = vec2f{30.f,30.f} };
	add<IA>(enemie) = AIComponent{.behaviour = Behaviours::BounceSimple};*/
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
		//engine.Wait(timeStep);

		switch (CurrentState)
		{
		case Menu:
		{
			engine.Clear(0, 255, 0);

			engine.Print("Press SPACE to Start GAME!",70.f, ScreenHeight / 2.5f);

			//Show best Score
			std::ostringstream ss;
			ss << std::fixed << std::setprecision(2) << gamedata.bestScore;
			engine.Print(("Best Record: " + engine.toString(ss.str()) + "s").c_str(), 100.f, ScreenHeight / 2.5f + 35.f, 0xff, 0xff, 0x00);

			if (engine.KeyDown(TK_SPACE))
			{
				CurrentState = States::Gameplay;
			}
		}
			break;
		case Gameplay:
		{
			engine.Clear(0, 0, 255);
			//check spawnable entities
			//for (auto id : system<spawner>()) {
			//	auto& sp = get<spawner>(id);

			//	sp.timer += delta;
			//	if (sp.timer >= sp.interval /* && sp.enemiesSpawned < sp.maxEnemies*/) {
			//		sp.timer = 0.0f;
			//		//sp.enemiesSpawned++;

			//		int enemyID = sp.nextEntityID++; // o lo que uses
			//		add<sprite>(enemyID) = new Sprite("../data/ball_mid.png");
			//		add<physics>(enemyID) = PhysicsComponent{
			//				.position = vec2f{ScreenWidth / 2.5f, 20.f},//coger de xml sp.spawnPosition,
			//				.velocity = vec2f{0.f, 60.f}
			//		};
			//		add<IA>(enemyID) = AIComponent{ .behaviour = Behaviours::BounceSimple };
			//	}
			//}
			printf("Enemies alive: %d\n",engine.GetMaxEntities());
			//input
			for (auto& id : join<input,physics>()) {
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
			//physics
			for (auto& id : join<physics, sprite,IA>())
			{
				auto& phy = get<physics>(id);
				auto& bh = get<IA>(id).behaviour;

				phy.lastposition = phy.position;
				
				if (bh == Behaviours::SquashStretch)
				{
					// Aplicar gravedad
					phy.velocity.second += phy.gravity * delta;
				}

				//update position
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
							if (engine.checkCircleCircle(phyenemy.position, sprenemy->image->w / 2.5f, pos, spr->image->w / 2.5f))
							{
								printf("Colision con player\n");
								CurrentState = States::Dead;
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

			//life
			for (auto& id : join<sprite, life>()) {
				auto& lifec = get<life>(id);
				lifec.current_time += delta;
				if (lifec.current_time >= lifec.Lifetime) {
					engine.MoveEnemies();
				}
			}

			//HUD
			engine.Print((engine.toString(fps) + " fps").c_str(), 0, 5);

			//update spawner
			spawner.Update(delta, engine);

			std::ostringstream ss;
			ss << std::fixed << std::setprecision(2) << gamedata.GlobalTimer;
			engine.Print(("Current Time: " + engine.toString(ss.str()) + "s").c_str(), 190, 5,0x00,0xff,0x00);

			std::ostringstream ss2;
			ss2 << std::fixed << std::setprecision(2) << gamedata.bestScore;
			engine.Print(("Hi-Score: " + engine.toString(ss2.str()) + "s").c_str(), 80, 5, 0xff,0xff,0x00);

			gamedata.GlobalTimer += delta;
		}
			break;
		case Dead:
		{
			engine.Clear(255, 0, 0);

			engine.Print("You died! :(", ScreenWidth / 2.5f, ScreenHeight / 2.5f);

			std::ostringstream ss;
			ss << std::fixed << std::setprecision(2) << gamedata.GlobalTimer;
			engine.Print(("Score: " + engine.toString(ss.str()) + "s").c_str(), 120.f, ScreenHeight / 2.5f + 35.f);

			engine.Print("Press SPACE for a new Try!", 70.f, ScreenHeight / 1.5f);


			if (engine.KeyDown(TK_SPACE))
			{
				//save score > bestscore
				if (gamedata.GlobalTimer > gamedata.bestScore)
				{
					gamedata.bestScore = gamedata.GlobalTimer;
					engine.SaveScore(gamedata.bestScore);
				}

				//reset timer
				gamedata.GlobalTimer = 0.f;

				//reposition player and remove enemies
				engine.ResetEntities(gamedata);
				spawner.Reset();

				//chage gameplay
				CurrentState = States::Gameplay;
			}
		}
		default:
			break;
		}

		engine.DoSystemEventsAndUpdateScreen();
	}
}
