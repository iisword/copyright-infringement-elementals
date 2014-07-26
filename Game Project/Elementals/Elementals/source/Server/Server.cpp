#include "Server.h"
#include "../Object/Player.h"
#include "../Object/Map.h"
#include "../Object/Spell.h"
#include "../Object/Projectile.h"
#include "../Networking/NetworkDefines.h"
#include "../Utilities/tinyxml.h"
#include "../CharacterFlyweight.h"
#include <Raknet\BitStream.h>
#include <fstream>
#include <iostream>
CServer::CServer(unsigned int nMaxClients) : m_nMaxClients(nMaxClients), m_fTotalTime(0.0f), m_nConnectedClients(0), m_nSequence(0), m_chState(SERVERSTATE_OFFLINE), m_nActiveClients(0), m_fSnapshotTime(0.0f), m_nSpellCasts(0)
{
	//m_pcPeer = RakNet::RakPeerInterface::GetInstance();
}

CServer::~CServer()
{
	for(unsigned int nIndex = 0; nIndex < m_vSpells.size(); nIndex++)
	{
		delete m_vSpells[nIndex];
	}

	for(unsigned int nIndex = 0; nIndex < m_vPlayers.size(); nIndex++)
	{
		delete m_vPlayers[nIndex];
	}

	//RakNet::RakPeerInterface::DestroyInstance(m_pcPeer);
}

bool CServer::LoadServerConfig(const char * szFilepath)
{
	if(szFilepath == nullptr)
	{
		cout << "No config file." << endl;
		return false;
	}

	ifstream inFile(szFilepath);
	if(inFile.good())
	{
		//TODO: load base stats
		inFile.close();
		return true;
	}
	return false;
}

bool CServer::LoadCharacterStats(const char * szFilepath)
{
	TiXmlDocument cDocument(szFilepath);
	if(cDocument.LoadFile() == false)
	{
		return false;
	}

	TiXmlElement * pcRoot = cDocument.RootElement();
	if(pcRoot == nullptr)
	{
		return false;
	}

	TiXmlElement * pcElement = pcRoot->FirstChildElement("character");
	if(pcElement == nullptr)
	{
		return false;
	}
#ifdef _DEBUG
	unsigned int nCount = 0;
#endif
	while(pcElement)
	{
#ifdef _DEBUG
		nCount++;
#endif
		TCharacterBaseStats * tStats = new TCharacterBaseStats();
		pcElement->Attribute("element", &tStats->m_eElementType);
		
		//Attributes
		TiXmlElement * pcAttribute = pcElement->FirstChildElement("attribute");
		if(pcAttribute == nullptr)
		{
			printf("WARNING: Stats %i does not have attributes.\n", tStats->m_eElementType);
		}
		else
		{
			double dGetVariable;
			pcAttribute->Attribute("health", &tStats->m_nStartHealth);
			pcAttribute->Attribute("mana", &tStats->m_nStartMana);
			pcAttribute->Attribute("speed", &dGetVariable);
			tStats->m_StartMoveSpeed = (float)dGetVariable;
			pcAttribute->Attribute("castSpeed", &dGetVariable);
			tStats->m_fCastSpeed = (float)dGetVariable;
		}

		//Spells
		TiXmlElement * pcSpell = pcElement->FirstChildElement("spell");
		if(pcSpell != nullptr)
		{
			while(pcSpell)
			{
				int nSlot = -1;
				if(pcSpell->Attribute("type", &nSlot) == nullptr)
				{
					printf("ERROR: No type specified for spell.\n");
					delete tStats;
					return false;
				}
				
				TSpellStats * ptCurrentSpell = &tStats->m_tSpellStats[nSlot];
				TiXmlElement * pcSpellAttribute = pcSpell->FirstChildElement("attribute");
				if(pcSpellAttribute)
				{
					double dGetVariable;
					pcSpellAttribute->Attribute("radius", &dGetVariable);
					ptCurrentSpell->m_fRadius = (float)dGetVariable;
					pcSpellAttribute->Attribute("aliveTime", &dGetVariable);
					ptCurrentSpell->m_fAliveTime = (float)dGetVariable;
					pcSpellAttribute->Attribute("speed", &dGetVariable);
					ptCurrentSpell->m_fBaseSpeed = (float)dGetVariable;
					pcSpellAttribute->Attribute("damage", &dGetVariable);
					ptCurrentSpell->m_fBaseDamage = (float)dGetVariable;
					pcSpellAttribute->Attribute("manaCost", &ptCurrentSpell->m_nManaCost);

				}
				else
				{
					printf("WARNING: The spell stats %i for character %i do not exist.\n", nSlot, tStats->m_eElementType);
				}

				pcSpell = pcSpell->NextSiblingElement("spell");
			}
		}
		else
		{
			printf("ERROR: There are no spells for character stats %i.\n", tStats->m_eElementType);
			delete tStats;
			return false;
		}
		
		m_vStats.push_back(tStats);
		pcElement = pcElement->NextSiblingElement("character");
	}

#ifdef _DEBUG
	printf("Stats were loaded: %i.\n", nCount);
#endif

	return true;
}

