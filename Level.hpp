#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <string.h>
#include <stdint.h>
#include "SDL2.hpp"
#include "Debug.hpp"
#include "Graphics.hpp"
#pragma once

typedef unsigned char Tile;
#define TILE_BLANK 0x00
#define TILE_SOLID 0x01

class Level {

public:

	Level();
	Level(const char* file);
	~Level();
	bool load(const char* file);
	void render(const Point<int> &camera, const Dimension<int> &screen);
	void free(void);
	bool loaded(void);

	int getTotalWidth(void) const { return m_totalWidth; };
	int getTotalHeight(void) const { return m_totalHeight; };
	int getTileRes(void) const { return m_res; }
	int getWidth(void) const { return m_width; }
	int getHeight(void) const { return m_height; }
	char getTile(const int& x, const int& y) const { return m_map[y][x]; }

private:
	std::vector<std::vector<Tile>> m_map;
	int m_width, m_height, m_totalWidth, m_totalHeight, m_res;
	bool m_loaded;
};

