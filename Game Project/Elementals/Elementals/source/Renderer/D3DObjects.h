#pragma once

#include <D3D11.h>

#pragma comment(lib,"d3d11.lib")

#include <DirectXMath.h>
//#include <DirectXPackedVector.h> 
//#include <DirectXColors.h> 
//#include <DirectXCollision.h>
using namespace DirectX;

#include "commonObject.h"
#include "OBJLoader.h"
#include "Matrix.h"

struct OBJ_IN   { XMFLOAT3 posL; XMFLOAT2 UV; XMFLOAT3 normals;};

class D3DObject
{
	CMatrix m_XMmatrix;
	OBJLoader * tempObj;

	ID3D11Buffer * buffer;

	ID3D11UnorderedAccessView * uav;
	ID3D11ShaderResourceView * srv;
	ID3D11Buffer * matrixBuffer;
	ID3D11PixelShader * shader;
	bool copy;

public:
	D3DObject(void);
	~D3DObject(void);

	void FileReader(const char * modelTexLoc);
	OBJLoader * GetObjLoader(){ return tempObj; };
	ID3D11Buffer ** GetBuffer(){ return &buffer; };
	void SetOBJPointer(OBJLoader * objPtr){ tempObj = objPtr; };
	void SetMatrixToIdentity();
	void SetMatrix(XMFLOAT4X4 inMat);
	ID3D11UnorderedAccessView ** GetUAV(){ return &uav; };
	ID3D11ShaderResourceView  ** GetSRV(){ return &srv; };
	ID3D11Buffer ** GetMatrixBuffer(){ return &matrixBuffer; };
	XMFLOAT4X4 * GetMatrix();
	CMatrix * GetCMatrix();
	XMFLOAT3 GetPosition();
	void SetPosition(XMFLOAT3 posIn);

	XMFLOAT3 GetForward();
	D3DObject & operator =(const D3DObject & RHS);

	void SetPShader(ID3D11PixelShader * pShaderIn){ shader = pShaderIn; };
	ID3D11PixelShader * GetShader(){ return shader; };

};

