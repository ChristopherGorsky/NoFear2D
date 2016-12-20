#include "SceneHandler.hpp"



SceneHandler::SceneHandler()
{
}


SceneHandler::~SceneHandler()
{
}

Scene * SceneHandler::addScene(Scene * scene) {
	m_scenes[scene->getName()] = scene;
	return scene;
}

void SceneHandler::setScene(std::string name) {
	
	Scene* s = m_scenes[name];
	if (s != NULL) {
		m_currentScene = s;
	} else {
		std::cout << "Scene not found: " << name.c_str() << std::endl;
	}
}

void SceneHandler::events(const float & delta, const Dimension<int>& screen, const SDL_Event & evnt) {
	
	if (m_currentScene != NULL) {
		m_currentScene->events(delta, screen, evnt);
	}
}

void SceneHandler::update(const float & delta, const Dimension<int>& screen) {
	
	if (m_currentScene != NULL) {
		m_currentScene->update(delta, screen);
	}
}

void SceneHandler::render(const float & delta, const Dimension<int>& screen) {
	
	if (m_currentScene != NULL) {
		m_currentScene->render(delta, screen);
	}
}
