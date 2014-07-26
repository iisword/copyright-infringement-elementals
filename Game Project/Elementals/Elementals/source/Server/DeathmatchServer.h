/***********************************************
* Filename:  		DeathMatchServer.h
* Date:      		6/12/2014
* Mod. Date: 		6/12/2014
* Mod. Initials:	MB
* Author:    		Michael Boodle
* Purpose:   		Used to handle the death match rules.
************************************************/


#ifndef DEATHMATCHSERVER_HEADER
#define DEATHMATCHSERVER_HEADER

#include "Server.h"



class CDeathmatchServer : public CServer
{
protected:
	unsigned int m_nMaxKills;
	float m_fTimeRemaining;

	vector<int> m_vnLeaderBoard;


public:
	CDeathmatchServer(unsigned int nMaxClients = 2, unsigned int nMaxKills = 15, float fTimeRemaining = -1.0f);
	~CDeathmatchServer();

	virtual bool Initialize();
	virtual void Update(float dt);
	void UpdateLeaderBoard(void);
	virtual void Shutdown();
};

#endif