/***********************************************
* Filename:  		Server.h
* Date:      		6/9/2014
* Mod. Date: 		7/23/2014
* Mod. Initials:	MB
* Author:    		Michael Boodle
* Purpose:   		The base class for a server. If you run an instance
*					of the class, you will get basic gameplay, but no
*					win condition.
*
*					The job of the class is to handle basic messages and
*					apply physics.
************************************************/

#ifndef SERVER_HEADER
#define SERVER_HEADER

#include <Raknet\RakPeerInterface.h>
#include <vector>
#include "..\Networking\NetworkDelegate.h"
#include "..\Networking\NetworkConnection.h"
using namespace std;

#include "..\Collision\Collision.h"

class CPlayer;
class CPickup;
class CProjectile;
class CMap;
struct TCharacterBaseStats;

typedef unsigned char ServerState;

#define SERVERSTATE_UNINITIALIZED	0
#define SERVERSTATE_OFFLINE			1
#define SERVERSTATE_RUNNING			2
#define SERVERSTATE_SHUTDOWN		3

#define DEFAULT_SPAWN_TIME			5.0f

#ifdef _DEBUG
#define SNAPSHOT_RATE				0.1f
#else
#define SNAPSHOT_RATE				0.03f
#endif

class CServer : public INetworkDelegate
{

protected:
	vector<CPlayer *> m_vPlayers;
	vector<unsigned char> m_vToBeDeleted;
	vector<CPickup *> m_vPickups;
	vector<CProjectile *> m_vSpells;
	vector<TCharacterBaseStats *> m_vStats;
	CMap * m_pcMap;

	CCollision m_cCol;

	//RakNet::RakPeerInterface * m_pcPeer;
	CNetworkConnection m_cConnection;

	float m_fSpawnTime;
	float m_fTotalTime;
	float m_fSnapshotTime;
	unsigned int m_nSequence;
	unsigned int m_nMaxClients;
	unsigned int m_nActiveClients;
	unsigned int m_nConnectedClients;
	ServerState m_chState;
	unsigned char m_nSpellCasts;		//use this to get the ID for the spell

	int FindProjectile(unsigned char chGameID) const;
public:
#pragma region Constructor and Destructor
	CServer(unsigned int nMaxClients = 2);
	virtual ~CServer();
#pragma endregion

#pragma region Server Functions
	/**********************************
	*	LoadServerConfig:		Used to load a server configuration file. The base loads all stats
	*							required for normal play.
	*
	*	In:						const char * szFilepath - The filepath to the server config file.
	*
	*	Return:					bool
	*
	*	Mod. Date:				6/13/2014
	*	Mod. Initials:			MB
	**********************************/
	bool LoadServerConfig(const char * szFilepath);

	/**********************************
	*	LoadCharacterStats:		Loads the character stats file into memory.
	*
	*	In:						const char * szFilepath - The filepath to the character XML file.
	*
	*	Return:					bool
	*
	*	Mod. Date:				7/23/2014
	*	Mod. Initials:			MB
	**********************************/
	bool LoadCharacterStats(const char * szFilepath);

	/**********************************
	*	HandleIncomingData:			A virtual function by which you handle a network
	*								message that was received.
	*
	*	In:							RakNet::Packet * pcIncomingPacket - The packet that was received from peer.
	*
	*	Return:						void
	*
	*	Mod. Date:					6/19/2014
	*	Mod. Initials:				MB
	**********************************/
	virtual void HandleIncomingData(RakNet::Packet * pcIncomingPacket);

	/**********************************
	*	Initialize:			Initializes the server.
	*
	*	In:                 Add parameters here
	*
	*	Return:             bool
	*
	*	Mod. Date:          7/10/2014
	*	Mod. Initials:      MB
	**********************************/
	virtual bool Initialize();

	/**********************************
	*	Update:				Used to update the server. Place things such as
	*						gamestate updates.
	*
	*	Return:             fDt - The delta time.
	*
	*	Mod. Date:          6/9/2014
	*	Mod. Initials:      MB
	**********************************/
	virtual void Update(float fDt);

	/**********************************
	*  Shutdown:			Used to terminate the server.
	*
	*  Return:				void
	*
	*  Mod. Date:			6/10/2014
	*  Mod. Initials:		MB
	**********************************/
	virtual void Shutdown();

	/**********************************
	*	SendSnapshots:      Sends the snapshots to the clients.
	*
	*	Return:				void
	*
	*	Mod. Date:			7/21/2014
	*	Mod. Initials:		MB
	**********************************/
	void SendSnapshots();
#pragma endregion

#pragma region Player Manipulation Functions
	/**********************************
	*	AddPlayer:			Attempts to add a player to the server.
	*
	*	In:					CPlayer * cPlayer - The player to add to the server.
	*
	*	Return:				bool
	*
	*	Mod. Date:			6/9/2014
	*	Mod. Initials:		MB
	**********************************/
	bool AddPlayer(CPlayer * cPlayer);

