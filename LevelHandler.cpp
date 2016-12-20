#include "LevelHandler.hpp"

LevelHandler::LevelHandler(){
}

LevelHandler::~LevelHandler(){
}

Level* LevelHandler::loadLevel(std::string name, const char* file) {
	
	Level* lvl = new Level(file);
	if (lvl->loaded()) {
		m_levels[name] = lvl;
		return lvl;
	}

	delete lvl;
	return false;
}

Level* LevelHandler::switchLevel(std::string name) {

	Level* lvl = m_levels[name];
	if (lvl != NULL) {
		m_currentLevel = lvl;
	}

	return lvl;
}

void LevelHandler::render(const Point<int> &camera, const Dimension<int> &screen) {

	if (m_currentLevel != NULL) {
		m_currentLevel->render(camera, screen);
	}
}

