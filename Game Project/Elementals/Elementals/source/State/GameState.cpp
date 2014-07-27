/***********************************************
* Filename:  		GameState.cpp
* Date:      		7/17/2014
* Mod. Date: 		7/17/2014
* Mod. Initials:	CP
* Author:    		Charles Peter
* Purpose:   		Controls the Game.
************************************************/

#include "../Object/Player.h"
#include "../Networking/NetworkConnection.h"
#include "../Networking/NetworkDefines.h"
#include "GameState.h"
#include "PauseState.h"
#include "WinState.h"
#include "../Object/Spell.h"
#include "../Utilities/tinyxml.h"
#include "../Utilities/tinystr.h"
#include "../Animation/Animation.h"
#include "../Animation/AnimationInterpolator.h"

#include "../Object/Projectile.h"
#include "../Message/MessageSystem.h"
#include "../Message/MessageCreateBaseAttack.h"
#include "../Message/MessageDestroyObject.h"

CGameState::CGameState(void) : m_pcConnection(nullptr), m_chNetworkStatus(NETWORKSTATUS_NOCONNECTION)
{
	SetState(GAME);
	m_bPaused = false;
	m_pPause = nullptr;
	m_pObjectManager = nullptr;
	m_pMessageSystem = nullptr;
}

CGameState::~CGameState(void)
{
	if(m_pPause != nullptr)
	{
		delete m_pPause;
		m_pPause = nullptr;
	}

	//m_pMS->UnregisterFromALL(this);
	if(m_pcConnection)
	{
		SendDisconnectMessage();
		delete m_pcConnection;
		m_pcConnection->Disconnect(m_tServerAddress);
		CNetworkConnection::Shutdown();
	}

	// Deallocate the Object Manager
	if(m_pObjectManager != nullptr)
	{
		m_pObjectManager->RemoveAll();
		delete m_pObjectManager;
		m_pObjectManager = nullptr;
	}	

	if(m_pMessageSystem != nullptr)
	{
		m_pMessageSystem->ShutdownMessageSystem();
		m_pMessageSystem = nullptr;
	}

	delete m_pPlayer;
	m_pPlayer = nullptr;
	delete m_pRadar;
	m_pRadar = nullptr;
	delete m_pReticle;
	m_pReticle = nullptr;
	delete m_pSpells;
	m_pSpells = nullptr;
	delete m_pHealth;
	m_pHealth = nullptr;
	delete m_pMana;
	m_pMana = nullptr;
	delete m_pSpellAOE;
	m_pSpellAOE = nullptr;
	delete m_pSpellBolt;
	m_pSpellBolt = nullptr;
	delete m_pSpellCone;
	m_pSpellCone = nullptr;
	delete m_pSpellSphere;
	m_pSpellSphere = nullptr;
	delete m_pScoreboard;
	m_pScoreboard = nullptr;
	delete m_map;
	m_map = nullptr;
}

bool CGameState::LoadMapInfo(const char* file)
{
	return false;
}




void CGameState::Init(CRenderer* r, CText* font)
{
	if(CNetworkConnection::ClientInitialize(nullptr) == false)
	{
		assert(false && "ERROR: FAIL TO START RAKNET");
		exit(23);
	}
	m_pcConnection = new CNetworkConnection(this);
	if(m_pcConnection->ConnectFromFile("assets/defaults/CI_TXT_ServerLocation.txt", &m_tServerAddress) == false)
	{
		assert(false && "ERROR: FAILED TO START CONNECTING");
		exit(24);
	}
	m_chNetworkStatus = NETWORKSTATUS_CONNECTING;

	m_pRenderer = r;
	m_pInput = CInput::GetInstance();
	m_pObjectManager = new CObjectManager;

	TCharacterBaseStats tStats;
	m_cpPlayer = new CPlayer(&tStats);
	m_cpPlayer->Init();
	m_cpPlayer->SetMaxMana(100);
	m_cpPlayer->SetMana(0);
	m_cpPlayer->SetMesh(m_pRenderer->AddModel(m_cpPlayer->GetMesh(), "assets/CharacterPlaceholder3.obj"));
	m_cpPlayer->SetTexture(new D2Dobject(L"assets/CharacterPlaceholder3.dds", 512.0f, 512.0f)); //CharacterPlaceholder3
	m_pRenderer->AddD2D(m_cpPlayer->GetTexture());
	m_pObjectManager->AddEntity(m_cpPlayer, m_cpPlayer->GetID());

	m_pSpellSphere = m_pRenderer->AddModel(m_pSpellSphere, "assets/sphere.obj");

	m_pFont = font;

	m_pMS = CEventSystem::GetInstance();
	m_pTimer = CTimer::GetInstance();
	m_pMessageSystem = CMessageSystem::GetInstance();
	m_pMessageSystem->SetGame(this);
	m_pMessageSystem->InitMessageSystem(&MessageProc);

	camera.SetCameraMatrix(m_pRenderer->GetCamera());
	camera.Init();

	m_fSpawnTimer = -1.0f;


	LoadPlayerStats("scripts/PlayerStats.xml"); //get ready to get rid of this

	m_map = new CMap;
	m_map->Init();
	m_map->GetMapObject()->SetMesh(m_pRenderer->AddModel(m_map->GetMapObject()->GetMesh(), "assets/LevelMesh.obj"));
	m_map->GetMapObject()->GetMesh()->GetCMatrix()->TranslateGlobalX(XMFLOAT3(50, 0, 0));
	m_pObjectManager->AddEntity(m_map->GetMapObject(), m_map->GetMapObject()->GetID());

	m_pSpells = new D2Dobject(L"assets/Hud/Abilities.dds", 512.0f, 128.0f);
	m_pRenderer->AddD2D(m_pSpells);
	m_pPlayer = new D2Dobject(L"assets/Hud/Health.dds", 1024.0f, 256.0f);
	m_pRenderer->AddD2D(m_pPlayer);
	m_pRadar = new D2Dobject(L"assets/Hud/Radar.dds", 256.0f, 256.0f);
	m_pRenderer->AddD2D(m_pRadar);
	m_pReticle = new D2Dobject(L"assets/Hud/Reticle.dds", 64.0f, 64.0f);
	m_pRenderer->AddD2D(m_pReticle);
	m_pHealth = new D2Dobject(L"assets/Hud/HealthBar.dds", 512.0f, 32.0f);
	m_pRenderer->AddD2D(m_pHealth);
	m_pMana = new D2Dobject(L"assets/Hud/ManaBar.dds", 512.0f, 32.0f);
	m_pRenderer->AddD2D(m_pMana);

	m_pSpellAOE = new D2Dobject(L"assets/Hud/Abilities_AOE.dds", 512.0f, 128.0f);
	m_pRenderer->AddD2D(m_pSpellAOE);
	m_pSpellCone = new D2Dobject(L"assets/Hud/Abilities_Cone.dds", 512.0f, 128.0f);
	m_pRenderer->AddD2D(m_pSpellCone);
	m_pSpellBolt = new D2Dobject(L"assets/Hud/Abilities_Bolt.dds", 512.0f, 128.0f);
	m_pRenderer->AddD2D(m_pSpellBolt);

	m_pScoreboard = new D2Dobject(L"assets/Hud/Scoreboard.dds", 512.0f, 512.0f);
	m_pRenderer->AddD2D(m_pScoreboard);

	//BULLSHIT TEST CODE - DELETE
	CObject * t = new CObject;
	t->SetMesh(m_pRenderer->AddModel(t->GetMesh(), "assets/sphere.obj"));
	m_cpPlayer->AddChild(t);
	m_pObjectManager->AddEntity(t, t->GetID());

}

