/***********************************************
* Filename:  		MainMenu.h
* Date:      		7/15/2014
* Mod. Date: 		7/15/2014
* Mod. Initials:	CP
* Author:    		Charles Peter
* Purpose:   		Controls the Main Menu.
************************************************/


#ifndef MAIN_MENU_STATE
#define MAIN_MENU_STATE

#include "BaseState.h"
#include "..\Renderer\Renderer.h"
#include "..\Text\Text.h"
#include "..\Message\EventSystem.h"

class CMainMenu : public CBaseState
{
private:
	enum selection{ msJoin, msCredit, msTest, msExit};

	D2Dobject* m_pMenuBG;
	CRenderer* m_pR;

	CText* m_pFont;
	int m_eCurrS;


public:
	CMainMenu(void);
	~CMainMenu(void);

	void Init(CRenderer* r, CText* font);
	void Update(float dTime);
	void Render(void);
};
#endif
