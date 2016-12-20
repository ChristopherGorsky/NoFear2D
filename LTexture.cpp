#include "LTexture.hpp"

LTexture::LTexture(void) {

}

LTexture::LTexture(SDL_Renderer* renderer,std::string file) {
	loadTexture(renderer, file);
}

LTexture::~LTexture() {
}

bool LTexture::loadFromRenderedText(SDL_Renderer* renderer, TTF_Font * font, SDL_Color textColor, std::string textureText) {
	
	//Get rid of preexisting texture
	free();

	if (textureText.size() <= 0) {
		return false;
	}

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, textureText.c_str(), textColor);
	if (textSurface == NULL) {
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
		return false;
	} else {

		//Create texture from surface pixels
		glGenTextures(1, &mtexture);
		glBindTexture(GL_TEXTURE_2D, mtexture);
		glTexImage2D(GL_TEXTURE_2D, 0, IMG_MODE_A, textSurface->w, textSurface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, textSurface->pixels);

		//Get image dimensions
		m_width = textSurface->w;
		m_height = textSurface->h;

		//Get rid of old surface
		SDL_FreeSurface(textSurface);
	}

	//Return success
	return false;
}

bool LTexture::loadFromRenderedText(SDL_Renderer* renderer, TTF_Font * font, SDL_Color textColor, const char* format, ...) {

	char buffer[255];

	va_list args;
	va_start(args, format);
	vsprintf_s(buffer, format, args);
	va_end(args);
	
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, buffer, textColor);
	if (textSurface == NULL) {

		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
		return false;

	} else {
		//Create texture from surface pixels
		glGenTextures(1, &mtexture);
		glBindTexture(GL_TEXTURE_2D, mtexture);
		glTexImage2D(GL_TEXTURE_2D, 0, IMG_MODE_A, textSurface->w ,textSurface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, textSurface->pixels);

		//Get image dimensions
		m_width = textSurface->w;
		m_height = textSurface->h;

		//Get rid of old surface
		SDL_FreeSurface(textSurface);
	}

	//Return success
	return true;;
}

bool LTexture::loadTexture(SDL_Renderer* renderer, std::string file) {
	
	Debug& dbg = Debug::GetInstance();
	if (renderer == NULL) {
		dbg.Print("No renderer to create surface texture");
		return false;
	}

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load((char*)file.c_str());
	if (loadedSurface == NULL) {

		dbg.Print("Unable to load image %s! SDL_image Error: %s\n", file, IMG_GetError());
		return false;

	} else {

		glGenTextures(1, &mtexture);
		glBindTexture(GL_TEXTURE_2D, mtexture);
		glTexImage2D(GL_TEXTURE_2D, 0, IMG_MODE_A, loadedSurface->w, loadedSurface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, loadedSurface->pixels);

		//Get image dimensions
		m_width = loadedSurface->w;
		m_height = loadedSurface->h;

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	return true;
}

void LTexture::free(void){

	glDeleteTextures(1, &mtexture);
	m_width = 0;
	m_height = 0;
}

void LTexture::render(SDL_Renderer* renderer, SDL_Rect dest) {
}

void LTexture::render(SDL_Renderer * renderer, int x, int y) {

}
