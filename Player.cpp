#include "Player.hpp"

Player::Player() {
	
	m_pos.x = 96;
	m_pos.y = 96;
	
	m_vel.x = 0;
	m_vel.y = 0;
	
	m_dim.width = 32;
	m_dim.height = 32;

	m_speed = 4;

	m_upPressed = false;
	m_downPressed = false;
	m_leftPressed = false;
	m_rightPressed = false;
}


Player::~Player() {
}

void Player::events(const float &delta, const Dimension<int> &screen, const SDL_Event& evnt) {
	
	if (evnt.type == SDL_KEYDOWN && evnt.key.repeat == 0) {

		switch (evnt.key.keysym.sym) {

		case SDLK_a: m_vel.x -= m_speed; m_leftPressed = true;  break;
		case SDLK_d: m_vel.x += m_speed; m_rightPressed = true; break;
		case SDLK_s: m_vel.y += m_speed; m_downPressed = true; break;
		case SDLK_w: m_vel.y -= m_speed; m_upPressed = true; break;

		}
	} else if (evnt.type == SDL_KEYUP && evnt.key.repeat == 0) {

		switch (evnt.key.keysym.sym) {

		case SDLK_a: if (m_leftPressed) m_vel.x += m_speed; break;
		case SDLK_d: if (m_rightPressed) m_vel.x -= m_speed; break;
		case SDLK_s: if (m_downPressed) m_vel.y -= m_speed; break;
		case SDLK_w: if (m_upPressed) m_vel.y += m_speed; break;

		}
	}
}

void Player::update(Point<int> &camera, const Level* level, const float &delta, const Dimension<int> &screen) {
	
	int tileRes = level->getTileRes();
	int yMax = (screen.height / tileRes);
	if (yMax > level->getHeight()) {
		yMax = level->getHeight();
	}

	int xMax = (screen.width / tileRes);
	if (xMax > level->getWidth()) {
		xMax = level->getWidth();
	}

	bool collision = false;
	int dx = 0, dy = 0;

	m_pos.x += m_vel.x;
	for (int y = camera.y / tileRes && !collision; y < yMax; y++) {
		for (int x = camera.x / tileRes && !collision; x < xMax; x++) {

			if (level->getTile(x, y) == TILE_SOLID) {

				dx = (x * tileRes) - camera.x;
				dy = (y * tileRes) - camera.y;

				if (isColliding(dx, dy, tileRes)) {

					if (m_vel.x > 0) {
						m_pos.x = dx - m_dim.width;
					} else if (m_vel.x < 0) {
						m_pos.x = dx + tileRes;
					}

					collision = true;
				}
			}
		}
	}

	collision = false;
	m_pos.y += m_vel.y;
	for (int y = camera.y / tileRes && !collision; y < yMax; y++) {
		for (int x = camera.x / tileRes && !collision; x < xMax; x++) {

			if (level->getTile(x, y) == TILE_SOLID) {

				dx = (x * tileRes) - camera.x;
				dy = (y * tileRes) - camera.y;

				if (isColliding(dx, dy, tileRes)) {

					if (m_vel.y > 0)
						m_pos.y = dy - m_dim.height;
					else if (m_vel.y < 0)
						m_pos.y = dy + tileRes;

					collision = true;
				}
			}
		}
	}

	updateCamera(camera, level);
}

void Player::render(Point<int> &camera, const Level* level, const float &delta, const Dimension<int> &screen) {
	Graphics2D::SetColor(1, 0, 0);
	Graphics2D::DrawFilledQuad(m_pos.x, m_pos.y, m_dim.width, m_dim.height);
}

void Player::updateCamera(Point<int>& camera, const Level* level) {

	camera.x = m_pos.x - level->getTotalWidth() / 2;
	camera.y = m_pos.y - level->getTotalHeight() / 2;

	//Keep the camera in bounds
	if (camera.x < 0) {
		camera.x = 0;
	}
	if (camera.y < 0) {
		camera.y = 0;
	}
	if (camera.x > level->getTotalWidth()) {
		camera.x = level->getTotalWidth();
	}
	if (camera.y > level->getTotalHeight()) {
		camera.y = level->getTotalHeight();
	}
}

bool Player::isColliding(const int & x, const int & y, const int& res){

	//The sides of the rectangles
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	//Calculate the sides of rect A PLAYER
	leftA = m_pos.x;
	rightA = m_pos.x + m_dim.width;
	topA = m_pos.y;
	bottomA = m_pos.y + m_dim.height;

	//Calculate the sides of rect B
	int tileRes = res;
	leftB = x;
	rightB = x + tileRes;
	topB = y;
	bottomB = y + tileRes;

	//If any of the sides from A are outside of B
	if (bottomA <= topB){
		return false;
	}

	if (topA >= bottomB){
		return false;
	}

	if (rightA <= leftB){
		return false;
	}

	if (leftA >= rightB){
		return false;
	}

	//If none of the sides from A are outside B
	return true;
}
