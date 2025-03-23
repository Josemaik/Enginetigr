//autor - Jose Manuel Soriano Bernabeu
#pragma once

#include "tigr.h"
#include <stdio.h>
#include <string>
#include <sstream>
//ECS
#include "ecs.hh"
#include "Spritemanager.hpp"

//global variables
static constexpr float timeStep = 1000.f / 30.f;
static constexpr float ScreenWidth = 320.f;
static constexpr float ScreenHeight = 240.f;

class Engine {
public:
	bool	m_isRunning = false;
	Tigr* m_screen = nullptr;
	float time = 0.f;

	//Inicializar
	bool Init();

	//bool Input();
	//bool KeyDownA();

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
	void Print(const char* text);

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
};