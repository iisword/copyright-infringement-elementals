
#ifndef GAME_LOOP
#define GAME_LOOP


#include "../Object/Player.h"
#include "..\Renderer\Renderer.h"
#include "..\State\StateMachine.h"
#include "../Input/Input.h"
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

class CGame
{

	CRenderer renderer;
	CStateMachine stateManager;

public:

	CGame(HINSTANCE hinst, WNDPROC proc);
	~CGame(void);
	void Run();
};
#endif
