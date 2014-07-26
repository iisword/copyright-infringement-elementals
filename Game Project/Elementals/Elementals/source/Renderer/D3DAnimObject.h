/***********************************************
* Filename:  		D3DAnimObject.h
* Date:      		7/23/2014
* Mod. Date: 		7/23/2014
* Mod. Initials:	RT
* Author:    		Roman Taran
* Purpose:   		This Class is responsible for rendering animations
************************************************/

#ifndef D3DANIM_OBJECT_H
#define D3DANIM_OBJECT_H

#include <D3D11.h>

#pragma comment(lib,"d3d11.lib")

#include <DirectXMath.h>
using namespace DirectX;
#include "Matrix.h"
#include "..\Animation\AnimationDefines.h"
#include "..\Animation\AnimMesh.h"


class D3DAnimObject
{
	CMatrix m_XMmatrix;
	//Mesh data is stored in CMesh
	CAnimMesh* m_Mesh;
	D3D11_SUBRESOURCE_DATA  VertResData;
	D3D11_BUFFER_DESC		VertBufferDesc;

	D3D11_SUBRESOURCE_DATA  IndexResData;
	D3D11_BUFFER_DESC		IndexBufferDesc;

	D3D11_SUBRESOURCE_DATA  subResBones;
	D3D11_BUFFER_DESC		bufDescBones;

		
	XMFLOAT4X4 * bufferBonesReturn;  //this is Bone Mats
	unsigned int sizeOfBoneBuffer;
	

	ID3D11Buffer * bone_buffer;
	ID3D11Buffer * vert_buffer;
	ID3D11Buffer * index_buffer;
	ID3D11Buffer * WorldMatrixBuffer;

	ID3D11UnorderedAccessView * uav;
	ID3D11ShaderResourceView  * srv;
	
	ID3D11PixelShader		  * pShader;
	ID3D11VertexShader		  * vShader;
	bool copy;

public:

	//void FileReader(const char * modelTexLoc);

	CAnimMesh* GetMeshData() {return m_Mesh;}
	XMFLOAT4X4* GetBoneData() {return bufferBonesReturn;}

	ID3D11Buffer ** GetVertBuffer(){ return &vert_buffer;}
	ID3D11Buffer ** GetIndexBuffer() {return &index_buffer;}
	ID3D11Buffer ** GetBoneBuffer(){ return &bone_buffer; }
	unsigned int GetNumBones(){return sizeOfBoneBuffer;}
	ID3D11UnorderedAccessView ** GetUAV(){ return &uav; }
	ID3D11ShaderResourceView  ** GetSRV(){ return &srv; }
	void SetPixelShader (ID3D11PixelShader  * pShaderIn){ pShader = pShaderIn; }
	void SetVertexShader(ID3D11VertexShader * pShaderIn){ vShader = pShaderIn; }

	ID3D11PixelShader  * GetPixelShader() { return pShader; };
	ID3D11VertexShader * GetVertexShader(){ return vShader; };

	void SetMatrixToIdentity();
	void SetMatrix(XMFLOAT4X4 inMat);

	
	ID3D11Buffer ** GetMatrixBuffer(){ return &WorldMatrixBuffer; };
	XMFLOAT4X4 * GetMatrix();
	CMatrix * GetCMatrix();
	XMFLOAT3 GetPosition();
	void SetPosition(XMFLOAT3 posIn);

	XMFLOAT3 GetForward();
	
// 	void SetVertBuffer(D3D11_SUBRESOURCE_DATA& DataObj, D3D11_BUFFER_DESC & OBJ_DESC, 
// 	 					tAnimVert * pBuff, unsigned int BUFsize);

	void SetBoneBuffer(D3D11_SUBRESOURCE_DATA& DataObj, D3D11_BUFFER_DESC & OBJ_DESC, 
					   XMFLOAT4X4 * pBuff, unsigned int BUFsize);
	
	bool Init(const char * fileName, ID3D11Device * device, XMFLOAT4X4 * BoneData);
	D3DAnimObject(void);
	~D3DAnimObject(void);
};
#endif

