#include "EventHandler.hpp"


EventHandler::EventHandler(){
	m_eventRunning = false;
	m_eventLoaded = false;
	ResourceManager& rm = ResourceManager::GetInstance();
	m_textFont = rm.loadFont("default22", "data/default.ttf", DRAW_TEXT_SIZE);
	m_titleFont = rm.loadFont("default26", "data/default.ttf", DRAW_TEXT_SIZE + 10);
}


EventHandler::~EventHandler(){
}

void EventHandler::playEvent(void) {

	if (m_eventLoaded) {
		m_eventRunning = true;
		m_currentBlock = m_events[0];
	}
}

void EventHandler::events(const SDL_Event& e) {

	if (m_eventRunning) {
		if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {

			switch (e.key.keysym.sym) {
			case SDLK_w: incrementSelection(); break;
			case SDLK_s: decrementSelection(); break;
			case SDLK_RETURN: next(); break;
			}
		}
	}
}

bool EventHandler::loadEvent(const char* file) {

	Debug& dbg = Debug::GetInstance();
	dbg.Print("Loading Event: %s", file);

	std::ifstream reader(file);
	if (!reader.good()) {
		dbg.Print("Error - Failed to read event file");
		return false;
	}

	std::string line, text, dest;
	size_t first, last;

	int txtLineCtr = -1;
	int index = -1;

	while (reader.good()) {

		std::getline(reader, line);
		switch (line[0]) {

		case '@':

			index++;
			txtLineCtr = -1;
			m_events.push_back(new Event());

			first = line.find("@");
			m_events[index]->name = line.substr(first + 1);
			m_events[index]->type = EBLOCK_TEXT;
			break;

		case '<':

			first = line.find('<');
			last = line.find('>');
			text = line.substr(first + 1, (last - 1) - first);

			first = line.find('>');
			dest = line.substr(first + 1, line.size());

			m_events[index]->branches.push_back(new Branch(dest, text));
			m_events[index]->type = EBLOCK_BRANCH;
			break;

		case '.':
			first = line.find('.');
			m_events[index]->retn = line.substr(first + 1);
			m_events[index]->type = EBLOCK_END;
			break;

		case '#':
			//This is just so comments are not read as dialog
			break;

		case '>':
			first = line.find(">");
			m_events[index]->jump = line.substr(first + 1);
			m_events[index]->type = EBLOCK_JUMP;
			break;

		default:

			if (txtLineCtr > -1) {
				m_events[index]->lines[txtLineCtr] = line;
			} else {
				m_events[index]->title = line;
			}

			txtLineCtr++;
			break;
		}
	}

	m_eventLoaded = true;
	reader.close();
	return true;
}

void EventHandler::next(void) {

	std::string temp;
	Event* nextBlock;
	switch (m_currentBlock->type) {

	case EBLOCK_TEXT:
		if (m_eventIndex + 1 < m_events.size()) {
			m_currentBlock = m_events[++m_eventIndex];
		}
		break;
	case EBLOCK_BRANCH:
		temp = m_currentBlock->branches[m_branchIndex]->dest;
		nextBlock = getEvent(temp);
		if (nextBlock != NULL) {
			m_currentBlock = nextBlock;
		}
		break;
	case EBLOCK_JUMP:
		nextBlock = getEvent(m_currentBlock->jump);
		if (nextBlock != NULL) {
			m_currentBlock = nextBlock;
		}
		break;
	case EBLOCK_END:
		m_return = m_currentBlock->retn;
		m_currentBlock = NULL;
		m_eventLoaded = false;
		m_eventRunning = false;
		m_events.clear();
		break;
	}

	for (int i = 0; i < MAXLINES; i++) {
		m_drawText[i].clear();
	}

	m_drawLine = 0;
	m_drawChar = 0;
}

Event * EventHandler::getEvent(std::string name) {

	for (int i = 0; i < m_events.size(); i++) {
		if (m_events[i]->name.compare(name) == 0) {
			return m_events[i];
		}
	}

	return NULL;
}

void EventHandler::incrementSelection(void) {

	if (m_branchIndex + 1 < m_currentBlock->branches.size()) {
		m_branchIndex++;
	} else {
		m_branchIndex = 0;
	}
}

