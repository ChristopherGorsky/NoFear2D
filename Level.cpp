#include "Level.hpp"

Level::Level() {

	m_loaded = false;
}

Level::Level(const char* file) {

	m_loaded = load(file);
}

Level::~Level() {
}

bool Level::load(const char* file) {

	Debug& dbg = Debug::GetInstance();
	dbg.Print("Loading map: %s", file);

	std::ifstream in(file);
	if (in.bad()) {
		dbg.Print("Failed to load file");
		return false;
	}

	int width, height, res;
	in >> width;
	in >> height;
	in >> res;

	m_map.resize(height);
	for (int i = 0; i < height; i++) {
		m_map[i].resize(width);
	}
	
	//create the buffer, add 1 slot for the newline char
	int tile = 0x00;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			in >> std::hex >> tile;
			m_map[y][x] = tile;
		}
	}

	m_width = width;
	m_height = height;

	m_totalWidth = width * res;
	m_totalHeight = height * res;

	m_res = res;

	in.close();
	dbg.Print("File loaded");

	m_loaded = true;
	return true;
}

void Level::render(const Point<int> &camera, const Dimension<int> &screen) {
	
	int dx = 0, dy = 0, hres = m_res / 2;
	
	int yMax = (screen.height / m_res);
	if (yMax > m_height) {
		yMax = m_height;
	}

	int xMax = (screen.width / m_res);
	if (xMax > m_width) {
		xMax = m_width;
	}

	for (int y = camera.y / m_res; y < yMax; y++) {
		for (int x = camera.x / m_res; x < xMax; x++) {
			
			dx = (x * m_res) - camera.x;
			dy = (y * m_res) - camera.y;
			
			switch (m_map[y][x]) {

				case TILE_BLANK:

					SDL_Rect topB;
					topB.x = dx;
					topB.y = dy;
					topB.w = hres;
					topB.h = hres;

					SDL_Rect bottomB;
					bottomB.x = dx + hres;
					bottomB.y = dy + hres;
					bottomB.w = hres;
					bottomB.h = hres;

					Graphics2D::SetColor(1, 1, 1);
					Graphics2D::DrawFilledQuad(topB.x, topB.y, topB.w, topB.h);
					Graphics2D::DrawFilledQuad(bottomB.x, bottomB.y, bottomB.w, bottomB.h);

				break;

				case TILE_SOLID:

					SDL_Rect topC;
					topC.x = dx;
					topC.y = dy;
					topC.w = m_res;
					topC.h = m_res;
					
					Graphics2D::SetColor(1, 1, 1);
					Graphics2D::DrawHollowQuad(topC.x, topC.y, topC.w, topC.h);
				break;
			}
		}
	}
}

bool Level::loaded(void) {
	return m_loaded;
}

