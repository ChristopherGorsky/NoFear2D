#include "Scene.hpp"

Scene::Scene(std::string name) {
	m_name = name;
}

std::string Scene::getName(void) {
	return m_name;
}
