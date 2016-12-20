#include <vector>
#include <string>
#include <string.h>
#include <fstream>
#include "Debug.hpp"
#include "SDL2.hpp"
#include "ResourceManager.hpp"
#include "Graphics.hpp"

#define MAXLINES	4
#define KEY		0
#define VALUE	1
#define EBLOCK_TEXT		0
#define EBLOCK_BRANCH	1
#define EBLOCK_JUMP		2
#define EBLOCK_END		3
#pragma once

struct Branch {
	Branch(std::string dest, std::string text) {
		this->dest = dest;
		this->text = text;
	}
	std::string dest, text;
};

struct Event {
	std::string lines[MAXLINES];
	std::vector<Branch*> branches;
	int type;
	std::string retn;
	std::string jump;
	std::string title;
	std::string name;
};

class EventHandler {

public:
	EventHandler();
	~EventHandler();

	void events(const SDL_Event& e);

	void playEvent(void);
	bool loadEvent(const char* file);
	std::string getResult(void);
	bool eventRunning(void) const { return m_eventRunning; }

	void render(const Dimension<int>& screen);
private:

	void next(void);
	Event* getEvent(std::string name);
	void incrementSelection(void);
	void decrementSelection(void);

	void drawTextBlock(const Dimension<int> &screen);
	void drawSwitchBlock(const Dimension<int>& screen);

	std::vector<Event*> m_events;
	Event* m_currentBlock;
	bool m_eventLoaded;
	bool m_eventRunning;
	int m_eventIndex;
	int m_branchIndex;
	std::string m_return;

	const int DRAW_WIDTH = 650;
	const int DRAW_HEIGHT = 128;
	const int DRAW_TEXT_OFFSET = 8;
	const int DRAW_SCREEN_PADDING_X = 50;
	const int DRAW_SCREEN_PADDING_Y = 100;
	const int DRAW_ARC = 22;
	const int DRAW_TEXT_SIZE = 22;

	TTF_Font* m_textFont;
	TTF_Font* m_titleFont;

	std::string m_drawText[MAXLINES];
	int m_drawLine;
	int m_drawChar;
	int m_drawCtr;
	Sprite m_drawTexture;
};