void CServer::HandleIncomingData(RakNet::Packet * pcIncomingPacket)
{
	NetworkMessageType eType = (NetworkMessageType)(pcIncomingPacket->data[0]);
	MessageID nReturnType;
	switch (eType)
	{
	case ID_NEW_INCOMING_CONNECTION:
		{
			printf("A person has connected to the server.\n");
		}
		break;
	case ID_CONNECTION_LOST:
		{
			printf("A client has lost connection.\n");
			for(unsigned int nIndex = 0; nIndex < m_nMaxClients; nIndex++)
			{
				if(m_vPlayers[nIndex] == nullptr)
				{
					continue;
				}

				if(pcIncomingPacket->guid == *(m_vPlayers[nIndex]->GetGUID()))
				{
					m_nConnectedClients--;
					if(m_vPlayers[nIndex]->GetPlayerMode() == PLAYERMODE_ACTIVE)
					{
						m_nActiveClients--;
					}
					m_vToBeDeleted.push_back(nIndex);
					RemoveSpellOwnerShip(m_vPlayers[nIndex]);
					delete m_vPlayers[nIndex];
					m_vPlayers[nIndex] = nullptr;
					break;
				}
			}
		}
		break;
	case ID_CLIENT_READY:
		{
			printf("A client is ready to play.\n");
			if(CheckPlayerIsCorrect(pcIncomingPacket->data[1], pcIncomingPacket->guid))
			{
				if(m_vPlayers[pcIncomingPacket->data[1]]->GetPlayerMode() == PLAYERMODE_SPECTATOR)
				{
					m_vPlayers[pcIncomingPacket->data[1]]->SetPlayerMode(PLAYERMODE_ACTIVE);
					m_nActiveClients++;
					//TODO: Spawn the player somewhere
					XMFLOAT4 tPosition(0.0f, 0.0f, 0.0f, 1.0f);
					XMFLOAT4 tForward(0.0f, 0.0f, 1.0f, 0.0f);
					m_vPlayers[pcIncomingPacket->data[1]]->Spawn(tPosition);

					nReturnType = ID_PLAYER_SPAWN;
					m_cConnection.WriteDataToStream((char *)&nReturnType, sizeof(MessageID));
					m_cConnection.WriteDataToStream((char *)pcIncomingPacket->data + 1, sizeof(unsigned char));
					m_cConnection.WriteDataToStream((char *)&tPosition, sizeof(XMFLOAT3));
					m_cConnection.WriteDataToStream((char *)&tForward, sizeof(XMFLOAT3));
					SendToAll();
				}
			}
		}
		break;
	case ID_REQUEST_ID:
		{
			printf("A person wants to have an id.\n");
			if(m_nConnectedClients < m_nMaxClients)
			{
				nReturnType = (MessageID)ID_REQUEST_SUCCESS;
				unsigned char nID;
				for(unsigned int nIndex = 0; nIndex < m_nMaxClients; nIndex++)
				{
					if(m_vPlayers[nIndex] == nullptr)
					{
						printf("Person's id is %i.\n", nIndex);
						m_nConnectedClients++;
						nID = nIndex;
						m_cConnection.WriteDataToStream((char *)&nReturnType, sizeof(MessageID));
						m_cConnection.WriteDataToStream((char *)&nID, sizeof(nID));
						m_vPlayers[nIndex] = CreatePlayer(pcIncomingPacket->guid, (char *)(pcIncomingPacket->data + 1));
						m_vPlayers[nIndex]->SetGameID(nID);
						m_vPlayers[nIndex]->AddRef();
						m_cConnection.PushStreamToNetwork(false, pcIncomingPacket->systemAddress);
						break;
					}
				}

				nReturnType = (MessageID)ID_NEW_SLOT;
				m_cConnection.WriteDataToStream((char *)&nReturnType, sizeof(MessageID));
				m_cConnection.WriteDataToStream((char *)&nID, sizeof(unsigned char));
				m_cConnection.WriteDataToStream((char *)pcIncomingPacket->data + 1, 32);
				SendToAll(nID);
			}
			else
			{
				printf("The server is full.\n");
				m_cConnection.WriteDataToStream((char *)&nReturnType, sizeof(MessageID));
				m_cConnection.PushStreamToNetwork(false, pcIncomingPacket->systemAddress);
			}
		}
		break;
	case ID_CLIENT_ALIVE:
		{
			
		}
		break;
	case ID_CHOSEN_ELEMENT:
		{
			//A client wants to change it's element
			if(CheckPlayerIsCorrect(pcIncomingPacket->data[1], pcIncomingPacket->guid))
			{
				m_vPlayers[pcIncomingPacket->data[1]]->SetStats(m_vStats[pcIncomingPacket->data[2]]);
			}
		}
		break;
	case ID_GET_PLAYER_LIST:
		{
			//A client wants the list of players connected
			if(CheckPlayerIsCorrect(pcIncomingPacket->data[1], pcIncomingPacket->guid))
			{
				unsigned char nNumOfPlayers = m_nConnectedClients;
				nReturnType = ID_PLAYER_LIST;
				m_cConnection.WriteDataToStream((char *)&nReturnType, sizeof(MessageID));
				m_cConnection.WriteDataToStream((char *)&nNumOfPlayers, sizeof(unsigned char));
				for(unsigned char nIndex = 0; nIndex < m_nMaxClients; nIndex++)
				{
					if(m_vPlayers[nIndex] == nullptr)
					{
						continue;
					}

					unsigned char chMode = m_vPlayers[nIndex]->GetPlayerMode();
					unsigned char chElement = m_vPlayers[nIndex]->GetType();
					m_cConnection.WriteDataToStream((char *)&nIndex, sizeof(unsigned char));
					m_cConnection.WriteDataToStream(m_vPlayers[nIndex]->GetName(), PLAYER_NAME_LENGTH);
					m_cConnection.WriteDataToStream((char *)&chMode, sizeof(unsigned char));
					m_cConnection.WriteDataToStream((char *)&chElement, sizeof(unsigned char));
				}

				m_cConnection.PushStreamToNetwork(false, pcIncomingPacket->guid);
			}
		}
		break;
	case ID_GET_SERVER_INFO:
		{
			//A client wants info about the server
			if(CheckPlayerIsCorrect(pcIncomingPacket->data[1], pcIncomingPacket->guid))
			{
				nReturnType = ID_SERVER_INFO;
				m_cConnection.WriteDataToStream((char *)&nReturnType, sizeof(MessageID));
				m_cConnection.WriteDataToStream((char *)&m_fTotalTime, sizeof(float));
				m_cConnection.WriteDataToStream((char *)&m_fSpawnTime, sizeof(float));
				m_cConnection.PushStreamToNetwork(false, pcIncomingPacket->guid);
			}
		}
		break;
	case ID_GET_CHARACTER_STATS:
		{
			//A client wants the character stats
			if(CheckPlayerIsCorrect(pcIncomingPacket->data[1], pcIncomingPacket->guid))
			{
				nReturnType = ID_CHARACTER_STATS;
				m_cConnection.WriteDataToStream((char *)&nReturnType, sizeof(MessageID));
				unsigned char nNumOfStats = m_vStats.size();
				m_cConnection.WriteDataToStream((char *)&nNumOfStats, sizeof(unsigned char));
				for(unsigned int nIndex = 0; nIndex < nNumOfStats; nIndex++)
				{
					m_cConnection.WriteDataToStream((char *)m_vStats[nIndex], sizeof(TCharacterBaseStats));
				}
				m_cConnection.PushStreamToNetwork(false, pcIncomingPacket->guid);
			}
		}
		break;
	case ID_GET_PICKUP:
		{
			//A client wants a list of all the pickups
			if(CheckPlayerIsCorrect(pcIncomingPacket->data[1], pcIncomingPacket->guid))
			{
				nReturnType = ID_PICKUP;
				m_cConnection.WriteDataToStream((char *)&nReturnType, sizeof(MessageID));
				for(unsigned int nIndex = 0; nIndex < m_vPickups.size(); nIndex++)
				{

				}
				m_cConnection.PushStreamToNetwork(false, pcIncomingPacket->guid);
			}
		}
		break;
	case ID_CLIENT_POSITION:
		{
			//A client is sending it's new position
			if(CheckPlayerIsCorrect(pcIncomingPacket->data[1], pcIncomingPacket->guid))
			{
				unsigned char nID = pcIncomingPacket->data[1];
				XMFLOAT3 tPosition, tForward;
				memcpy(&tPosition, pcIncomingPacket->data + 2, sizeof(tPosition));
				memcpy(&tForward, pcIncomingPacket->data + 2 + sizeof(tForward), sizeof(tPosition));

				m_vPlayers[pcIncomingPacket->data[1]]->SetPosition(tPosition);
				m_vPlayers[pcIncomingPacket->data[1]]->SetForward(tForward);
			}
		}
		break;
	case ID_CLIENT_FIRE:
		{
			//A client wants to fire
			if(CheckPlayerIsCorrect(pcIncomingPacket->data[1], pcIncomingPacket->guid))
			{
				unsigned char chType = pcIncomingPacket->data[2];
				if(m_vPlayers[pcIncomingPacket->data[1]]->CanFire(chType))
				{
					
					m_vPlayers[pcIncomingPacket->data[1]]->CastSpell();
					XMFLOAT3 * ptPosition = (XMFLOAT3 *)(pcIncomingPacket->data + 3);
					XMFLOAT3 * ptForward = (XMFLOAT3 *)(pcIncomingPacket->data + 3 + sizeof(XMFLOAT3));
					CPlayer * pcOwner = m_vPlayers[pcIncomingPacket->data[1]];
					CProjectile * cNewProjectile = CreateProjectile(m_nSpellCasts, chType, ptPosition, ptForward, pcOwner);

					m_vSpells.push_back(cNewProjectile);
				
					//Increase the spells
					
					m_nSpellCasts++;
					printf("Player %i is firing the %i spell of type %i.\n", pcIncomingPacket->data[1], m_nSpellCasts, chType);
				}
			}
		}
		break;
	case ID_CLIENT_DISCONNECT:
		{
			if(CheckPlayerIsCorrect(pcIncomingPacket->data[1], pcIncomingPacket->guid))
			{
				printf("Player %i is now disconnecting.\n", pcIncomingPacket->data[1]);
				m_nConnectedClients--;
				if(m_vPlayers[pcIncomingPacket->data[1]]->GetPlayerMode() == PLAYERMODE_ACTIVE)
				{
					m_nActiveClients--;
				}
				m_vToBeDeleted.push_back(pcIncomingPacket->data[1]);
				RemoveSpellOwnerShip(m_vPlayers[pcIncomingPacket->data[1]]);
				m_cConnection.Disconnect(pcIncomingPacket->guid, false);
				delete m_vPlayers[pcIncomingPacket->data[1]];
				m_vPlayers[pcIncomingPacket->data[1]] = nullptr;
			}
		}
		break;
	default:
		{
			printf("ERROR: Unhandled message: %i.\n", pcIncomingPacket->data[0]);
		}
		break;
	}
}

