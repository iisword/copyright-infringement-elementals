#include "ServerFactory.h"
#include "DeathmatchServer.h"
#include "..\Utilities\Utilities.h"
#include <string>
#include <assert.h>
using namespace std;
CServer * CServerFactory::CreateBaseServer(unsigned int nMaxPlayers)
{
	CServer * cReturnServer = new CServer(nMaxPlayers);

	return cReturnServer;
}

CServer * CServerFactory::CreateDeathMatchServer(unsigned int nMaxPlayers, unsigned int nMaxKills, float fMaxTime)
{
	CDeathmatchServer * cReturnServer = new CDeathmatchServer(nMaxPlayers, nMaxKills, fMaxTime);
	return cReturnServer;
}

void CServerFactory::CreateServerDataFolder()
{
	wstring szFolderPath = Utilities::GetCommonFolder(FOLDERID_Documents);
	szFolderPath += SERVER_FOLDER;
	if(!Utilities::PathIsValid(szFolderPath, true))
	{
		if(!SUCCEEDED(CreateDirectory(szFolderPath.c_str(), 0)))
		{
			assert(false && "ERROR: Failed to create data folder for server.");
			return;
		}
	}

	wstring szMapFolder;
	wstring szConfigFile = szMapFolder = szFolderPath;
	szConfigFile += SERVER_CONFIG_FILE;
	if(!Utilities::PathIsValid(szConfigFile, false))
	{
		if(!SUCCEEDED(CopyFile(L"assets\\defaults\\config.con", szConfigFile.c_str(), true)))
		{
			assert(false && "ERROR: Failed to create default config file.");
			return;
		}
	}
}

CServer * CServerFactory::CreateServerUsingConfigFile()
{
	return nullptr;
}