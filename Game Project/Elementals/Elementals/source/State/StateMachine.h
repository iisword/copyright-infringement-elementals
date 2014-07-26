/***********************************************
* Filename:  		StateMachine.h
* Date:      		7/15/2014
* Mod. Date: 		7/15/2014
* Mod. Initials:	CP
* Author:    		Charles Peter
* Purpose:   		Controls the currentState of the Game.
************************************************/
#ifndef STATE_MACHINE
#define STATE_MACHINE

#include "BaseState.h"
#include "..\Renderer\Renderer.h"
#include "..\Message\Listener.h"
#include "..\Message\Event.h"
#include "..\Message\EventSystem.h"
#include "..\Text\Text.h"

class CStateMachine : public IListener
{
private:
	CBaseState* m_cCurrState;
	CRenderer* m_pR;

	bool ChangeState(CBaseState* nState);
	CEventSystem* m_pMS;

	CText* m_pFont;

public:
	CStateMachine(void);
	~CStateMachine(void);

	//Starts Game to Main Menu;
	void Initialize(CRenderer* renderer);

	const CBaseState* GetState(void) { return m_cCurrState; }

	void ProcessEvent(CEvent * Event);
	void Update(float dTime);
};
#endif
