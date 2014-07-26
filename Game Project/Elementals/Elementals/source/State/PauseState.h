/***********************************************
* Filename:  		PauseState.h
* Date:      		7/15/2014
* Mod. Date: 		7/15/2014
* Mod. Initials:	CP
* Author:    		Charles Peter
* Purpose:   		Controls the pause Behavior of the game.
************************************************/

#ifndef PAUSE_STATE
#define PAUSE_STATE

#include "BaseState.h"
#include "..\Renderer\Renderer.h"
#include "..\Text\Text.h"
#include "..\Message\EventSystem.h"

class CPauseState : public CBaseState
{
private:
	enum selection{ msReturn, msOptions, msExit};
	int m_eCurrS;

	CRenderer* m_pR;
	D2Dobject* m_pPauseBG;

	CText* m_pFont;

	CBaseState* m_pPrevState;

public:
	CPauseState(void);
	CPauseState(CBaseState* prev);
	~CPauseState(void);

	void Init(CRenderer* r, CText* font);
	void Update(float dTime);
	void Render(void);
};
#endif
