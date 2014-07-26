/***********************************************
* Filename:  		SpellEffect.h
* Date:      		7/15/2014
* Mod. Date: 		7/16/2014
* Mod. Initials:	TD
* Author:    		Trevor DeMarco
* Purpose:   		Will apply update and render the  
*					effects currently applied to the
*					player.
************************************************/

#ifndef SPELLEFFECT_HEADER
#define SPELLEFFECT_HEADER

enum ElementType { FIRE, AIR, WATER, EARTH };

class CSpellEffect
{
private:
	float m_fDuration;
	float m_fCurrTime;
	float m_fEffectPercentage;
	bool m_bActive;
	ElementType m_eType;
public:
	CSpellEffect(void);
	~CSpellEffect(void);

	void Render();
	void Update(float fDT);

	float GetDuration() const;
	float GetcurrTime() const;
	float GetEffectPercentage() const;
	bool GetActive() const;
	ElementType GetType() const;

	void SetType(ElementType eType);
};

#endif