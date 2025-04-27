#include "AnimationSystem.h"
#include "../managers/Engine.h"

void AnimationSystem::update(Engine& engine, float& delta)
{
	for (auto& id : join<sprite, anim>()) {
		auto& animc = get<anim>(id);

        if (animc.numframes == 0) continue;

        //update time
        animc.currentTime += delta;
        float frametime = 1.0f / animc.framerate;

        if (animc.currentTime >= frametime)
        {
            animc.currentTime -= frametime;
            animc.currentframe++;

            if (animc.currentframe >= animc.numframes)
            {
                animc.currentframe = 0; // Loop back to first frame
            }
        }
	}
}
