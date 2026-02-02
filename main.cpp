#include <SDL3/SDL_main.h>
#include "CGameEngine.h"
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL3/SDL_opengles2.h>
#else
#include <SDL3/SDL_opengl.h>
#endif

int main(int, char**)
{
	CGameEngine& GameEngine = CGameEngine::GetInstance();

	if (!GameEngine.Init())
		return -1;

	GameEngine.Loop();
	GameEngine.Shutdown();
    
	return 0;
}