bool CServer::Initialize()
{
	//initialize Raknet
	CNetworkConnection::ServerInitialize(m_nMaxClients + 1);
	m_cConnection.SetNetworkDelegate(this);
	
	//create slots for players
	m_vPlayers.reserve(m_nMaxClients);
	for(unsigned int nIndex = 0; nIndex < m_nMaxClients; nIndex++)
	{
		m_vPlayers.push_back(nullptr);
	}
	
	//TODO: Load Map
	
	//TODO: Get Pickups from Map
	
	return true;
}

void CServer::Update(float fDt)
{
	//increase all times
	//m_nSequence++;
	m_fTotalTime += fDt;
	m_fSnapshotTime += fDt;

	//projectile update
	for(int nIndex = (int)m_vSpells.size() - 1; nIndex >= 0; nIndex--)
	{
		m_vSpells[nIndex]->Update(fDt);
		if(m_vSpells[nIndex]->IsDead())
		{
			delete m_vSpells[nIndex];
			m_vSpells.erase(m_vSpells.cbegin() + nIndex);
		}
	}
	
	//player update
	for(unsigned int nIndex = 0; nIndex < m_nMaxClients; nIndex++)
	{
		if(m_vPlayers[nIndex] == nullptr)
		{
			continue;
		}

		m_vPlayers[nIndex]->Update(fDt);
	}

	//TODO: Update Pickups

	//Handle network messages
	m_cConnection.GiveRaknetTime();
	m_cConnection.ProcessNetworkMessages();

	//Snapshot check
	if(m_nConnectedClients > 0 && m_fSnapshotTime > SNAPSHOT_RATE)
	{
		SendSnapshots();
		m_nSequence++;
		m_fSnapshotTime = 0.0f;
	}

	//TODO: kill update
	for(unsigned int nIndex = 0; nIndex < m_vPlayers.size(); nIndex++)
	{
		if(m_vPlayers[nIndex] == nullptr)
		{
			continue;
		}
		else
		{
			//Collision Logic an Updates
			if(m_vPlayers[nIndex]->GetBounds() != nullptr)
			{ //Updates the players position. 
				m_vPlayers[nIndex]->GetBounds()->Update(m_vPlayers[nIndex]->GetPosition()); 
			}

			for(int nJ = (int)m_vSpells.size() - 1; nJ >= 0;  --nJ)
			{
				if(m_vSpells[nJ]->GetBounds() != nullptr)
				{ 
					m_vSpells[nJ]->GetBounds()->Update(m_vSpells[nJ]->GetPosition()); 
				}

				bool bCol = m_cCol.CheckCollision(m_vPlayers[nIndex]->GetBounds(), m_vSpells[nJ]->GetBounds());
				if(bCol == true)
				{
					//Makes sure the spell have a valid owner and check to make sure you're not killing yourself.
					if(m_vSpells[nJ]->GetOwner() != nullptr && (m_vPlayers[nIndex] != m_vSpells[nJ]->GetOwner()))
					{
						m_vPlayers[nIndex]->TakeDamage((int)m_vSpells[nJ]->GetOwner()->GetStats()->m_tSpellStats[m_vSpells[nJ]->GetType()].m_fBaseDamage);
						if(m_vPlayers[nIndex]->GetHealth() <= 0)
						{
							m_vPlayers[nIndex]->Kill();
							m_vSpells[nJ]->GetOwner()->IncreaseKill();

							//Kill message
							MessageID nMessageType = ID_KILL;
							unsigned char chKilled = m_vPlayers[nIndex]->GetGameID();
							unsigned char chKiller = m_vSpells[nJ]->GetOwner()->GetGameID();
							unsigned char chSpell = m_vSpells[nJ]->GetGameID();
							XMFLOAT3 tSpellLocation = m_vSpells[nJ]->GetPosition();
							m_cConnection.WriteDataToStream((char *)&nMessageType, sizeof(MessageID));
							m_cConnection.WriteDataToStream((char *)&chKilled, sizeof(unsigned char));
							m_cConnection.WriteDataToStream((char *)&chSpell, sizeof(unsigned char));
							m_cConnection.WriteDataToStream((char *)&tSpellLocation, sizeof(XMFLOAT3));
							SendToAll();
						}
						else
						{
							//send a collided message
							MessageID nMessageType = ID_COLLISION;
							unsigned char chColType = COLLISION_PLAYER_TO_SPELL;
							unsigned char chPlayerID = m_vPlayers[nIndex]->GetGameID();
							unsigned char chSpellID = m_vSpells[nIndex]->GetGameID();
							XMFLOAT3 tSpellLocation = m_vSpells[nIndex]->GetPosition();
							m_cConnection.WriteDataToStream((char *)&nMessageType, sizeof(MessageID));
							m_cConnection.WriteDataToStream((char *)&chColType, sizeof(unsigned char));
							m_cConnection.WriteDataToStream((char *)&chPlayerID, sizeof(unsigned char));
							m_cConnection.WriteDataToStream((char *)&chSpellID, sizeof(unsigned char));
							m_cConnection.WriteDataToStream((char *)&tSpellLocation, sizeof(XMFLOAT3));
							SendToAll();
						}

						//clean up spell
						delete m_vSpells[nJ];
						m_vSpells.erase(m_vSpells.begin() + nJ);
						
					}
					else 
					{ //Take a preset amount of damage just to prove collision is working over the network.
						//m_vPlayers[nIndex]->TakeDamage(20); 
					}
				}
			}
		}
	}

	SendOutDeletedClients();


}

