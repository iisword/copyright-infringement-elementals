/***********************************************
* Filename:  		SpellEffect.cpp
* Date:      		7/15/2014
* Mod. Date: 		7/16/2014
* Mod. Initials:	TD
* Author:    		Trevor DeMarco
* Purpose:   		Will apply update and render the  
*					effects currently applied to the
*					player.
************************************************/

#include "SpellEffect.h"

CSpellEffect::CSpellEffect(void)
{

}

CSpellEffect::~CSpellEffect(void)
{

}

void CSpellEffect::Render()
{

}

void CSpellEffect::Update(float fDT)
{
	if(m_fCurrTime >= m_fDuration)
	{
		m_bActive = false;
	}
	else
	{
		m_fCurrTime += fDT;
	}
}

float CSpellEffect::GetDuration() const
{
	return m_fDuration;
}

float CSpellEffect::GetcurrTime() const
{
	return m_fCurrTime;
}

float CSpellEffect::GetEffectPercentage() const
{
	return m_fEffectPercentage;
}

bool CSpellEffect::GetActive() const
{
	return m_bActive;
}

ElementType CSpellEffect::GetType() const
{
	return m_eType;
}

void CSpellEffect::SetType(ElementType eType)
{
	m_eType = eType;
}