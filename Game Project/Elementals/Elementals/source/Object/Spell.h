/***********************************************
* Filename:  		Spell.h
* Date:      		7/15/2014
* Mod. Date: 		7/16/2014
* Mod. Initials:	TD
* Author:    		Trevor DeMarco
* Purpose:   		Handles the current spell the player has.
************************************************/

#ifndef SPELL_HEADER
#define SPELL_HEADER

#include <DirectXMath.h>
using namespace DirectX;

class CPlayer;
class CProjectile;
class CRenderer;

enum SpellType { BASE, BOLT, CONE, AOE, NUMSPELLS };

class CSpell
{
private:
	bool m_bActive;
	int m_nManaCost;
	int m_nDamage;
	int m_nDistance;
	float m_fSpeed;
	float m_fRadius;
	SpellType m_eType;
	CPlayer * m_cOwner;
public:
	CSpell(CPlayer * cPlayer);
	~CSpell(void);

	/*****************************************************************
	* ChangeType(SpellType _type);  Change the spells values to reflect
	* 								the new type.
	*		
	* Ins:							_type
	*
	* Outs:							void
	*
	* Returns:						void
	*
	* Mod. Date:					07/16/2014
	* Mod. Initials:				TD
	*****************************************************************/
	void ChangeType(SpellType eType);

	/*****************************************************************
	* Cast();						Cast the current spell in the 
	*								direction the player is facing
	*		
	* Ins:							void
	*
	* Outs:							void
	*
	* Returns:						void
	*
	* Mod. Date:					07/16/2014
	* Mod. Initials:				TD
	*****************************************************************/
	void Cast();

	/*****************************************************************
	* Render();						Render the visuals of the spell
	*								while it is active.
	*		
	* Ins:							void
	*
	* Outs:							void
	*
	* Returns:						void
	*
	* Mod. Date:					07/16/2014
	* Mod. Initials:				TD
	*****************************************************************/
	void Render(CRenderer * cpRenderer);

	/*****************************************************************
	* RenderMarker();				If the spell is selected but not 
	*								cast yet this will render the marker
	*								for the spell on the ground in front
	*								of the player.
	*		
	* Ins:							void
	*
	* Outs:							void
	*
	* Returns:						void
	*
	* Mod. Date:					07/16/2014
	* Mod. Initials:				TD
	*****************************************************************/
	void RenderMarker();

	/*****************************************************************
	* Update(float fDT);			Update the position and visual  
	*								effects of the spell.
	*		
	* Ins:							fDT
	*
	* Outs:							void
	*
	* Returns:						void
	*
	* Mod. Date:					07/16/2014
	* Mod. Initials:				TD
	*****************************************************************/
	void Update(float fDT);

	/********************
		Accesors
	********************/
	int GetDamage(void) const { return m_nDamage; }
	CPlayer * GetOwner() const { return m_cOwner; }
	SpellType GetType() const { return m_eType; }
};

#endif