#ifndef MESSAGESYSTEM_HEADER
#define MESSAGESYSTEM_HEADER

#include "IMessage.h"
#include <queue>

class CGameState;

typedef void (*MESSAGEPROC)(IMessage*, CGameState *);

class CMessageSystem
{
private:
	std::queue<IMessage*> m_MsgQueue;				
	MESSAGEPROC m_pfnMsgProc;			
	CGameState * m_pGame;

	CMessageSystem() { m_pfnMsgProc = NULL;	}
	CMessageSystem(const CMessageSystem&);
	CMessageSystem& operator=(const CMessageSystem&);

	~CMessageSystem() {}

public:
	
	static CMessageSystem* GetInstance(void);
	int GetNumMessages(void);
	void InitMessageSystem(MESSAGEPROC pfnMsgProc);
	void SendMsg(IMessage* pMsg);
	void ProcessMessages(void);
	void ClearMessages(void);
	void ShutdownMessageSystem(void);
	void SetGame(CGameState * pGame);
	CGameState * GetGame() const;
};

#endif