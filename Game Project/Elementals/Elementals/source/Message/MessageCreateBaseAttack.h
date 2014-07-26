#pragma once
#include "Message.h"

#include <DirectXMath.h>
using namespace DirectX;

class CMessageCreateBaseAttack : public CMessage
{
public:
	CMessageCreateBaseAttack(XMFLOAT3 fPosition, XMFLOAT3 fForward, XMFLOAT3 fVelocity, int nElement, float fLifetime);
	~CMessageCreateBaseAttack(void);

	float GetLifetime() const;
	int GetElement() const;
	XMFLOAT3 GetPosition() const; 
	XMFLOAT3 GetForward() const; 
	XMFLOAT3 GetVelocity() const; 

private:
	XMFLOAT3 m_fPosition;
	XMFLOAT3 m_fForward;
	XMFLOAT3 m_fVelocity;
	int m_nElement;
	float m_fLifetime;
};