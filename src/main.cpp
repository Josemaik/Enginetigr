//MIT LICENSE
//AUTHOR: JOSE MANUEL SORIANO BERNABEU


#pragma once

#include "../libs/minilua/minilua.h"
#include "../src/managers/Engine.h"
#include "../src/managers/Game.h"
#include <Windows.h>
#include <iostream>

int main()
{
	//Bloque Lua
	lua_State* L = luaL_newstate();
	if (L == NULL)
		return -1;
	luaL_openlibs(L);
	luaL_loadstring(L, "print 'TigrEngine ready!'");
	lua_call(L, 0, 0);
	lua_close(L);

	//Initialize engine
	Engine engine;
	if (engine.Init())
	{
		Game game{engine};

		//Start Game
		game.Run();

		engine.Quit();
	}
	return 0;
}