	/**********************************
	*	RemovePlayer:       Attempts to remove the player from the server.
	*						This function will not delete the player.
	*
	*	In:                 CPlayer * cPlayer - The player to be removed.
	*
	*	Return:             void
	*
	*	Mod. Date:          6/9/2014
	*	Mod. Initials:      MB
	**********************************/
	void RemovePlayer(CPlayer * cPlayer);

	/**********************************
	*	RemovePlayer:       Attempts to remove the player from the server.
	*						This function will delete the player.
	*
	*	In:                 int nIndex - The index of the player to be removed.
	*
	*	Return:             void
	*
	*	Mod. Date:          6/9/2014
	*	Mod. Initials:      MB
	**********************************/
	void RemovePlayer(int nIndex);

	/**********************************
	*	ContainsPlayer:		Attempts to find the player on the server. If the player
	*						is not in the server, -1 is returned.
	*
	*	In:					PlayerType * cPlayer - The player to find.
	*
	*	Return:				int
	*
	*	Mod. Date:			6/10/2014
	*	Mod. Initials:		MB
	**********************************/
	int ContainsPlayer(CPlayer * cPlayer);

	/**********************************
	*	CreatePlayer:		Used to create player classes. Do not add this player to the
	*						list in the function.
	*	
	*	In:					RakNet::RakNetGUID & tGUID - The guid from the REQUEST_ID packet
	*						const char * szName - The name of the player.
	*
	*	Return:				CPlayer *
	*
	*	Mod. Date:			6/19/2014
	*	Mod. Initials:		MB
	**********************************/
	virtual CPlayer * CreatePlayer(RakNet::RakNetGUID & tGUID, const char * szName);

	/**********************************
	*	CreateProjectile:		Used to create projectile classes. This will make appropiate adjustments
	*							to the spell position based on type. Do not add this projectiles to the
	*							list in the function.
	*	
	*	In:						unsigned char chGameID - The id for the new projectile.
	*							unsigned char chType - The type of spell being created.
	*							XMFLOAT3 * ptPosition - The position of the player.
	*							XMFLOAT3 * ptForward - The forward of the player.
	*							CPlayer * pcOwner - The owner of the spell.
	*							
	*
	*	Return:					CPlayer *
	*
	*	Mod. Date:				7/24/2014
	*	Mod. Initials:			MB
	**********************************/
	CProjectile * CreateProjectile(unsigned char chGameID, unsigned char chType, XMFLOAT3 * ptPosition, XMFLOAT3 * ptForward, CPlayer * pcOwner);

	/**********************************
	*	CheckPlayerIsCorrect:		Determines if the player is correct.
	*
	*	In:							char chID - The ID the player gave us.
	*								RakNet::RakNetGUID & tGUID - The GUID of the packet.
	*
	*	Return:						bool
	*
	*	Mod. Date:					7/19/2014
	*	Mod. Initials:				MB
	**********************************/
	bool CheckPlayerIsCorrect(char chID, RakNet::RakNetGUID & tGUID);

	/**********************************
	*  SendToAll:			Sends data to everyone but the id specified.
	*
	*  In:					char nID - The ID of who not to send to.
	*
	*  Return:				void
	*
	*  Mod. Date:			7/22/2014
	*  Mod. Initials:		MB
	**********************************/
	void SendToAll(char nID = -1);

	/**********************************
	*	SendOutDeletedClients:		Sends out the clients who have disconnected.
	*
	*	Return:						void
	*
	*	Mod. Date:					7/23/2014
	*	Mod. Initials:				MB
	**********************************/
	void SendOutDeletedClients();


	/**********************************
	*	RespawnPlayer:		Finds a good spawn point for the player the tell them they can come "BACK TO LIFE".
	*
	*	Return:						void
	*
	*	Mod. Date:					7/25/2014
	*	Mod. Initials:				CP
	**********************************/
	void RespawnPlayer(CPlayer* pDeadGuy);

	/**********************************
	*	RemoveSpellOwnerShip:		Removes the player as owner from spells in the array.
	*
	*	In:							CPlayer * pcPlayer - The player who is the owner of spells
	*
	*	Return:						void
	*
	*	Mod. Date:					7/25/2014
	*	Mod. Initials:				MB
	**********************************/
	void RemoveSpellOwnerShip(CPlayer * pcPlayer);
#pragma endregion

#pragma region Accessors and Mutators
	/***************
	*  Accessors
	***************/
	CPlayer * GetPlayer(int nIndex) const;
	unsigned int GetMaxPlayers() const;
	unsigned int GetNumberOfPlayers() const;
	float GetTotalTime() const;
#pragma endregion
};
#endif