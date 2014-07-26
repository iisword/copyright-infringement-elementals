#include "Message.h"

CMessage::CMessage(MessageTypes type)
{
	msgType = type;
}

CMessage::~CMessage()
{
}
	
int CMessage::GetMessageID(void)
{
	return msgType;
}