#include "SDL2.hpp"
#include "Debug.hpp"
#define PI 3.1415926535897932384626433832795
#define STRBUFF 255
#pragma once

namespace Graphics2D {

	class Sprite {

	public:

		Sprite();
		Sprite(const char* file);
		~Sprite(void);

		bool load(const char* file);
		void free(void);

		void frameCrop(float w, float h);
		void clearFrames(void);
		void setFrame(int frame);
		int getFrame(void);

		bool loaded(void) const { return mLoaded; }
		bool loadFromRenderedText(TTF_Font* font, float* textColor, const char* format, ...);
		bool loadFromRenderedText(TTF_Font* font, float* textColor, std::string text);

		void render(float x, float y);
		void render(float x, float y, float w, float h);
		void render(float src[], float dest[]);

	private:

		int mFrame, mCropW, mCropH;
		int mWidth, mHeight;
		bool mLoaded;
		GLuint mTexture;
	};

	void DrawHollowQuad(float x, float y, float w, float h);
	void DrawFilledQuad(float x, float y, float w, float h);

	void DrawHollowPolygon(float x, float y, float radius, int angles, const float color[][3]);
	void DrawHollowPolygon(float x, float y, float radius, int angles, const float color[][4]);
	void DrawHollowPolygon(float x, float y, float radius, int angles);

	void DrawLifeCircle(float health, float healthMax, float x, float y, float size);

	void DrawRoundedQuad(float x, float y, float w, float h, int curve, int pts);
	void DrawRoundedQuadEX(float x, float y, float w, float h, int curve, int pts, float colors[][3]);
	void DrawRoundedQuadEX(float x, float y, float w, float h, int curve, int pts, float colors[][4]);

	void SetColor(float r, float g, float b, float a);
	void SetColor(float r, float g, float b);
};