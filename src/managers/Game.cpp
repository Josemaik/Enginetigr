#include "Game.h"
#include "../src/managers/Engine.h"
#include "../utils/sngtn/GameData.h"
#include <iomanip>
#include <sstream>
#include <iostream>



void Game::Run()
{
	GameData gamedata; //convertir a singleton
	engine.LoadSprites("../data/entities.txt");
	engine.CreatePlayer(gamedata);
	//Load Record
	engine.LoadRecord(gamedata);

	Spawner spawner{ 5.f };
	
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

		switch (gamedata.CurrentState)
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
				gamedata.CurrentState = States::Gameplay;
			}
		}
			break;
		case Gameplay:
		{
			engine.Clear(0, 0, 255);

			printf("Enemies alive: %d\n",engine.GetMaxEntities());
			
			//Update systems
			inputsystem.update(engine, delta);
			physicssystem.update(engine, delta);
			collisionsystem.update(engine, delta,gamedata);
			lifesystem.update(engine, delta);
			rendersystem.update(engine,gamedata,fps);
			//update spawner
			spawner.Update(delta, engine);
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
				gamedata.CurrentState = States::Gameplay;
			}
		}
		default:
			break;
		}

		engine.DoSystemEventsAndUpdateScreen();
	}
}
