#ifndef __SDL2__
#define __SDL2__

//Windows
#ifdef _WIN32
#include <SDL_main.h>
#include <SDL.h>

//#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

#include <GL\glew.h>
#include <GL\GL.h>
#include <SDL_opengl.h>

#define IMG_MODE GL_RGB
#define IMG_MODE_A GL_RGBA
#endif

#ifndef TRUE
#define TRUE 1
#endif 

#ifndef FALSE
#define FALSE 0
#endif

//Mac
#ifdef __APPLE__
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>

#define IMG_MODE GL_BGR
#define IMG_MODE_A GL_BGRA
#endif

//Raspberry PI
#ifdef _arm

#endif

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
#define RMASK 0xff000000
#define GMASK 0x00ff0000
#define BMASK 0x0000ff00
#define AMASK 0x000000ff
#else
#define RMASK 0x000000ff
#define GMASK 0x0000ff00
#define BMASK 0x00ff0000
#define AMASK 0xff000000
#endif

struct Texture {
	SDL_Texture* texture;
	int width, height;
	const char* file;
};

template<typename T>
struct Dimension {
	Dimension() {}
	Dimension(T width, T height) {
		this->width = width;
		this->height = height;
	}

	T width, height;
};

template<typename T>
struct Point {
	Point() {}
	Point(T x, T y) {
		this->x = x;
		this->y = y;
	}

	T x, y;
};

#endif