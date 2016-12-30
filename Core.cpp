#include "Core.hpp"

Core::Core(){
	m_running = false;
}


Core::~Core(){
}

void Core::run(SDL_Window* wnd, lua_State* l) {
	
	Debug& dbg = Debug::GetInstance();

	Dimension<int> screen;
	SDL_GetWindowSize(wnd, &screen.width, &screen.height);

	int bRunning = 1;
	int frameRate = 60;
	int frameMs = 1000 / frameRate; //calculate the length of each frame
	float delta = 0;
	int startMs, endMs;

	ResourceManager& rsm = ResourceManager::GetInstance();
	TTF_Font* sysFont = rsm.loadFont("test18", "data/test.ttf", SYS_FONT_SIZE);

	bool consoleOpen = false;
	bool consoleVisible = false;
	std::string command = "";
	Graphics2D::Sprite inputConsole;

	registerLuaCFunctions(l);

	SDL_Event evnt;
	m_running = true;
	while (m_running) {

		startMs = SDL_GetTicks();

		//events
		while (SDL_PollEvent(&evnt)) {

			if (!consoleOpen) {
				if (evnt.type == SDL_QUIT) {
					m_running = false;
				} else {

					if (evnt.type == SDL_KEYDOWN && evnt.key.repeat == 0) {
						
						switch (evnt.key.keysym.sym) {
						
						case SDLK_F1:
							lua_stackDump(l);
							break;

						case SDLK_TAB:
							consoleOpen = true;
							SDL_StartTextInput();
							break;
						}
					}
				}

			} else {

				switch (evnt.type) {

				case SDL_TEXTINPUT:
					command += evnt.text.text;
					break;

				case SDL_KEYDOWN:
					switch (evnt.key.keysym.sym) {

					case SDLK_TAB:
						SDL_StopTextInput();
						consoleOpen = false;
						consoleVisible = true;
						break;

					case SDLK_BACKSPACE:
						if (!command.empty())
							command.pop_back();
						break;

					case SDLK_RETURN:

						if (luaL_dostring(l, (const char*) command.c_str()) != 0) {
							dbg.Print("%s", lua_tostring(l, -1));
							lua_pop(l, -1);
						}

						command.clear();
						break;
					}
					break;
				}
			}
		}

		glMatrixMode(GL_PROJECTION);
		glClear(GL_COLOR_BUFFER_BIT);

		glLoadIdentity();
		glOrtho(0.0f, screen.width, screen.height, 0.0f, -1.0f, 1.0f);
		glViewport(0, 0, screen.width, screen.height);

		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		if (consoleOpen) {
			if (inputConsole.loadFromRenderedText(sysFont, Graphics2D::WHITE, "] %s", (const char*)command.c_str())) {
				inputConsole.render(0, screen.height - (SYS_FONT_SIZE + 4));
				inputConsole.free();
			}
		}

		renderLogConsole(screen, sysFont);
		SDL_GL_SwapWindow(wnd);
		endMs = SDL_GetTicks();
	}
}

void Core::renderLogConsole(const Dimension<int>& screen, TTF_Font* sysFont) {

	Debug& dbg = Debug::GetInstance();
	int logLen = dbg.getLog().size();

	if (logLen > MAX_CONSOLE_LINES)
		logLen = MAX_CONSOLE_LINES;

	float padding = 2;
	float y = SYS_FONT_SIZE;
	float w = 640;
	float x = (screen.width / 2) - (w / 2);
	float h = ((logLen + 1) * SYS_FONT_SIZE) + padding;

	//background box
	glPushMatrix();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBegin(GL_QUADS);
	glColor4f(0, 0, 1, .8);
		glVertex2f(x, 0);
		glVertex2f(x + w, 0);
		glColor4f(0, 0, 0, .8);
		glVertex2f(x + w, SYS_FONT_SIZE);
		glVertex2f(x, SYS_FONT_SIZE);
	glEnd();

	glDisable(GL_BLEND);
	glPopMatrix();

	//background box
	glPushMatrix();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColor4f(0, 0, 0, .8);
	glBegin(GL_QUADS);
		glVertex2f(x, y);
		glVertex2f(x + w, y);
		glVertex2f(x + w, y + h);
		glVertex2f(x, y + h);
	glEnd();

	glDisable(GL_BLEND);
	glPopMatrix();

	Sprite text;
	float x2 = x + padding;
	float y2 = y + 2;

	std::string applicationTitle;

#ifdef _WIN32
	applicationTitle = IsDebuggerPresent()? "Release Build: " : "Debug Build: ";
#endif

	applicationTitle += __DATE__;
	applicationTitle += " - ";
	applicationTitle += __TIME__;

	text.loadFromRenderedText(sysFont, Graphics2D::WHITE, "%s", (const char*)applicationTitle.c_str());
	text.render(x + (w / 2), 0, true);

	for (int i = logLen; i > 0; i--) {
		text.loadFromRenderedText(sysFont, Graphics2D::WHITE, "%s", dbg.getLog()[dbg.getLog().size() - i].c_str());
		text.render(x2, y2);
		y2 += SYS_FONT_SIZE + 2;
	}

	text.free();
}

void Core::registerLuaCFunctions(lua_State* l) {
	lua_pushcfunction(l, lua_debugPrint);
	lua_setglobal(l, "DbgPrint");
}

static int lua_debugPrint(lua_State* l) {
	
	Debug& dbg = Debug::GetInstance();
	const char* str = luaL_checkstring(l, 1);
	dbg.Print("%s", str);

	return 0;
}