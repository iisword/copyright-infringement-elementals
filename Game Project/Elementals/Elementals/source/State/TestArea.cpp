/***********************************************
* Filename:  		TestArea.cpp
* Date:      		7/17/2014
* Mod. Date: 		7/17/2014
* Mod. Initials:	CP
* Author:    		Charles Peter
* Purpose:   		Gives a non-networked test area for effect and logic testing.
************************************************/

#include "TestArea.h"
#include "MainMenu.h"

#include "..\Message\Event.h"
#include "..\Message\EventSystem.h"
#include "../Utilities/tinyxml.h"
#include "../Utilities/tinystr.h"
#include "../Animation/Animation.h"
#include "../Animation/AnimationInterpolator.h"
#include "../Animation/AnimMesh.h"

CTestArea::CTestArea(void)
{
	SetState(TEST_STATE);
}


CTestArea::~CTestArea(void)
{
	// Deallocate the Object Manager
	if(m_pObjectManager != nullptr)
	{
		m_pObjectManager->RemoveAll();
		delete m_pObjectManager;
		m_pObjectManager = nullptr;
	}	

	delete m_map;
	m_map = nullptr;


	if(TestAnim)
		delete TestAnim;
	

	if(_DEBUG_ANIM)
		delete _DEBUG_ANIM;
	
	if(_DEBUG_INTERPOLATOR)
		delete _DEBUG_INTERPOLATOR;

	if(_ANIM_INTERP)
		delete _ANIM_INTERP;

}

