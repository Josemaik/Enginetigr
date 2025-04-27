#include "RenderSystem.h"
#include "../managers/Engine.h"
#include <iomanip>

void RenderSystem::update(Engine& engine, GameData& gamedata, float fps)
{
	//render
	for (auto& id : join<sprite, physics>()) {
		auto& phy = get<physics>(id);
		auto* spr = get<sprite>(id);
		spr->x = phy.position.first;
		spr->y = phy.position.second;

		bool hasanim = false;
		if (has<anim>(id)) //animated sprites
		{
			auto& animc = get<anim>(id);
			if (animc.framerate > 0.f) //animation on
			{
				hasanim = true;
				get<sprite>(id)->DrawAnim(engine.getScreen(), animc.frames[animc.currentframe], animc.framewidth);
			}
		}	

		if(!hasanim) //sprite witout animation component
		{
			get<sprite>(id)->Draw(engine.getScreen());
		}
	}

	//HUD
	engine.Print((engine.toString(fps) + " fps").c_str(), 0, 5);

	std::ostringstream ss;
	ss << std::fixed << std::setprecision(2) << gamedata.GlobalTimer;
	engine.Print(("Current Time: " + engine.toString(ss.str()) + "s").c_str(), 190, 5, 0x00, 0xff, 0x00);

	std::ostringstream ss2;
	ss2 << std::fixed << std::setprecision(2) << gamedata.bestScore;
	engine.Print(("Hi-Score: " + engine.toString(ss2.str()) + "s").c_str(), 80, 5, 0xff, 0xff, 0x00);

	//Pause Menu
	if (engine.isPaused())
	{
		TPixel resume_back = tigrRGB(0, 0, 0), exit_back = tigrRGB(0, 0, 0);
		if (engine.current_option == 0)
		{
			resume_back = tigrRGB(255, 165, 0);
			exit_back = tigrRGB(205, 205, 205);
		}
		if (engine.current_option == 1)
		{
			resume_back = tigrRGB(205, 205, 205);
			exit_back = tigrRGB(255, 165, 0);
		}
		engine.PrintButtom("Resume", 120.f, 120.f, resume_back, tigrRGB(255, 255, 255));
		engine.PrintButtom("Exit", 130.f, 140.f, exit_back, tigrRGB(255, 255, 255));
	}
}
