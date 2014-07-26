#include "MessageCreateBaseAttack.h"

CMessageCreateBaseAttack::CMessageCreateBaseAttack(XMFLOAT3 fPosition, XMFLOAT3 fDirection, XMFLOAT3 fVelocity, int nElement, float fLifetime) : CMessage(MESSAGE_CREATEBASEATTACK)
{
	m_fPosition = fPosition;
	m_fForward = fDirection;
	m_nElement = nElement;
	m_fLifetime = fLifetime;
}

CMessageCreateBaseAttack::~CMessageCreateBaseAttack(void)
{
}

XMFLOAT3 CMessageCreateBaseAttack::GetPosition() const 
{
	return m_fPosition; 
}

XMFLOAT3 CMessageCreateBaseAttack::GetForward() const 
{
	return m_fForward; 
}

XMFLOAT3 CMessageCreateBaseAttack::GetVelocity() const 
{
	return m_fVelocity; 
}

int CMessageCreateBaseAttack::GetElement() const
{
	return m_nElement;
}

float CMessageCreateBaseAttack::GetLifetime() const
{
	return m_fLifetime;
}