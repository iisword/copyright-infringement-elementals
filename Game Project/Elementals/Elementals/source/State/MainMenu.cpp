/***********************************************
* Filename:  		MainMenu.cpp
* Date:      		7/15/2014
* Mod. Date: 		7/15/2014
* Mod. Initials:	CP
* Author:    		Charles Peter
* Purpose:   		Controls the Main Menu.
************************************************/

#include "GameState.h"
#include "MainMenu.h"
#include "TestArea.h"
#include "..\Input\Input.h"

CMainMenu::CMainMenu(void)
{
	SetState(MAIN_MENU);
}


CMainMenu::~CMainMenu(void)
{
	(*m_pMenuBG->GetDataBuffer())->Release();
	delete m_pMenuBG;
}

void CMainMenu::Init(CRenderer* r, CText* font)
{
	//Init and Load the stuff for the Main Menu
	m_pMenuBG = new D2Dobject(L"assets/BG/tunnelBG.dds", 1920.0f, 1080.0f);
	m_pR = r;
	m_pR->AddD2D(m_pMenuBG);

	m_pFont = font;
	m_eCurrS = msJoin;
}


void CMainMenu::Update(float dTime)
{
	//Do menu things
//	if(CInput::GetInstance()->IsW() )
	if(CInput::GetInstance()->IsKeyOnce('W') )
	{
		if(m_eCurrS > msJoin)
		{
			m_eCurrS--;
		}
		else
		{
			m_eCurrS = msExit;
		}
	}
	else if(CInput::GetInstance()->IsS())
	{
		if(m_eCurrS < msExit)
		{
			m_eCurrS++;
		}
		else
		{
			m_eCurrS = msJoin;
		}
	}
	else if (CInput::GetInstance()->IsEnter() || (CInput::GetInstance()->GetMousePressedL() && CInput::GetInstance()->GetMouseX() > 300 ))
	{
		if(m_eCurrS == msJoin)
		{
			CBaseState* pNState = new CGameState();
			pNState->Init(m_pR, m_pFont);
			CEventSystem::GetInstance()->SendEvent(CHANGE_STATE,HIGH, pNState);
		}
		else if(m_eCurrS == msTest)
		{
			CBaseState* pNState = new CTestArea();
			pNState->Init(m_pR, m_pFont);
			CEventSystem::GetInstance()->SendEvent(CHANGE_STATE,HIGH, pNState);
		}
		else if(m_eCurrS == msExit)
		{
			PostQuitMessage( 0 );
		}

	}
	else if(CInput::GetInstance()->IsKeyOnce(VK_ESCAPE))
	{
		PostQuitMessage( 0 );
	}
	
}

void CMainMenu::Render(void)
{
	//m_pR->PreRender();
	//Draw the Menu and stuff
	m_pR->Render2D(m_pMenuBG, XMFLOAT4(0.0f, 0.0f, 1280.0f, 768.0f), XMFLOAT4(0.0f, 0.0f, 1920.0f, 1080.0f));
	string main = "Main Menu";
	m_pFont->WriteText(XMFLOAT2(200.0f, 100.0f), 1.0f, main, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	main = "Join Game";
	if(m_eCurrS == msJoin)
	{
		m_pFont->WriteText(XMFLOAT2(240.0f, 150.0f), 1.0f, main, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	}
	else
	{
		m_pFont->WriteText(XMFLOAT2(220.0f, 150.0f), 1.0f, main, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	}

	main = "Credits";
	if(m_eCurrS == msCredit)
	{
		m_pFont->WriteText(XMFLOAT2(240.0f, 175.0f), 1.0f, main, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	}
	else
	{
		m_pFont->WriteText(XMFLOAT2(220.0f, 175.0f), 1.0f, main, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	}

	main = "TestArea";
	if(m_eCurrS == msTest)
	{
		m_pFont->WriteText(XMFLOAT2(240.0f, 200.0f), 1.0f, main, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	}
	else
	{
		m_pFont->WriteText(XMFLOAT2(220.0f, 200.0f), 1.0f, main, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	}

	main = "Exit";
	if(m_eCurrS == msExit)
	{
		m_pFont->WriteText(XMFLOAT2(240.0f, 225.0f), 1.0f, main, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	}
	else
	{
		m_pFont->WriteText(XMFLOAT2(220.0f, 225.0f), 1.0f, main, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	}

	//m_pR->PostRender();
}
