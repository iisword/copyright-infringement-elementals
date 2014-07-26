
#include "WinState.h"
#include "GameState.h"
#include "MainMenu.h"
#include "..\Message\EventSystem.h"
#include "..\Input\Input.h"

CWinState::CWinState(void)
{
	SetState(WIN_STATE);
	m_bWon = false;
}


CWinState::~CWinState(void)
{
}


void CWinState::Init(CRenderer* r, CText* font)
{
	m_pRenderer = r;
	m_pFont = font;
}

void CWinState::Update(float dTime)
{
	if(CInput::GetInstance()->IsEnter())
	{
		CBaseState* pNState = new CGameState();
		pNState->Init(m_pRenderer, m_pFont);
		CEventSystem::GetInstance()->SendEvent(CHANGE_STATE,HIGH, pNState);
	}
	else if(CInput::GetInstance()->IsEsc())
	{
		CBaseState* pNState = new CMainMenu();
		pNState->Init(m_pRenderer, m_pFont);
		CEventSystem::GetInstance()->SendEvent(CHANGE_STATE,HIGH, pNState);
	}
	CInput::GetInstance()->Reset();
}

void CWinState::Render(void)
{
	std::string win = "GAME OVER";
	if(m_bWon == true)
	{ win += " YOU WON!!!"; }
	m_pFont->WriteText(XMFLOAT2(200.0f, 100.0f), 1.5f, win, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	win = "Push Enter for New Game";
	m_pFont->WriteText(XMFLOAT2(240.0f, 150.0f), 1.0f, win, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	win = "Push Escape to Exit";
	m_pFont->WriteText(XMFLOAT2(240.0f, 175.0f), 1.0f, win, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
}