void CServer::Shutdown()
{
	m_nConnectedClients = 0;
	
	for(unsigned int nIndex = 0; nIndex < m_vSpells.size(); nIndex++)
	{
		delete m_vSpells[nIndex];
	}
	for(unsigned int nIndex = 0; nIndex < m_vPlayers.size(); nIndex++)
	{
		delete m_vPlayers[nIndex];
	}
	
	m_cConnection.SetNetworkDelegate(nullptr);
	CNetworkConnection::Shutdown();
}

void CServer::SendSnapshots()
{
	//player snapshot
	MessageID nSendType = ID_SNAPSHOT_PLAYER;
	unsigned char nActiveClients = m_nActiveClients;
	m_cConnection.WriteDataToStream((char *)&nSendType, sizeof(MessageID));
	m_cConnection.WriteDataToStream((char *)&nActiveClients, sizeof(unsigned char));
	for(unsigned char nIndex = 0; nIndex < m_nMaxClients; nIndex++)
	{
		if(m_vPlayers[nIndex] == nullptr || m_vPlayers[nIndex]->GetPlayerMode() == PLAYERMODE_SPECTATOR)
		{
			continue;
		}

		XMFLOAT3 tPosition = m_vPlayers[nIndex]->GetPosition();
		XMFLOAT3 tForward = m_vPlayers[nIndex]->GetForward();
		int nH = m_vPlayers[nIndex]->GetHealth();
		int nM = m_vPlayers[nIndex]->GetMana();
		//int nK = m_vPlayers[nIndex]->GetKills();
		m_cConnection.WriteDataToStream((char *)&nIndex, sizeof(unsigned char));
		m_cConnection.WriteDataToStream((char *)&tPosition, sizeof(XMFLOAT3));
		m_cConnection.WriteDataToStream((char *)&tForward, sizeof(XMFLOAT3));
		m_cConnection.WriteDataToStream((char *)&nH, sizeof(int));
		m_cConnection.WriteDataToStream((char *)&nM, sizeof(int));
		//m_cConnection.WriteDataToStream((char *)&nK, sizeof(int));
	}
	SendToAll();

	//Spell snapshot
	nSendType = ID_SNAPSHOT_SPELL;
	
	unsigned char nNumberOfSpells = m_vSpells.size();
	if(nNumberOfSpells > 0)
	{
		m_cConnection.WriteDataToStream((char *)&nSendType, sizeof(MessageID));
		m_cConnection.WriteDataToStream((char *)&nNumberOfSpells, sizeof(unsigned char));
		for(unsigned char nIndex = 0; nIndex < nNumberOfSpells; nIndex++)
		{
			unsigned char chID = m_vSpells[nIndex]->GetGameID();
			XMFLOAT3 tPosition = m_vSpells[nIndex]->GetPosition();
			XMFLOAT3 tForward = m_vSpells[nIndex]->GetForward();
			float fSpellTime = m_vSpells[nIndex]->GetTime();
			m_cConnection.WriteDataToStream((char *)&chID, sizeof(unsigned char));	//GameID
			chID = m_vSpells[nIndex]->GetType();
			m_cConnection.WriteDataToStream((char *)&chID, sizeof(unsigned char));  //SpellType
			chID = m_vSpells[nIndex]->GetOwner()->GetGameID();
			m_cConnection.WriteDataToStream((char *)&chID, sizeof(unsigned char));  //Owner
			m_cConnection.WriteDataToStream((char *)&tPosition, sizeof(XMFLOAT3));  //Position
			m_cConnection.WriteDataToStream((char *)&tForward, sizeof(XMFLOAT3));	//Forward
			m_cConnection.WriteDataToStream((char *)&fSpellTime, sizeof(float));	//CurrentTime
		}
		SendToAll();
	}

	//TODO: Pickup snapshot
	nSendType = ID_SNAPSHOT_PICKUP;
}

