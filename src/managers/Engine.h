//autor - Jose Manuel Soriano Bernabeu
#pragma once

#include "tigr.h"
#include <stdio.h>
#include <string>
#include <sstream>
#include <map>
#include <unordered_map>
//libs
#include "ecs.hh"
#include "../libs/miniaudio.h"
//components
#include "../components/AIComponent.h"
#include "../components/AnimationComponent.h"
#include "../components/PhysicsComponent.h"
#include "../components/LifeComponent.h"
//Managers
#include "Spawner.h"
#include "Sprite.hpp"
//utils
#include "../utils/Vector2.h"
#include "../utils/sngtn/GameData.h"

//global variables
static constexpr float timeStep = 1000.f / 60.f;
static constexpr float ScreenWidth = 320.f;
static constexpr float ScreenHeight = 240.f;

//components
using sprite = component< Sprite*, 'spr' >;
using input = component < bool, 'inp' >;
using physics = component < PhysicsComponent , 'phy'>; 
using IA = component < AIComponent, 'ia' >;
using life = component < LifeComponent, 'life' >;
using anim = component <AnimationComponent, 'anim' >;

class Engine {
public:
	bool	m_isRunning = false;
	bool	m_ispaused = false;
	Tigr* m_screen = nullptr;
	float time = 0.f;
	ma_engine SoundEngine;
	//pause menu
	int pause_options = 2;
	int current_option = 0;
private:
	GameData& gd = GameData::Instance();
	//Sprites
	std::map<int, std::unique_ptr<Sprite>> spritesPool;
	const char* filenameEntities;
	//entities ids
	int nextEntityID = 0;
	int enemyEntities = 0;
	//sounds
	std::unordered_map<std::string, ma_sound> sounds;
public:
	//singleton
	GameData& GetGameData() { return gd; }

	//////////////////////////////////////
	//Data-driving methods
	void LoadSprites(const char* filename);
	void CreatePlayer();
	void CreateEnemy();

	//helpers
	Behaviours strToBehaviour(const std::string& str);

	/////////////////////////////////////
	//score
	void LoadRecord();
	void SaveScore(float newscore);

	////////////////////////////////////
	//entitymanager
	void ResetEntities(); //reset entities
	inline int GetMaxEntities() const { return nextEntityID; }
	void DeleteEnemy(int id); //delete entity
	void MoveEnemies(int idToRemove); //swap-pop

	///////////////////////////////////
	//Tigr methods
	bool Init();

	//bool Input();
	bool KeyDown(int key);

	//Free Memory
	bool Quit();

	//Check if screen continue running or not
	bool isRunning();

	void StopRunning();

	//Pause Menu methods
	bool isPaused();
	void SetPaused(bool value);

	//Clear background with colour
	void Clear(unsigned char r, unsigned char g, unsigned char b);

	//Update Screen
	void DoSystemEventsAndUpdateScreen();

	void Log(const char* text);

	//Print text
	void Print(const char* text, int x, int y, unsigned char r = 0xff, unsigned char g = 0xff, unsigned b = 0xff);
	void PrintButtom(const char* text, int x, int y, TPixel backgroundColor, TPixel textColor);
	//Update deltaTime
	double getTime();

	Tigr* getScreen() const;

	//Function to change to string format
	template<typename T>
	std::string toString(const T& t)
	{
		std::stringstream ss{};
		ss << t;
		return ss.str();
	}

	//Wait milisecond
	void Wait(float ms);

	//////////////////////////////////////////
	//Sound
	void LoadAllsounds();
	bool Loadsound(const std::string& name, const char* path);
	void Playsound(const char* file);
	void Startsound(const std::string& name);
	void Stopsound(const std::string& name);
	bool CheckIfSoundends(const std::string& name);
	void ResetSound(const std::string& name);

	//////////////////////////////////////////
	//Collisions
	float euclidean_distance(const vec2f& _other,const vec2f& _other1) const
	{
		auto dx{ _other.first - _other1.first };
		auto dy{ _other.second - _other1.second };
		return std::sqrt(dx * dx + dy * dy);
	}
	////////////////////////////////////
	//CIRCLE-CIRCLE
	bool checkCircleCircle(const vec2f& pos1, float radius1, const vec2f& pos2, float radius2) const
	{
		float distance = std::abs(euclidean_distance(pos1,pos2));
		if (distance < (radius1 + radius2))
		{
			return true;
		}
		return false;
	}
	////////////////////////////////////
	//CIRCLE-RECT
	bool checkCircleRect(const vec2f& circlePos, float circleRadius, const vec2f& rectPos, const vec2f& rectSize)
	{
		// Obtener el punto más cercano en el rectángulo al centro del círculo
		float closestX = std::max(rectPos.first, std::min(circlePos.first, rectPos.first + rectSize.first));
		float closestY = std::max(rectPos.second, std::min(circlePos.second, rectPos.second + rectSize.second));

		float dx = circlePos.first - closestX;
		float dy = circlePos.second - closestY;

		return (dx * dx + dy * dy) < (circleRadius * circleRadius);
	}
};