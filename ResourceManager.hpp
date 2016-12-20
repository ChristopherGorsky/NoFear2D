#include <map>
#include <iostream>
#include "SDL2.hpp"
#include "Debug.hpp"
#include "Graphics.hpp"
#pragma once
using namespace Graphics2D;

class ResourceManager {

public:
	ResourceManager();
	~ResourceManager();

	static ResourceManager& GetInstance(void) {
		static ResourceManager instance;
		return instance;
	}

	//Texture* loadTexture(std::string name, char* file);
	Sprite* getSprite(std::string name);

	TTF_Font* loadFont(std::string name, const char* file, int size);
	TTF_Font* getFont(std::string name);

private:
	std::map<std::string, Sprite*> mSprites;
	std::map<std::string, TTF_Font*> mFonts;
};