#pragma region Player Manipulation Functions
bool CServer::AddPlayer(CPlayer * cPlayer)
{
	if(ContainsPlayer(cPlayer) != -1)
	{
		return false;
	}

	if(m_nMaxClients >= m_vPlayers.size())
	{
		return false;
	}

	int nInsertIndex = -1;
	for(unsigned int nIndex = 0; nIndex < m_nMaxClients; nIndex++)
	{
		if(m_vPlayers[nIndex] == nullptr)
		{
			nInsertIndex = nIndex;
			break;
		}
	}

	m_vPlayers[nInsertIndex] = cPlayer;
	m_nConnectedClients++;
	return true;
}

void CServer::RemovePlayer(CPlayer * cPlayer)
{
	int nRemoveIndex = -1;
	for(unsigned int nIndex = 0; nIndex < m_nMaxClients; nIndex++)
	{
		if(m_vPlayers[nIndex] == cPlayer)
		{
			nRemoveIndex = nIndex;
			break;
		}
	}

	if(nRemoveIndex == -1)
	{
		return;
	}

	m_vPlayers[nRemoveIndex] = nullptr;
	m_nConnectedClients--;
}

void CServer::RemovePlayer(int nIndex)
{
	if(nIndex < 0 || nIndex >= (int)m_nMaxClients)
	{
		return;
	}
	
	if(m_vPlayers[nIndex] != nullptr)
	{
		delete m_vPlayers[nIndex];
		m_vPlayers[nIndex] = nullptr;
		m_nConnectedClients--;
	}
}


