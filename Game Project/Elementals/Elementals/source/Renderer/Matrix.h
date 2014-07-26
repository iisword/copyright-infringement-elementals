#pragma once

//#include "Renderer.h"
#include <DirectXMath.h>
using namespace DirectX;
#include <string.h>

class CMatrix
{
	XMFLOAT4X4 m_XMFloat4X4;

public:

	CMatrix(){};
	~CMatrix(){};

	void SetPosition(float fx, float fy, float fz);
	void SetPosition(XMFLOAT3 tNewPosition);
	void SetForward(float fx, float fy, float fz);
	void SetForward(XMFLOAT3 tNewForward);
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
	XMFLOAT4X4 * GetMatrix(){ return &m_XMFloat4X4; };
	XMFLOAT3 GetPosition() { return XMFLOAT3(m_XMFloat4X4._41, m_XMFloat4X4._42, m_XMFloat4X4._43); };
	void SetMatrix(XMFLOAT4X4 inMat){ m_XMFloat4X4 = inMat; };
	XMFLOAT3 CMatrix::GetForward();
	void MatMult(XMFLOAT4X4 * matIn);
	void MakeIdentity();


};
