#include "Graphics.hpp"

namespace Graphics2D {

	Sprite::Sprite(void) {
		mLoaded = false;
		mTexture = 0;
		mFrame = -1;
		mCropW = 0;
		mCropH = 0;
	}

	Sprite::Sprite(const char* file) {
		mLoaded = load(file);
		mFrame = -1;
	}

	Sprite::~Sprite(void) {
		free();
	}

	bool Sprite::load(const char* file) {

		Debug& dbg = Debug::GetInstance();

		dbg.Print("Loading: %s", file);
		SDL_Surface* surface = IMG_Load(file);
		if (surface == NULL) {
			dbg.Print("Failed to load file");
			mLoaded = false;
			return false;
		}

		dbg.Print("Attempting to generate texture");
		glGenTextures(1, &mTexture);
		glBindTexture(GL_TEXTURE_2D, mTexture);
		
		int mode = (surface->format->BytesPerPixel == 4) ? IMG_MODE_A : IMG_MODE;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, mode, GL_UNSIGNED_BYTE, surface->pixels);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		dbg.Print("Populating texture info");
		mWidth = surface->w;
		mHeight = surface->h;
		dbg.Print("Texture info: %ix%i", mWidth, mHeight);

		dbg.Print("Freeing surface");
		SDL_FreeSurface(surface);
		glDisable(GL_TEXTURE_2D);
		