int CServer::ContainsPlayer(CPlayer * cPlayer)
{
	for(unsigned int nIndex = 0; nIndex < m_vPlayers.size(); nIndex++)
	{
		if(m_vPlayers[nIndex] == cPlayer)
		{
			return nIndex;
		}
	}
	return -1;
}

CPlayer * CServer::CreatePlayer(RakNet::RakNetGUID & tGUID, const char * szName)
{
	//TODO: Create a player
	CPlayer * pcNewPlayer = new CPlayer();
	pcNewPlayer->SetGUID(&tGUID);
	pcNewPlayer->SetName(szName);

	//Gives the player a bounds, should be set correctly in the Update, when update is called on the bounds. 
	CBaseColObj* ctempC = new CCapsule();
	((CCapsule*)ctempC)->SetTop(XMFLOAT3(0.0f, 4.0f, 0.0f));
	((CCapsule*)ctempC)->SetBottom(XMFLOAT3(0.0f, 0.0f, 0.0f));
	((CCapsule*)ctempC)->SetRad(1.5f);
	pcNewPlayer->SetBounds(ctempC);

	return pcNewPlayer;
}

bool CServer::CheckPlayerIsCorrect(char chID, RakNet::RakNetGUID & tGUID)
{
	if(m_vPlayers[chID] != nullptr)
	{
		if(tGUID == *(m_vPlayers[chID]->GetGUID()))
		{
			return true;
		}
	}
	return false;
}

