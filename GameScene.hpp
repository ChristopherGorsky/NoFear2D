#pragma once
#include "Scene.hpp"
#include "LevelHandler.hpp"
#include "EventHandler.hpp"

class GameScene : public Scene {

public:
	GameScene(std::string name);
	~GameScene();

	void events(const float &delta, const Dimension<int> &screen, const SDL_Event& evnt);
	void update(const float &delta, const Dimension<int> &screen);
	void render(const float &delta, const Dimension<int> &screen);
private:
	LevelHandler m_levelHndlr;
	Point<int> m_camera;
	EventHandler m_eventHandler;
};

