//MIT LICENSE
//AUTHOR: JOSE MANUEL SORIANO BERNABEU


#pragma once

#include "../libs/minilua/minilua.h"
#include "../src/managers/Engine.h"
#include <Windows.h>
#include <iostream>

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
	Sprite spr("data/squinkle.png");
	spr.x = 40.f;
	spr.y = 40.f;
	//Serializar
	std::string data;
	if (spr.Save(data, spr))
	{
		std::printf("Guardado correctamente\n");
	}
	//Load de un objeto serializado
	Sprite empty;
	empty.Load(empty, data);

	//Load de un xml
	Sprite spr2;
	spr2.Load(spr2, "<?xml version=\"1.0\"?><sprite x = \"123\" y = \"456\" texture_file = \"data/squinkle.png\" />");
	spr2.x = (ScreenWidth / 2.f) - (spr2.image->w / 2.f);
	spr2.y = ScreenHeight - spr2.image->h;
	//float previousTime = engine.getTime();
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
			engine.Print((engine.toString(fps) + " fps").c_str());

			//input

			spr2.Draw(engine.getScreen(), 0, 0);

			engine.DoSystemEventsAndUpdateScreen();
		}
		//Free memory
		engine.Quit();
	}
	return 0;
}