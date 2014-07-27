#include "Player.h"
#include "../CharacterFlyweight.h"
#include "Spell.h"
#include "../Renderer/Renderer.h"
#include "..\Collision\CollisionDefines.h"

#pragma region Constructor and Destructor
CPlayer::CPlayer(const TCharacterBaseStats * tStats) : m_chPlayerMode(PLAYERMODE_SPECTATOR), m_nHealth(0), m_nKills(0), m_nDeaths(0), m_fStreakTimer(0.0f), m_tpStats(tStats), m_chGameID(UCHAR_MAX)
{
	ZeroMemory(&m_szPlayerName, 32);
	ZeroMemory(&m_fLastKnownPosition, sizeof(m_fLastKnownPosition));
	SetID(PLAYER);
	m_nRotation = 0.0f;
	m_fCastTimer = 0.0f;
	m_fManaRegenTimer = 0.0f;
	m_fHealthRegenTimer = 0.0f;
	m_nMana = 100;
	m_nMaxMana = 100;
	m_fRespawnTimer = 0.0f;
	//GetMesh()->SetMatrixToIdentity();
}

CPlayer::~CPlayer()
{
	delete m_cSpell;
	m_cSpell = nullptr;
}
#pragma endregion

#pragma region Game Functions
void CPlayer::Kill()
{
	m_nHealth = 0;
	m_nMana = 0;
	m_fRespawnTimer = 0.0f;
	m_fHealthRegenTimer = 0.0f;
	m_fStreakTimer = 0.0f;
	m_nDeaths++;
}

void CPlayer::IncreaseKill()
{
	m_nKills++;
	m_nStreak++;
	m_fStreakTimer = 0.0f;
}

void CPlayer::Spawn(XMFLOAT4 fSpawnPoint)
{
	m_nHealth = m_tpStats->m_nStartHealth;
	m_nMana = m_tpStats->m_nStartMana;
	m_nStreak = 0;

#ifndef SERVER_BUILD
	m_cSpell->ChangeType(SpellType::BASE);
#endif

	XMFLOAT3 tPosition((float *)&fSpawnPoint);
	SetPosition(tPosition);
}

void CPlayer::Init()
{
	if(m_tpStats)
	{
		m_nHealth = m_tpStats->m_nStartHealth;
		m_nMaxHealth = m_tpStats->m_nStartHealth;
		m_nMana = m_tpStats->m_nStartMana;
		m_nMaxMana = m_tpStats->m_nStartMana;
	
	//	m_fDamage =	m_tpStats->m_fStartDamage;
		m_fMoveSpeed = m_tpStats->m_StartMoveSpeed;
		m_fCastSpeed = m_tpStats->m_fCastSpeed;
	//	m_fSpellCost = m_tpStats->m_fSpellCost;

		m_eType = m_tpStats->m_eElementType;
		m_cSpell = new CSpell(this);
		m_cSpell->ChangeType(SpellType::BASE);
	}

}

void CPlayer::Update(float fDT)
{
	if(m_nHealth == 0)
	{
		m_fRespawnTimer += fDT;
	}
	else
	{
		m_fCastTimer += fDT;
		

#ifndef SERVER_BUILD
		m_cSpell->Update(fDT);
#else
		m_fManaRegenTimer += fDT;
		m_fHealthRegenTimer += fDT;

		if(m_fManaRegenTimer > 0.5f)
		{
			SetMana(m_nMana + 1);
			m_fManaRegenTimer = 0.0f;
		}

		if(m_fHealthRegenTimer > 5.0f)
		{
			SetHealth(m_nHealth + 1);
			m_fHealthRegenTimer = 0.0f;
		}
#endif
		if(GetBounds() != nullptr)
		{
			GetBounds()->Update(GetPosition());
		}
	}
}

void CPlayer::Render(CRenderer * cpRenderer)
{
	if(m_nHealth)
	{
		CObject::Render(cpRenderer);
		m_cSpell->Render(cpRenderer);
	}
}

void CPlayer::CastSpell()
{
	if(m_tpStats)
	{
#ifndef SERVER_BUILD
		if(CanFire(m_cSpell->GetType()))
		{
			m_fCastTimer = 0.0f;
		}
#else
		m_fCastTimer = 0.0f;
		m_fManaRegenTimer = 0.0f;
#endif
	}
	else
	{
		if(m_fCastTimer > 0.5f)
		{
			m_cSpell->Cast();
			m_fCastTimer = 0.0f;
		}
	}
	
}

