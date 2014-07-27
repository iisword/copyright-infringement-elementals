#include "Game.h"
#include "..\Message\EventSystem.h"
#include "..\Utilities\Timer.h"
CGame::CGame(HINSTANCE hinst, WNDPROC proc)
{
	renderer.Init(hinst, proc);

	stateManager.Initialize(&renderer);
}


CGame::~CGame(void)
{
	renderer.ShutDown();
	CEventSystem::GetInstance()->Terminate();
}

void CGame::Run()
{
	CEventSystem::GetInstance()->ProcessEvents();
	stateManager.Update((float)CTimer::GetInstance()->Delta());
	CInput::GetInstance()->Reset();
	renderer.Update();
}




