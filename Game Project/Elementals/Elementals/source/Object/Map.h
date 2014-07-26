/***********************************************
* Filename:  		Map.h
* Date:      		6/12/2014
* Mod. Date: 		6/12/2014
* Mod. Initials:	MB
* Author:    		Michael Boodle
* Purpose:   		Used to build the map for the server.
************************************************/

#ifndef MAP_HEADER
#define MAP_HEADER

#include "Object.h"
#include <vector>

//class CObject;

class CMap
{
	CObject * c_pMapObject;

	struct tSpawnPoint
	{
		//float3 fPos;
		int nVal;
	};

	std::vector<tSpawnPoint> m_vSpawnPoints; //Get rid of this function, Server side

public:
	CMap();
	~CMap();

	//TODO: add a method to find objects by tag
	CObject * GetMapObject(){ return c_pMapObject; };
	void Init();


};

#endif