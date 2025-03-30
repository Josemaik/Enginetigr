//MIT LICENSE
//AUTHOR: JOSE MANUEL SORIANO BERNABEU


#pragma once

#include "../libs/minilua/minilua.h"
#include "../src/managers/Engine.h"
#include <Windows.h>
#include <iostream>
#include <iomanip>
#include <sstream>

int main()
{
	//Bloque Lua
	lua_State* L = luaL_newstate();
	if (L == NULL)
		return -1;
	luaL_openlibs(L);
	luaL_loadstring(L, "print 'hello world xd'");
	lua_call(L, 0, 0);
	lua_close(L);

	//Initialize engine
	Engine engine;


	//Creo Sprite
	/*Sprite spr("data/squinkle.png");
	spr.x = 40.f;
	spr.y = 40.f;*/
	//Serializar
	/*std::string data;
	if (spr.Save(data, spr))
	{
		std::printf("Guardado correctamente\n");
	}*/
	//Load de un objeto serializado
	/*Sprite empty;
	empty.Load(empty, data);*/

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
	add<position>(enemie) = vec2f((ScreenWidth / 2.5f), -20.f);
	add<lastposition>(enemie) = vec2f{};
	add<IA>(enemie) = true;
	//float previousTime = engine.getTime();
	static float GlobalTimer = 0.f;
	if (engine.Init())
	{
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
			
			for (auto& id : join<lastposition,position>()) {
				vec2f& pos = get<position>(id);
				vec2f& laspos = get<lastposition>(id);
				laspos.first = pos.first;
				laspos.second = pos.second;
			}

			//input
			for (auto& id : join<input,speedX>()) {
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
			for (auto& id : join<position,sprite>()) {
				vec2f& laspos = get<lastposition>(id);
				vec2f& pos = get<position>(id);
				Sprite& spr = get<sprite>(id);

				//collision with screen bounds
				bool leftWall = pos.first < 0;
				bool rightWall = (pos.first + spr.image->w) > ScreenWidth;
				bool buttomWall = pos.second > ScreenHeight;
				//bool upWall = (pos.second + spr.image->h) > 
				
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
						if(rightWall || leftWall)
						{
							//lateral derecho
							auto& speed = get<speedX>(id);
							speed *= -1;
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

			engine.DoSystemEventsAndUpdateScreen();
		}
		//Free memory
		engine.Quit();
	}
	return 0;
}