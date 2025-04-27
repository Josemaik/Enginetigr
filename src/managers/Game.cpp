#include "Game.h"
#include "../src/managers/Engine.h"
#include <iomanip>
#include <sstream>
#include <iostream>



void Game::Run()
{
	//Load resources
	engine.LoadSprites("../data/entities.txt");
	engine.LoadRecord();
	engine.LoadAllsounds();
	//Create player
	engine.CreatePlayer();

	//Spawner
	Spawner spawner{ 5.f };
	//GameData singleton
	auto& gd = engine.GetGameData();
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

		switch (gd.CurrentState)
		{
		case Menu:
		{
			engine.Clear(0, 255, 0);

			engine.Print("Press SPACE to Start GAME!",70.f, ScreenHeight / 2.5f);

			//Show best Score
			std::ostringstream ss;
			ss << std::fixed << std::setprecision(2) << gd.bestScore;
			engine.Print(("Best Record: " + engine.toString(ss.str()) + "s").c_str(), 100.f, ScreenHeight / 2.5f + 35.f, 0xff, 0xff, 0x00);

			if (engine.KeyDown(TK_SPACE))
			{
				engine.Playsound("sounds/startgame.wav");
				engine.Startsound("arcade_music");
				gd.CurrentState = States::Gameplay;
			}
		}
			break;
		case Gameplay:
		{
			//loop music
			if (engine.CheckIfSoundends("arcade_music"))
			{
				engine.ResetSound("arcade_music");
				engine.Startsound("arcade_music");
			}

			engine.Clear(0, 0, 255);
			
			//Update systems
			if (!engine.isPaused())
			{
				physicssystem.update(engine, delta);
				collisionsystem.update(engine, delta,gd);
				lifesystem.update(engine, delta);
				animsystem.update(engine, delta);
				//update spawner
				spawner.Update(delta, engine);
				//update score
				gd.GlobalTimer += delta;
			}
			//update even in pause mode
			inputsystem.update(engine, delta);
			rendersystem.update(engine,gd,fps);
		}
			break;
		case Dead:
		{
			engine.Clear(255, 0, 0);
			//Stop music
			engine.Stopsound("arcade_music",true);

			//Sound fail reproduction management
			if (!gd.soundplayed)
			{
				engine.Startsound("dead_sound");
				gd.soundplayed = true;
			}
			//in three seconds stop 
			if (gd.UpdateSoundFail(delta))
			{
				engine.Stopsound("dead_sound",true);
			};
			//-------------------------------------

			engine.Print("You died! :(", ScreenWidth / 2.5f, ScreenHeight / 2.5f);

			std::ostringstream ss;
			ss << std::fixed << std::setprecision(2) << gd.GlobalTimer;
			engine.Print(("Score: " + engine.toString(ss.str()) + "s").c_str(), 120.f, ScreenHeight / 2.5f + 35.f);

			engine.Print("Press SPACE for a new Try!", 70.f, ScreenHeight / 1.5f);


			if (engine.KeyDown(TK_SPACE))
			{
				//save score > bestscore
				if (gd.GlobalTimer > gd.bestScore)
				{
					gd.bestScore = gd.GlobalTimer;
					engine.SaveScore(gd.bestScore);
				}

				//reset timer
				gd.GlobalTimer = 0.f;

				//reposition player and remove enemies
				engine.ResetEntities();
				//reset spawner
				spawner.Reset();

				//change to gameplay
				gd.soundplayed = false;
				engine.Startsound("arcade_music");
				gd.CurrentState = States::Gameplay;
			}
		}
		default:
			break;
		}

		engine.DoSystemEventsAndUpdateScreen();
	}
}
