#include <map>
#include "Level.hpp"
#pragma once

class LevelHandler {
public:
	LevelHandler();
	~LevelHandler();
	Level* loadLevel(std::string name, const char* file);
	Level* switchLevel(std::string name);

	void render(const Point<int> &camera, const Dimension<int> &screen);
	Level* getCurrentLevel(void) { return m_currentLevel; }
private:
	std::map<std::string, Level*> m_levels;
	Level* m_currentLevel;
};

