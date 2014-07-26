/***********************************************
* Filename:  		NetworkDelegate.h
* Date:      		7/18/2014
* Mod. Date: 		7/18/2014
* Mod. Initials:	MB
* Author:    		Michael Boodle
* Purpose:   		An interface which is used to handle the incoming
*					packets of data.
************************************************/

#ifndef NETWORKDELEGATE_HEADER
#define NETWORKDELEGATE_HEADER
#include <Raknet\RakNetTypes.h>
class INetworkDelegate
{
public:
	virtual ~INetworkDelegate() {};

	virtual void HandleIncomingData(RakNet::Packet * pcIncomingPacket) = 0;

private:

};

#endif