#ifndef PROJECTILE_HEADER
#define PROJECTILE_HEADER

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>
#include <DirectXMath.h>
#include <vector>
#include "Object.h"

using namespace DirectX;

struct TSpellStats;
class CRenderer;
class CPlayer;

class CProjectile : public CObject
{
private:
	XMFLOAT3 m_fVelocity;
	CPlayer * m_pcOwner;
	//int m_nElement;
	float m_fLifetime;
	float m_fCurrLifetime;
	unsigned char m_chType;			//This refers to the slot in the players stats.
	unsigned char m_chGameID;
	const TSpellStats * m_ptStats;

public:
	CProjectile();
	virtual ~CProjectile();

	virtual void Update(float fDT);
	virtual void Render(CRenderer * cpRenderer);

	/**********************************
	*	CalculateVelocity:		Used to calculate the velocity of the spell based
	*							on stats and the forward vector. Stats must be set for this
	*							to be effective.
	*
	*	Return:					void
	*
	*	Mod. Date:				7/24/2014
	*	Mod. Initials:			MB
	**********************************/
	void CalculateVelocity();

	/**********************************
	*	IsDead:				Tells if the spell has 
	*
	*	Return:				bool
	*
	*	Mod. Date:			7/24/2014
	*	Mod. Initials:		MB
	**********************************/
	bool IsDead() const;

	XMFLOAT3 GetVelocity() const;
	int GetElement() const;
	float GetLifetime() const;
	float GetTime() const;
	unsigned char GetGameID() const;
	CPlayer * GetOwner() const;
	unsigned char GetType() const;

	void SetVelocity(XMFLOAT3 fVelocity);
	void SetElement(int nElement);
	void SetLifetime(float flifetime);
	void SetTime(float fCurrentTime);
	void SetGameID(unsigned char m_chGameID);
	void SetOwner(CPlayer * pcOwner);
	void SetType(unsigned char chType);
};

#endif