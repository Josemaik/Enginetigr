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
		get<sprite>(id)->Draw(engine.getScreen());
	}

	//HUD
	engine.Print((engine.toString(fps) + " fps").c_str(), 0, 5);

	std::ostringstream ss;
	ss << std::fixed << std::setprecision(2) << gamedata.GlobalTimer;
	engine.Print(("Current Time: " + engine.toString(ss.str()) + "s").c_str(), 190, 5, 0x00, 0xff, 0x00);

	std::ostringstream ss2;
	ss2 << std::fixed << std::setprecision(2) << gamedata.bestScore;
	engine.Print(("Hi-Score: " + engine.toString(ss2.str()) + "s").c_str(), 80, 5, 0xff, 0xff, 0x00);
}
