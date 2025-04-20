//autor - Jose Manuel Soriano Bernabeu
#pragma once

#include "tigr.h"
#include <stdio.h>
#include <string>
#include <sstream>
//ECS
#include "ecs.hh"
#include "Spritemanager.hpp"
//components
#include "../components/AIComponent.h"
#include "../components/AnimationComponent.h"
#include "../components/PhysicsComponent.h"
//utils
#include "../utils/Vector2.h"

//global variables
static constexpr float timeStep = 1000.f / 60.f;
static constexpr float ScreenWidth = 320.f;
static constexpr float ScreenHeight = 240.f;

//components
using sprite = component< Sprite*, 'spr' >;
using input = component < bool, 'inp' >;
using physics = component < PhysicsComponent , 'phy'>; 
using IA = component < AIComponent, 'ia' >;
//using Anim = component < AnimationComponent, 'anim'>;

class Engine {
public:
	bool	m_isRunning = false;
	Tigr* m_screen = nullptr;
	float time = 0.f;

	//Inicializar
	bool Init();

	//bool Input();
	bool KeyDown(int key);

	//Free Memory
	bool Quit();

	//Check if screen continue running or not
	bool isRunning();

	//Clear background with colour
	void Clear();

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

	void CreateEntity();


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
};