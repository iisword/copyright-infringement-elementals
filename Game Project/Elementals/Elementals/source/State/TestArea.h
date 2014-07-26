/***********************************************
* Filename:  		TestArea.h
* Date:      		7/17/2014
* Mod. Date: 		7/17/2014
* Mod. Initials:	CP
* Author:    		Charles Peter
* Purpose:   		Gives a non-networked test area for effect and logic testing.
************************************************/



#ifndef TEST_STATE_H
#define TEST_STATE_H

#include "../Networking/NetworkDelegate.h"
#include <Raknet\RakNetTypes.h>

#include "BaseState.h"
#include "..\Renderer\Renderer.h"
#include "..\Renderer\Camera.h"
#include "..\Object\Player.h"
#include "..\Object\ObjectManager.h"
#include "..\Object\Map.h"
#include "../CharacterFlyweight.h"
#include "../Renderer/D3DAnimObject.h"

class CAnimation;
class CAnimationInterpolator;

class CTestArea : public CBaseState
{
private:
	CRenderer* m_pRenderer;
	CText* m_pFont;

	

	CInput* m_pInput;
	CObjectManager* m_pObjectManager;
	CTimer * m_pTimer;
	CCamera camera;
	
	CPlayer * m_cpPlayer;
	TCharacterBaseStats tPlayerStats[4];

	D3DAnimObject* TestAnim;
	
	CMap * m_map;	

	void TestInput(void);

public:
	CTestArea(void);
	~CTestArea(void);

	void Init(CRenderer* r, CText* font);
	void Update(float dTime);
	void Render(void);


	/**********************************
	*	Kill:			Load the base stats 
	*					for all player types
	*					into a struct from an 
	*					xml file
	*
	*	In:				char * pFilePath
	*
	*	Mod. Date:		07/19/2014
	*	Mod. Initials:	TD
	**********************************/
	void LoadPlayerStats(char * pFilePath); //Moving to server side

	//DEBUG RT Please do not DELETE 7-21-2014
	CAnimation* _DEBUG_ANIM;
	CAnimationInterpolator* _DEBUG_INTERPOLATOR;
	CAnimationInterpolator* _ANIM_INTERP;
	CMatrix m_AnimMatrix;
};

#endif