		dbg.Print("Done");
		mLoaded = true;
		return true;
	}

	bool Sprite::loadFromRenderedText(TTF_Font* font, float* textColor, const char* format, ...) {

		char buffer[STRBUFF];

		va_list args;
		va_start(args, format);
		vsprintf_s(buffer, format, args);
		va_end(args);

		//Get rid of preexisting texture
		free();

		if (strlen(buffer) <= 0) {
			mLoaded = false;
			return false;
		}

		SDL_Color sColor;
		sColor.r = (textColor[0] * 255);
		sColor.g = (textColor[1] * 255);
		sColor.b = (textColor[2] * 255);
		sColor.a = (textColor[3] * 255);

		//Render text surface
		SDL_Surface* textSurface = TTF_RenderText_Solid(font, buffer, sColor);
		if (textSurface == NULL) {

			printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
			mLoaded = false;
			return false;

		} else {

			SDL_Surface* surface = SDL_CreateRGBSurface(0, textSurface->w, textSurface->h, 32, RMASK, GMASK, BMASK, AMASK);
			if (surface == NULL) {
				SDL_FreeSurface(textSurface);
				printf("Failed create new RGB surface");
				return false;
			}

			SDL_BlitSurface(textSurface, NULL, surface, NULL);

			//Create texture from surface pixels
			glGenTextures(1, &mTexture);
			glBindTexture(GL_TEXTURE_2D, mTexture);

			int mode = (surface->format->BytesPerPixel == 4) ? IMG_MODE_A : IMG_MODE;
			glTexImage2D(GL_TEXTURE_2D, 0, mode, surface->w, surface->h, 0,
				mode, GL_UNSIGNED_BYTE, surface->pixels);

			//Get image dimensions
			mWidth = surface->w;
			mHeight = surface->h;

			glDisable(GL_TEXTURE_2D);

			//Get rid of old surface
			SDL_FreeSurface(textSurface);
			SDL_FreeSurface(surface);
		}

		//Return success
		mLoaded = true;
		return true;
	}

	bool Sprite::loadFromRenderedText(TTF_Font* font, float* textColor, std::string text) {
		return loadFromRenderedText(font, textColor, (const char*) text.c_str());
	}

	void Sprite::free(void) {

		if (mLoaded) {
			mWidth = 0;
			mHeight = 0;
			mFrame = -1;
			mLoaded = false;
			glDeleteTextures(1, &mTexture);
		}
	}

	void Sprite::frameCrop(float w, float h) {
	
	}

	void Sprite::clearFrames(void) {
		mFrame = 0;
	}

	void Sprite::setFrame(int frame) {
		mFrame = frame;
	}

	int Sprite::getFrame(void) {
		return mFrame;
	}

	void Sprite::render(float x, float y) {

		if (mLoaded) {

			glPushMatrix();

			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, mTexture);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glColor3f(1, 1, 1);

			glBegin(GL_QUADS);
				glTexCoord2f(0, 0); glVertex2f(x, y);
				glTexCoord2f(1, 0); glVertex2f(x + mWidth, y);
				glTexCoord2f(1, 1); glVertex2f(x + mWidth, y + mHeight);
				glTexCoord2f(0, 1); glVertex2f(x, y + mHeight);
			glEnd();

			glDisable(GL_TEXTURE_2D);
			glPopMatrix();
		}
	}

	void Sprite::render(float x, float y, float w, float h) {

		if (mLoaded) {

			glPushMatrix();

			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, mTexture);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glColor3f(1, 1, 1);
			
			glBegin(GL_QUADS);
				glTexCoord2f(0, 0); glVertex2f(x, y);
				glTexCoord2f(1, 0); glVertex2f(x + w, y);
				glTexCoord2f(1, 1); glVertex2f(x + w, y + h);
				glTexCoord2f(0, 1); glVertex2f(x, y + h);
			glEnd();

			glDisable(GL_TEXTURE_2D);
			glPopMatrix();
		}
	}

	void Sprite::render(float src[], float dest[]) {

	}

	void DrawHollowQuad(float x, float y, float w, float h) {
		
		glPushMatrix();
		glBegin(GL_LINE_LOOP);
			glVertex2f(x, y);
			glVertex2f(x + w, y);
			glVertex2f(x + w, y + h);
			glVertex2f(x, y + h);
		glEnd();
		glPopMatrix();
	}

	void DrawFilledQuad(float x, float y, float w, float h) {

		glPushMatrix();
		glBegin(GL_QUADS);
			glVertex2f(x, y);
			glVertex2f(x + w, y);
			glVertex2f(x + w, y + h);
			glVertex2f(x, y + h);
		glEnd();
		glPopMatrix();
	}

	void DrawHollowPolygon(float x, float y, float radius, int angles, const float color[][3]) {

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glBegin(GL_LINE_LOOP);

		float theta, x1, y1;
		uint8_t vertCtr = 0;

		for (int i = 0; i < angles; i++) {

			theta = (2.0f * PI) * (float(i) / float(angles));
			x1 = radius * cosf(theta);
			y1 = radius * sinf(theta);

			glColor4fv(color[vertCtr]);
			glVertex2f(x1 + x, y1 + y);
			vertCtr++;
		}

		glEnd();
		glDisable(GL_BLEND);
	}

	void DrawHollowPolygon(float x, float y, float radius, int angles, const float color[][4]) {

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glBegin(GL_LINE_LOOP);

		float theta, x1, y1;
		uint8_t vertCtr = 0;

		for (int i = 0; i < angles; i++) {

			theta = (2.0f * PI) * (float(i) / float(angles));
			x1 = radius * cosf(theta);
			y1 = radius * sinf(theta);

			glColor4fv(color[vertCtr]);
			glVertex2f(x1 + x, y1 + y);
			vertCtr++;
		}

		glEnd();
		glDisable(GL_BLEND);
	}

	void DrawHollowPolygon(float x, float y, float radius, int angles) {

		glBegin(GL_LINE_LOOP);

		float theta, x1, y1;
		for (int i = 0; i < angles; i++) {

			theta = (2.0f * PI) * (float(i) / float(angles));
			x1 = radius * cosf(theta);
			y1 = radius * sinf(theta);

			glVertex2f(x1 + x, y1 + y);
		}

		glEnd();
	}

	void DrawLifeCircle(float health, float healthMax, float x, float y, float size) {

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glPushMatrix();

		float healthNormal = health / healthMax;
		GLint slices = 30;

		GLUquadric* gluQuad = gluNewQuadric();
		glTranslated(x, y, 0);

		glColor4f(0, 0, 0, .5);
		gluPartialDisk(gluQuad, 0.0, size, slices, 1.0, 0.0, 360.0);

		if (healthNormal < .20) {
			glColor4f(1, 0, 0, 1);
		} else if (healthNormal < .50) {
			glColor4f(1, 1, 0, 1);
		} else {
			glColor4f(0, 1, 0, 1);
		}

		gluPartialDisk(gluQuad, 0.0, size, slices, 1.0, 180.0, 360.0 * healthNormal);

		glColor3f(0, 0, 0);
		gluPartialDisk(gluQuad, size, size + 4, slices, 1.0, 180.0, 360.0);

		gluDeleteQuadric(gluQuad);
		glPopMatrix();
		glDisable(GL_BLEND);
	}

	void SetColor(float r, float g, float b, float a) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(r, g, b, a);
	}

	void SetColor(float r, float g, float b) {
		glDisable(GL_BLEND);
		glColor3f(r, g, b);
	}

	void DrawRoundedQuad(float x, float y, float w, float h, int curve, int pts) {
		
		DrawFilledQuad(x, y, w, h);
		
		DrawFilledQuad(x, y - curve, w, curve);
		DrawFilledQuad(x, y + h, w, curve);
		DrawFilledQuad(x - curve, y, curve, h);
		DrawFilledQuad(x + w, y, curve, h);

		GLUquadric* gluQuad = gluNewQuadric();
		
		glPushMatrix();
			glTranslatef(x + w, y + h, 0);
			gluPartialDisk(gluQuad, 0.0, curve, pts, 1.0, 0.0, 90);
		glPopMatrix();

		glPushMatrix();
			glTranslatef(x, y + h, 0);
			gluPartialDisk(gluQuad, 0.0, curve, pts, 1.0, 0.0, -90);
		glPopMatrix();

		glPushMatrix();
			glTranslatef(x, y, 0);
			glRotatef(180, 0, 0, 1);
			gluPartialDisk(gluQuad, 0.0, curve, pts, 1.0, 0.0, 90);
		glPopMatrix();

		glPushMatrix();
			glTranslatef(x + w, y, 0);
			glRotatef(-90, 0, 0, 1);
			gluPartialDisk(gluQuad, 0.0, curve, pts, 1.0, 0.0, 90);
		glPopMatrix();

		gluDeleteQuadric(gluQuad);
	}
	
	void DrawRoundedQuadEX(float x, float y, float w, float h, int curve, int pts, float colors[][3]) {
		
		SetColor(colors[0][0], colors[0][1], colors[0][2]);
		DrawRoundedQuad(x - curve, y - curve, w + (curve * 2), h + (curve * 2), curve, pts);
		SetColor(colors[1][0], colors[1][1], colors[1][2]);
		DrawRoundedQuad(x, y, w, h, curve, pts);
	}

	void DrawRoundedQuadEX(float x, float y, float w, float h, int curve, int pts, float colors[][4]) {
		
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		SetColor(colors[0][0], colors[0][1], colors[0][2], colors[0][3]);
		DrawRoundedQuad(x - curve, y - curve, w + (curve * 2), h + (curve * 2), curve, pts);
		SetColor(colors[0][0], colors[0][1], colors[0][2], colors[0][3]);
		DrawRoundedQuad(x, y, w, h, curve, pts);

		glDisable(GL_BLEND);
	}
}