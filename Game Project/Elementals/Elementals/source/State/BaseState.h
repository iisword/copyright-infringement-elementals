/***********************************************
* Filename:  		BaseState.h
* Date:      		7/15/2014
* Mod. Date: 		7/15/2014
* Mod. Initials:	CP
* Author:    		Charles Peter
* Purpose:   		Parent of anyother state in the Game.
************************************************/

#ifndef BASE_STATE
#define BASE_STATE

#include "..\Renderer\Renderer.h"
#include "..\Text\Text.h"

enum state { MAIN_MENU, GAME, CREDITS, LOBBY, LOAD, OPTIONS, PAUSE, TEST_STATE, WIN_STATE, INVALID_STATE};

class CBaseState
{
private:
	state m_eID;

protected:
	void SetState(state nId) { m_eID = nId; }
public:
	CBaseState(void);
	virtual ~CBaseState(void);

	const int GetID(void) { return m_eID; }

	virtual void Init(CRenderer* r, CText* font) = 0;
	virtual void Update(float dTime) = 0;
	virtual void Render(void) = 0;


};
#endif
