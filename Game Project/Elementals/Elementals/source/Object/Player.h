/***********************************************
* Filename:  		Player.h
* Date:      		6/12/2014
* Mod. Date: 		7/16/2014
* Mod. Initials:	TD
* Author:    		Michael Boodle
* Purpose:   		The base class for the player(server side).
************************************************/


#ifndef PLAYER_HEADER
#define PLAYER_HEADER

#define PLAYERMODE_SPECTATOR		0
#define PLAYERMODE_ACTIVE			1

#define PLAYER_NAME_LENGTH			32

#define PLAYER_KILL_STREAK_TIME		5.0f

#define PLAYER_FLAG_BURNING			0x01
#define PLAYER_FLAG_SLOW			0x02

#include <Raknet\RakNetTypes.h>

#include "Object.h"

struct TCharacterBaseStats;
using namespace DirectX;

class CSpell;
class CEvent;
class CRenderer;
enum ElementType;

class CPlayer : public CObject
{
protected:
	int m_nHealth;
	int m_nMaxHealth;
	int m_nMana;
	int m_nMaxMana;
	char m_szPlayerName[PLAYER_NAME_LENGTH];
	int m_eType;
	CSpell * m_cSpell;

	float m_fCastTimer;
	float m_fManaRegenTimer;
	float m_fHealthRegenTimer;

	// 0-1 values
	float m_fMoveSpeed;
	float m_fDamage;
	float m_fCastSpeed;
	float m_fSpellCost;

	unsigned int m_nKills;
	unsigned int m_nDeaths;
	unsigned int m_nStreak;
	float m_fStreakTimer;
	float m_fRespawnTimer;
	float m_nRotation;

	const TCharacterBaseStats * m_tpStats;		//Holds the base stats for a character.
	RakNet::RakNetGUID m_tGUID;

	XMFLOAT4 m_fLastKnownPosition;
	unsigned char m_chPlayerMode;
	unsigned char m_chPlayerFlags;
	unsigned char m_chGameID;
public:

	CPlayer(const TCharacterBaseStats * tpStats = nullptr);
	~CPlayer();

#pragma region Game Functions
	/**********************************
	*	Kill:				Kills the player. Causes the player's death
	*						counter to increase and sets the respawn timer
	*						to the appropriate amount.
	*
	*	Return:				void
	*
	*	Mod. Date:			6/13/2014
	*	Mod. Initials:		MB
	**********************************/
	virtual void Kill();

	/**********************************
	*	IncreaseKill:		Increases the kill count for the player.
	*
	*	Return:				void
	*
	*	Mod. Date:			6/1/2014
	*	Mod. Initials:		MB
	**********************************/
	virtual void IncreaseKill();

	/**********************************
	*	Respawn:				Forces the player to spawn at the point specificied.
	*
	*	In:						XMFLOAT4 spawnPoint
	*
	*	Return:					void
	*
	*	Mod. Date:				6/13/2014
	*	Mod. Initials:			MB
	**********************************/
	virtual void Spawn(XMFLOAT4 fSpawnPoint);

	/**********************************
	*	Init:				Initialize all player variables
	*
	*	Mod. Date:			07/17/2014
	*	Mod. Initials:		TD
	**********************************/
	virtual void Init();

	/**********************************
	*	Update:				Causes the player to update.
	*
	*	In:                 float fDt - Delta time
	*
	*	Return:             void
	*
	*	Mod. Date:          6/1/2014
	*	Mod. Initials:      MB
	**********************************/
	virtual void Update(float fDT) override;

	/**********************************
	*	Render:				Render the player.
	*
	*	Mod. Date:			07/16/2014
	*	Mod. Initials:		TD
	**********************************/
	virtual void Render(CRenderer * cpRenderer) override;

	/**********************************
	*	CastSpell:			Casts currently selected spell
	*
	*	Mod. Date:			07/21/2014
	*	Mod. Initials:		TD
	**********************************/
	virtual void CastSpell();
	
	/**********************************
	*	CanFire:			Determines if the player is able to fire a
	*						spell.
	*
	*	In:					unsigned char chType - The type of spell you want to cast.
	*
	*	Return:             bool
	*
	*	Mod. Date:			7/24/2014
	*	Mod. Initials:		MB
	**********************************/
	bool CanFire(unsigned char chType) const;
#pragma endregion

#pragma region Network Functions
	
#pragma endregion

	/***************
	*  Accessors
	***************/
	unsigned int GetKills() const;
	unsigned int GetDeaths() const;
	XMFLOAT4 GetLastKnownPosition() const;
	int GetType() const;
	float GetMoveSpeed() const;
	CSpell * GetSpell() const;
	float GetRotation()const { return m_nRotation; };
	const RakNet::RakNetGUID * GetGUID() const;
	unsigned char GetPlayerMode() const;
	unsigned char GetGameID() const;
	const char * GetName() const;
	int GetHealth() const;
	int GetMaxHealth() const;
	int GetMana() const;
	int GetMaxMana() const;
	float GetRespawnTime() const;
	const TCharacterBaseStats * GetStats() const;

	/***************
	*  Mutators
	***************/
	void SetStats(const TCharacterBaseStats * tpStats);
	void SetName(const char * szName);
	void SetLastKnownPosition(XMFLOAT4 & fLastPosition);
	void AddRotation(float rot){ m_nRotation += rot; };
	void SetGUID(const RakNet::RakNetGUID * ptGUID);
	void SetPlayerMode(unsigned char chPlayerMode);
	void SetGameID(unsigned char chID);
	void SetMana(int nMana);
	void SetMaxMana(int nMana);
	void SetHealth(int nHealth);
	void SetMaxHealth(int nHealth);
	void TakeDamage(int nDmg);
	void SetRespawnTimer(float fTime);
};

#endif