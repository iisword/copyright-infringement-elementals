/***********************************************
* Filename:  		CollisionDefines.h
* Date:      		7/16/2014
* Mod. Date: 		7/16/2014
* Mod. Initials:	RT
* Author:    		Roman Taran
* Purpose:   		Common defines for collison objects
************************************************/

#ifndef COLLISION_DEFINES_H
#define COLLISION_DEFINES_H

#include <DirectXMath.h>
using namespace DirectX;

enum colObj { _AABB, _SPHERE, _CAPSULE, _UNDEFCOL };

class CBaseColObj
{
	int ID;

public:
	CBaseColObj(void);
	virtual ~CBaseColObj(void);

	virtual void Update(XMFLOAT3 fNPos) = 0;

	const int GetID(void) { return ID; } 
	void SetID(colObj _id) { ID = _id; }
};


class CAABB : public CBaseColObj
{
	XMFLOAT3 m_fMax;
	XMFLOAT3 m_fMin;

public:
	CAABB(void);
	~CAABB(void);

	void Update(XMFLOAT3 fNPos);

	const XMFLOAT3 GetMax(void) { return m_fMax; }
	const XMFLOAT3 GetMin(void) { return m_fMin; }

	void SetMax(XMFLOAT3 max) { m_fMax = max; }
	void SetMin(XMFLOAT3 min) { m_fMin = min; }
};

class CSphere : public CBaseColObj
{
	XMFLOAT3 m_fPos;
	float m_fRad;

public:
	CSphere(void);
	~CSphere(void);

	void Update(XMFLOAT3 fNPos);

	const XMFLOAT3 GetPos(void) { return m_fPos; }
	const float GetRad(void) { return m_fRad; }

	void SetPos(XMFLOAT3 pos) { m_fPos = pos; }
	void SetRad(float r) { m_fRad = r; }
};

class CCapsule : public CBaseColObj
{
	XMFLOAT3 m_fTop;
	XMFLOAT3 m_fBottom;
	float m_fRad;

public:
	CCapsule(void);
	~CCapsule(void);

	void Update(XMFLOAT3 fNPos);

	const XMFLOAT3 GetTop(void) { return m_fTop; }
	const XMFLOAT3 GetBottom(void) { return m_fBottom; }
	const float GetRad(void) { return m_fRad; }

	void SetTop(XMFLOAT3 top) { m_fTop = top; }
	void SetBottom(XMFLOAT3 bot) { m_fBottom = bot; }
	void SetRad(float r) { m_fRad = r; }

};

#endif