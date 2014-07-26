/***********************************************
* Filename:  		NetworkConnection.cpp
* Date:      		7/18/2014
* Mod. Date: 		7/18/2014
* Mod. Initials:	MB
* Author:    		Michael Boodle
* Purpose:   		A class used to control the network.
************************************************/
#include "NetworkConnection.h"
#include "NetworkDelegate.h"
#include <fstream>

RakNet::RakPeerInterface * CNetworkConnection::m_pcPeer = nullptr;

CNetworkConnection::CNetworkConnection(INetworkDelegate * pcDelegate) : m_pcDelegate(pcDelegate)
{

}

CNetworkConnection::~CNetworkConnection()
{

}

bool CNetworkConnection::ClientInitialize(const char * szServerAddress, unsigned short sServerPort)
{
	if(m_pcPeer == nullptr)
	{
		m_pcPeer = RakNet::RakPeerInterface::GetInstance();
		RakNet::StartupResult eResult = m_pcPeer->Startup(1, &RakNet::SocketDescriptor(), 1, THREAD_PRIORITY_NORMAL);
		if(eResult != RakNet::StartupResult::RAKNET_STARTED)
		{
			return false;
		}

		if(szServerAddress != nullptr)
		{
			m_pcPeer->Connect(szServerAddress, sServerPort, 0, 0);
			Sleep(2);	//Play nice with RakNet
		}
		return true;
	}
	return false;
}

bool CNetworkConnection::ServerInitialize(unsigned int nMaxConnections, unsigned short sPort)
{
	if(m_pcPeer == nullptr)
	{
		m_pcPeer = RakNet::RakPeerInterface::GetInstance();
		RakNet::StartupResult eResult = m_pcPeer->Startup(nMaxConnections, &RakNet::SocketDescriptor(sPort, 0), 1, THREAD_PRIORITY_NORMAL);
		if(eResult != RakNet::StartupResult::RAKNET_STARTED)
		{
			return false;
		}
		printf("Raknet has been initialized.\n");
		printf("Raknet GUID: %s.\n", m_pcPeer->GetMyGUID().ToString());
		printf("Raknet IP: %s|%i.\n", m_pcPeer->GetLocalIP(0), m_pcPeer->GetMyBoundAddress().GetPort());
		m_pcPeer->SetMaximumIncomingConnections(nMaxConnections);
		return true;
	}
	return false;
}

void CNetworkConnection::Shutdown()
{
	if(m_pcPeer)
	{
		RakNet::RakPeerInterface::DestroyInstance(m_pcPeer);
		m_pcPeer = nullptr;
	}

}

void CNetworkConnection::GiveRaknetTime()
{
	Sleep(5);
}

unsigned int CNetworkConnection::ProcessNetworkMessages(unsigned int nMaxMessages)
{
	unsigned int nNumOfPacketsServed = 0;
	if(m_pcPeer)
	{
		for(RakNet::Packet * pcIncomingPacket = m_pcPeer->Receive(); pcIncomingPacket; pcIncomingPacket = m_pcPeer->Receive())
		{
			nNumOfPacketsServed++;
			if(m_pcDelegate)
			{
				m_pcDelegate->HandleIncomingData(pcIncomingPacket);
			}
			m_pcPeer->DeallocatePacket(pcIncomingPacket);
			if(nMaxMessages < nNumOfPacketsServed)
			{
				break;
			}
		}
	}
	return nNumOfPacketsServed;
}

void CNetworkConnection::PushStreamToNetwork(bool bIsBroadcast, const RakNet::AddressOrGUID tReceiver, bool bResetStream)
{
	if(m_pcPeer)
	{
		m_pcPeer->Send(&m_cBitStream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, tReceiver, bIsBroadcast);

		if(bResetStream)
		{
			m_cBitStream.Reset();
		}
	}
}

void CNetworkConnection::WriteDataToStream(const char * szData, unsigned int nDataLength)
{
	m_cBitStream.Write(szData, nDataLength);
}

void CNetworkConnection::ResetStream()
{
	m_cBitStream.Reset();
}

bool CNetworkConnection::Connect(const char * szAddress, unsigned short sPort, const char * szPassword, int nPasswordLength)
{
	if(m_pcPeer)
	{
		RakNet::ConnectionAttemptResult eResult = m_pcPeer->Connect(szAddress, sPort, szPassword, nPasswordLength);
		if(eResult != RakNet::CONNECTION_ATTEMPT_STARTED)
		{
			return false;
		}
		printf("Raknet is attempting to connect to %s:%i.\n",szAddress, sPort);
		return true;
	}
	return false;
}

bool CNetworkConnection::ConnectFromFile(const char * szFile, RakNet::SystemAddress * ptAddress)
{
	std::ifstream cFin(szFile);
	if(cFin.good())
	{
		unsigned short sPort = 0;
		char szIpAddress[16] = {};
		cFin.getline(szIpAddress, 16, ':');
		cFin >> sPort;

		cFin.close();
		if(ptAddress != nullptr)
		{
			ptAddress->FromString(szIpAddress);
			if(sPort > 1024)
			{
				ptAddress->SetPortHostOrder(sPort);
			}
		}
		if(sPort < 1024)
		{
			return Connect(szIpAddress, DEFAULT_PORT);
		}
		else
		{
			return Connect(szIpAddress, sPort);
		}
	}
	return false;
}

int CNetworkConnection::CheckConnection(RakNet::AddressOrGUID tAddressOrGUID)
{
	if(m_pcPeer)
	{
		RakNet::ConnectionState eState = m_pcPeer->GetConnectionState(tAddressOrGUID);
		if(eState == RakNet::ConnectionState::IS_CONNECTED)
		{
			return 1;
		}
		else if(eState == RakNet::ConnectionState::IS_CONNECTING)
		{
			return 0;
		}
	}
	return -1;
}

void CNetworkConnection::Disconnect(RakNet::AddressOrGUID tAddressOrGUID, bool bSendNotification)
{
	if(m_pcPeer)
	{
		m_pcPeer->CloseConnection(tAddressOrGUID, bSendNotification);
	}
}

void CNetworkConnection::SetNetworkDelegate(INetworkDelegate * pcNetworkDelegate)
{
	m_pcDelegate = pcNetworkDelegate;
}

