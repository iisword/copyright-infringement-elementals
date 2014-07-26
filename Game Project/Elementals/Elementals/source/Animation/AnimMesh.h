/***********************************************
* Filename:  		AnimMesh.h
* Date:      		7/23/2014
* Mod. Date: 		7/23/2014
* Mod. Initials:	RT
* Author:    		Roman Taran
* Purpose:   		This class is a container for animtated meshes
************************************************/
#ifndef _ANIM_MESH_H_
#define _ANIM_MESH_H_

#include "..\Utilities\tinyxml.h"
#include "AnimationDefines.h"


class CAnimMesh
{

	tAnimVert* m_pVerts;
	tTRIANGLE* m_pFaces;
	unsigned int m_nNumVerts;
	unsigned int m_NumTriangles;

	XMFLOAT4X4 * m_pBindPoseMats;
	unsigned int m_nNumBones;
public:

	inline tAnimVert* GetVerts() {return m_pVerts;}
	inline tTRIANGLE* GetTris() {return m_pFaces;}
	inline XMFLOAT4X4* GetBindInverses() {return m_pBindPoseMats;}
	inline unsigned int GetNumBones(){return m_nNumBones;}
	inline unsigned int GetNumVerts(void) {return m_nNumVerts;}
	inline unsigned int GetNumTris(void) {return m_NumTriangles;}
	bool LoadAnimMeshFromXML(const char* fileName);
	bool LoadAnimMeshFromBIN(const char* fileName);

	CAnimMesh(void);
	~CAnimMesh(void);
};

#endif