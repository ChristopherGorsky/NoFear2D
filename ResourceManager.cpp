#include "ResourceManager.hpp"

ResourceManager::ResourceManager() {
}


ResourceManager::~ResourceManager() {
}

/*
Texture* ResourceManager::loadTexture(std::string name, const char* file) {
	
	Debug& dbg = Debug::GetInstance();
	if (m_renderer == NULL) {
		dbg.Print("No renderer attached to Resource Manager");
		return NULL;
	}

	Texture* loadedtexture = m_textures[file];
	if (loadedtexture != NULL) {
		return loadedtexture;
	}

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(file);
	if (loadedSurface == NULL) {
		dbg.Print("Unable to load image %s! SDL_image Error: %s\n", file, IMG_GetError());
	} else {
		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(m_renderer, loadedSurface);
		if (newTexture == NULL) {
			dbg.Print("Unable to create texture from %s! SDL Error: %s\n", file, SDL_GetError());
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	return NULL;
}
*/
Sprite* ResourceManager::getSprite(std::string name) {

	return mSprites[name];
}

TTF_Font* ResourceManager::loadFont(std::string name, const char* file, int size) {

	Debug& dbg = Debug::GetInstance();
	TTF_Font* font = TTF_OpenFont(file, size);
	if (font != NULL) {
		dbg.Print("Loaded font: %s", file);
		mFonts[name] = font;
		return font;
	}

	dbg.Print("Failed to load font: %s", file);
	return NULL;
}

TTF_Font* ResourceManager::getFont(std::string name) {
	return mFonts[name];
}


