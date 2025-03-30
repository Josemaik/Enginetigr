#include "Game.h"
#include "../src/managers/Engine.h"
#include <iomanip>
#include <sstream>



void Game::Run()
{
	engine.Init();

	//Load de un xml
	Sprite spr2;
	spr2.Load(spr2, "<?xml version=\"1.0\"?><sprite x = \"123\" y = \"456\" texture_file = \"../data/squinkle.png\" />");
	spr2.x = (ScreenWidth / 2.f) - (spr2.image->w / 2.f);
	spr2.y = ScreenHeight - spr2.image->h;
	
	//Creo player
	int player = 0;
	add<sprite>(player) = spr2;
	add<position>(player) = vec2f((ScreenWidth / 2.f) - (spr2.image->w / 2.f), ScreenHeight - spr2.image->h);
	add<speedX>(player) = 60.f;
	add<input>(player) = true;
	add<lastposition>(player) = vec2f{};
	//Creo enemie
	Sprite spritenemie("../data/ball.png");
	int enemie = 1;
	add<sprite>(enemie) = spritenemie;
	add<speedY>(enemie) = 30.f;
	add<speedX>(enemie) = 30.f;
	add<position>(enemie) = vec2f((ScreenWidth / 2.5f), 0.f);
	add<lastposition>(enemie) = vec2f{};
	add<IA>(enemie) = true;
	//Creo enemie
	Sprite spritenemie1("../data/ball_mid.png");
	int enemiemid = 2;
	add<sprite>(enemiemid) = spritenemie1;
	add<speedY>(enemiemid) = 60.f;
	add<speedX>(enemiemid) = -60.f;
	add<position>(enemiemid) = vec2f(30.f, 0.f);
	add<lastposition>(enemiemid) = vec2f{};
	add<IA>(enemiemid) = true;
	//Creo enemie
	//Sprite spritenemie1("../data/ball_mid.png");
	/*int enemiemid2 = 3;
	add<sprite>(enemiemid2) = spritenemie1;
	add<speedY>(enemiemid2) = 60.f;
	add<speedX>(enemiemid2) = 60.f;
	add<position>(enemiemid2) = vec2f(30.f, 0.f);
	add<lastposition>(enemiemid2) = vec2f{};
	add<IA>(enemiemid2) = true;*/

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
			for (auto& id : join<lastposition, position>()) {
				vec2f& pos = get<position>(id);
				vec2f& laspos = get<lastposition>(id);
				laspos.first = pos.first;
				laspos.second = pos.second;
			}

			//input
			for (auto& id : join<input, speedX>()) {
				float& speed = get<speedX>(id);
				vec2f& pos = get<position>(id);
				if (engine.KeyDown('A'))
				{
					pos.first -= speed * delta;
				}
				if (engine.KeyDown('D'))
				{
					pos.first += speed * delta;
				}
			}
			//physics
			for (auto& id : join<speedY, position>())
			{
				float& speedx = get<speedX>(id);
				float& speedy = get<speedY>(id);
				vec2f& pos = get<position>(id);
				pos.first += speedx * delta;
				pos.second += speedy * delta;
			}
			//collisions
			for (auto& id : join<position, sprite>()) {
				vec2f& laspos = get<lastposition>(id);
				vec2f& pos = get<position>(id);
				Sprite& spr = get<sprite>(id);

				//collision with screen bounds
				bool leftWall = pos.first < 0;
				bool rightWall = (pos.first + spr.image->w) > ScreenWidth;
				bool buttomWall = (pos.second + spr.image->h) > ScreenHeight;
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
							auto& speed = get<speedX>(id);
							speed *= -1;
						}
						if (buttomWall || upWall)
						{
							auto& speed = get<speedY>(id);
							speed *= -1;
							//colisiona abajo
						}
						//colision con otra esfera
						if (id == 1)
						{
							//es el 2
							Sprite& spr2 = get<sprite>(2);
							vec2f& pos2 = get<position>(2);

							if (engine.checkCircleCircle(pos, spr.image->w / 2.5f, pos2, spr2.image->w / 2.5f))
							{
								printf("Colision entre circulos\n");
								auto& speedy2 = get<speedY>(2);
								auto& speedx2 = get<speedX>(2);
								speedy2 *= -1;
								speedx2 *= -1;
								auto& speedy = get<speedY>(id);
								auto& speedx = get<speedX>(id);
								speedy *= -1;
								speedx *= -1;
							}
						}
					}
				}
			}

			//render
			for (auto& id : join<position, sprite>()) {
				auto& pos = get<position>(id);
				auto& spr = get<sprite>(id);
				spr.x = pos.first;
				spr.y = pos.second;
				get<sprite>(id).Draw(engine.getScreen());
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
