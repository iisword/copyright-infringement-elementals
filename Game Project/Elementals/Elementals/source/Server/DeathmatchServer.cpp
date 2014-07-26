#include "DeathmatchServer.h"
#include "../Object/Player.h"


CDeathmatchServer::CDeathmatchServer(unsigned int nMaxClients,unsigned int nMaxKills, float fTimeRemaining) : CServer(nMaxClients), m_nMaxKills(nMaxKills), m_fTimeRemaining(fTimeRemaining)
{
	m_vnLeaderBoard.reserve(m_nMaxClients);
	for(int i = 0; i < (int)m_vnLeaderBoard.size(); ++i)
	{
		m_vnLeaderBoard[i] = -1;
	}

}

CDeathmatchServer::~CDeathmatchServer()
{

}

bool CDeathmatchServer::Initialize()
{
	return CServer::Initialize();
}

void CDeathmatchServer::Update(float dt)
{
	CServer::Update(dt);
	if(m_fTimeRemaining != -1.0f)		//check if the game has a time limit
	{
		m_fTimeRemaining -= dt;
		if(m_fTimeRemaining <= 0.0f)
		{
			//TODO: Send win message
		}
	}


	for(unsigned int nIndex = 0; nIndex < m_vPlayers.size(); nIndex++)
	{
		if(m_vPlayers[nIndex]->GetKills() > m_nMaxKills)
		{
			//TODO: Send win message
		}
	}
}

void CDeathmatchServer::UpdateLeaderBoard(void)
{
	for(int nI = 0; nI < (int)m_vPlayers.size()-1; nI++)
	{
		m_vnLeaderBoard[nI] = nI;
	}

	//for(int nJ = 0; nJ < (int)m_vPlayers.size()-1; nJ++)
	//{
	//	if( m_vPlayers[nJ]->GetKills() < m_vPlayers[nJ]->GetKills())
	//	{

	//		int temp = m_vnLeaderBoard[nJ+1];
	//		m_vnLeaderBoard[nJ+1] = m_vnLeaderBoard[nJ] ;
	//		m_vnLeaderBoard[nJ]  = temp;

	//	}
	//}
}

void CDeathmatchServer::Shutdown()
{
	CServer::Shutdown();
}