//////////////////////////////////////////////////////////////////////////
//	File	:	"CSGD_MessageSystem.cpp"
//
//	Author	:	David Brown (DB)
//
//	Purpose	:	Handles sending the messages to the specified proc function.
///////////////////////////////////////////////////////////////////////////

#include "MessageSystem.h"
using std::queue;


CMessageSystem* CMessageSystem::GetInstance(void)
{
	static CMessageSystem instance;
	return &instance;
}

void CMessageSystem::SetGame(CGameState * pGame)
{
	m_pGame = pGame;
}

CGameState * CMessageSystem::GetGame() const
{
	return m_pGame;
}

int CMessageSystem::GetNumMessages(void)
{
	return (int)m_MsgQueue.size(); 
}

void CMessageSystem::InitMessageSystem(MESSAGEPROC pfnMsgProc)
{
	if(!pfnMsgProc)	
	{
		return;
	}

	m_pfnMsgProc = pfnMsgProc;
}

void CMessageSystem::SendMsg(IMessage* pMsg)
{
	if(!pMsg)	
	{
		return;
	}

	m_MsgQueue.push(pMsg);
}

void CMessageSystem::ProcessMessages(void)
{
	if(!m_pfnMsgProc)
	{
		return;
	}

	while(!m_MsgQueue.empty())
	{
		m_pfnMsgProc(m_MsgQueue.front(), m_pGame);		
		delete m_MsgQueue.front();				
		m_MsgQueue.pop();						
	}
}

void CMessageSystem::ClearMessages(void)
{
	while(!m_MsgQueue.empty())
	{
		delete m_MsgQueue.front();
		m_MsgQueue.pop();
	}
}

void CMessageSystem::ShutdownMessageSystem(void)
{
	ClearMessages();
	m_pfnMsgProc = NULL;
}