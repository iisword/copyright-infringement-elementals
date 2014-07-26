/***********************************************
* Filename:  		StateMachine.cpp
* Date:      		7/15/2014
* Mod. Date: 		7/15/2014
* Mod. Initials:	CP
* Author:    		Charles Peter
* Purpose:   		Controls the currentState of the Game.
************************************************/

#include "GameState.h"
#include "StateMachine.h"
#include "MainMenu.h"


CStateMachine::CStateMachine(void)
{
	m_cCurrState = nullptr;
	m_pFont = nullptr;
}

CStateMachine::~CStateMachine(void)
{
	if(m_cCurrState != nullptr)
	{
		delete m_cCurrState;
		m_cCurrState = nullptr;
	}

	if(m_pFont != nullptr)
	{
		delete m_pFont;
		m_pFont = nullptr;
	}

	m_pMS->UnregisterFromALL(this);
}

bool CStateMachine::ChangeState(CBaseState* nState)
{
	if(nState->GetID() != INVALID_STATE )
	{
		if(m_cCurrState != nullptr)
		{
			delete m_cCurrState;
		}
		m_cCurrState = nState;
		return true;
	}
	else 
		return false;
}

void CStateMachine::Initialize(CRenderer* renderer)
{
	m_pFont = new CText();
	m_pFont->Init(renderer, "assets/Font/simpleFont2.0.fnt");

	CMainMenu* start = new CMainMenu();
	m_pR = renderer;
	start->Init(renderer, m_pFont);
	ChangeState(start);
	m_pMS = CEventSystem::GetInstance();
	m_pMS->RegisterForEvent(CHANGE_STATE,this);
}

void CStateMachine::Update(float dTime)
{
	m_pR->PreRender();
	m_cCurrState->Update(dTime);
	m_cCurrState->Render();

	std::string mousePos = "x, ";
	
	char buff[10];
	_itoa_s((int)CInput::GetInstance()->GetMouse().x, buff, 10);
	mousePos += buff;

	//char buffY[10];
	mousePos += "  y, ";
	_itoa_s((int)CInput::GetInstance()->GetMouse().y, buff, 10);
	mousePos += buff;

	m_pFont->WriteText(XMFLOAT2(1000.0f, 650.0f), 0.5f, mousePos, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	m_pR->PostRender();
}

void CStateMachine::ProcessEvent( CEvent* Event )
{
	switch(Event->getID())
	{
	case CHANGE_STATE:
		{
			ChangeState((CBaseState*)Event->GetData());
		}
		break;
	}
}
