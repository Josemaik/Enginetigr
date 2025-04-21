#include "Engine.h"

#include <cassert>
#include <string>
#include <iostream>
#include <random>
#include "../utils/types.h"
#include "../utils/sngtn/GameData.h"

void Engine::LoadSprites(const char* filename)
{
  filenameEntities = filename;
  //Load sprites from xml doc
  pugi::xml_document doc;
  pugi::xml_parse_result result = doc.load_file(filename);
  if (result)
  {
    pugi::xml_node entities = doc.child("entities");
    for (pugi::xml_node entityNode = entities.child("entity");
      entityNode; // Condición correcta
      entityNode = entityNode.next_sibling("entity"))
    {
      int gid = entityNode.child("id").text().as_int();
      std::string spriteSource = entityNode.child("sprite").attribute("texture_file").as_string();
      spritesPool[gid] = std::make_unique<Sprite>(spriteSource.c_str());
    }
  }
}

void Engine::CreatePlayer(GameData& gd)
{
  pugi::xml_document doc;
  pugi::xml_parse_result result = doc.load_file(filenameEntities);
  if (result)
  {
    pugi::xml_node playerNode = doc.child("entities").first_child();
    //read values from xml
    float x = playerNode.child("position").attribute("x").as_float();
    float y = playerNode.child("position").attribute("y").as_float();
    float velx = playerNode.child("velocity").attribute("x").as_float();
    float vely = playerNode.child("velocity").attribute("y").as_float();
    //add components
    int player = nextEntityID++;
    add<sprite>(player) = spritesPool.at(player).get();
    add<physics>(player) = PhysicsComponent{ .position = vec2f(x, y),.velocity = vec2f{velx,vely} };
    add<input>(player) = true;

    gd.spawnpoint = vec2f(x, y);
  }
}

void Engine::CreateEnemy()
{
  int N = 3; //num enemies

  std::random_device rd; // Semilla
  std::mt19937 gen(rd()); 
  std::uniform_int_distribution<> distrib(1, N); // Rango [1, N]

  int random_number = distrib(gen);

  printf("Random: %d\n", random_number);

  pugi::xml_document doc;
  pugi::xml_parse_result result = doc.load_file(filenameEntities);
  if (result)
  {
    pugi::xml_node entitiesNode = doc.child("entities");
    for (pugi::xml_node entityNode = entitiesNode.child("entity");
      entityNode;
      entityNode = entityNode.next_sibling("entity"))
    {
      int gid = entityNode.child("id").text().as_int();
      if (gid == random_number)
      {
        std::string name = entityNode.child("name").text().as_string();
        printf("Lo he encontrado: %s\n", name.c_str());

        //read values from xml
        float x = entityNode.child("position").attribute("x").as_float();
        float y = entityNode.child("position").attribute("y").as_float();
        float velx = entityNode.child("velocity").attribute("x").as_float();
        float vely = entityNode.child("velocity").attribute("y").as_float();

        float gravity = entityNode.child("gravity").text().as_float();
        float bounciness = entityNode.child("bounciness").text().as_float();
        std::string behaviourStr = entityNode.child("ai").attribute("behaviour").as_string();

        //create entity
        int enemy = nextEntityID++;
        add<sprite>(enemy) = spritesPool.at(gid).get();
        add<physics>(enemy) = PhysicsComponent{ .position = vec2f(x,y),.velocity = vec2f{velx,vely}, .gravity = gravity,.bounciness = bounciness };
        add<IA>(enemy) = AIComponent{ .behaviour = strToBehaviour(behaviourStr) };
        break;
      }
    }
  }
}

Behaviours Engine::strToBehaviour(const std::string& str)
{
  if (str == "SquashStretch") return Behaviours::SquashStretch;
  if (str == "BounceSimple") return Behaviours::BounceSimple;
  return Behaviours::BounceSimple;
}

void Engine::LoadRecord(GameData& gd)
{
  pugi::xml_document doc;
  pugi::xml_parse_result result = doc.load_file("../data/score.txt");
  if (result)
  {
    gd.bestScore = doc.child("score").text().as_float();
  }
}

void Engine::SaveScore(float newscore)
{
  pugi::xml_document doc;

  // Crear el nodo raíz "score"
  pugi::xml_node scoreNode = doc.append_child("score");

  // Establecer el texto con el valor del récord
  scoreNode.text().set(newscore);

  // Guardar el documento en el archivo
  bool saveSucceeded = doc.save_file("../data/score.txt");

  if (!saveSucceeded)
  {
    std::cerr << "Error al guardar el archivo de récord." << std::endl;
  }
}

void Engine::ResetEntities(GameData& gd)
{
  auto& phy = get<physics>(0);
  phy.position = gd.spawnpoint;

  //clear enemies
  for (int id = 1; id < nextEntityID; id++)
  {
    del<physics>(id);
    del<IA>(id);
    del<sprite>(id);
  }

  nextEntityID = 1;
}

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

void Engine::Clear(unsigned char r, unsigned char g, unsigned char b)
{
	tigrClear(m_screen, tigrRGB(r, g, b));
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