bool CPlayer::CanFire(unsigned char chType) const
{
	if(m_tpStats)
	{
		if(m_nHealth != 0)
		{
			if(m_nMana > m_tpStats->m_tSpellStats[chType].m_nManaCost)
			{
				return m_tpStats->m_fCastSpeed <= m_fCastTimer;
			}
		}
	}
	return false;
}

void CPlayer::DecreaseManaBasedOnSpell(unsigned char chType)
{
	if(m_tpStats)
	{
		m_nMana -= m_tpStats->m_tSpellStats[chType].m_nManaCost;
		if(m_nMana < 0)
		{
			m_nMana = 0;
		}
	}
}
#pragma endregion

#pragma region Accessors and Mutators
unsigned int CPlayer::GetKills() const
{
	return m_nKills;
}

unsigned int CPlayer::GetDeaths() const
{
	return m_nDeaths;
}

int CPlayer::GetType() const
{
	if(m_tpStats)
	{
		return m_tpStats->m_eElementType;
	}
	return m_eType;
}

float CPlayer::GetMoveSpeed() const
{
	if(m_tpStats)
	{
		return m_tpStats->m_StartMoveSpeed;
	}
	return m_fMoveSpeed;
}

CSpell * CPlayer::GetSpell() const
{
	return m_cSpell;
}

const RakNet::RakNetGUID * CPlayer::GetGUID() const
{
	return &m_tGUID;
}

unsigned char CPlayer::GetPlayerMode() const
{
	return m_chPlayerMode;
}

unsigned char CPlayer::GetGameID() const
{
	return m_chGameID;
}

const char * CPlayer::GetName() const
{
	return m_szPlayerName;
}

int CPlayer::GetHealth() const
{
	return m_nHealth;
}

int CPlayer::GetMaxHealth() const
{
	if(m_tpStats)
	{
		return m_tpStats->m_nStartHealth;
	}
	return m_nMaxHealth;
}

int CPlayer::GetMana() const
{
	return m_nMana;
}

int CPlayer::GetMaxMana() const
{
	if(m_tpStats)
	{
		return m_tpStats->m_nStartMana;
	}
	return m_nMaxMana;
}

const TCharacterBaseStats * CPlayer::GetStats() const
{
	return m_tpStats;
}

float CPlayer::GetRespawnTime() const
{
	return m_fRespawnTimer;
}

void CPlayer::SetStats(const TCharacterBaseStats * tpStats)
{
	m_tpStats = tpStats;
}

void CPlayer::SetName(const char * szName)
{
	memcpy(m_szPlayerName, szName, 32);
}

void CPlayer::SetLastKnownPosition(XMFLOAT4 & fLastPosition)
{
	m_fLastKnownPosition = fLastPosition;
}

void CPlayer::SetGUID(const RakNet::RakNetGUID * ptGUID)
{
	memcpy(&m_tGUID, ptGUID, sizeof(m_tGUID));
}

void CPlayer::SetPlayerMode(unsigned char chPlayerMode)
{
	m_chPlayerMode = chPlayerMode;
}

void CPlayer::SetGameID(unsigned char chGameID)
{
	m_chGameID = chGameID;
}

void CPlayer::SetMana(int nMana)
{
	m_nMana = nMana;

	if(m_nMana > m_tpStats->m_nStartMana)
	{
		m_nMana = m_tpStats->m_nStartMana;
	}
	else if(m_nMana < 0)
	{
		m_nMana = 0;
	}
}

void CPlayer::SetMaxMana(int nMana)
{
	m_nMaxMana = nMana;
}

void CPlayer::SetMaxHealth(int nHealth)
{
	m_nMaxHealth = nHealth;
}

void CPlayer::SetHealth(int nHealth)
{
	m_nHealth = nHealth;

	if(m_nHealth > m_tpStats->m_nStartHealth)
	{
		m_nHealth = m_tpStats->m_nStartHealth;
	}
	else if(m_nHealth < 0)
	{
		m_nHealth = 0;
	}
}

void CPlayer::TakeDamage(int nDmg)
{
	m_fHealthRegenTimer = 0.0f;
	m_nHealth -= nDmg;
	if(m_nHealth < 0)
	{
		m_nHealth = 0;
	}
	else if(m_nHealth > m_tpStats->m_nStartHealth)
	{
		m_nHealth = m_tpStats->m_nStartHealth;
	}
}

void CPlayer::SetRespawnTimer(float fTime)
{
	m_fRespawnTimer = fTime;
}
#pragma endregion