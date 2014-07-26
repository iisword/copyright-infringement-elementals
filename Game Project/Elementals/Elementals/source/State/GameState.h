/***********************************************
* Filename:  		GameState.h
* Date:      		7/17/2014
* Mod. Date: 		7/17/2014
* Mod. Initials:	CP
* Author:    		Charles Peter
* Purpose:   		Controls the Game.
************************************************/


#ifndef GAME_STATE
#define GAME_STATE

#include "../Networking/NetworkDelegate.h"
#include <Raknet\RakNetTypes.h>
#include "../Renderer/Camera.h"

#include <vector>
//convert these to forward declarations
#include "..\Message\Listener.h"
#include "..\Message\Event.h"
#include "..\Message\EventSystem.h"
#include "..\Renderer\Renderer.h"
#include "..\Input\Input.h"
#include "..\Text\Text.h"
#include "../Utilities/Timer.h"
#include "../Object/ObjectManager.h"
#include "../CharacterFlyweight.h"
#include "../Object/Map.h"
#include "BaseState.h"

class CPlayer;
class CNetworkConnection;
class CMessageSystem;
class IMessage;
class CProjectile;
class CCollision;

#define NETWORKSTATUS_NOCONNECTION		0
#define NETWORKSTATUS_CONNECTING		1
#define NETWORKSTATUS_CONNECTED			2
#define NETWORKSTATUS_DISCONNECTING		3

class CGameState : public CBaseState, public INetworkDelegate //, public IListener
{
private:
	CRenderer* m_pRenderer;
	CInput* m_pInput;
	CObjectManager* m_pObjectManager;
	CMessageSystem * m_pMessageSystem;
	CPlayer * m_cpPlayer;
	CTimer * m_pTimer;
	CText* m_pFont;
	CNetworkConnection * m_pcConnection;
	TCharacterBaseStats tPlayerStats[4];
	
	// Player hud 2d objects
	D2Dobject * m_pSpells;
	D2Dobject * m_pPlayer;
	D2Dobject * m_pRadar;
	D2Dobject * m_pReticle;
	D2Dobject * m_pHealth;
	D2Dobject * m_pMana;
	D2Dobject * m_pSpellCone;
	D2Dobject * m_pSpellBolt;
	D2Dobject * m_pSpellAOE;
	D2Dobject * m_pScoreboard;

	// Spell objects
	D3DObject * m_pSpellSphere;

	//Pause State Stuff
	bool m_bPaused;
	CBaseState* m_pPause;

	CEventSystem* m_pMS;

	RakNet::SystemAddress m_tServerAddress;
	vector<CPlayer *> m_vPlayers;
	vector<CProjectile *> m_vProjectile;

	CCamera camera;
	CMap * m_map;	

	CCollision m_cCol;

	//Respawn things
	float m_fServerSpawnTimer;
	float m_fSpawnTimer;
	void RenderDead(void);
	
	unsigned char m_chNetworkStatus;

	//Loads spawn points, pick-up points etc;
	bool LoadMapInfo(const char* file);
	int FindPlayer(unsigned char chID);
	int FindSpell(unsigned char chID);
	static void MessageProc(IMessage* pMsg, CGameState * pGame);
	
	void Renderhud();
	void RenderScoreBoard();
	void RenderPlayerHud();
	string HudNameResize(const char * szName);

	CPlayer * CreatePlayer();
	CPlayer * CreatePlayer(unsigned char chGameID, const char * szName, unsigned char chMode = 0, unsigned char chStats = UCHAR_MAX);
	CProjectile * CreateProjectile();
	CProjectile * CreateProjectile(unsigned char chGameID, unsigned char chType, CPlayer * pcOwner, XMFLOAT3 * tPosition, XMFLOAT3 * tForward, float fCurrentTime);

public:
	CGameState(void);
	~CGameState(void);

	void Init(CRenderer* r,CText* font);
	void Update(float dTime);
	void Render(void);

	void SetPause(bool b) { m_bPaused = b; }

	void ProcessEvent(CEvent * Event);
	void GameInput();

	void HandleIncomingData(RakNet::Packet * pcIncomingPacket);

	void SendDisconnectMessage();

	/**********************************
	*	Kill:			Load the base stats 
	*					for all player types
	*					into a struct from an 
	*					xml file
	*
	*	In:				char * pFilePath
	*
	*	Mod. Date:		07/19/2014
	*	Mod. Initials:	TD
	**********************************/
	void LoadPlayerStats(char * pFilePath); //Moving to server side

	void CreateProjectile(IMessage * pMsg);

	void RemoveObject(CObject * pObject);
};

#endif