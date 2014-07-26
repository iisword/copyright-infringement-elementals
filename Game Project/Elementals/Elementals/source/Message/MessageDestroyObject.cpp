#include "MessageDestroyObject.h"
#include "../Object/Object.h"

CMessageDestroyObject::CMessageDestroyObject(CObject * pObject) : CMessage(MESSAGE_DESTROYOBJECT)
{
	m_pObject = pObject;
	m_pObject->AddRef();
}

CMessageDestroyObject::~CMessageDestroyObject(void)
{
	m_pObject->Release();
	m_pObject = nullptr;
}

CObject * CMessageDestroyObject::GetEntity() const
{
	return m_pObject;
}