void CGameState::Update(float dTime)
{

	switch (m_chNetworkStatus)
	{
	case NETWORKSTATUS_NOCONNECTION:
		{

		}
		break;
	case NETWORKSTATUS_CONNECTING:
		{
			m_pcConnection->GiveRaknetTime();
			int nState = m_pcConnection->CheckConnection(m_tServerAddress);
			if(nState == NETWORKCONNECTION_STATE_CONNECTED)
			{
				m_chNetworkStatus = NETWORKSTATUS_CONNECTED;
			}
			else if(nState == NETWORKCONNECTION_STATE_UNKNOWN)
			{
				m_chNetworkStatus = NETWORKSTATUS_NOCONNECTION;
			}
		}
		break;
	case NETWORKSTATUS_CONNECTED:
		{
			m_pcConnection->GiveRaknetTime();
			m_pcConnection->ProcessNetworkMessages(10);
			if(m_bPaused == false && m_fSpawnTimer <= 0.0f)
			{
				if(m_pPause != nullptr)
				{
					delete m_pPause;
					m_pPause = nullptr;
				}
				GameInput();
				m_pMessageSystem->ProcessMessages();
			}
			else if(m_pPause != nullptr)
			{
				m_pPause->Update(dTime);
			}
			else if(m_fSpawnTimer > 0.0f)
			{
				m_fSpawnTimer -= dTime;
			}
		}
		break;
	default:
		{

		}
		break;
	}
	//m_pTimer->Signal();//Suggest moving this outside to main loop

	m_pObjectManager->UpdateAll(dTime);
}

