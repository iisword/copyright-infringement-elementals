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

D2DObject * CObject::GetTexture() const
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

void CObject::SetTexture(D2DObject * cpTexture)
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
	for(unsigned int i = 0; i < m_pChildren.size(); i++)
		m_pChildren[i]->SetPosition(tNewPosition);
}

void CObject::SetForward(XMFLOAT3 & tNewForward)
{
#ifndef SERVER_BUILD
	m_cpMesh->GetCMatrix()->SetForward(tNewForward);
#else
	m_tForward = tNewForward;
#endif
	for(unsigned int i = 0; i < m_pChildren.size(); i++)
		m_pChildren[i]->SetForward(tNewForward);
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

void CObject::LocalRotateX(float rot)
{
	m_cpMesh->GetCMatrix()->LocalRotateX(rot);
	for(unsigned int i=0; i < m_pChildren.size(); i++)
		m_pChildren[i]->LocalRotateX(rot);
}

void CObject::LocalRotateY(float rot)
{
	m_cpMesh->GetCMatrix()->LocalRotateY(rot);
	for(unsigned int i=0; i < m_pChildren.size(); i++)
		m_pChildren[i]->LocalRotateY(rot);
}

void CObject::LocalRotateZ(float rot)
{
	m_cpMesh->GetCMatrix()->LocalRotateZ(rot);
	for(unsigned int i=0; i < m_pChildren.size(); i++)
		m_pChildren[i]->LocalRotateZ(rot);
}

void CObject::WorldRotateX(float rot)
{
	m_cpMesh->GetCMatrix()->WorldRotateX(rot);
	for(unsigned int i=0; i < m_pChildren.size(); i++)
		m_pChildren[i]->WorldRotateX(rot);
}

void CObject::WorldRotateY(float rot)
{
	m_cpMesh->GetCMatrix()->WorldRotateY(rot);
	for(unsigned int i=0; i < m_pChildren.size(); i++)
		m_pChildren[i]->WorldRotateY(rot);
}

void CObject::WorldRotateZ(float rot)
{
	m_cpMesh->GetCMatrix()->WorldRotateZ(rot);
	for(unsigned int i=0; i < m_pChildren.size(); i++)
		m_pChildren[i]->WorldRotateZ(rot);
}

void CObject::TranslateLocal(XMFLOAT3 trans)
{
	m_cpMesh->GetCMatrix()->TranslateLocal(trans);
	for(unsigned int i=0; i < m_pChildren.size(); i++)
		m_pChildren[i]->TranslateLocal(trans);
}

void CObject::TranslateGlobalX(XMFLOAT3 trans)
{
	m_cpMesh->GetCMatrix()->TranslateGlobalX(trans);
	for(unsigned int i=0; i < m_pChildren.size(); i++)
		m_pChildren[i]->TranslateGlobalX(trans);
}

void CObject::TranslateGlobalZ(XMFLOAT3 trans)
{
	m_cpMesh->GetCMatrix()->TranslateGlobalZ(trans);
	for(unsigned int i=0; i < m_pChildren.size(); i++)
		m_pChildren[i]->TranslateGlobalZ(trans);
}

void CObject::TranslateGlobalZ(XMFLOAT3 trans, XMFLOAT3 forward)
{
	m_cpMesh->GetCMatrix()->TranslateGlobalZ(trans, forward);
	for(unsigned int i=0; i < m_pChildren.size(); i++)
		m_pChildren[i]->TranslateGlobalZ(trans, forward);
}

void CObject::Scale(XMFLOAT3 scale)
{
	m_cpMesh->GetCMatrix()->Scale(scale);
	for(unsigned int i=0; i < m_pChildren.size(); i++)
		m_pChildren[i]->Scale(scale);
}

void CObject::MatMult(XMFLOAT4X4 * matIn)
{
	m_cpMesh->GetCMatrix()->MatMult(matIn);
	for(unsigned int i=0; i < m_pChildren.size(); i++)
		m_pChildren[i]->MatMult(matIn);
}

void CObject::MakeIdentity()
{
	m_cpMesh->GetCMatrix()->MakeIdentity();
	for(unsigned int i=0; i < m_pChildren.size(); i++)
		m_pChildren[i]->MakeIdentity();
}


