#include "Matrix.h"


void CMatrix::LocalRotateX(float rot)
{
	XMMATRIX temp = XMMatrixRotationX(rot);
	XMMATRIX matCopy = XMLoadFloat4x4(&m_XMFloat4X4);
	matCopy = XMMatrixMultiply(temp, matCopy);
	XMStoreFloat4x4(&m_XMFloat4X4, matCopy);
}

void CMatrix::LocalRotateY(float rot)
{
	XMMATRIX temp = XMMatrixRotationY(rot);
	XMMATRIX matCopy = XMLoadFloat4x4(&m_XMFloat4X4);
	matCopy = XMMatrixMultiply(temp, matCopy);
	XMStoreFloat4x4(&m_XMFloat4X4, matCopy);
}

void CMatrix::LocalRotateZ(float rot)
{
	XMMATRIX temp = XMMatrixRotationZ(rot);
	XMMATRIX matCopy = XMLoadFloat4x4(&m_XMFloat4X4);
	matCopy = XMMatrixMultiply(temp, matCopy);
	XMStoreFloat4x4(&m_XMFloat4X4, matCopy);
}

void CMatrix::WorldRotateX(float rot)
{
	XMMATRIX temp = XMMatrixRotationX(rot);
	XMMATRIX matCopy = XMLoadFloat4x4(reinterpret_cast<XMFLOAT4X4*>(&m_XMFloat4X4));
	XMFLOAT4 tempLoc;
	memcpy(&tempLoc, &m_XMFloat4X4._41, sizeof(XMFLOAT4)); 
	XMStoreFloat4x4(&m_XMFloat4X4, XMMatrixMultiply(matCopy, temp));
	memcpy(&m_XMFloat4X4._41, &tempLoc, sizeof(XMFLOAT4));
}

void CMatrix::WorldRotateY(float rot)
{	
	XMMATRIX temp = XMMatrixRotationY( rot);
	XMMATRIX matCopy = XMLoadFloat4x4(&m_XMFloat4X4);
	XMFLOAT4 tempLoc;
	memcpy(&tempLoc, &m_XMFloat4X4._41, sizeof(XMFLOAT4)); 
	XMStoreFloat4x4(&m_XMFloat4X4, XMMatrixMultiply(matCopy, temp));
	memcpy(&m_XMFloat4X4._41, &tempLoc, sizeof(XMFLOAT4));
}

void CMatrix::WorldRotateZ(float rot)
{
	XMMATRIX temp = XMMatrixRotationZ(rot);
	XMMATRIX matCopy = XMLoadFloat4x4(reinterpret_cast<XMFLOAT4X4*>(&m_XMFloat4X4));
	XMFLOAT4 tempLoc;
	memcpy(&tempLoc, &m_XMFloat4X4._41, sizeof(XMFLOAT4)); 
	XMStoreFloat4x4(&m_XMFloat4X4, XMMatrixMultiply(matCopy, temp));
	memcpy(&m_XMFloat4X4._41, &tempLoc, sizeof(XMFLOAT4));
}

void CMatrix::TranslateLocal(XMFLOAT3 trans)
{
	//XMMATRIX matCopy = XMLoadFloat4x4(&m_XMFloat4X4);
	XMVECTOR newPos = XMLoadFloat3(&trans);
	XMVECTOR position;
	memcpy(&position, &m_XMFloat4X4._41, sizeof(XMVECTOR));
	//XMVECTOR newPos   = XMVectorSet(trans.x, trans.y, trans.z, 0);
	position += newPos;
	XMStoreFloat3((XMFLOAT3*)&m_XMFloat4X4._41, position);
}

void CMatrix::TranslateGlobalX(XMFLOAT3 trans)
{
	XMMATRIX temp = XMMatrixTranslation( m_XMFloat4X4._11*trans.x, m_XMFloat4X4._12*trans.x, m_XMFloat4X4._13*trans.x); //trans.x, trans.y, trans.z );
	XMMATRIX matCopy = XMLoadFloat4x4(&m_XMFloat4X4);
	XMStoreFloat4x4(&m_XMFloat4X4, XMMatrixMultiply(matCopy, temp));
}

void CMatrix::TranslateGlobalZ(XMFLOAT3 trans)
{
	XMMATRIX temp = XMMatrixTranslation( m_XMFloat4X4._31*trans.z, m_XMFloat4X4._32*trans.z, m_XMFloat4X4._33*trans.z); //trans.x, trans.y, trans.z );
	XMMATRIX matCopy = XMLoadFloat4x4(&m_XMFloat4X4);
	XMStoreFloat4x4(&m_XMFloat4X4, XMMatrixMultiply(matCopy, temp));
}

void CMatrix::TranslateGlobalZ(XMFLOAT3 trans, XMFLOAT3 forward)
{
	XMMATRIX temp = XMMatrixTranslation( forward.x+trans.x, forward.y+trans.y, forward.z+trans.z); //trans.x, trans.y, trans.z );
	XMMATRIX matCopy = XMLoadFloat4x4(&m_XMFloat4X4);
	XMStoreFloat4x4(&m_XMFloat4X4, XMMatrixMultiply(matCopy, temp));
}

void CMatrix::Scale(XMFLOAT3 scale)
{
	XMMATRIX matScal = XMMatrixScaling(scale.x, scale.y, scale.z);
	XMMATRIX matTemp =  XMLoadFloat4x4(&m_XMFloat4X4);
	matScal = XMMatrixMultiply(matScal, matTemp);
	XMStoreFloat4x4(&m_XMFloat4X4, matScal);
}

void CMatrix::SetPosition(float fx, float fy, float fz)
{
	m_XMFloat4X4.m[3][0] = fx;
	m_XMFloat4X4.m[3][1] = fy;
	m_XMFloat4X4.m[3][2] = fz;
}

void CMatrix::SetPosition(XMFLOAT3 tNewPosition)
{
	memcpy(&m_XMFloat4X4.m[3], &tNewPosition, sizeof(XMFLOAT3));
}

void CMatrix::SetForward(float fx, float fy, float fz)
{
	SetForward(XMFLOAT3(fx, fy, fz));
}

void CMatrix::SetForward(XMFLOAT3 tNewForward)
{
	memcpy(&m_XMFloat4X4.m[2], &tNewForward, sizeof(XMFLOAT3));
	memcpy(&m_XMFloat4X4.m[1], &XMFLOAT3(0, 1, 0), sizeof(XMFLOAT3));
	XMVECTOR vxVec = XMVector3Cross(XMLoadFloat3((XMFLOAT3*)m_XMFloat4X4.m[1]), XMLoadFloat3((XMFLOAT3*)m_XMFloat4X4.m[2]));
	XMStoreFloat3((XMFLOAT3*)&m_XMFloat4X4._11, vxVec); 
}

XMFLOAT3 CMatrix::GetForward()
{
	return *((XMFLOAT3 *)(&m_XMFloat4X4.m[2]));
}  

void CMatrix::MatMult(XMFLOAT4X4 * matIn)
{
	XMMATRIX matTempa =  XMLoadFloat4x4(&m_XMFloat4X4);
	XMMATRIX matTempb =  XMLoadFloat4x4(matIn);
	matTempa = XMMatrixMultiply(matTempa, matTempb);
	XMStoreFloat4x4(&m_XMFloat4X4, matTempa);
}

void CMatrix::MakeIdentity()
{
	XMStoreFloat4x4(&m_XMFloat4X4, XMMatrixIdentity());
}