void CServer::SendToAll(char nID)
{
	if(nID == -1)
	{
		for(unsigned int nIndex = 0; nIndex < m_nMaxClients; nIndex++)
		{
			if(m_vPlayers[nIndex] == nullptr)
			{
				continue;
			}

			m_cConnection.PushStreamToNetwork(false, *(m_vPlayers[nIndex]->GetGUID()), false);
		}
		m_cConnection.ResetStream();
	}
	else
	{
		for(unsigned int nIndex = 0; nIndex < m_nMaxClients; nIndex++)
		{
			if(m_vPlayers[nIndex] == nullptr || nID == nIndex)
			{
				continue;
			}

			m_cConnection.PushStreamToNetwork(false, *(m_vPlayers[nIndex]->GetGUID()), false);
		}
		m_cConnection.ResetStream();
	}
}

void CServer::SendOutDeletedClients()
{
	MessageID nReturnType = ID_PLAYER_DISCONNECT;
	for(unsigned int nIndex = 0; nIndex < m_vToBeDeleted.size(); nIndex++)
	{
		m_cConnection.WriteDataToStream((char *)&nReturnType, sizeof(MessageID));
		m_cConnection.WriteDataToStream((char *)&m_vToBeDeleted[nIndex], sizeof(unsigned char));
		SendToAll();
	}
	m_vToBeDeleted.clear();
}

