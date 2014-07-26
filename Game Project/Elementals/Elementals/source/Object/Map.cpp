#include "Map.h"

CMap::CMap()
{
	c_pMapObject = new CObject;
}

CMap::~CMap()
{
//	delete c_pMapObject;
}

void CMap::Init()
{
	c_pMapObject->SetID(LEVEL);

}