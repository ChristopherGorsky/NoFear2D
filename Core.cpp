#include "Core.hpp"

Core::Core(){
	m_running = false;
}


Core::~Core(){
}

void Core::run(SDL_Window* wnd, lua_State* l) {
	
	m_running = true;
	Dimension<int> screen;
	SDL_GetWindowSize(wnd, &screen.width, &screen.height);

	SceneHandler sceneHandler;
	sceneHandler.addScene(new GameScene("test"));
	sceneHandler.setScene("test");

	int bRunning = 1;
	int frameRate = 60;
	int frameMs = 1000 / frameRate; //calculate the length of each frame
	float delta = 0;
	int startMs, endMs;

	SDL_Renderer* renderer = SDL_CreateRenderer(wnd, -1, SDL_RENDERER_ACCELERATED);
	ResourceManager& rsm = ResourceManager::GetInstance();
	TTF_Font* sysFont = rsm.loadFont("test18", "data/test.ttf", 18);

	SDL_Event evnt;
	while (m_running) {

		startMs = SDL_GetTicks();

		//events
		while (SDL_PollEvent(&evnt)) {

			if (evnt.type == SDL_QUIT) {
				m_running = false;
			} else {
				
				if (evnt.type == SDL_KEYDOWN && evnt.key.repeat == 0) {
					switch (evnt.key.keysym.sym) {
					case SDLK_TAB:
						lua_stackDump(l);
						break;
					}
				}
				sceneHandler.events(delta, screen, evnt);
			}
		}

		// Anything put inside this while loop will be executed 60 times per second
		sceneHandler.update(delta, screen);

		glMatrixMode(GL_PROJECTION);
		glClear(GL_COLOR_BUFFER_BIT);

		glLoadIdentity();
		glOrtho(0.0f, screen.width, screen.height, 0.0f, -1.0f, 1.0f);
		glViewport(0, 0, screen.width, screen.height);

		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		//BEGIN DRAWING
		sceneHandler.render(delta, screen);
		//END DRAWING
		SDL_GL_SwapWindow(wnd);

		endMs = SDL_GetTicks();
	}
}

static int lua_debugPrint(lua_State* l) {
	Debug& dbg = Debug::GetInstance();
	dbg.Print("-- Lua Stack Dump --");
}