#ifndef IMESSAGE_HEADER
#define IMESSAGE_HEADER

class IMessage
{
public:
	IMessage(void) {}
	virtual ~IMessage(void) = 0 {}

	virtual int	GetMessageID(void)	= 0 {}

private:
	IMessage(const IMessage&);
	IMessage& operator= (const IMessage&);
};

#endif