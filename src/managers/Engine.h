//autor - Jose Manuel Soriano Bernabeu
#pragma once

#include "tigr.h"
#include <stdio.h>
#include <string>
#include <sstream>
#include <map>
//ECS
#include "ecs.hh"
#include "Sprite.hpp"
//components
#include "../components/AIComponent.h"
#include "../components/AnimationComponent.h"
#include "../components/PhysicsComponent.h"
#include "../components/SpawnerComponent.h"
//utils
#include "../utils/Vector2.h"
//gamelay
#include "Spawner.h"

struct GameData;

//global variables
static constexpr float timeStep = 1000.f / 60.f;
static constexpr float ScreenWidth = 320.f;
static constexpr float ScreenHeight = 240.f;

//components
using sprite = component< Sprite*, 'spr' >;
using input = component < bool, 'inp' >;
using physics = component < PhysicsComponent , 'phy'>; 
using spawner = component < SpawnerComponent, 'spw'>;
using IA = component < AIComponent, 'ia' >;
//using Anim = component < AnimationComponent, 'anim'>;

class Engine {
public:
	bool	m_isRunning = false;
	Tigr* m_screen = nullptr;
	float time = 0.f;
private:
	//Sprites
	std::map<int, std::unique_ptr<Sprite>> spritesPool;
	const char* filenameEntities;
	//entities ids
	int nextEntityID = 0;
public:

	//Data-driving functions
	void LoadSprites(const char* filename);
	void CreatePlayer(GameData& gd);
	void CreateEnemy();
	Behaviours strToBehaviour(const std::string& str);

	//score
	void LoadRecord(GameData& gd);
	void SaveScore(float newscore);
	//entitymanager
	void ResetEntities(GameData& gd);

	//Inicializar
	bool Init();

	//bool Input();
	bool KeyDown(int key);

	//Free Memory
	bool Quit();

	//Check if screen continue running or not
	bool isRunning();

	//Clear background with colour
	void Clear(unsigned char r, unsigned char g, unsigned char b);

	//Update Screen
	void DoSystemEventsAndUpdateScreen();

	void Log(const char* text);

	//Print text
	void Print(const char* text, int x, int y);

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

	void PlayDemo();


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
		float left = rectPos.first;
		float right = rectPos.first + rectSize.first;
		float top = rectPos.second;
		float bottom = rectPos.second + rectSize.second;
		vec2f lefttop{ left ,top };
		vec2f leftbottom{ left,bottom };
		vec2f righttop{ right,top };
		vec2f rightbottom{ right,bottom };

		// Comprobamos si está dentro del circulo
		if (circlePos.first > left && circlePos.first < right &&
			circlePos.second >= top && circlePos.second < bottom)
		{
			return true;
		}
		else {
			// Comprobamos cuadrantes esquinas
			if (circlePos.first < left && circlePos.second < top)
			{
				// esquina superior izquierda
				if (euclidean_distance(lefttop,circlePos) < circleRadius)
				{
					return true;
				}
			}
			if (circlePos.first < left && circlePos.second > bottom)
			{
				// esquina inferior izquierda
				if (euclidean_distance(leftbottom,circlePos) < circleRadius)
				{
					return true;
				}
			}
			if (circlePos.first > right && circlePos.second < top)
			{
				// esquina superior deerecha
				if (euclidean_distance(righttop,circlePos) < circleRadius)
				{
					return true;
				}
			}
			if (circlePos.first > right && circlePos.second > bottom)
			{
				// esquina inferior deerecha
				if (euclidean_distance(rightbottom,circlePos) < circleRadius)
				{
					return true;
				}
			}
			// Comprobamos bordes horizontales y verticales
			if (circlePos.first >= left && circlePos.second <= right)
			{
				// Bordes superior e inferior
				if (std::abs(circlePos.second - top) < circleRadius || std::abs(circlePos.second - bottom) < circleRadius)
				{
					return true;
				}
			}
			if (circlePos.second >= top && circlePos.second <= bottom)
			{
				// Bordes izquierdo y derecho
				if (std::abs(circlePos.first - left) < circleRadius || std::abs(circlePos.first - right) < circleRadius)
				{
					return true;
				}
			}
		}
		return false;
	}
};