void CTestArea::Init(CRenderer* r, CText* font)
{
	m_pInput = CInput::GetInstance();
	m_pRenderer = r;
	m_pFont = font;

	//Re-draws the screen so you know something is happening.
	string load = "Loading...";
	m_pRenderer->PreRender();
	m_pFont->WriteText(XMFLOAT2(1000, 600), 1.0f, load, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	m_pRenderer->PostRender();
	
	m_pObjectManager = new CObjectManager;

	TCharacterBaseStats tStats;
	m_cpPlayer = new CPlayer(&tStats);
	m_cpPlayer->Init();
	m_cpPlayer->SetMesh(m_pRenderer->AddModel(m_cpPlayer->GetMesh(), "assets/CharacterPlaceholder3.obj"));
	m_cpPlayer->SetTexture(new D2DObject(L"assets/noise.dds", 512.0f, 512.0f));
	CBaseColObj* tempP = new CCapsule();
	((CCapsule*)tempP)->SetTop(XMFLOAT3(m_cpPlayer->GetPosition().x, m_cpPlayer->GetPosition().y+3.0f, m_cpPlayer->GetPosition().z));
	((CCapsule*)tempP)->SetBottom(m_cpPlayer->GetPosition());
	((CCapsule*)tempP)->SetRad(1.5f);
	m_cpPlayer->SetBounds(tempP);
	m_pRenderer->AddD2D(m_cpPlayer->GetTexture());
	m_pObjectManager->AddEntity(m_cpPlayer, m_cpPlayer->GetID());

	m_pTimer = CTimer::GetInstance();

	camera.SetCameraMatrix(m_pRenderer->GetCamera());
	camera.Init();

	LoadPlayerStats("scripts/PlayerStats.xml"); //get ready to get rid of this

	m_map = new CMap;
	m_map->Init();
	m_map->GetMapObject()->SetMesh(m_pRenderer->AddModel(m_map->GetMapObject()->GetMesh(), "assets/LevelMesh.obj"));
	m_map->GetMapObject()->TranslateGlobalX(XMFLOAT3(50, 0, 0));
	m_pObjectManager->AddEntity(m_map->GetMapObject(), m_map->GetMapObject()->GetID());

	//BULLSHIT TEST CODE - DELETE
	CObject * t = new CObject;
	t->SetMesh(m_pRenderer->AddModel(t->GetMesh(), "assets/sphere.obj"));
	t->SetPosition(XMFLOAT3(12.0f, 1.0f, 0.0f));
	CBaseColObj* tempS = new CSphere();
	((CSphere*)tempS)->SetPos(t->GetPosition());
	((CSphere*)tempS)->SetRad(1.0f);
	t->SetBounds(tempS);
	m_pObjectManager->AddEntity(t, t->GetID());

	CObject * cylinder = new CObject;
	cylinder->SetMesh(m_pRenderer->AddModel(cylinder->GetMesh(), "assets/Cylinder.obj"));
	cylinder->Scale(XMFLOAT3(.25f, .25f, .25f));
	cylinder->SetTexture(new D2DObject(L"assets/noise.dds", 512.0f, 512.0f));
	cylinder->SetPShader(m_pRenderer->GetPShader("LavaPShaderNEW"));
	m_pRenderer->AddD2D(cylinder->GetTexture());
	m_pObjectManager->AddEntity(cylinder, cylinder->GetID());
	m_cpPlayer->AddChild(cylinder);

	//Please DO NOT DELETE BELOW-----RT 7-21-2014
#if _DEBUG
	

	

	m_AnimMatrix.MakeIdentity();
	
	m_AnimMatrix.SetPosition(XMFLOAT3(0,5,0));
	m_AnimMatrix.TranslateGlobalX(XMFLOAT3(5.0f,0.0f,0.0f));
	_DEBUG_ANIM = new CAnimation();
	_DEBUG_INTERPOLATOR = new CAnimationInterpolator();
	_ANIM_INTERP = new CAnimationInterpolator();

	bool _D_LOAD = _DEBUG_ANIM->LoadAnimationXML("assets/TestAnim.xml");
	if(_D_LOAD)
		_DEBUG_INTERPOLATOR->SetAnimation(_DEBUG_ANIM);
		_ANIM_INTERP->SetAnimation(_DEBUG_ANIM);
	

	_DEBUG_INTERPOLATOR->CreateBones(m_pRenderer);
	

	TestAnim = new D3DAnimObject();
	bool Result = TestAnim->Init("assets/TestMesh.xml",m_pRenderer->GetDevice(),_ANIM_INTERP->GetBones());


	TestAnim->GetCMatrix()->SetPosition(XMFLOAT3(0,5,0));
	_DEBUG_INTERPOLATOR->SetBindPoses(TestAnim->GetMeshData()->GetBindInverses());
	_ANIM_INTERP->SetBindPoses(TestAnim->GetMeshData()->GetBindInverses());
	
	_DEBUG_INTERPOLATOR->DebugProcess(m_AnimMatrix);
	
	//this is to render the bones
 	for(unsigned int i =0; i < _DEBUG_INTERPOLATOR->InterpolatedJoints().size();++i)
 	{
 		//_DEBUG_INTERPOLATOR->m_vBones[0]->GetMesh()->GetCMatrix()->Scale(XMFLOAT3(0.5f,0.5f,0.5f));
 		m_pObjectManager->AddEntity(_DEBUG_INTERPOLATOR->m_vBones[i],
 			_DEBUG_INTERPOLATOR->m_vBones[i]->GetID());
 	}
#endif
}

void CTestArea::Update(float dTime)
{

	TestInput();
	m_pObjectManager->UpdateAll(dTime);
	//_ANIM_INTERP->AddTime(dTime);
	_ANIM_INTERP->Process();
	_DEBUG_INTERPOLATOR->DebugProcess(m_AnimMatrix);
	//m_pTimer->Signal();//Suggest moving this outside to main loop
}

void CTestArea::Render(void)
{

	m_pObjectManager->RenderAll(m_pRenderer);
#if _DEBUG
	m_pRenderer->RenderAnim3D(TestAnim,nullptr,XMFLOAT4(1.0f,0,0,0));
#endif
	string gme = "Test State";
	m_pFont->WriteText(XMFLOAT2(200.0f, 100.0f), 1.0f, gme, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

}


void CTestArea::TestInput(void)
{
	int speed = 20;
	float time = (float)m_pTimer->Delta();

	if(m_pInput->IsKey('W'))
	{
		m_cpPlayer->TranslateGlobalZ(XMFLOAT3(0, 0, speed*time));
		//m_AnimMatrix.TranslateGlobalZ(XMFLOAT3(0,0,speed*time));
		//TestAnim->GetCMatrix()->TranslateGlobalZ(XMFLOAT3(0,0,speed*time));
		//_DEBUG_INTERPOLATOR->GoToNextKeyFrame();
		//_DEBUG_INTERPOLATOR->AddTime(time);
		//_DEBUG_INTERPOLATOR->Process(m_AnimMatrix);
#if _DEBUG
		//_DEBUG_INTERPOLATOR->AddTime(time);
		//_DEBUG_INTERPOLATOR->Process(m_AnimMatrix);
#endif
		//_DEBUG_INTERPOLATOR->AddTime(time);
		
		//_ANIM_INTERP->AddTime(time);
	
		
		
		//TestAnim->GetCMatrix()->TranslateGlobalZ(XMFLOAT3(0,0,speed*time));

	}
	if(m_pInput->IsKey('A'))
	{
		m_cpPlayer->TranslateGlobalX(XMFLOAT3(-speed*time, 0, 0));
		//m_AnimMatrix.TranslateGlobalX(XMFLOAT3(-speed*time, 0, 0));
//		_DEBUG_INTERPOLATOR->Process(m_AnimMatrix);
//		m_AnimMatrix.TranslateGlobalX(XMFLOAT3(-speed*time, 0, 0));
#if _DEBUG
		//_DEBUG_INTERPOLATOR->Process(m_AnimMatrix);
#endif
		//m_AnimMatrix.TranslateGlobalX(XMFLOAT3(-speed*time, 0, 0));
		
	}
	if(m_pInput->IsKey('S'))
	{
		m_cpPlayer->TranslateGlobalZ(XMFLOAT3(0, 0, -speed*time));
		//m_AnimMatrix.TranslateGlobalZ(XMFLOAT3(0,0,-speed*time));
		//_DEBUG_INTERPOLATOR->GoToPrevKeyFrame();
		//_DEBUG_INTERPOLATOR->AddTime(-time);
		//_DEBUG_INTERPOLATOR->Process(m_AnimMatrix);
		//m_AnimMatrix.TranslateGlobalZ(XMFLOAT3(0,0,-speed*time));
		//_DEBUG_INTERPOLATOR->GoToPrevKeyFrame();
#if _DEBUG
		//_DEBUG_INTERPOLATOR->AddTime(-time);
		//_DEBUG_INTERPOLATOR->Process(m_AnimMatrix);
#endif
		//m_AnimMatrix.TranslateGlobalZ(XMFLOAT3(0,0,-speed*time));
		
		//_DEBUG_INTERPOLATOR->AddTime(-time);
		//_DEBUG_INTERPOLATOR->DebugProcess(m_AnimMatrix);

		//TestAnim->GetCMatrix()->TranslateGlobalZ(XMFLOAT3(0,0,-speed*time));
	}
	if(m_pInput->IsKey('D'))
	{
		m_cpPlayer->TranslateGlobalX(XMFLOAT3(speed*time, 0, 0));
		//m_AnimMatrix.TranslateGlobalX(XMFLOAT3(speed*time, 0, 0));
		//_DEBUG_INTERPOLATOR->Process(m_AnimMatrix);
		//m_AnimMatrix.TranslateGlobalX(XMFLOAT3(speed*time, 0, 0));
#if _DEBUG
		//_DEBUG_INTERPOLATOR->Process(m_AnimMatrix);
#endif
		//m_AnimMatrix.TranslateGlobalX(XMFLOAT3(speed*time, 0, 0));
		
	}
	if(m_pInput->GetPrevPos()->x != m_pInput->GetMouseX())
	{
		m_cpPlayer->AddRotation(.008f*(m_pInput->GetMouseX()-m_pInput->GetPrevPos()->x));
		m_cpPlayer->WorldRotateY(.008f*(m_pInput->GetMouseX()-m_pInput->GetPrevPos()->x));
#if _DEBUG		
		//m_AnimMatrix.WorldRotateY(.008f*(m_pInput->GetMouseX()-m_pInput->GetPrevPos()->x));
		//_DEBUG_INTERPOLATOR->Process(m_AnimMatrix);
		//m_AnimMatrix.WorldRotateY(.008f*(m_pInput->GetMouseX()-m_pInput->GetPrevPos()->x));

		//_DEBUG_INTERPOLATOR->Process(m_AnimMatrix);
#endif
		//m_AnimMatrix.WorldRotateY(.008f*(m_pInput->GetMouseX()-m_pInput->GetPrevPos()->x));
		
	}
	if(m_pInput->GetPrevPos()->y != m_pInput->GetMouseY())
	{
		camera.AdjustFocus(-.1f*(m_pInput->GetMouseY()-m_pInput->GetPrevPos()->y));
		m_pInput->ChangeWheelDelta(0.0f);
	}

	camera.Input(m_cpPlayer->GetMesh()->GetMatrix(), m_cpPlayer->GetRotation());
	m_pRenderer->MoveCamera(camera.GetCameraMatrix());

	if(m_pInput->IsKeyOnce(VK_ESCAPE))
	{
		CBaseState* temp = new CMainMenu();
		temp->Init(m_pRenderer, m_pFont);
		CEventSystem::GetInstance()->SendEvent(CHANGE_STATE, HIGH, temp);
	}

#if _DEBUG

	if(m_pInput->IsKey('O'))
	/*if(GetAsyncKeyState('O') & 0x1)*/
	{
		
		_DEBUG_INTERPOLATOR->AddTime(time);
		_ANIM_INTERP->AddTime(time);
		
	}
	if(m_pInput->IsKey('P'))
	/*if(GetAsyncKeyState('P') & 0x1)*/
	{
		_DEBUG_INTERPOLATOR->AddTime(-time);
		_ANIM_INTERP->AddTime(-time);
	}
#endif
	m_pInput->Reset();
}

void CTestArea::LoadPlayerStats(char * pFilePath)
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
		double dGetVariable;
		pPlayer->Attribute("Health", &tPlayerStats[nPlayerStatIndex].m_nStartHealth);
		pPlayer->Attribute("Mana", &tPlayerStats[nPlayerStatIndex].m_nStartMana);
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