void EventHandler::decrementSelection(void) {

	if (m_branchIndex - 1 >= 0) {
		m_branchIndex--;
	} else {
		m_branchIndex = m_currentBlock->branches.size() - 1;
	}
}

void EventHandler::drawTextBlock(const Dimension<int>& screen) {

	if (m_drawChar < m_currentBlock->lines[m_drawLine].size()) {
		char c = m_currentBlock->lines[m_drawLine].at(m_drawChar);
		m_drawText[m_drawLine] = m_drawText[m_drawLine] + c;
		m_drawChar++;
	} else {
		if (m_drawLine < MAXLINES - 1) {
			m_drawChar = 0;
			m_drawLine++;
		}
	}

	if (m_drawCtr < 2) {
		m_drawCtr++;
	} else {
		m_drawCtr = 0;
	}

	int x1 = (screen.width / 2) - (DRAW_WIDTH / 2);
	int y1 = (screen.height - DRAW_HEIGHT) - DRAW_SCREEN_PADDING_Y;

	SDL_Rect innerBox;
	innerBox.x = x1;
	innerBox.y = y1;
	innerBox.w = DRAW_WIDTH;
	innerBox.h = DRAW_HEIGHT;

	float rectColor[][3] = { {1,1,1},{ 0,0,0 } };
	Graphics2D::DrawRoundedQuadEX(x1, y1, DRAW_WIDTH, DRAW_HEIGHT, 4, 10, rectColor);

	int x2 = x1 + DRAW_TEXT_OFFSET + 4;
	int y2 = y1;

	float white[] = { 1,1,1,1 };
	m_drawTexture.loadFromRenderedText(m_titleFont, white, m_currentBlock->title);
	m_drawTexture.render(x2, y2 - 2);

	y2 += DRAW_TEXT_SIZE + 14;
	Graphics2D::DrawFilledQuad(x1, y2 - 4, DRAW_WIDTH, 2);

	for (int i = 0; i < MAXLINES; i++) {

		m_drawTexture.loadFromRenderedText(m_textFont, white, m_drawText[i]);
		m_drawTexture.render(x2, y2);
		y2 += DRAW_TEXT_SIZE;
	}
}

void EventHandler::drawSwitchBlock(const Dimension<int>& screen) {

	const int x1 = (screen.width / 2) - (DRAW_WIDTH / 2);
	const int y1 = (screen.height - DRAW_HEIGHT) - DRAW_SCREEN_PADDING_Y;
	
	Graphics2D::SetColor(0, 0, 0);
	
	float rectColor[][3] = { { 1,1,1 },{ 0,0,0 } };
	Graphics2D::DrawRoundedQuadEX(x1, y1, DRAW_WIDTH, DRAW_HEIGHT, 4, 10, rectColor);

	int x2 = x1 + DRAW_TEXT_OFFSET + 4;
	int y2 = y1;
	
	float white[] = { 1,1,1,1 };
	float selectColor[] = { 1,1,0,1 };
	m_drawTexture.loadFromRenderedText(m_titleFont, white, m_currentBlock->title);
	m_drawTexture.render(x2, y2);

	y2 += DRAW_TEXT_SIZE;

	for (int i = 0; i < m_currentBlock->branches.size(); i++) {

		if (i == m_branchIndex) {
			m_drawTexture.loadFromRenderedText(m_textFont, selectColor, "-> " + m_currentBlock->branches[i]->text);
			m_drawTexture.render(x2, y2 += DRAW_TEXT_SIZE);
		} else {
			m_drawTexture.loadFromRenderedText(m_textFont, white, m_currentBlock->branches[i]->text);
			m_drawTexture.render(x2, y2 += DRAW_TEXT_SIZE);
		}
	}
}

std::string EventHandler::getResult(void) {

	std::string result = m_return;
	m_return.clear();
	return result;
}

void EventHandler::render(const Dimension<int>& screen) {
	
	if(m_eventRunning) {
		if (m_currentBlock != NULL) {
			if (m_currentBlock->type == EBLOCK_BRANCH) {
				drawSwitchBlock(screen);
			} else {
				drawTextBlock(screen);
			}
		}
	}
}
