/***********************************************
* Filename:  		CharacterFlyweight.h
* Date:      		6/13/2014
* Mod. Date: 		6/13/2014
* Mod. Initials:	MB
* Author:    		Michael Boodle
* Purpose:   		Defines the character and spell flyweight.
************************************************/

#ifndef CHARACTERFLYWEIGHT_HEADER
#define CHARACTERFLYWEIGHT_HEADER

enum ElementType { FIRE, AIR, WATER, EARTH };

//use these defines in m_tSpellStats
#define BASIC_SPELL_INDEX	0
#define BOLT_SPELL_INDEX	1
#define CONE_SPELL_INDEX	2
#define AOE_SPELL_INDEX		3

struct TSpellStats
{
	float m_fBaseDamage;
	float m_fBaseSpeed;			//On 
	float m_fRadius;
	float m_fAliveTime;
	int m_nManaCost;
	//unsigned char m_chSpellShape;
};

struct TCharacterBaseStats
{
	int m_eElementType;
	int m_nStartHealth;
	int m_nStartMana;
	float m_StartMoveSpeed;
	float m_fCastSpeed;
	TSpellStats m_tSpellStats[4];
	
};


#endif