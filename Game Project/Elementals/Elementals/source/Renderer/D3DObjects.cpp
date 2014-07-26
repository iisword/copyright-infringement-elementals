#include "D3DObjects.h"
#include "Renderer.h"

D3DObject::D3DObject(void)
{
	tempObj = nullptr;
	XMStoreFloat4x4(m_XMmatrix.GetMatrix(), XMMatrixIdentity());
	copy = false;
	shader = nullptr;
}

D3DObject::~D3DObject(void)
{
	if(tempObj && !copy && tempObj->Size())
	{	
		delete tempObj;
	}
	tempObj = nullptr;
}

void D3DObject::FileReader(const char * modelTexLoc)
{
	tempObj = new OBJLoader;
	tempObj->FileReader(modelTexLoc);
}


// Assignment operator
D3DObject& D3DObject::operator=(const D3DObject& _RHS)
{
	// Make sure we aren't trying to copy ourself
	if(this != &_RHS)
	{
		m_XMmatrix = _RHS.m_XMmatrix;
		tempObj = _RHS.tempObj;

		buffer = _RHS.buffer;
		
		uav = _RHS.uav;
		srv = _RHS.srv;
		matrixBuffer = _RHS.matrixBuffer;
		copy = true;
	}

	// Return our updated self
	return *this;
}

void D3DObject::SetMatrixToIdentity()
{
	XMStoreFloat4x4(m_XMmatrix.GetMatrix(), XMMatrixIdentity());
}

void D3DObject::SetMatrix(XMFLOAT4X4 inMat)
{ 
	m_XMmatrix.SetMatrix(inMat); 
}

XMFLOAT4X4 * D3DObject::GetMatrix()
{ 
	return m_XMmatrix.GetMatrix(); 
}

CMatrix * D3DObject::GetCMatrix()
{ 
	return &m_XMmatrix; 
}

XMFLOAT3 D3DObject::GetPosition()
{ 
	return m_XMmatrix.GetPosition(); 
}

void D3DObject::SetPosition(XMFLOAT3 posIn)
{ 
	m_XMmatrix.SetPosition(posIn); 
}

XMFLOAT3 D3DObject::GetForward()
{
	return m_XMmatrix.GetForward(); 
}
