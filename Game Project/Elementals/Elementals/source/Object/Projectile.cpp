#include "Projectile.h"
#include "Player.h"
#include "../CharacterFlyweight.h"
#include "../Renderer/Renderer.h"
#include "../Message/MessageSystem.h"
#include "../Message/MessageDestroyObject.h"

CProjectile::CProjectile() : m_pcOwner(nullptr), m_ptStats(nullptr), m_chType(0)
{
	SetID(PROJECTILE);
	m_fCurrLifetime = 0.0f;
}

CProjectile::~CProjectile()
{
	if(m_pcOwner)
	{
		m_pcOwner->Release();
	}
}

void CProjectile::Update(float fDT)
{
	m_fCurrLifetime += fDT;

#ifndef SERVER_BUILD
	if(m_ptStats)
	{
		if(IsDead())
		{
			CMessageDestroyObject * pMsg = new CMessageDestroyObject(this);
			CMessageSystem::GetInstance()->SendMsg(pMsg);
			pMsg = nullptr;
		}
	}
	else if(m_fLifetime <= m_fCurrLifetime)
	{
		CMessageDestroyObject * pMsg = new CMessageDestroyObject(this);
		CMessageSystem::GetInstance()->SendMsg(pMsg);
		pMsg = nullptr;
	}
	/*
	XMFLOAT3 tVelocity;
	XMVECTOR tTimeBaseVelocity = XMLoadFloat3(&m_fVelocity);
	tTimeBaseVelocity *= fDT;
	XMStoreFloat3(&tVelocity, tTimeBaseVelocity);


	GetMesh()->GetCMatrix()->TranslateGlobalZ(tVelocity);//, GetForward());
	*/

	XMFLOAT3 tPosition = GetPosition();
	XMVECTOR tNewPosition = XMLoadFloat3(&tPosition);
	XMVECTOR tVelocity = XMLoadFloat3(&m_fVelocity) * fDT;
	tNewPosition += tVelocity;
	XMStoreFloat3(&tPosition, tNewPosition);
	SetPosition(tPosition);
#else
	XMFLOAT3 tPosition = GetPosition();
	XMVECTOR tNewPosition = XMLoadFloat3(&tPosition);
	XMVECTOR tVelocity = XMLoadFloat3(&m_fVelocity);
	tNewPosition += tVelocity * fDT;
	XMStoreFloat3(&tPosition, tNewPosition);
	SetPosition(tPosition);
#endif
}

void CProjectile::CalculateVelocity()
{
	XMFLOAT3 tForward = GetForward();
	XMVECTOR tNewVelocity = XMLoadFloat3(&tForward);
	if(m_ptStats)
	{
		tNewVelocity *= m_ptStats->m_fBaseSpeed;
	}
	XMStoreFloat3(&m_fVelocity, tNewVelocity);
}

bool CProjectile::IsDead() const
{
	if(m_ptStats)
	{
		return m_fCurrLifetime >= m_ptStats->m_fAliveTime;
	}
	return m_fLifetime <= m_fCurrLifetime;
}

void CProjectile::Render(CRenderer * cpRenderer)
{
	cpRenderer->Render3D(GetMesh(), GetTexture(), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
}

XMFLOAT3 CProjectile::GetVelocity() const
{
	return m_fVelocity;
}

int CProjectile::GetElement() const
{
	if(m_pcOwner)
	{
		return m_pcOwner->GetType();
	}
	return -1;
}

float CProjectile::GetLifetime() const
{
	if(m_ptStats)
	{
		return m_ptStats->m_fAliveTime;
	}
	return m_fLifetime;
}

float CProjectile::GetTime() const
{
	return m_fCurrLifetime;
}

unsigned char CProjectile::GetGameID() const
{
	return m_chGameID;
}

CPlayer * CProjectile::GetOwner() const
{
	return m_pcOwner;
}

unsigned char CProjectile::GetType() const
{
	return m_chType;
}

const TSpellStats * CProjectile::GetStats() const
{
	return m_ptStats;
}

void CProjectile::SetVelocity(XMFLOAT3 fVelocity)
{
	m_fVelocity = fVelocity;
}

void CProjectile::SetElement(int nElement)
{
	//m_nElement = nElement;
}

void CProjectile::SetLifetime(float fLifetime)
{
	m_fLifetime = fLifetime;
}

void CProjectile::SetTime(float fCurrentTime)
{
	m_fCurrLifetime = fCurrentTime;
}

void CProjectile::SetGameID(unsigned char chGameID)
{
	m_chGameID = chGameID;
}

void CProjectile::SetOwner(CPlayer * pcOwner)
{
	if(m_pcOwner)
	{
		m_pcOwner->Release();
	}

	m_pcOwner = pcOwner;

	if(pcOwner)
	{
		pcOwner->AddRef();
		if(pcOwner->GetStats())
		{
			m_ptStats = &pcOwner->GetStats()->m_tSpellStats[m_chType];
		}
	}
}

void CProjectile::SetType(unsigned char chType)
{
	m_chType = chType;
}

void CProjectile::SetStats(const TSpellStats * pcStats)
{
	m_ptStats = pcStats;
}