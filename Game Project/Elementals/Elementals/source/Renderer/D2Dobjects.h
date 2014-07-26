#pragma once

#include <D3D11.h>

#pragma comment(lib,"d3d11.lib")

#include <DirectXMath.h>
#include <DirectXPackedVector.h> 
#include <DirectXColors.h> 
#include <DirectXCollision.h>
using namespace DirectX;

#include "commonObject.h"

struct V_IN   { XMFLOAT2 posL; XMFLOAT2 UV; };


class D2Dobject
{

	V_IN texQuad[6];
	D3D11_BUFFER_DESC lowQuadDesc;
	ID3D11Buffer * dataBuffer;

	ID3D11SamplerState * sampler;
	D3D11_SAMPLER_DESC samplerDesc;

//	D3D11_INPUT_ELEMENT_DESC layoutDesc[2];

	ID3D11Texture2D * tex2D;
	ID3D11ShaderResourceView * textureSRV;
	
	const wchar_t * textureLoc;

	DISPLAY_SIZE size;

public:
	D2Dobject(const wchar_t * location, float WIDTH, float  HEIGHT);
	D2Dobject(ID3D11Texture2D * texture, float WIDTH, float  HEIGHT);
	~D2Dobject(void);

	const wchar_t *			GetTextureLocation(){ return textureLoc; };
	ID3D11SamplerState **	GetSampler(){ return &sampler; };
	D3D11_SAMPLER_DESC	*	GetSamplerDesc(){ return &samplerDesc; };

//	D3D11_INPUT_ELEMENT_DESC * GetLayoutDesc(){ return layoutDesc; };

	ID3D11Texture2D ** GetTexture(){ return &tex2D; };
	ID3D11ShaderResourceView ** GetSRV(){ return &textureSRV; };

	V_IN * GetQuadLoc(){ return texQuad; };
	D3D11_BUFFER_DESC GetDataBufferDesc(){ return lowQuadDesc; };
	ID3D11Buffer ** GetDataBuffer(){ return &dataBuffer; };
	DISPLAY_SIZE * GetSize(){ return &size; };

	void ChangeCoords(XMFLOAT4 coords, XMFLOAT4 imgPart);
};

