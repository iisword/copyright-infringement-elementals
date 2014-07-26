#pragma once

#include <vector>
#include "..\Collision\Collision.h"
#include "Object.h"
//Changed to #include so i could use the Object Enum - Charlie.
//class CObject;
class CRenderer;

class CObjectManager
{
private:
	// Use typedefs to clarify names
	typedef std::vector< CObject* >	ObjectVector;
	typedef ObjectVector::iterator OVIterator;
	typedef std::vector< ObjectVector >	ObjectTable;

	ObjectTable	m_tObjects;	
	bool m_bIterating;	

	CCollision m_cCol;

public:

	CObjectManager(void);
	~CObjectManager(void);

	void AddEntity (CObject* cpObject, unsigned int nType);
	void RemoveEntity (CObject* cpObject, unsigned int nType);
	void RemoveAll (void);

	void UpdateAll (float fElapsedTime);
	void RenderAll (CRenderer * pRenderer);

	std::vector<CObject*> GetLvlObjs(void) { return m_tObjects[LEVEL]; }

	void Shutdown();
};

