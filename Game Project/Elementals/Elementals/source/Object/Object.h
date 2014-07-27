/***********************************************
* Filename:  		Object.h
* Date:      		7/15/2014
* Mod. Date: 		7/15/2014
* Mod. Initials:	MB
* Author:    		Michael Boodle
* Purpose:   		The base object class.
************************************************/

#ifndef OBJECT_HEADER
#define OBJECT_HEADER
/*
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>


#include <vector>
*/
#include <DirectXMath.h>
using namespace DirectX;
class D3DObject;
class D2DObject;
class CRenderer;

class CBaseColObj;
#include <vector>
#include "../Renderer//Effects.h"
enum OBJECT_ID { PLAYER, PROJECTILE, PICKUP, LEVEL };

class CObject
{
private:
	D3DObject * m_cpMesh;
	D2DObject * m_cpTexture;
#ifdef SERVER_BUILD
	XMFLOAT3 m_tPosition;
	XMFLOAT3 m_tForward;
#endif

	OBJECT_ID m_eID;
	unsigned int m_nRefCount;

	CBaseColObj* m_cpBounds;
	bool m_bColliding;

	std::vector<CObject*> m_pChildren;

public:
	CObject();
	virtual ~CObject();

	virtual void Update(float fDT);
	virtual void Render(CRenderer * cpRenderer);
	virtual void AddRef (void);
	virtual void Release(void);

	void SetMesh(D3DObject * cpMesh);
	void CopyMesh(D3DObject * cpMesh);
	void SetTexture(D2DObject * cpTexture);
	void SetID(OBJECT_ID eID);
	void SetPosition(XMFLOAT3 & tNewPosition);
	void SetForward(XMFLOAT3 & tNewForward);


	D3DObject * GetMesh() const;
	D2DObject * GetTexture() const;
	OBJECT_ID GetID() const;
	XMFLOAT3 GetPosition() const;
	XMFLOAT3 GetForward() const;
	unsigned int GetRefCount() const;

	CBaseColObj* GetBounds(void) { return m_cpBounds; }
	void SetBounds(CBaseColObj* nBounds);

	void AddChild(CObject * childIn){ m_pChildren.push_back(childIn); };
//	CObject * GetChild(int i){ return m_pChildren[i]; };
//	unsigned int ChildCount(){ return m_pChildren.size(); };

	bool GetCol(void) { return m_bColliding; }
	void SetCol(bool c) { m_bColliding = c; }

	void SetPShader(ID3D11PixelShader * pShaderIn);
	ID3D11PixelShader * GetShaders();

	//MATRIX FUNCTIONALITY SO AN OBJECT CAN CONTROL ITS CHILDREN
	void LocalRotateX(float rot);
	void LocalRotateY(float rot);
	void LocalRotateZ(float rot);
	void WorldRotateX(float rot);
	void WorldRotateY(float rot);
	void WorldRotateZ(float rot);
	void TranslateLocal(XMFLOAT3 trans);
	void TranslateGlobalX(XMFLOAT3 trans);
	void TranslateGlobalZ(XMFLOAT3 trans);
	void TranslateGlobalZ(XMFLOAT3 trans, XMFLOAT3 forward);
	void Scale(XMFLOAT3 scale);
	void MatMult(XMFLOAT4X4 * matIn);
	void MakeIdentity();


};

#endif