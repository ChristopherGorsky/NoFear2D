#include "Engine.hpp"

Engine::Engine() {

	m_window = NULL;
	m_devMode = false;
	m_width = 640;
	m_height = 480;

	srand(time(NULL));
}

Engine::~Engine() {
	SDL_Quit();
}

bool Engine::Initialize(const char* title, int width, int height, bool fullScreen) {

	#ifdef NDEBUG
	m_devMode = false;
	#else
	m_devMode = true;
	#endif

	Debug& dbg = Debug::GetInstance();
	dbg.Print("Initializing Engine:");

	dbg.Print("Initializing video subsystem");
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		dbg.Print("Failed to initilize video subsystem");
		dbg.Print(SDL_GetError());
		return false;
	}

	dbg.Print("Initializing audio subsystem");
	if (SDL_Init(SDL_INIT_AUDIO) < 0) {
		dbg.Print("Failed to initilize audio subsystem");
		dbg.Print(SDL_GetError());
		return false;
	}

	dbg.Print("Initializing timer subsystem");
	if (SDL_Init(SDL_INIT_TIMER) < 0) {
		dbg.Print("Failed to initilize timer subsystem");
		dbg.Print(SDL_GetError());
		return false;
	}

	dbg.Print("Initializing event subsystem");
	if (SDL_Init(SDL_INIT_EVENTS) < 0) {
		dbg.Print("Failed to initilize events subsystem");
		dbg.Print(SDL_GetError());
		return false;
	}

	dbg.Print("Initializing SDL_IMG [PNG,JPG] subsystem");
	if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) < 0) {
		dbg.Print("Failed to initilize SDL_IMG");
		dbg.Print(IMG_GetError());
		return false;
	}

	dbg.Print("Initializing SDL_TTF subsystem");
	if (TTF_Init() < 0) {
		dbg.Print("Failed to initilize SDL_TTF");
		dbg.Print(TTF_GetError());
		return false;
	}

	TTF_Init();

	dbg.Print("Creating application window"); 
	if (fullScreen)
		m_window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN);
	else
		m_window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);

	if (m_window == NULL) {
		dbg.Print("Failed to create game window");
		return false;
	}

	m_context = SDL_GL_CreateContext(m_window);
	glewInit();

	dbg.Print("Enabling Double Buffer Mode");
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetSwapInterval(1);

	std::string applicationTitle;
	#ifdef NDEBUG
	applicationTitle = "Release Build: ";	
	#else
	applicationTitle = "Debug Build: ";
	#endif

	applicationTitle += __DATE__;
	applicationTitle += " - ";
	applicationTitle += __TIME__;

	if (m_devMode) {
		SDL_SetWindowTitle(m_window, applicationTitle.c_str());
	}

	dbg.Print("Engine initialization complete");
	Core test;
	test.run(m_window, m_luaState);

	return true;
}

bool Engine::RunConfigAndInitilize(const char* file) {
	
	Debug& dbg = Debug::GetInstance();
	
	m_luaState = lua_createState();
	if (m_luaState == NULL) {
		dbg.Print("Failed to create Lua state.");
		return false;
	}

	if (luaL_dofile(m_luaState, file) == 0) {
		
		lua_getglobal(m_luaState, "Window_Width");
		lua_getglobal(m_luaState, "Window_Height");
		lua_getglobal(m_luaState, "Fullscreen");
		lua_getglobal(m_luaState, "Window_Title");

		int width				= lua_tointeger(m_luaState, 1);
		int height				= lua_tointeger(m_luaState, 2);
		bool fullscreen			= bool(lua_toboolean(m_luaState, 3));
		const char* title		= lua_tostring(m_luaState, 4);

		lua_pop(m_luaState, 4);

		m_width = width;
		m_height = height;

		dbg.Print("Resolution: %ix%i Fullscreen: %s", m_width, m_height, ((fullscreen) ? "Enabled" : "Disabled"));
		return Initialize(title, width, height, fullscreen);

	}
   
	dbg.Print("Failed to read config file, applying defaults");
	
	dbg.Print("Error: %s", lua_tostring(m_luaState, 1));
	lua_pop(m_luaState, 1);

    dbg.Print("Resolution: 640x480 Fullscreen: Disabled");
	return Initialize("Application", 640, 480, false);
}