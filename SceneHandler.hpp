#include "Scene.hpp"
#include <map>
#include <string.h>
#pragma once

class SceneHandler {
public:
	SceneHandler();
	~SceneHandler();
	Scene* addScene(Scene* scene);
	void setScene(std::string name);

	void events(const float &delta, const Dimension<int> &screen, const SDL_Event& evnt);
	void update(const float &delta, const Dimension<int> &screen);
	void render(const float &delta, const Dimension<int> &screen);

private:
	std::map<std::string, Scene*> m_scenes;
	Scene* m_currentScene;
};

