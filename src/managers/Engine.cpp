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
      if (gid > 0) enemyEntities++;
    }
    printf("num enemies: %d", enemyEntities);
  }
}

void Engine::CreatePlayer(/*GameData& gd*/)
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
  Playsound("sounds/spawnsound.wav");

  std::random_device rd; // Semilla
  std::mt19937 gen(rd()); 
  std::uniform_int_distribution<> distrib(1, enemyEntities); // Rango [1, N]

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

        float lifetime = entityNode.child("lifetime").text().as_float();

        //create entity
        int enemy = nextEntityID++;
        add<sprite>(enemy) = spritesPool.at(gid).get();
        add<physics>(enemy) = PhysicsComponent{ .position = vec2f(x,y),.velocity = vec2f{velx,vely}, .gravity = gravity,.bounciness = bounciness };
        add<IA>(enemy) = AIComponent{ .behaviour = strToBehaviour(behaviourStr) };
        add<life>(enemy) = LifeComponent{ .Lifetime = lifetime };
        break;
      }
    }
  }
}

void Engine::DeleteEnemy(int id)
{
  del<sprite>(id);
  del<physics>(id);
  del<IA>(id);
  del<life>(id);
}

void Engine::MoveEnemies()
{
  //desplazamos los enemigos y eliminamos el último
  for (int id = 1; id < nextEntityID - 1; ++id)
  {
    if (has<sprite>(id + 1)) get<sprite>(id) = get<sprite>(id + 1);
    if (has<physics>(id + 1)) get<physics>(id) = get<physics>(id + 1);
    if (has<IA>(id + 1)) get<IA>(id) = get<IA>(id + 1);
    if (has<life>(id + 1)) get<life>(id) = get<life>(id + 1);
  }

  nextEntityID--;
  //DeleteEnemy(--nextEntityID);
}

Behaviours Engine::strToBehaviour(const std::string& str)
{
  if (str == "SquashStretch") return Behaviours::SquashStretch;
  if (str == "BounceSimple") return Behaviours::BounceSimple;
  return Behaviours::BounceSimple;
}

void Engine::LoadRecord(/*GameData& gd*/)
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

void Engine::ResetEntities(/*GameData& gd*/)
{
  auto& phy = get<physics>(0);
  phy.position = gd.spawnpoint;

  //clear enemies
  for (int id = 1; id <= nextEntityID; id++)
  {
    DeleteEnemy(id);
  }

  nextEntityID = 1;
}

bool Engine::Init() {
	m_screen = tigrWindow(ScreenWidth, ScreenHeight, "Arquitectura", 0);
	if (m_screen == nullptr) return false;

    ma_result result;
    result = ma_engine_init(NULL, &SoundEngine);
    if (result != MA_SUCCESS) {
        return -1;
    }
    else
    {
        printf("MiniAudio initialized succesfully!\n");
    }

	m_isRunning = true;
	return true;
}

bool Engine::KeyDown(int key)
{
  return tigrKeyHeld(m_screen, key);
}

//bool Engine::RightMouse()
//{
//  return tigrMouse(m_screen,)
//}

bool Engine::Quit() {
	tigrFree(m_screen);
	m_screen = nullptr;

    //Uninit sounds and sound engine
    for (auto& sound : sounds)
    {
        ma_sound_uninit(&sound.second);
    }
    ma_engine_uninit(&SoundEngine);
	return true;
};

bool Engine::isRunning() {
	if (tigrClosed(m_screen))
		m_isRunning = false;
	return m_isRunning;
}

void Engine::StopRunning()
{
    m_isRunning = false;
}

bool Engine::isPaused()
{
    return m_ispaused;
}

void Engine::SetPaused(bool value)
{
    m_ispaused = value;
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

void Engine::Print(const char* text, int x ,int y, unsigned char r, unsigned char g, unsigned b)
{
	tigrPrint(m_screen, tfont, x, y, tigrRGB(r, g, b), text);
}

void Engine::PrintButtom(const char* text, int x, int y, TPixel backgroundColor, TPixel textColor)
{
    int width = tigrTextWidth(tfont, text);
    int height = tigrTextHeight(tfont, text);
    tigrFillRect(m_screen, x - 5.f,y - 5.f,width + 8.f,height + 8.f, backgroundColor);
    tigrPrint(m_screen, tfont, x, y, textColor, text);
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

void Engine::LoadAllsounds()
{
    if (!Loadsound("arcade_music", "sounds/backgroundmusic.wav") ||
        !Loadsound("dead_sound", "sounds/dead.wav"))
    {
        printf("Load Sound Fail\n");
    }
}

//Sounds
bool Engine::Loadsound(const std::string& name, const char* path)
{
    if (ma_sound_init_from_file(&SoundEngine, path, 0, nullptr, nullptr, &sounds[name]) != MA_SUCCESS)
        return false;

    return true;
}

void Engine::Playsound(const char* file)
{
    ma_engine_play_sound(&SoundEngine, file, NULL);
}

void Engine::Startsound(const std::string& name)
{
    if (sounds.count(name))
        ma_sound_start(&sounds[name]);
}

void Engine::Stopsound(const std::string& name)
{
    if (sounds.count(name))
    {
        ma_sound_stop(&sounds[name]);
        ResetSound(name);
    }
}

bool Engine::CheckIfSoundends(const std::string& name)
{
    return ma_sound_at_end(&sounds[name]);
}

void Engine::ResetSound(const std::string& name)
{
    ma_sound_seek_to_pcm_frame(&sounds[name], 0);
}
