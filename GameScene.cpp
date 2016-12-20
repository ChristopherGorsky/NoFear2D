#include "GameScene.hpp"

GameScene::GameScene(std::string name) : Scene(name) {
	m_levelHndlr.loadLevel("test", "data/test.txt");
	m_levelHndlr.switchLevel("test");
	m_eventHandler.loadEvent("data/test.event");
	m_eventHandler.playEvent();
}

GameScene::~GameScene(void) {

}

void GameScene::events(const float &delta, const Dimension<int> &screen, const SDL_Event& evnt) {

	m_eventHandler.events(evnt);
	if (!m_eventHandler.eventRunning()) {
	}
}

void GameScene::update(const float &delta, const Dimension<int> &screen) {
}

void GameScene::render(const float &delta, const Dimension<int> &screen) {
	m_levelHndlr.render(m_camera, screen);
	m_eventHandler.render(screen);
}
