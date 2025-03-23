#pragma once
#include <string>
#include <tigr.h>
#include <vector>
#include <sstream>
//pugiXML
#include "pugixml.hpp"

struct Sprite
{
	Sprite() {};
	Sprite(const std::string& file)
	{
		texture_file = file;
		image = tigrLoadImage(file.c_str());
	}
	~Sprite()
	{
		if (image)
		{
			tigrFree(image);
			image = nullptr;
		}
	}

	void Draw(Tigr* window)
	{
		//int dx = x, dy = y; //coordinates
		int sx = 0, sy = 0; // source co - ordinates
		int w = image->w, h = image->h;
		tigrBlit(window, image, x, y, sx, sy, image->w, image->h);
	}

	static
	bool Save(std::string& out, Sprite& obj)
	{
		pugi::xml_document doc;
		pugi::xml_node node = doc.append_child("sprite");
		node.append_attribute("x").set_value(obj.x);
		node.append_attribute("y").set_value(obj.y);
		node.append_attribute("texture_file").set_value(obj.texture_file.c_str());

		try
		{
			std::stringstream ss;
			doc.save(ss);
			out = ss.str();
			return true;
		}
		catch (...) { return false; }
		

		return true;
	}
	static 
	bool Load(Sprite& obj,const std::string& source)
	{
		try {
			pugi::xml_document doc;
			pugi::xml_parse_result result = doc.load_string(source.c_str());

			if (result)
			{
				std::string texture_file = doc.child("sprite").attribute("texture_file").value();
				obj.x = doc.child("sprite").attribute("x").as_float();
				obj.y = doc.child("sprite").attribute("y").as_float(); //asci a float

				obj.texture_file = texture_file;
				obj.image = tigrLoadImage(obj.texture_file.c_str());
				return true;
			}
		}
		catch (...){
		}
		return false;
	}

	/*unsigned width{ 0 }, height{ 0 }, channels{ 0 };*/ //R,G,G,A
	float x = 0, y = 0;
	//std::vector<uint32_t> pixels;
	//void *pixels{NULL};
	std::string texture_file;
	//imagen
	Tigr* image = NULL;

	//std::vector<uint32_t> pixels;
};

//template<typename T>
//T Load(const std::string& file)
//{
//
//}
//
//template<typename T>
//bool Load(T& object, const std::string& file)
//{
//	return false;
//}



