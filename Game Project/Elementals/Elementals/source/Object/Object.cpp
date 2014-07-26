/***********************************************
* Filename:  		Object.cpp
* Date:      		7/15/2014
* Mod. Date: 		7/15/2014
* Mod. Initials:	MB
* Author:    		Michael Boodle
* Purpose:   		The base object class.
************************************************/
#include "Object.h"
#include "../Renderer/Renderer.h"
#include "..\Collision\CollisionDefines.h"
#include "../Renderer/D3DObjects.h"

CObject::CObject()
{
	SetID(LEVEL);
	m_nRefCount = 0;
	m_cpMesh = nullptr;
	m_cpTexture = nullptr;
	m_cpBounds = nullptr;
	m_pChildren.push_back(this);
	m_bColliding = false;
}

CObject::~CObject()
{
	if(m_cpMesh)
		delete m_cpMesh;
	m_cpMesh = nullptr;
	if(m_cpTexture)
		delete m_cpTexture;
	m_cpTexture = nullptr;

	if(m_cpBounds != nullptr)
	{
		delete m_cpBounds;
		m_cpBounds = nullptr;
	}

}

void CObject::Update(float fDT)
{
	if(m_cpBounds != nullptr)
	{
		//I think this should call the childs update
		m_cpBounds->Update(GetPosition());
	}

}

void CObject::Render(CRenderer * cpRenderer)
{
	if(m_bColliding == false)
	{
		cpRenderer->Render3D(m_cpMesh, m_cpTexture, XMFLOAT4(.5f, .5f, .5f, 1.0f));
	}
	else
	{
		cpRenderer->Render3D(m_cpMesh, m_cpTexture, XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f));
	}
}

void CObject::AddRef(void)
{
	++m_nRefCount;
}

void CObject::Release(void)
{
	--m_nRefCount;

	if(m_nRefCount == 0)
	{
		delete this;
	}
}

D3DObject * CObject::GetMesh() const
{
	return m_cpMesh;
}

D2Dobject * CObject::GetTexture() const
{
	return m_cpTexture;
}

OBJECT_ID CObject::GetID() const
{
	return m_eID;
}

XMFLOAT3 CObject::GetPosition() const
{
#ifndef SERVER_BUILD
	return GetMesh()->GetPosition();
#else
	return m_tPosition;
#endif
}

XMFLOAT3 CObject::GetForward() const
{
#ifndef SERVER_BUILD
	return GetMesh()->GetForward();
#else
	return m_tForward;
#endif
}

unsigned int CObject::GetRefCount() const
{
	return m_nRefCount;
}

void CObject::SetMesh(D3DObject * cpMesh)
{
	m_cpMesh = cpMesh;
}

void CObject::CopyMesh(D3DObject * cpMesh)
{
	m_cpMesh = new D3DObject;
	*m_cpMesh = *cpMesh;
}

void CObject::SetTexture(D2Dobject * cpTexture)
{
	m_cpTexture = cpTexture;
}

void CObject::SetID(OBJECT_ID eID)
{
	m_eID = eID;
}

void CObject::SetPosition(XMFLOAT3 & tNewPosition)
{
#ifndef SERVER_BUILD
	m_cpMesh->SetPosition(tNewPosition);
#else
	m_tPosition = tNewPosition;
#endif
	//for(unsigned int i = 0; i < m_pChildren.size(); i++)
	//	m_pChildren[i]->SetPosition(tNewPosition);
}

void CObject::SetForward(XMFLOAT3 & tNewForward)
{
#ifndef SERVER_BUILD
	m_cpMesh->GetCMatrix()->SetForward(tNewForward);
#else
	m_tForward = tNewForward;
#endif
	//for(unsigned int i = 0; i < m_pChildren.size(); i++)
	//	m_pChildren[i]->SetForward(tNewForward);
}

void CObject::SetPShader(ID3D11PixelShader * pShaderIn)
{ 
	m_cpMesh->SetPShader(pShaderIn); 
}

ID3D11PixelShader * CObject::GetShaders()
{ 
	return m_cpMesh->GetShader(); 
}


void CObject::SetBounds(CBaseColObj* nBounds) 
{ 
	if(m_cpBounds != nullptr)
	{
		delete m_cpBounds;
		m_cpBounds = nullptr;
	}
	m_cpBounds = nBounds; 
}
