/***********************************************
* Filename:  		NetworkConnection.h
* Date:      		7/18/2014
* Mod. Date: 		7/18/2014
* Mod. Initials:	MB
* Author:    		Michael Boodle
* Purpose:   		A class used to control the network.
************************************************/

#ifndef NETWORKCONNECTION_HEADER
#define NETWORKCONNECTION_HEADER

#include <Raknet\RakPeerInterface.h>
#include <Raknet\BitStream.h>
#include "NetworkDefines.h"

#define NETWORKCONNECTION_STATE_CONNECTING		0
#define NETWORKCONNECTION_STATE_UNKNOWN			-1
#define NETWORKCONNECTION_STATE_CONNECTED		1

class INetworkDelegate;

class CNetworkConnection
{
	static RakNet::RakPeerInterface * m_pcPeer;
	INetworkDelegate * m_pcDelegate;
	RakNet::BitStream m_cBitStream;

public:
	CNetworkConnection(INetworkDelegate * pcDelegate = nullptr);
	~CNetworkConnection();

	/**********************************
	*	ClientInitialize:		Initializes Raknet for a client.
	*
	*	In:						Add parameters here
	*
	*	Return:					bool
	*
	*	Mod. Date:				7/18/2014
	*	Mod. Initials:			MB
	**********************************/
	static bool ClientInitialize(const char * szServerAddress, unsigned short sServerPort = DEFAULT_PORT);

	/**********************************
	*	ServerInitialize:		Initializes Raknet for a server.
	*
	*	In:						Add parameters here
	*
	*	Return:					bool
	*
	*	Mod. Date:				7/18/2014
	*	Mod. Initials:			MB
	**********************************/
	static bool ServerInitialize(unsigned int nMaxConnections, unsigned short sPort = DEFAULT_PORT);

	/**********************************
	*	Shutdown:			Add details here
	*
	*	Return:				void
	*
	*	Mod. Date:			7/18/2014
	*	Mod. Initials:		MB
	**********************************/
	static void Shutdown();

	/**********************************
	*	GiveRaknetTime:			Causes the application to sleep so Raknet can do stuff
	*
	*	Return:					void
	*
	*	Mod. Date:				7/18/2014
	*	Mod. Initials:			MB
	**********************************/
	void GiveRaknetTime();

	/**********************************
	*	ProcessNetworkMessages:			Begins looking at network messages and passing them to the 
	*									delegate to be handled.
	*
	*	Return:							unsigned int
	*
	*	Mod. Date:						7/18/2014
	*	Mod. Initials:					MB
	**********************************/
	unsigned int ProcessNetworkMessages(unsigned int nMaxMessages = UINT_MAX);

	/**********************************
	*	PushStreamToNetwork:		Causes the Bitstream to send it's data across the network.
	*
	*	In:							bool bIsBroadcast - Determines if the stream should be broadcasted.
	*								const AddressOrGUID tReceiver - The connection to receive the data.
	*								bool bResetStream - Determines if the stream is reseted.
	*
	*	Return:						void
	*
	*	Mod. Date:					7/18/2014
	*	Mod. Initials:				MB
	**********************************/
	void PushStreamToNetwork(bool bIsBroadcast, const RakNet::AddressOrGUID tReceiver, bool bResetStream = true);

	/**********************************
	*	WriteDataToStream:		Writes data to the bit stream.
	*
	*	In:						const char * szData - The data to be copied into the data stream
	*							unsigned int nDataLength - The length of the data
	*
	*	Return:					void
	*
	*	Mod. Date:				7/18/2014
	*	Mod. Initials:			MB
	**********************************/
	void WriteDataToStream(const char * szData, unsigned int nDataLength);

	/**********************************
	*	ResetStream:			Resets the bit stream.
	*
	*	Return:					void
	*
	*	Mod. Date:				7/18/2014
	*	Mod. Initials:			MB
	**********************************/
	void ResetStream();

	/**********************************
	*	Connect:			Attempts to connect to an address.
	*
	*	In:					const char * szAddress - The address to connect to.
	*						unsigned short sPort - The port to connect to.
	*						const char * szPassword - The password for the server.
	*						int nPasswordLength - The length of the password.
	*
	*	Return:				bool
	*
	*	Mod. Date:			7/18/2014
	*	Mod. Initials:		MB
	**********************************/
	bool Connect(const char * szAddress, unsigned short sPort, const char * szPassword = nullptr, int nPasswordLength = 0);

	/**********************************
	*	ConnectFromFile:	Connects to an address located in a txt file.
	*
	*	In:					const char * szFile - The file containing the address to connect to.
	*
	*	Return:             bool
	*
	*	Mod. Date:          7/21/2014
	*	Mod. Initials:      MB
	**********************************/
	bool ConnectFromFile(const char * szFile, RakNet::SystemAddress * ptAddress = nullptr);

	/**********************************
	*	Disconnect:			Causes Raknet to disconnect a connection.
	*
	*	In:					RakNet::AddressOrGUID tAddressOrGUID - The Address or GUID to disconnect from.
	*						bool bSendNotification - Determines if a notification is sent.
	*
	*	Return:				void
	*
	*	Mod. Date:			7/21/2014
	*	Mod. Initials:		MB
	**********************************/
	void Disconnect(RakNet::AddressOrGUID tAddressOrGUID, bool bSendNotification = true);

	/**********************************
	*	CheckConnection:			Checks to see if a connection has been established.
	*
	*	In:							RakNet::AddressOrGUID tAddressOrGUID - The address/GUID to check a connection with
	*
	*	Return:						int
	*
	*	Mod. Date:					7/21/2014
	*	Mod. Initials:				MB
	**********************************/
	int CheckConnection(RakNet::AddressOrGUID tAddressOrGUID);
	/////////////
	//Accessors//
	/////////////

	/////////////
	//Mutators///
	/////////////
	void SetNetworkDelegate(INetworkDelegate * pcNetworkDelegate);


private:

};

#endif