#pragma once
#include "Message.h"

class CObject;

class CMessageDestroyObject : public CMessage
{
public:
	CMessageDestroyObject(CObject * pObject);
	virtual ~CMessageDestroyObject(void);

	CObject * GetEntity() const;

private:
	CObject * m_pObject;
};