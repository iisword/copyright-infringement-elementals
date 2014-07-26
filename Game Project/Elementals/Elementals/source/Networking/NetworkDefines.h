/***********************************************
* Filename:  		NetworkDefines.h
* Date:      		6/9/2014
* Mod. Date: 		6/9/2014
* Mod. Initials:	MB
* Author:    		Michael Boodle
* Purpose:   		Defines for the network are stored here.
************************************************/

#ifndef NETWORKDEFINE_HEADER
#define NETWORKDEFINE_HEADER

#include <Raknet\MessageIdentifiers.h>

#define DEFAULT_PORT		8336

typedef unsigned char MessageID;

//These can be used to set offsets in CNetworkDelegate::HandleIncomingData() if you plan to use pointers

enum NetworkDataOffset {SNAP_PLAYER_OFFSET = 33, SNAP_SPELL_OFFSET = 31, SNAP_PICKUP_OFFSET = 2, PLAYER_LIST_OFFSET = 35, PICKUP_LIST_OFFSET = 14};

enum NetworkMessageType
{
	//Client to server
	ID_CLIENT_READY = ID_USER_PACKET_ENUM,		//Data: playerID(1)
	ID_CLIENT_ALIVE,							//Data: playerID(1)
	ID_REQUEST_ID,								//Data: playerName(32)
	ID_CHOSEN_ELEMENT,							//Data: playerID(1), elementID(1)
	ID_GET_PLAYER_LIST,							//Data: playerID(1)
	ID_GET_SERVER_INFO,							//Data: playerID(1)
	ID_GET_CHARACTER_STATS,						//Data: playerID(1)
	ID_GET_PICKUP,								//Data:	playerID(1)
	ID_CLIENT_POSITION,							//Data: playerID(1), playerPosition(12), playerForward(12)
	ID_CLIENT_FIRE,								//Data: playerID(1), spellType(1), playerPosition(12), playerForward(12) 
	ID_CLIENT_DISCONNECT,						//Data: playerID(1)

	//Server to client
	ID_SNAPSHOT_PLAYER,							//Data: numOfPlayers(1), {playerID(1), playerPosition(12), playerForward(12)}
	ID_SNAPSHOT_SPELL,							//Data: numOfSpell(1), {spellID(1), spellType(1), ownerID(1), spellStart(12), spellForward(12), spellTime(4)}
	ID_SNAPSHOT_PICKUP,							//Data: numOfPickup(1), {pickupID(1), pickupActive(1)}
	ID_REQUEST_SUCCESS,							//Data: issuedID(1)
	ID_REQUEST_FAIL,							//Data:
	ID_PLAYER_LIST,								//Data: numOfPlayers(1), {playerID(1), playerName(32), playerMode(1), elementID(1)}
	ID_SERVER_INFO,								//Data: serverTime(4), serverType(1)
	ID_CHARACTER_STATS,							//Data: numOfStats(1), {characterFlyweight(sizeof(T}
	ID_PICKUP,									//Data: numOfPickup(1), {pickupType(1), pickupActive(1), pickupPosition(12)}
	ID_KILL,									//Data: killedID(1), killerID(1), spellID(1), location(12)
	ID_WIN,										//Data: winnerID(1)
	ID_COLLISION,								//Data: collisionType(1), playerID(1), objectID(1)
	ID_NEW_SLOT,								//Data: playerID(1), playerName(32)
	ID_PLAYER_SPAWN,							//Data: playerID(1), playerPosition(12), playerForward(12)
	ID_PROJECTILE_SPAWN,						//Data: spellID(1), spellType(1), ownerID(1), spellPosition(12), spellForward(12)  
	ID_PLAYER_DISCONNECT,						//Data: playerID(1)
};

//These are used with the ID_COLLISION
#define COLLISION_PLAYER_TO_SPELL	0

#endif