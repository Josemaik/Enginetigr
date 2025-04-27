#pragma once

//Spawner - Manage enemies spawn

class Engine;

class Spawner {

private:
  float interval = 0.f; //time to spawn new enemy
  float timer = 0.f; //current time

public:
  Spawner(float _interval)
    :interval(_interval)
  {}

  void Reset();

  void Update(float& delta, Engine& engine);
  
};