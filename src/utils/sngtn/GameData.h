#pragma once

#include "../Vector2.h"

//GameData - Global class that contains game data

//Game States
enum States { Menu, Gameplay, Dead, Pause };

class GameData
{
public:
    // Global access
    static GameData& Instance()
    {
        static GameData instance;
        return instance;
    }

    // Delete copy and assigment constructors
    GameData(const GameData&) = delete;
    GameData& operator=(const GameData&) = delete;

    // Public members
    States CurrentState = Menu;
    vec2f spawnpoint{ 0.f, 0.f };
    float bestScore = 0.f;
    float GlobalTimer = 0.f;
    //fail sound counter
    float timer = 0.f;
    float interval = 3.f;
    bool soundplayed = false;
    bool UpdateSoundFail(float& delta)
    {
        timer += delta;
        if (timer >= interval) {
            timer = 0;
            return true;
        }
        return false;
    }
private:
    // Private constructor
    GameData() = default;
};