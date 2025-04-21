#include "Spawner.h"
#include "Engine.h"
#include <iomanip>
#include <sstream>
#include <iostream>

void Spawner::Update(float& delta, Engine& engine)
{
  timer += delta;
  if (timer >= interval) {
    timer = 0;
    engine.CreateEnemy();
  }
}