void CServer::RespawnPlayer(CPlayer* pDeadGuy)
{
	//do things
}


CProjectile * CServer::CreateProjectile(unsigned char chGameID, unsigned char chType, XMFLOAT3 * ptPosition, XMFLOAT3 * ptForward, CPlayer * pcOwner)
{
	CProjectile * pcReturnProjectile = new CProjectile();
	pcReturnProjectile->SetGameID(chGameID);
	pcReturnProjectile->SetType(chType);
	pcReturnProjectile->SetOwner(pcOwner);
	ptPosition->y += 3.0f;
	pcReturnProjectile->SetPosition(*ptPosition);
	pcReturnProjectile->SetForward(*ptForward);
	pcReturnProjectile->CalculateVelocity();

	//Gives the projectile a bounds, Will only work for bolt and base attack.
	CBaseColObj* cTempS = new CSphere();
	((CSphere*)cTempS)->SetPos(pcReturnProjectile->GetPosition());
	((CSphere*)cTempS)->SetRad(1.0f);
	pcReturnProjectile->SetBounds(cTempS);

	return pcReturnProjectile;
}

int CServer::FindProjectile(unsigned char chGameID) const
{
	for(int nProjectileIndex = 0; nProjectileIndex < (int)m_vSpells.size(); nProjectileIndex++)
	{
		if(m_vSpells[nProjectileIndex]->GetGameID() == chGameID)
		{
			return nProjectileIndex;
		}
	}
	return -1;
}

void CServer::RemoveSpellOwnerShip(CPlayer * pcPlayer)
{
	for(unsigned int nProjectileIndex = 0; nProjectileIndex < m_vSpells.size(); nProjectileIndex++)
	{
		if(m_vSpells[nProjectileIndex]->GetOwner() == pcPlayer)
		{
			m_vSpells[nProjectileIndex]->SetOwner(nullptr);
		}
	}
}
#pragma endregion

#pragma region Accessors and Mutators
CPlayer * CServer::GetPlayer(int nIndex) const
{
	if(nIndex < 0 || nIndex >= (int)m_nMaxClients)
	{
		return nullptr;
	}

	return m_vPlayers[nIndex];
}

unsigned int CServer::GetMaxPlayers() const
{
	return m_nMaxClients;
}

unsigned int CServer::GetNumberOfPlayers() const
{
	return m_nConnectedClients;
}

float CServer::GetTotalTime() const
{
	return m_fTotalTime;
}
#pragma endregion