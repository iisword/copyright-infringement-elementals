/***********************************************
* Filename:  		ServerFactory.h
* Date:      		7/6/2014
* Mod. Date: 		7/6/2014
* Mod. Initials:	MB
* Author:    		Michael Boodle
* Purpose:   		A factory used to create server classes
************************************************/

#ifndef SERVERFACTORY_HEADER
#define SERVERFACTORY_HEADER

class CServer;

#define SERVER_FOLDER						L"\\Elemental Server"
#define SERVER_CONFIG_FILE					L"\\config.con"

class CServerFactory
{
public:
	/**********************************
	*	CreateBaseServer:		Creates a no-win server.
	*
	*	In:						unsigned int nMaxPlayer - The number of players allowed onto the server.
	*
	*	Return:					CServer *
	*
	*	Mod. Date:				7/14/2014
	*	Mod. Initials:			MB
	**********************************/
	static CServer * CreateBaseServer(unsigned int nMaxPlayers);

	/**********************************
	*	CreateDeathMatchServer:			Creates a deathmatch server.
	*
	*	In:								unsigned int nMaxPlayer - The number of players allowed onto the server.
	*									unsigned int nMaxKills - The number of kills needed to win.
	*									float fMaxTime - The amount of time before the server declares a winner.
	*
	*	Return:							CServer *
	*
	*	Mod. Date:						7/14/2014
	*	Mod. Initials:					MB
	**********************************/
	static CServer * CreateDeathMatchServer(unsigned int nMaxPlayers, unsigned int nMaxKills = 15, float fMaxTime = -1.0f);

	/**********************************
	*	CreateServerDataFolder:			Creates the server data folder. This includes the default map, stats, and config file.
	*
	*	Return:							void
	*
	*	Mod. Date:						7/14/2014
	*	Mod. Initials:					MB
	**********************************/
	static void CreateServerDataFolder();

	/**********************************
	*	CreateServerUsingConfigFile:			Creates a server based on the config file.
	*
	*	Return:									CServer *
	*
	*	Mod. Date:								7/14/2014
	*	Mod. Initials:							MB
	**********************************/
	static CServer * CreateServerUsingConfigFile();
};
#endif