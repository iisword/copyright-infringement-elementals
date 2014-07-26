/***********************************************
* Filename:  		CMesh.h
* Date:      		7/18/2014
* Mod. Date: 		7/18/2014
* Mod. Initials:	RT
* Author:    		Roman Taran
* Purpose:   		This is a container class that will hold mesh data
************************************************/


#include "..\Utilities\tinyxml.h"
#include "AnimationDefines.h"


#ifndef _CMESH_H_
#define _CMESH_H_


class CMesh
{
	vector <tDXVERT> m_pVerts;
	vector <tTRIANGLE> m_vFaces;
	
public:

	inline vector<tDXVERT>& GetVerts() {return m_pVerts;}
	inline vector<tTRIANGLE>& GetTris() {return m_vFaces;}
	bool LoadMeshFromXML(const char* fileName);

	
	CMesh(void);
	~CMesh(void);
};

#endif