void CGameState::Render(void)
{
	if(m_chNetworkStatus == NETWORKSTATUS_CONNECTED)
	{
		if(m_fSpawnTimer > 0.0f)
		{ 
			RenderDead(); 
		}

		m_pObjectManager->RenderAll(m_pRenderer);
		Renderhud();
		string gme = "Game State";
		m_pFont->WriteText(XMFLOAT2(200.0f, 100.0f), 1.0f, gme, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

		if(m_bPaused == true)
		{
			m_pPause->Render();
		}
	}
}

//Pretty much justs moves the camera above the world. 
void CGameState::RenderDead(void)
{
	XMFLOAT4X4 tempCam;
	XMStoreFloat4x4(&tempCam, XMMatrixIdentity());
	tempCam;
	camera.SetCameraMatrix(tempCam);
	camera.GetMesh()->GetCMatrix()->TranslateLocal(XMFLOAT3(-25.0f, 15.0f, 0.0f));
	XMStoreFloat4x4(camera.GetCameraMatrix(), XMMatrixLookAtLH(XMVectorSet(-25.0f, 15.0f, 0.0f, 1.0f), XMVectorSet(10.0f, 0.0f, 0.0f, 0.0f), XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f)));
	m_pRenderer->MoveCamera(camera.GetCameraMatrix());

	string res = "Respawning: ";
	//res += m_fSpawnTimer;
	m_pFont->WriteText(XMFLOAT2(400.0f, 300.0f), 1.0f, res, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

}

//void CGameState::ProcessEvent( CEvent* Event )
//{
//	switch(Event->getID())
//	{
//	case UN_PAUSE:
//		{
//			m_bPaused = false;
//			if(m_pPause != nullptr)
//			{
//				delete m_pPause;
//				m_pPause = nullptr;
//			}
//			//m_pMS->UnregisterFromEvent(UN_PAUSE, this);
//		}
//		break;
//	}
//}


void CGameState::GameInput()
{
	if(m_chNetworkStatus == NETWORKSTATUS_CONNECTED)	//Wait until the network has finished setting up
	{
		int speed = 20;
		float time = (float)m_pTimer->Delta();

		// Movement
		XMFLOAT3 fPrePos = m_cpPlayer->GetPosition();

		if(m_pInput->IsKey('W'))
		{
			for(unsigned int i = 0; i < m_cpPlayer->ChildCount(); i++)
			{
				m_cpPlayer->GetChild(i)->GetMesh()->GetCMatrix()->TranslateGlobalZ(XMFLOAT3(0, 0, speed*time));
			}
		}
		if(m_pInput->IsKey('A'))
		{
			for(unsigned int i = 0; i < m_cpPlayer->ChildCount(); i++)
				m_cpPlayer->GetChild(i)->GetMesh()->GetCMatrix()->TranslateGlobalX(XMFLOAT3(-speed*time, 0, 0));
		}
		if(m_pInput->IsKey('S'))
		{
			for(unsigned int i = 0; i < m_cpPlayer->ChildCount(); i++)
				m_cpPlayer->GetChild(i)->GetMesh()->GetCMatrix()->TranslateGlobalZ(XMFLOAT3(0, 0, -speed*time));
		}
		if(m_pInput->IsKey('D'))
		{
			for(unsigned int i = 0; i < m_cpPlayer->ChildCount(); i++)
				m_cpPlayer->GetChild(i)->GetMesh()->GetCMatrix()->TranslateGlobalX(XMFLOAT3(speed*time, 0, 0));
		}
		
		for(int i = 0; i < (int)m_pObjectManager->GetLvlObjs().size(); ++i)
		{
			if(m_cpPlayer->GetBounds() != nullptr && m_pObjectManager->GetLvlObjs()[i]->GetBounds() != nullptr)
			{
				bool ckCol = m_cCol.CheckCollision(m_cpPlayer->GetBounds(), m_pObjectManager->GetLvlObjs()[i]->GetBounds());
				if(ckCol == true)
				{
					m_cpPlayer->SetPosition(fPrePos);
				}
			}
		}

		// Mouse look
		if(m_pInput->GetPrevPos()->x != m_pInput->GetMouseX())
		{
			m_cpPlayer->AddRotation(.008f*(m_pInput->GetMouseX()-m_pInput->GetPrevPos()->x));
			for(unsigned int i = 0; i < m_cpPlayer->ChildCount(); i++)
				m_cpPlayer->GetChild(i)->GetMesh()->GetCMatrix()->WorldRotateY(.008f*(m_pInput->GetMouseX()-m_pInput->GetPrevPos()->x));
		}
		if(m_pInput->GetPrevPos()->y != m_pInput->GetMouseY())
		{
			camera.AdjustFocus(-.1f*(m_pInput->GetMouseY()-m_pInput->GetPrevPos()->y));
			m_pInput->ChangeWheelDelta(0.0f);
		}

		// select spell
		if(m_pInput->IsKeyOnce('1') || m_pInput->IsKeyOnce('Z'))
		{
			SpellType temp = m_cpPlayer->GetSpell()->GetType();

			if(temp == SpellType::AOE)
			{
				m_cpPlayer->GetSpell()->ChangeType(SpellType::BASE);
			}
			else
			{
				m_cpPlayer->GetSpell()->ChangeType(SpellType::AOE);
			}
		}
		if(m_pInput->IsKeyOnce('2') || m_pInput->IsKeyOnce('X'))
		{
			SpellType temp = m_cpPlayer->GetSpell()->GetType();

			if(temp == SpellType::CONE)
			{
				m_cpPlayer->GetSpell()->ChangeType(SpellType::BASE);
			}
			else
			{
				m_cpPlayer->GetSpell()->ChangeType(SpellType::CONE);
			}
		}
		if(m_pInput->IsKeyOnce('3') || m_pInput->IsKeyOnce('C'))
		{
			SpellType temp = m_cpPlayer->GetSpell()->GetType();

			if(temp == SpellType::BOLT)
			{
				m_cpPlayer->GetSpell()->ChangeType(SpellType::BASE);
			}
			else
			{
				m_cpPlayer->GetSpell()->ChangeType(SpellType::BOLT);
			}
		}

		// cast spell
		if(m_pInput->GetMousePressedL())
		{
			unsigned char chType = (unsigned char)m_cpPlayer->GetSpell()->GetType();
			if(m_cpPlayer->CanFire(chType) && m_cpPlayer->GetPlayerMode() == PLAYERMODE_ACTIVE)
			{
				m_cpPlayer->CastSpell();
				MessageID nMessageType = ID_CLIENT_FIRE;
				unsigned char chID = m_cpPlayer->GetGameID();
				
				XMFLOAT3 tPosition = m_cpPlayer->GetPosition();
				XMFLOAT3 tForward = m_cpPlayer->GetForward();
				m_pcConnection->WriteDataToStream((char *)&nMessageType, sizeof(MessageID));
				m_pcConnection->WriteDataToStream((char *)&chID, sizeof(unsigned char));
				m_pcConnection->WriteDataToStream((char *)&chType, sizeof(unsigned char));
				m_pcConnection->WriteDataToStream((char *)&tPosition, sizeof(XMFLOAT3));
				m_pcConnection->WriteDataToStream((char *)&tForward, sizeof(XMFLOAT3));
				m_pcConnection->PushStreamToNetwork(false, m_tServerAddress);
			}
		}

		camera.Input(m_cpPlayer->GetMesh()->GetMatrix(), m_cpPlayer->GetRotation());
		m_pRenderer->MoveCamera(camera.GetCameraMatrix());

		if(m_pInput->IsKeyOnce(VK_ESCAPE) && m_pPause == nullptr)
		{
			m_pPause = new CPauseState(this);
			m_pPause->Init(m_pRenderer, m_pFont);
			m_bPaused = true;
		}
	}
	CInput::GetInstance()->Reset();

	//	input->Reset();
}

void CGameState::HandleIncomingData(RakNet::Packet * pcIncomingPacket)
{
	NetworkMessageType eMessageType = (NetworkMessageType)(pcIncomingPacket->data[0]);
	MessageID nReturnType;
	switch (eMessageType)
	{
	case ID_CONNECTION_REQUEST_ACCEPTED:
		{
			printf("The connection to the server was successful.\n");
			char szTemp[32] = "WHAT WHAT";
			nReturnType = ID_REQUEST_ID;
			m_pcConnection->WriteDataToStream((char *)&nReturnType, sizeof(MessageID));
			m_pcConnection->WriteDataToStream(szTemp, 32);
			m_pcConnection->PushStreamToNetwork(false, pcIncomingPacket->systemAddress);
			memcpy(&m_tServerAddress,&(pcIncomingPacket->systemAddress), sizeof(RakNet::RakNetGUID));
		}
		break;
	case ID_SNAPSHOT_PLAYER:
		{
			//printf("The snapshot for the players have arrived.\n");
			int nPlayerDataOffset = 1 + (sizeof(XMFLOAT3) * 2) + 8;//12;
			for(unsigned int nIndex = 0; nIndex < pcIncomingPacket->data[1]; nIndex++)
			{
				int nInternalOffset = nIndex * nPlayerDataOffset;
				int nPlayerIndex = FindPlayer(pcIncomingPacket->data[2 + nInternalOffset]);
				nInternalOffset++;
				if(nPlayerIndex != -1)
				{
					if(m_vPlayers[nPlayerIndex] != m_cpPlayer)
					{
						//Update the position of the players
						XMFLOAT3 tNewPosition = *((XMFLOAT3 *)(pcIncomingPacket->data + 2 + nInternalOffset));
						nInternalOffset += sizeof(XMFLOAT3);
						XMFLOAT3 tNewForward = *((XMFLOAT3 *)(pcIncomingPacket->data + 2 + nInternalOffset));
						nInternalOffset += sizeof(XMFLOAT3);
						m_vPlayers[nPlayerIndex]->SetPosition(tNewPosition);
						m_vPlayers[nPlayerIndex]->SetForward(tNewForward);
					}
					else
					{
						nInternalOffset += sizeof(XMFLOAT3) * 2;
					}
					

					int nHealth = *((int *)(pcIncomingPacket->data + 2 + nInternalOffset));
					nInternalOffset += sizeof(int);
					int nMana = *((int *)(pcIncomingPacket->data + 2 + nInternalOffset));
					nInternalOffset += sizeof(int);
					//int nKills = *((int*)(pcIncomingPacket->data + 2 + nInternalOffset));
					

					m_vPlayers[nPlayerIndex]->SetHealth(nHealth);
					m_vPlayers[nPlayerIndex]->SetMana(nMana);
				}
				else
				{
					//You missed a packet
				}
			}

			if(m_cpPlayer->GetPlayerMode() != PLAYERMODE_SPECTATOR)
			{
				nReturnType = ID_CLIENT_POSITION;
				XMFLOAT3 tPosition = m_cpPlayer->GetPosition();
				XMFLOAT3 tForward = m_cpPlayer->GetForward();
				unsigned char nId = m_cpPlayer->GetGameID();
				m_pcConnection->WriteDataToStream((char *)&nReturnType, sizeof(MessageID));
				m_pcConnection->WriteDataToStream((char *)&nId, sizeof(unsigned char));
				m_pcConnection->WriteDataToStream((char *)&tPosition, sizeof(XMFLOAT3));
				m_pcConnection->WriteDataToStream((char *)&tForward, sizeof(XMFLOAT3));
				m_pcConnection->PushStreamToNetwork(false, m_tServerAddress);
			}
		}
		break;
	case ID_SNAPSHOT_SPELL:
		{
			//printf("The snapshot for the spells has arrived.\n");
			unsigned int nSpellOffset = 1 + 1 + 1 + sizeof(XMFLOAT3) * 2 + 4; //id + type + owner + position + forward + time
			for(unsigned char nIndex = 0; nIndex < pcIncomingPacket->data[1]; nIndex++)
			{
				int nInternalOffset = nSpellOffset * nIndex;
				int nSpellIndex = FindSpell(pcIncomingPacket->data[2 + nInternalOffset]);

				if(nSpellIndex != -1)
				{
					//We have the spell
				}
				else
				{
					//The spell doesn't exist
					unsigned char chID = pcIncomingPacket->data[2 + nInternalOffset];
					nInternalOffset++;
					unsigned char chType = pcIncomingPacket->data[2 + nInternalOffset];
					nInternalOffset++;
					int nPlayerIndex = FindPlayer(pcIncomingPacket->data[2 + nInternalOffset]);
					if(nPlayerIndex == -1)
					{
						assert(false && "ERROR: Player does not exist.");
					}
					nInternalOffset++;

					XMFLOAT3 * tPosition = (XMFLOAT3 *)(pcIncomingPacket->data + 2 + nInternalOffset);
					nInternalOffset += sizeof(XMFLOAT3);
					XMFLOAT3 * tForward = (XMFLOAT3 *)(pcIncomingPacket->data + 2 + nInternalOffset);
					nInternalOffset += sizeof(XMFLOAT3);
					float fTime = *((float *)(pcIncomingPacket->data + 2 + nInternalOffset));
					nInternalOffset += 4;
					CProjectile * pcNewProjectile = CreateProjectile(chID, chType, m_vPlayers[nPlayerIndex], tPosition, tForward, fTime);
					m_vProjectile.push_back(pcNewProjectile);
					m_pObjectManager->AddEntity(pcNewProjectile, pcNewProjectile->GetID());
				}
			}
		}
		break;
	case ID_SNAPSHOT_PICKUP:
		{
			printf("The snapshot for the pickups has arrived.\n");
		}
		break;
	case ID_REQUEST_SUCCESS:
		{
			printf("Obtained an id to play with.\n");

			//Set ID to our player
			m_cpPlayer->SetGameID(pcIncomingPacket->data[1]);
			m_vPlayers.push_back(m_cpPlayer);

			

			//Temp setting of stats(REMOVE LATER)
			nReturnType = ID_CHOSEN_ELEMENT;
			m_pcConnection->WriteDataToStream((char *)&nReturnType, sizeof(MessageID));
			unsigned char chElement = 0;
			m_pcConnection->WriteDataToStream((char *)pcIncomingPacket->data + 1, sizeof(unsigned char));
			m_pcConnection->WriteDataToStream((char *)&chElement, sizeof(unsigned char));
			m_pcConnection->PushStreamToNetwork(false, m_tServerAddress);

			//Send for Server information
			nReturnType = ID_GET_SERVER_INFO;
			m_pcConnection->WriteDataToStream((char *)&nReturnType, sizeof(MessageID));
			m_pcConnection->WriteDataToStream((char *)pcIncomingPacket->data + 1, sizeof(unsigned char));
			m_pcConnection->PushStreamToNetwork(false, m_tServerAddress);

			//Send for Player Stats
			nReturnType = ID_GET_CHARACTER_STATS;
			m_pcConnection->WriteDataToStream((char *)&nReturnType, sizeof(MessageID));
			m_pcConnection->WriteDataToStream((char *)pcIncomingPacket->data + 1, sizeof(unsigned char));
			m_pcConnection->PushStreamToNetwork(false, pcIncomingPacket->systemAddress);

			//Send for Player List
			nReturnType = ID_GET_PLAYER_LIST;
			m_pcConnection->WriteDataToStream((char *)&nReturnType, sizeof(MessageID));
			m_pcConnection->WriteDataToStream((char *)pcIncomingPacket->data + 1, sizeof(unsigned char));
			m_pcConnection->PushStreamToNetwork(false, pcIncomingPacket->systemAddress);
		}
		break;
	case ID_REQUEST_FAIL:
		{
			printf("Failed to obtain an id to play.\n");
			//TODO: Change network mode

		}
		break;
	case ID_PLAYER_LIST:
		{
			printf("The list of players has arrived.\n");
			unsigned int nPlayerPacketOffset = 1 + PLAYER_NAME_LENGTH + 2; //id + name + mode + element
			for(unsigned char nIndex = 0; nIndex < pcIncomingPacket->data[1]; nIndex++)
			{
				int nInternalOffset = nIndex * nPlayerPacketOffset;
				int nPlayerIndex = FindPlayer(pcIncomingPacket->data[2 + nInternalOffset]);

				if(nPlayerIndex != -1)
				{
					//have player
				}
				else
				{
					//don't have player
					CPlayer * pcNewPlayer = new CPlayer;
					pcNewPlayer->SetGameID(pcIncomingPacket->data[2 + nInternalOffset]);
					nInternalOffset += 1;
					pcNewPlayer->SetName((char *)(pcIncomingPacket->data + 2 + nInternalOffset));
					nInternalOffset += PLAYER_NAME_LENGTH;
					pcNewPlayer->SetPlayerMode(pcIncomingPacket->data[2 + nInternalOffset]);
					nInternalOffset += 1;
					pcNewPlayer->SetStats(&tPlayerStats[pcIncomingPacket->data[2 + nInternalOffset]]);
					pcNewPlayer->Init();
					pcNewPlayer->SetMesh(m_pRenderer->AddModel(pcNewPlayer->GetMesh(), "assets/CharacterPlaceholder3.obj"));
					pcNewPlayer->SetTexture(new D2Dobject(L"assets/CharacterPlaceholder3.dds", 512.0f, 512.0f));
					m_pRenderer->AddD2D(pcNewPlayer->GetTexture());
					if(pcNewPlayer->GetPlayerMode() == PLAYERMODE_ACTIVE)
					{
						m_pObjectManager->AddEntity(pcNewPlayer, pcNewPlayer->GetID());
					}
					m_vPlayers.push_back(pcNewPlayer);
				}
			}

			nReturnType = ID_CLIENT_READY;
			m_pcConnection->WriteDataToStream((char *)&nReturnType, sizeof(MessageID));
			unsigned char nID = m_cpPlayer->GetGameID();
			m_pcConnection->WriteDataToStream((char *)&nID, sizeof(unsigned char));
			m_pcConnection->PushStreamToNetwork(false, m_tServerAddress);

		}
		break;
	case ID_SERVER_INFO:
		{
			printf("The information for the server has arrived.\n");
			m_fServerSpawnTimer = *((float *)(pcIncomingPacket->data + sizeof(float)));
		}
		break;
	case ID_CHARACTER_STATS:
		{
			printf("The character stats have been received.\n");
			if(pcIncomingPacket->data[1] > 4)
			{
				assert(false && "ERROR: There are more stats loaded on the server than client can handle.");
				printf("ERROR: There are more stats loaded on the server than client can handle: %i.", pcIncomingPacket->data[1]);
			}
			else if(pcIncomingPacket->data[1] != 0)
			{
				for(unsigned int nIndex = 0; nIndex < 4 && nIndex < pcIncomingPacket->data[1]; nIndex++)
				{
					TCharacterBaseStats * ptIncomingStats = (TCharacterBaseStats *)&pcIncomingPacket->data[2];
					tPlayerStats[nIndex] = *ptIncomingStats;
				}
			}

			m_cpPlayer->SetStats(&tPlayerStats[0]); //Temp remove
		}
		break;
	case ID_PICKUP:
		{
			printf("The pickup information has been received.\n");
		}
		break;
	case ID_KILL:
		{
			printf("A player was killed.\n");
			int nKilledIndex = FindPlayer(pcIncomingPacket->data[1]);
			if(nKilledIndex != -1)
			{
				int nKillerIndex = FindPlayer(pcIncomingPacket->data[2]);
				int nSpellIndex = FindSpell(pcIncomingPacket->data[3]);
				XMFLOAT3 tLocation = *((XMFLOAT3 *)(pcIncomingPacket->data + 4));
				if(m_vPlayers[nKilledIndex] == m_cpPlayer)
				{
					//We were killed
					m_fSpawnTimer = m_fServerSpawnTimer;		//Set the spawn time to the appropiate time
				}

				printf("Player %i killed player %i", pcIncomingPacket->data[2], pcIncomingPacket->data[1]);
				m_vPlayers[nKilledIndex]->Kill();	//This should cause the death animation to start.


			}
		}
		break;
	case ID_WIN:
		{
			printf("Someone has won.\n");
			//Change to the win state and set if player won or not. 
		}
		break;
	case ID_COLLISION:
		{
			printf("Something has caused an important collision.\n");
			unsigned char chColType = pcIncomingPacket->data[1];
			unsigned char chPlayerID = pcIncomingPacket->data[2];
			unsigned char chOtherID = pcIncomingPacket->data[3];
			XMFLOAT3 tLocation = *((XMFLOAT3 *)(pcIncomingPacket->data + 4));

			//TODO: Spawn any cool effects necessary based on the collision
			//TODO: Cause the player to flinch or something
			if(chColType == COLLISION_PLAYER_TO_SPELL)
			{
				int nPlayerIndex = FindPlayer(chPlayerID);
				int nSpellIndex = FindSpell(chOtherID);
				if(nPlayerIndex == -1)
				{
					assert(false && "ERROR: The player does not exist on the client side.");
				}

				if(nSpellIndex == -1)
				{
					assert(false && "ERROR: The spell does not exist on the client side.");
				}
				
				//Clean up spell
				m_pObjectManager->RemoveEntity(m_vProjectile[nSpellIndex], m_vProjectile[nSpellIndex]->GetID());
				m_vProjectile.erase(m_vProjectile.begin() + nSpellIndex);
			}
		}
		break;
	case ID_NEW_SLOT:
		{
			printf("A player has connected.\n");
			CPlayer * pcNewPlayer = CreatePlayer(pcIncomingPacket->data[1], (char *)(pcIncomingPacket->data + 2));
			pcNewPlayer->SetStats(&tPlayerStats[0]); //Remove this in the future
			pcNewPlayer->Init();
			//m_pObjectManager->AddEntity(m_cpPlayer, m_cpPlayer->GetID());
			m_vPlayers.push_back(pcNewPlayer);
		}
		break;
	case ID_PLAYER_SPAWN:
		{
			printf("A player has spawned.\n");
			int nPlayerIndex = FindPlayer(pcIncomingPacket->data[1]);
			if(nPlayerIndex != -1)
			{
				
				XMFLOAT3 * tPosition = ((XMFLOAT3 *)(pcIncomingPacket->data + 2));
				XMFLOAT4 tSpawnPoint(tPosition->x, tPosition->y, tPosition->z, 1.0f);
				m_vPlayers[nPlayerIndex]->Spawn(tSpawnPoint);
				m_vPlayers[nPlayerIndex]->SetForward(*((XMFLOAT3 *)(pcIncomingPacket->data + 2 + sizeof(XMFLOAT3))));
				if(m_vPlayers[nPlayerIndex]->GetPlayerMode() == PLAYERMODE_SPECTATOR)
				{
					m_vPlayers[nPlayerIndex]->SetPlayerMode(PLAYERMODE_ACTIVE);
					if(m_vPlayers[nPlayerIndex] != m_cpPlayer)
					{
						m_pObjectManager->AddEntity(m_vPlayers[nPlayerIndex], m_vPlayers[nPlayerIndex]->GetID());
					}
				}
			}
			else
			{

			}
		}
		break;
	case ID_PLAYER_DISCONNECT:
		{
			int nPlayerIndex = FindPlayer(pcIncomingPacket->data[1]);
			if(nPlayerIndex != -1)
			{
				if(m_vPlayers[nPlayerIndex]->GetPlayerMode() == PLAYERMODE_ACTIVE)
				{
					m_pObjectManager->RemoveEntity(m_vPlayers[nPlayerIndex], m_vPlayers[nPlayerIndex]->GetID());
				}
				vector<CPlayer *>::const_iterator playerIter = m_vPlayers.cbegin();
				m_vPlayers.erase(playerIter + nPlayerIndex);
			}
		}
		break;
	default:
		{
			printf("Unknown message: %i.\n", eMessageType);
		}
		break;
	}
}

int CGameState::FindPlayer(unsigned char chID)
{
	for(int nIndex = 0; nIndex < (int)m_vPlayers.size(); nIndex++)
	{
		if(m_vPlayers[nIndex]->GetGameID() == chID)
		{
			return nIndex;
		}
	}
	return -1;
}

int CGameState::FindSpell(unsigned char chID)
{
	for(int nIndex = 0; nIndex < (int)m_vProjectile.size(); nIndex++)
	{
		if(m_vProjectile[nIndex]->GetGameID() == chID)
		{
			return nIndex;
		}
	}
	return -1;
}

void CGameState::LoadPlayerStats(char * pFilePath)
{
	TiXmlDocument cDoc(pFilePath);

	if(cDoc.LoadFile() == false)
		return;

	TiXmlElement * pRoot = cDoc.RootElement(); 

	if(pRoot == nullptr)
		return;

	TiXmlElement * pPlayer = pRoot->FirstChildElement("Player");
	int nPlayerStatIndex = 0;

	while(pPlayer != nullptr)
	{
		pPlayer->Attribute("Health", &tPlayerStats[nPlayerStatIndex].m_nStartHealth);
		pPlayer->Attribute("Mana", &tPlayerStats[nPlayerStatIndex].m_nStartMana);
		double dGetVariable;
		pPlayer->Attribute("Damage", &dGetVariable);
		//tPlayerStats[nPlayerStatIndex].m_fStartDamage = (float)dGetVariable;
		pPlayer->Attribute("SpellCost", &dGetVariable);
		//tPlayerStats[nPlayerStatIndex].m_fSpellCost = (float)dGetVariable;
		pPlayer->Attribute("MoveSpeed", &dGetVariable);
		tPlayerStats[nPlayerStatIndex].m_StartMoveSpeed = (float)dGetVariable;
		pPlayer->Attribute("CastSpeed", &dGetVariable);
		tPlayerStats[nPlayerStatIndex].m_fCastSpeed = (float)dGetVariable;
		pPlayer->Attribute("Element", &tPlayerStats[nPlayerStatIndex].m_eElementType);
		nPlayerStatIndex++;
		pPlayer = pPlayer->NextSiblingElement("Player");
	}
}

void CGameState::Renderhud()
{
	m_pRenderer->Render2D(m_pRadar, XMFLOAT4((float)m_pRenderer->GetWindowWidth() - 122.5f, 5.0f, (float)m_pRenderer->GetWindowWidth() - 5.0f, 122.5f), XMFLOAT4(6.0f, 4.0f, 241.0f, 239.0f));

	RenderScoreBoard();

	RenderPlayerHud();
}

void CGameState::RenderScoreBoard()
{
	m_pRenderer->Render2D(m_pScoreboard, XMFLOAT4(5.0f, 5.0f, 124.0f, 194.5f), XMFLOAT4(14.0f, 9.0f, 252.0f, 388.0f));
	
	const char * temp = "abcdefghijk";
	string name = HudNameResize(temp); // string name = HudNameResize(m_vPlayers[0].GetName());
	m_pFont->WriteText(XMFLOAT2(10.0f, 25.0f), 0.75f, name, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	
	name = HudNameResize(temp); // name = HudNameResize(m_vPlayers[1].GetName());
	m_pFont->WriteText(XMFLOAT2(10.0f, 65.0f), 0.75f, name, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	name = HudNameResize(temp); // name = HudNameResize(m_vPlayers[2].GetName());
	m_pFont->WriteText(XMFLOAT2(10.0f, 110.0f), 0.75f, name, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	
	name = HudNameResize(temp); // name = HudNameResize(m_vPlayers[3].GetName());
	m_pFont->WriteText(XMFLOAT2(10.0f, 150.0f), 0.75f, name, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
}

void CGameState::RenderPlayerHud()
{
	float right = (float)m_pRenderer->GetWindowWidth();
	float bottom = (float)m_pRenderer->GetWindowHeight();
	float middle = right / 2.0f;

	float healthBarLength = 334.0f / 2.0f;
	float healthBarHeight = 20.0f / 2.0f;
	float healthRatio = (float)m_cpPlayer->GetHealth() / (float)m_cpPlayer->GetMaxHealth();
	float healthLeft = middle + 35.0f - (healthBarLength / 2.0f);
	m_pRenderer->Render2D(m_pHealth, XMFLOAT4(healthLeft,
		                                      bottom - 123.0f - healthBarHeight,
											  healthLeft + (healthBarLength * healthRatio),
											  bottom - 123.0f), 
											  XMFLOAT4(0.0f, 0.0f, 336.0f, 22.0f));
	
	float manaBarLength = 339.0f / 2.0f;
	float manaBarHeight = 20.0f / 2.0f;	
	float manaRatio = (float)m_cpPlayer->GetMana() / (float)m_cpPlayer->GetMaxMana();
	float manaLeft = middle + 33.0f - (manaBarLength / 2.0f);
	m_pRenderer->Render2D(m_pMana, XMFLOAT4(manaLeft,
		                                    bottom - 92.0f - manaBarHeight,
											manaLeft + (manaBarLength* manaRatio),
											bottom - 92.0f), 
											XMFLOAT4(0.0f, 0.0f, 339.0f, 20.0f));

	m_pRenderer->Render2D(m_pPlayer, XMFLOAT4(middle - 134.25f, bottom - 156.0f, middle + 134.25f, bottom - 75.0f), XMFLOAT4(6.0f, 5.0f, 543.0f, 175.0f));

	switch(m_cpPlayer->GetSpell()->GetType())
	{
	case SpellType::BASE:
		{
			m_pRenderer->Render2D(m_pSpells, XMFLOAT4(middle - 124.25f, bottom - 75.6f, middle + 124.25f, bottom), XMFLOAT4(7.0f, 3.0f, 362.0f, 111.0f));
		
		}
		break;
	case SpellType::AOE:
		{
			m_pRenderer->Render2D(m_pSpellAOE, XMFLOAT4(middle - 124.25f, bottom - 75.6f, middle + 124.25f, bottom), XMFLOAT4(7.0f, 3.0f, 362.0f, 111.0f));
		}
		break;
	case SpellType::BOLT:
		{
			m_pRenderer->Render2D(m_pSpellBolt, XMFLOAT4(middle - 124.25f, bottom - 75.6f, middle + 124.25f, bottom), XMFLOAT4(7.0f, 3.0f, 362.0f, 111.0f));		
		}
		break;
	case SpellType::CONE:
		{
			m_pRenderer->Render2D(m_pSpellCone, XMFLOAT4(middle - 124.25f, bottom - 75.6f, middle + 124.25f, bottom), XMFLOAT4(7.0f, 3.0f, 362.0f, 111.0f));		
		}
		break;
	}
}

string CGameState::HudNameResize(const char * szName)
{
	string name = szName;
	name.resize(4);
	name += ".";
	return name;
}

void CGameState::CreateProjectile(IMessage * pMsg)
{
	CMessageCreateBaseAttack * pCreateMsg = dynamic_cast<CMessageCreateBaseAttack *>(pMsg);
	CProjectile * pSpell = new CProjectile;
	pSpell->CopyMesh(m_pSpellSphere);
	pSpell->SetElement(pCreateMsg->GetElement());
	pSpell->SetForward(pCreateMsg->GetForward());
	pSpell->SetPosition(pCreateMsg->GetPosition());
	pSpell->SetVelocity(pCreateMsg->GetForward());
	pSpell->SetLifetime(pCreateMsg->GetLifetime());
	pSpell->GetMesh()->GetCMatrix()->TranslateLocal(XMFLOAT3(0.0f, 3.0, 0.0f));
	CBaseColObj* tempS = new CSphere();
	((CSphere*)tempS)->SetPos(pSpell->GetPosition());
	((CSphere*)tempS)->SetRad(1.0f);
	pSpell->SetBounds(tempS);
	m_pObjectManager->AddEntity(pSpell, pSpell->GetID());
}

void CGameState::RemoveObject(CObject * pObject)
{
	if(pObject->GetID() == PROJECTILE)
	{
		unsigned char chGameID = ((CProjectile *)pObject)->GetGameID();
		unsigned int nSpellIndex= FindSpell(chGameID);
		if(nSpellIndex != -1)
		{
			m_vProjectile.erase(m_vProjectile.begin() + nSpellIndex);
		}
	}
	m_pObjectManager->RemoveEntity(pObject, pObject->GetID());
}

void CGameState::MessageProc(IMessage* pMsg, CGameState * pGame)
{
	switch( pMsg->GetMessageID() )
	{
	case MESSAGE_CREATEBASEATTACK:
		{
			pGame->CreateProjectile(pMsg);
		}
		break;
	case MESSAGE_DESTROYOBJECT:
		{
			CMessageDestroyObject* pDestroyMsg = dynamic_cast< CMessageDestroyObject* >( pMsg );
			CObject* pObject = pDestroyMsg->GetEntity();
			
			pGame->RemoveObject(pObject);

		}
		break;
	}
}

void CGameState::SendDisconnectMessage()
{
	if(m_cpPlayer->GetGameID() != UCHAR_MAX)
	{
		unsigned char chID = m_cpPlayer->GetGameID();
		MessageID nMessageType = ID_CLIENT_DISCONNECT;
		m_pcConnection->WriteDataToStream((char *)&nMessageType, sizeof(MessageID));
		m_pcConnection->WriteDataToStream((char *)&chID, sizeof(unsigned char));
		m_pcConnection->PushStreamToNetwork(false, m_tServerAddress);
		Sleep(15);
	}
}

CPlayer * CGameState::CreatePlayer()
{
	CPlayer * pcReturnPlayer = new CPlayer();
	pcReturnPlayer->SetName("Default Name");
	pcReturnPlayer->SetPlayerMode(PLAYERMODE_SPECTATOR);
	pcReturnPlayer->SetStats(nullptr);
	pcReturnPlayer->SetGameID(UCHAR_MAX);
	pcReturnPlayer->SetMesh(m_pRenderer->AddModel(pcReturnPlayer->GetMesh(), "assets/CharacterPlaceholder3.obj"));
	pcReturnPlayer->SetTexture(new D2Dobject(L"assets/CharacterPlaceholder3.dds", 512.0f, 512.0f));
	m_pRenderer->AddD2D(pcReturnPlayer->GetTexture());
	return pcReturnPlayer;
}

CPlayer * CGameState::CreatePlayer(unsigned char chGameID, const char * szName, unsigned char chMode, unsigned char chStats)
{
	CPlayer * pcReturnPlayer = new CPlayer();
	pcReturnPlayer->SetGameID(chGameID);
	pcReturnPlayer->SetName(szName);
	pcReturnPlayer->SetPlayerMode(chMode);
	if(chStats != UCHAR_MAX)
	{
		pcReturnPlayer->SetStats(&tPlayerStats[chStats]);
	}

	pcReturnPlayer->SetMesh(m_pRenderer->AddModel(pcReturnPlayer->GetMesh(), "assets/CharacterPlaceholder3.obj"));
	pcReturnPlayer->SetTexture(new D2Dobject(L"assets/CharacterPlaceholder3.dds", 512.0f, 512.0f));
	m_pRenderer->AddD2D(pcReturnPlayer->GetTexture());
	return pcReturnPlayer;
}

CProjectile * CGameState::CreateProjectile()
{
	CProjectile * pcReturnProjectile = new CProjectile();
	return pcReturnProjectile;
}

CProjectile * CGameState::CreateProjectile(unsigned char chGameID, unsigned char chType, CPlayer * pcOwner, XMFLOAT3 * tPosition, XMFLOAT3 * tForward, float fCurrentTime)
{
	CProjectile * pcReturnProjectile = new CProjectile();
	pcReturnProjectile->SetGameID(chGameID);
	pcReturnProjectile->SetType(chType);
	pcReturnProjectile->SetOwner(pcOwner);
	pcReturnProjectile->SetTime(fCurrentTime);
	pcReturnProjectile->SetMesh(m_pRenderer->AddModel(nullptr, "assets/sphere.obj"));
	pcReturnProjectile->SetPosition(*tPosition);
	pcReturnProjectile->SetForward(*tForward);
	pcReturnProjectile->CalculateVelocity();
	return pcReturnProjectile;
}