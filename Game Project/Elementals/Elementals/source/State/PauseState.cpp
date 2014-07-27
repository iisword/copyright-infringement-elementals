/***********************************************
* Filename:  		PauseState.h
* Date:      		7/15/2014
* Mod. Date: 		7/15/2014
* Mod. Initials:	CP
* Author:    		Charles Peter
* Purpose:   		Controls the pause Behavior of the game.
************************************************/

#include "GameState.h"
#include "PauseState.h"
#include "MainMenu.h"

#include "..\Input\Input.h"

CPauseState::CPauseState(void)
{
	SetState(PAUSE);
	m_pPrevState = nullptr;
}

CPauseState::CPauseState(CBaseState* prev)
{
	SetState(PAUSE);
	m_pPrevState = prev;
}

CPauseState::~CPauseState(void)
{
	(*m_pPauseBG->GetDataBuffer())->Release();
	delete m_pPauseBG;
}

void CPauseState::Init(CRenderer* r, CText* font)
{
	m_pR = r;

	m_pPauseBG = new D2DObject(L"assets/BG/alphaBG.dds", 256.0f, 256.0f);
	m_pR = r;
	m_pR->AddD2D(m_pPauseBG);

	m_pFont = font;

	m_eCurrS = msReturn;
}

void CPauseState::Update(float dTime)
{
	CInput::GetInstance()->Reset();

	if(CInput::GetInstance()->IsKeyOnce('W'))
	{
		if(m_eCurrS > msReturn)
		{
			m_eCurrS--;
		}
		else
		{
			m_eCurrS = msExit;
		}
	}
	else if(CInput::GetInstance()->IsKeyOnce('S'))
	{
		if(m_eCurrS < msExit)
		{
			m_eCurrS++;
		}
		else
		{
			m_eCurrS = msReturn;
		}
	}
	else if(CInput::GetInstance()->IsKeyOnce(VK_RETURN))
	{
		if(m_eCurrS == msReturn)
		{
			//CMessageSystem::GetInstance()->SendEvent(UN_PAUSE,HIGH);
			if(m_pPrevState->GetID() == GAME)
			{
				((CGameState*)m_pPrevState)->SetPause(false);
			}
		}
		else if(m_eCurrS == msExit)
		{
			CBaseState* temp = new CMainMenu();
			temp->Init(m_pR, m_pFont);
			CEventSystem::GetInstance()->SendEvent(CHANGE_STATE, HIGH, temp);
		}
	}
	else if(CInput::GetInstance()->IsKeyOnce(VK_ESCAPE))
	{
		//CMessageSystem::GetInstance()->SendEvent(UN_PAUSE,HIGH);
		if(m_pPrevState->GetID() == GAME)
		{
			//((CGameState*)m_pPrevState)->SetPause(false);
		}
	}

}

void CPauseState::Render(void)
{
	//m_pR->Render2D(m_pPauseBG, XMFLOAT4(180.0f, 45.0f, 436.0f, 301.0f), XMFLOAT4(0.0f, 0.0f, 256.0f, 256.0f));

	string Pause = "Pause Menu";
	m_pFont->WriteText(XMFLOAT2(200.0f, 50.0f), 1.0f, Pause, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));


	Pause = "Return";
	if(m_eCurrS == msReturn)
	{
		m_pFont->WriteText(XMFLOAT2(240.0f, 150.0f), 1.0f, Pause, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	}
	else
	{
		m_pFont->WriteText(XMFLOAT2(220.0f, 150.0f), 1.0f, Pause, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	}

	Pause = "Options";
	if(m_eCurrS == msOptions)
	{
		m_pFont->WriteText(XMFLOAT2(240.0f, 175.0f), 1.0f, Pause, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	}
	else
	{
		m_pFont->WriteText(XMFLOAT2(220.0f, 175.0f), 1.0f, Pause, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	}


	Pause = "Exit";
	if(m_eCurrS == msExit)
	{
		m_pFont->WriteText(XMFLOAT2(240.0f, 200.0f), 1.0f, Pause, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	}
	else
	{
		m_pFont->WriteText(XMFLOAT2(220.0f, 200.0f), 1.0f, Pause, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	}

}
