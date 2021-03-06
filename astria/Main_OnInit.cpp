#include "Main.h"

bool CMain::OnInit()
{
	//Load parameters
	CParams::Load(CParams::File);

	//Initialize SDL
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL initiation error", SDL_GetError(), NULL);
		return false;
	}

#if _DEBUG
	//Enable console output
	//freopen("CON", "w", stdout); // redirects stdout
	//freopen("CON", "w", stderr); // redirects stderr
#endif

	//Initialize SDL_image for texture and image loading
	if(IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) != (IMG_INIT_PNG | IMG_INIT_JPG))
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL_image initiation error", IMG_GetError(), NULL);
		return false;
	}
	
	//Initialize SDL_ttf for font 
	if (TTF_Init() < 0)
	{
		MessageBox(NULL, TTF_GetError(), "SDL_ttf initiation error", MB_ICONERROR);
		return false;
	}

	//Set OpenGL 3.3 core
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, OPENGL_MAJOR_VERSION);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, OPENGL_MINOR_VERSION);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);				//Enable core profil
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);	//Backward compatibility
	
	//Create the main window
	Window_Main = SDL_CreateWindow(CParams::WindowName, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, CParams::WindowWidth, CParams::WindowHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if(Window_Main == NULL)
	{
		MessageBox(NULL, SDL_GetError(), "Window creation error", MB_ICONERROR);
		return false;
	}
	//Grab its dimension for future use
	SDL_GetWindowSize(Window_Main, &window_width, &window_height);

	//Set window icon
	if ((Surf_Icon = IMG_Load(CParams::IconImage)) == NULL)
	{
		Log("Unable to load window icon");
	}
	SDL_SetWindowIcon(Window_Main, Surf_Icon);

	//Setup the renderer
	if ((Renderer = SDL_CreateRenderer(Window_Main, -1, SDL_RENDERER_ACCELERATED)) == NULL)
	{
		Log("Unable to create renderer");
		return false;
	}
	SDL_SetRenderDrawColor(Renderer, 0x00, 0x00, 0x10, 0xFF);

	//Create OpenGL context
	glContext = SDL_GL_CreateContext(Window_Main);
	if(glContext == NULL)
	{
		MessageBox(NULL, SDL_GetError(), "OpenGL context creation failed!", MB_ICONERROR);
		return false;
	}

	//Initialize GLEW
	glewExperimental = GL_TRUE;
	GLenum glewInitStatus = glewInit();
	if(glewInitStatus != GLEW_OK)
	{
		MessageBox(NULL, (char*)(glewGetErrorString(glewInitStatus)), "GLEW initialization failed!", MB_ICONERROR);
		return false;
	}

	//Enable Vsync
	if (SDL_GL_SetSwapInterval(1) < 0)
	{
		MessageBox(NULL, SDL_GetError(), "Warning: Unable to set VSync!", MB_ICONWARNING);
	}

	CAppStateManager::SetActiveAppState(APPSTATE_INTRO);

	return true;
}