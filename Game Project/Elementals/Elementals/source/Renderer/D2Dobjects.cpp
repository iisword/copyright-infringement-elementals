#include "D2Dobjects.h"


D2Dobject::D2Dobject(const wchar_t * textLoc, float WIDTH, float HEIGHT)
{
	dataBuffer = nullptr;
	tex2D = nullptr;
	textureSRV = nullptr;
	textureLoc = nullptr;

	texQuad[0].posL = XMFLOAT2(-1, 1); //UL
	texQuad[1].posL = XMFLOAT2( 1, 1);  //UR
	texQuad[2].posL = XMFLOAT2(-1,-1); //LL
	texQuad[3].posL = XMFLOAT2( 1, 1);  //UR
	texQuad[4].posL = XMFLOAT2( 1,-1);  //LR
	texQuad[5].posL = XMFLOAT2(-1,-1); //LL
	for(int i = 0; i < 6; i++)
	{
		if(texQuad[i].posL.x == -1)
			texQuad[i].UV.x = 0;
		else
			texQuad[i].UV.x = 1;
	
		if(texQuad[i].posL.y == 1)
			texQuad[i].UV.y = 0;
		else
			texQuad[i].UV.y = 1;
	}

	lowQuadDesc.Usage = D3D11_USAGE_IMMUTABLE;
	lowQuadDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	lowQuadDesc.CPUAccessFlags = NULL;
	lowQuadDesc.ByteWidth = sizeof(V_IN) * 6;
	lowQuadDesc.MiscFlags = 0;

	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MinLOD = -FLT_MAX;
	samplerDesc.MaxLOD = FLT_MAX;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.BorderColor[0] = 0.0f;
	samplerDesc.BorderColor[1] = 0.0f;
	samplerDesc.BorderColor[2] = 0.0f;
	samplerDesc.BorderColor[3] = 0.0f;

	textureLoc = textLoc;

	size.IMG_SIZE.x = WIDTH;
	size.IMG_SIZE.y = HEIGHT;
	size.SIZE_WH.x = 384;
	size.SIZE_WH.y = 384;
	size.TOP_LEFT.x = 384;
	size.TOP_LEFT.y = 384;

	size.SCREEN_COORDS.x = 100;
	size.SCREEN_COORDS.y = 100;
	size.SCREEN_COORDS.z = 200;
	size.SCREEN_COORDS.w = 200;
}

D2Dobject::D2Dobject(ID3D11Texture2D * texture, float WIDTH, float HEIGHT)
{
	dataBuffer = nullptr;
	tex2D = texture;
	textureSRV = nullptr;
	textureLoc = nullptr;

	texQuad[0].posL = XMFLOAT2(-1, 1); //UL
	texQuad[1].posL = XMFLOAT2( 1, 1);  //UR
	texQuad[2].posL = XMFLOAT2(-1,-1); //LL
	texQuad[3].posL = XMFLOAT2( 1, 1);  //UR
	texQuad[4].posL = XMFLOAT2( 1,-1);  //LR
	texQuad[5].posL = XMFLOAT2(-1,-1); //LL
	for(int i = 0; i < 6; i++)
	{
		if(texQuad[i].posL.x == -1)
			texQuad[i].UV.x = 0;
		else
			texQuad[i].UV.x = 1;
	
		if(texQuad[i].posL.y == 1)
			texQuad[i].UV.y = 0;
		else
			texQuad[i].UV.y = 1;
	}

	lowQuadDesc.Usage = D3D11_USAGE_IMMUTABLE;
	lowQuadDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	lowQuadDesc.CPUAccessFlags = NULL;
	lowQuadDesc.ByteWidth = sizeof(V_IN) * 6;
	lowQuadDesc.MiscFlags = 0;

	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MinLOD = -FLT_MAX;
	samplerDesc.MaxLOD = FLT_MAX;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.BorderColor[0] = 0.0f;
	samplerDesc.BorderColor[1] = 0.0f;
	samplerDesc.BorderColor[2] = 0.0f;
	samplerDesc.BorderColor[3] = 0.0f;

//	textureLoc = textLoc;

	size.IMG_SIZE.x = WIDTH;
	size.IMG_SIZE.y = HEIGHT;
	size.SIZE_WH.x = 384;
	size.SIZE_WH.y = 384;
	size.TOP_LEFT.x = 384;
	size.TOP_LEFT.y = 384;

	size.SCREEN_COORDS.x = 100;
	size.SCREEN_COORDS.y = 100;
	size.SCREEN_COORDS.z = 200;
	size.SCREEN_COORDS.w = 200;
}

D2Dobject::~D2Dobject(void)
{
}

void D2Dobject::ChangeCoords(XMFLOAT4 coords, XMFLOAT4 imgPart)
{
	memcpy(&size.SCREEN_COORDS, &coords, sizeof(float4));
	memcpy(&size.TOP_LEFT, &imgPart, sizeof(float4));
}