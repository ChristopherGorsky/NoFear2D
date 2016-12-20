#include "SDL2.hpp"
#include<string.h>
#include<iostream>
#pragma once

class Scene {

public:
	Scene(std::string name);
	std::string getName(void);

	virtual void events(const float &delta, const Dimension<int> &screen, const SDL_Event& evnt) = 0;
	virtual void update(const float &delta, const Dimension<int> &screen) = 0;
	virtual void render(const float &delta, const Dimension<int> &screen) = 0;

private:
	std::string m_name;
};