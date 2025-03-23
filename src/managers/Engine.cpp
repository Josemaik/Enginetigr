#include "Engine.h"

#include <cassert>
#include <string>
#include <iostream>

bool Engine::Init() {
	m_screen = tigrWindow(ScreenWidth, ScreenHeight, "Arquitectura", 0);
	if (m_screen == nullptr) return false;

	m_isRunning = true;
	return true;
}

bool Engine::KeyDown(int key)
{
  return tigrKeyHeld(m_screen, key);
}

bool Engine::Quit() {
	tigrFree(m_screen);
	m_screen = nullptr;
	return true;
};

bool Engine::isRunning() {
	if (tigrClosed(m_screen))
		m_isRunning = false;
	return m_isRunning;
}

void Engine::Clear()
{
	tigrClear(m_screen, tigrRGB(0, 0, 255));
}

void Engine::DoSystemEventsAndUpdateScreen()
{
	tigrUpdate(m_screen);
}

void Engine::Log(const char* text)
{
	puts(text);
}

void Engine::Print(const char* text, int x ,int y)
{
	tigrPrint(m_screen, tfont, x, y, tigrRGB(0xff, 0xff, 0xff), text);
}

double Engine::getTime()
{
	static double t = 0;
	t += tigrTime();
	return t;
}

Tigr* Engine::getScreen() const
{
  return m_screen;
}

void Engine::Wait(float ms)
{
	float then = getTime();
	do {
		float now = getTime();
		float dt = now - then;
		then = now;
		ms -= dt * 1000;
	} while (ms > 0);
}

void Engine::PlayDemo()
{
  const vec2f zero = { 0.f, 0.f }, one = { 1.f, 1.f };
  // component aliases
  using friendly = component<        bool, 'team' >;
  using health = component<         int, 'heal' >;
  using mana = component<         int, 'mana' >;
  using coins = component<         int, 'coin' >;
  using name = component< std::string, 'name' >;
  using position = component<       vec2f, 'pos2' >;
  // entities
  int none = 0, player = 1, enemy = 2;

  // components
  assert(!has<name>(player));
  assert(!has<position>(player));
  assert(!has<coins>(enemy));
  assert(!has<health>(enemy));

  add<name>(player) = "Hero";
  add<position>(player) = zero;
  add<health>(player) = 100;
  add<coins>(player) = 200;
  add<mana>(player) = 4000;
  add<friendly>(player) = true;

  add<name>(enemy) = "Orc";
  add<position>(enemy) = one;
  add<health>(enemy) = 200;
  add<coins>(enemy) = 50;
  add<mana>(enemy) = 10;

  assert(get<health>(player) == 100); // :>

  assert(has<name>(player));
  assert(!has<vec2i>(player));
  assert(has<position>(player));
  assert(has<health>(player));

  assert(get<name>(player) == "Hero");
  assert(get<position>(player) == zero);
  assert(get<health>(player) == 100);

  // systems; here we intersect a system of all elements with <name> and <position>.
  assert((join<name, position>().size() == 2));

  // systems; render game state
  auto display = [this]() {
    std::cout << "- ";
    for (auto& id : join<name, coins, health, position>()) {
      /*std::cout
        << get<name>(id) << " at "
        << "(" << get<position>(id).first << "," << get<position>(id).second << ")"
        << " " << get<health>(id) << "HP"
        << " " << get<coins>(id) << "$, ";*/
      //Print(toString(get<name>(id) + " at ").c_str());
    }
    std::cout << std::endl;
    };

  display();

  // systems; simulate movement
  for (auto& id : join<name, position>()) {
    std::cout << get<name>(id) << " says: im moving!" << std::endl;
    vec2f& pos = get<position>(id);
    pos.first += 10;
    pos.second++;
  }

  // systems; simulate a spell bomb in entities of any type
  for (auto& id : system<mana>()) {
    std::cout << "spellboomb!!!" << std::endl;
    get<mana>(id) -= 50;
  }

  // systems; simulate a powerup (+$100) for all players
  for (auto& id : join<name, coins, friendly>()) {
    get<coins>(id) += 100;
    std::cout << get<name>(id) << " says: money! :)" << std::endl;
  }

  // systems; simulate a poison (-50%HP) to all entities that are not friendly (so enemies)
  for (auto& id : exclude<friendly>(join<name, health>())) {
    get<health>(id) *= 0.5;
    std::cout << get<name>(id) << " says: ugh! poisoned :(" << std::endl;
  }

  display();

  assert(get<health>(player) == 100 + 0);
  assert(get<health>(enemy) == 200 / 2);
  assert(get<coins>(player) == 200 + 100);
  assert(get<coins>(enemy) == 50 + 0);
  assert(get<mana>(player) == 4000 - 50);
  assert(get<mana>(enemy) == 10 - 50);

  assert(del<position>(player));
  assert(!has<position>(player));
  assert(del<name>(player));
  assert(!has<name>(player));

  assert((join<name, position>().size() == 1));
}

void Engine::CreateEntity()
{
  
}
