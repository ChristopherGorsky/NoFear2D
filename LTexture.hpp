#include <iostream>
#include <string>
#include "SDL2.hpp"
#include "Graphics.hpp"
#include "Debug.hpp"
#pragma once

class LTexture {

public:
	
	LTexture(void);
	LTexture(SDL_Renderer* renderer, std::string file);
	~LTexture();
	
	bool loadFromRenderedText(SDL_Renderer* renderer, TTF_Font* font, SDL_Color textColor, std::string textureText);
	bool loadFromRenderedText(SDL_Renderer* renderer, TTF_Font * font, SDL_Color textColor, const char* format, ...);
	bool loadTexture(SDL_Renderer* renderer, std::string file);

	void free(void);

	void render(SDL_Renderer* renderer, SDL_Rect clip, SDL_Rect dest);
	void render(SDL_Renderer* renderer, SDL_Rect dest);
	void render(SDL_Renderer* renderer, int x, int y);
	void render(SDL_Renderer* renderer, int cx, int cy, int cw, int ch, int dx, int dy, int dw, int dh);
private:

	const char* m_path;
	int m_width, m_height;
	GLuint mtexture;
};

