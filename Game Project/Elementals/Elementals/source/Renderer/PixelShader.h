#pragma once

#include <D3D11.h>
#pragma comment(lib,"d3d11.lib")

#include <string>
//using namespace DirectX;


class CPixelShader
{
	ID3D11PixelShader * m_pShader;
	std::string m_cName;

public:

	CPixelShader(ID3D11PixelShader * pixShaderIn, std::string nameIn);
	~CPixelShader(void);
	std::string GetName(){ return m_cName; };
	ID3D11PixelShader * GetShader(){ return m_pShader; };
};

