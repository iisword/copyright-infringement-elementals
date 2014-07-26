/***********************************************
* Filename:  		Spell.cpp
* Date:      		7/15/2014
* Mod. Date: 		7/16/2014
* Mod. Initials:	TD
* Author:    		Trevor DeMarco
* Purpose:   		Handles the current spell the player has.
************************************************/

#include "Spell.h"
#include "Player.h"
#include "Projectile.h"
#include "../Message/MessageSystem.h"
#include "../Message/MessageCreateBaseAttack.h"

CSpell::CSpell(CPlayer * cPlayer)
{
	m_bActive = false;
	m_nManaCost = 0;
	m_nDamage = 0;
	m_nDistance = 0;
	m_fSpeed = 0.0f;
	m_fRadius = 0.0f;
	m_eType = SpellType::NUMSPELLS;
	m_cOwner = cPlayer;
}

CSpell::~CSpell(void)
{

}

void CSpell::ChangeType(SpellType eType)
{
	if(eType == m_eType)
		return;

	switch(eType)
	{
	case SpellType::BASE:
		{
			m_bActive = false;
			m_nManaCost = 5;
			m_nDamage = 5;
			m_nDistance = 6;
			m_fSpeed = 1.0f;
			m_fRadius = 1.0f;
			m_eType = eType;
		}
		break;
	case SpellType::BOLT:
		{
			m_bActive = false;
			m_nManaCost = 15;
			m_nDamage = 45;
			m_nDistance = 10;
			m_fSpeed = 2.0f;
			m_fRadius = 2.5f;
			m_eType = eType;
		}
		break;
	case SpellType::CONE:
		{
			m_bActive = false;
			m_nManaCost = 25;
			m_nDamage = 35;
			m_nDistance = 3;
			m_fSpeed = 3.0f;
			m_fRadius = 4.0f;	
			m_eType = eType;
		}
		break;
	case SpellType::AOE:
		{
			m_bActive = false;
			m_nManaCost = 40;
			m_nDamage = 65;
			m_nDistance = 7;
			m_fSpeed = 1.5f;
			m_fRadius = 4.0f;		
			m_eType = eType;
		}
		break;
	}
}

void CSpell::Cast()
{
	m_bActive = true;

	switch(m_eType)
	{
	case SpellType::BASE:
		{
			// Create the appropriate projectile of the correct element. Via message/event system.
			CMessageCreateBaseAttack* pMsg = new CMessageCreateBaseAttack(m_cOwner->GetPosition(), m_cOwner->GetForward(), m_cOwner->GetForward(), m_cOwner->GetType(), 0.75f);
			CMessageSystem::GetInstance()->SendMsg(pMsg);
			pMsg = nullptr;
		}
		break;
	case SpellType::BOLT:
		{
			// Create the appropriate projectile of the correct element. Via message/event system.
		}
		break;
	case SpellType::CONE:
		{
			// Create the appropriate projectile of the correct element. Via message/event system.
		}
		break;
	case SpellType::AOE:
		{
			// Create the appropriate projectile of the correct element. Via message/event system.
		}
		break;
	}

	m_cOwner->SetMana(m_cOwner->GetMana() - m_nManaCost);

	if(m_cOwner->GetMana() < 0)
	{
		m_cOwner->SetMana(0);
	}
}

void CSpell::Render(CRenderer * cpRenderer)
{
	if(false == m_bActive)
	{
		RenderMarker();
	}	

	// Potentially render the spell itself (at lease for core).
}

void CSpell::RenderMarker()
{
	switch(m_eType)
	{
	case SpellType::BASE:
		{
			// Draw spell marker on ground in front of player.
		}
		break;
	case SpellType::BOLT:
		{
			// Draw spell marker on ground in front of player.
		}
		break;
	case SpellType::CONE:
		{
			// Draw spell marker on ground in front of player.
		}
		break;
	case SpellType::AOE:
		{
			// Draw spell marker on ground in front of player.
		}
		break;
	}	
}

void CSpell::Update(float fDT)
{

}