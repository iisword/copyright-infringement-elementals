#include "CollisionDefines.h"


CBaseColObj::CBaseColObj(void)
{
	ID = _UNDEFCOL;
}

CBaseColObj::~CBaseColObj(void)
{

}



CAABB::CAABB(void)
{
	SetID(_AABB);

	m_fMax = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_fMin = XMFLOAT3(0.0f, 0.0f, 0.0f);
}

CAABB::~CAABB(void)
{

}

void CAABB:: Update(XMFLOAT3 fNPos)
{
	XMFLOAT2 extent;

	XMFLOAT3 abCenter;
	abCenter.x = (m_fMin.x + m_fMax.x) * 0.5f;
	abCenter.y = (m_fMin.y + m_fMax.y) * 0.5f;
	abCenter.z = (m_fMin.z + m_fMax.z) * 0.5f;

	extent.x = m_fMax.x - abCenter.x;
	extent.y = m_fMax.z - abCenter.z;


	abCenter.x = fNPos.x;
	abCenter.z = fNPos.z;

	m_fMax.x = abCenter.x + extent.x;
	m_fMax.z = abCenter.z + extent.y;

	m_fMin.x = abCenter.x - extent.x;
	m_fMin.z = abCenter.z - extent.y;
}

CSphere::CSphere(void)
{
	SetID(_SPHERE);

	m_fPos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_fRad = 0.0f;
}

CSphere::~CSphere(void)
{

}

void CSphere:: Update(XMFLOAT3 fNPos)
{
	m_fPos.x = fNPos.x;
	m_fPos.z = fNPos.z;
}

CCapsule::CCapsule(void)
{
	SetID(_CAPSULE);

	m_fTop = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_fBottom = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_fRad = 0.0f;

}

CCapsule::~CCapsule(void)
{

}

void CCapsule:: Update(XMFLOAT3 fNPos)
{
	m_fTop.x = fNPos.x;
	m_fTop.z = fNPos.z;

	m_fBottom.x = fNPos.x;
	m_fBottom.z = fNPos.z;
}