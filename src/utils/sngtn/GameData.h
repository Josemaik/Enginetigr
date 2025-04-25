#pragma once

#include "../Vector2.h" // Asegúrate de incluir el header donde esté vec2f

enum States { Menu, Gameplay, Dead };

class GameData
{
public:
    // Acceso global al singleton
    static GameData& Instance()
    {
        static GameData instance;
        return instance;
    }

    // Eliminar operaciones de copia y asignación
    GameData(const GameData&) = delete;
    GameData& operator=(const GameData&) = delete;

    // Miembros públicos
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
    // Constructor privado
    GameData() = default;
};