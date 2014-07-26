#include "D3DAnimObject.h"


D3DAnimObject::D3DAnimObject(void)
{
	vert_buffer = nullptr;
	bone_buffer = nullptr;
	index_buffer = nullptr;
	WorldMatrixBuffer = nullptr;
	
	bufferBonesReturn=nullptr;
	
	sizeOfBoneBuffer = 0;
	m_Mesh = nullptr;
	uav=nullptr;
	srv=nullptr;

	pShader=nullptr;
	vShader=nullptr;
	SetMatrixToIdentity(); //set initial world matrix to identity
}
D3DAnimObject::~D3DAnimObject(void)
{
	//TODO::RELEASE THE BUFFERS
	if(m_Mesh)
	{
		delete m_Mesh;
		m_Mesh = nullptr;
	}
}
bool D3DAnimObject::Init( const char * fileName , ID3D11Device* device , XMFLOAT4X4 * BoneData)
{
	m_Mesh = new CAnimMesh();
	bool Result = m_Mesh->LoadAnimMeshFromXML(fileName);
	if(Result == false)
		return false;
	if(device==nullptr)
		return false;

	sizeOfBoneBuffer = m_Mesh->GetNumBones();

		VertBufferDesc.Usage			= D3D11_USAGE_IMMUTABLE;
		VertBufferDesc.BindFlags		= D3D11_BIND_VERTEX_BUFFER;
		VertBufferDesc.CPUAccessFlags	= NULL;
		VertBufferDesc.ByteWidth		= (sizeof(tAnimVert) * m_Mesh->GetNumVerts());
		VertBufferDesc.MiscFlags		= 0;
	
		VertResData.pSysMem = m_Mesh->GetVerts();

		device->CreateBuffer(&VertBufferDesc,&VertResData,&vert_buffer); //create the vertex buffer
		
		IndexBufferDesc.Usage			= D3D11_USAGE_IMMUTABLE;
		IndexBufferDesc.BindFlags		= D3D11_BIND_INDEX_BUFFER;
		IndexBufferDesc.CPUAccessFlags	= NULL;
		IndexBufferDesc.ByteWidth		= sizeof(tTRIANGLE) * m_Mesh->GetNumTris();
		IndexBufferDesc.MiscFlags		= 0;

		IndexResData.pSysMem = m_Mesh->GetTris();
		device->CreateBuffer(&IndexBufferDesc,&IndexResData,&index_buffer); //create index buffer

		//TODO:: Init and create the bone buffer
		
		
		bufDescBones.Usage			= D3D11_USAGE_DYNAMIC;
		bufDescBones.BindFlags		= D3D11_BIND_CONSTANT_BUFFER;
		bufDescBones.CPUAccessFlags	= D3D11_CPU_ACCESS_WRITE;
		bufDescBones.ByteWidth		= sizeof(XMFLOAT4X4) * sizeOfBoneBuffer;
		bufDescBones.MiscFlags		= 0;

		//this pointer is sent from the interpolator and points to the interpolated bones
		subResBones.pSysMem = BoneData;
		bufferBonesReturn = BoneData;
		HRESULT hr = device->CreateBuffer(&bufDescBones,&subResBones,&bone_buffer);

		return true;

}



void D3DAnimObject::SetMatrixToIdentity()
{
	XMStoreFloat4x4(m_XMmatrix.GetMatrix(), XMMatrixIdentity());
}

void D3DAnimObject::SetMatrix( XMFLOAT4X4 inMat )
{
	m_XMmatrix.SetMatrix(inMat);
}

XMFLOAT4X4 * D3DAnimObject::GetMatrix()
{
	return m_XMmatrix.GetMatrix();
}

CMatrix * D3DAnimObject::GetCMatrix()
{
	return &m_XMmatrix; 
}

DirectX::XMFLOAT3 D3DAnimObject::GetPosition()
{
	return m_XMmatrix.GetPosition();
}

void D3DAnimObject::SetPosition( XMFLOAT3 posIn )
{
	m_XMmatrix.SetPosition(posIn);
}

DirectX::XMFLOAT3 D3DAnimObject::GetForward()
{
	return m_XMmatrix.GetForward(); 
}

// void D3DAnimObject::SetVertBuffer( D3D11_SUBRESOURCE_DATA& DataObj, D3D11_BUFFER_DESC & OBJ_DESC, tAnimVert * pBuff, unsigned int BUFsize )
// {
// 	
// 		subResVert = DataObj;
// 		bufDescVert = OBJ_DESC;
// 		bufferVertReturn = pBuff;
// 		sizeOfvertBuffer = BUFsize;
// 	
// }

void D3DAnimObject::SetBoneBuffer( D3D11_SUBRESOURCE_DATA& DataObj, D3D11_BUFFER_DESC & OBJ_DESC, XMFLOAT4X4 * pBuff, unsigned int BUFsize )
{
	subResBones = DataObj;
	bufDescBones = OBJ_DESC;
	bufferBonesReturn = pBuff;
	sizeOfBoneBuffer = BUFsize;
}


