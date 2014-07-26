#ifndef MESSAGE_HEADER
#define MESSAGE_HEADER

#include "IMessage.h"

enum MessageTypes { MESSAGE_CREATEBASEATTACK, MESSAGE_DESTROYOBJECT };


class CMessage : public IMessage
{
private:
	MessageTypes msgType;

public:
	CMessage(MessageTypes type);
	virtual ~CMessage();
	
	virtual int GetMessageID(void) final;
};

#endif