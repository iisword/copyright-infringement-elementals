#include "ObjectManager.h"
//#include "Object.h"
#include "../Renderer/Renderer.h"

CObjectManager::CObjectManager(void)
{
	m_bIterating = false;
}

CObjectManager::~CObjectManager(void)
{
	RemoveAll();
}

void CObjectManager::AddEntity (CObject* pObject, unsigned int nType)
{
	if(nType >= m_tObjects.size())
		m_tObjects.resize(nType + 1);

	m_tObjects[nType].push_back(pObject);
	pObject->AddRef();
}

void CObjectManager::RemoveEntity (CObject* pObject, unsigned int nType)
{
	ObjectVector & vec = m_tObjects[nType];

	for(int i = (int)vec.size() - 1; i >= 0; i--)
	{
		if(vec[i] == pObject)
		{
			pObject->Release();
			vec.erase(vec.begin() + i);
			break;
		}
	}
}

void CObjectManager::RemoveAll (void)
{
	m_bIterating = true;

	for(int type = (int)m_tObjects.size() - 1; type >= 0; type--)
	{
		ObjectVector & vec = m_tObjects[type];

		for(int i = (int)vec.size() - 1; i >= 0; i--)
		{
			vec[i]->Release();
		}
	}

	m_tObjects.clear();

	m_bIterating = false;
}

void CObjectManager::UpdateAll (float fDT)
{
	m_bIterating = true;

	for(int type = (int)m_tObjects.size() - 1; type >= 0; type--)
	{
		ObjectVector & vec = m_tObjects[type];
		
		for(int i = (int)vec.size() - 1; i >= 0; i--)
		{
			vec[i]->Update(fDT);
			//Checks the player's collsion againt lvl objects.
			if(type == PLAYER)
			{
				for(int j = (int)m_tObjects[LEVEL].size() - 1; j >= 0; j--)
				{
					//Should acctually send messages to the proper colliding objects to tell them. "Hey we're colliding"
					bool temp = m_cCol.CheckCollision(vec[i]->GetBounds(), m_tObjects[LEVEL][j]->GetBounds());
					m_tObjects[LEVEL][j]->SetCol(temp);
				}
			}
		}
	}

	m_bIterating = false;
}

void CObjectManager::RenderAll (CRenderer * pRenderer)
{
	m_bIterating = true;
	
	for(int type = (int)m_tObjects.size() - 1; type >= 0; type--)
	{
		ObjectVector & vec = m_tObjects[type];

		for(int i = (int)vec.size() - 1; i >= 0; i--)
		{
			vec[i]->Render(pRenderer);
		}
	}

	m_bIterating = false;
}
