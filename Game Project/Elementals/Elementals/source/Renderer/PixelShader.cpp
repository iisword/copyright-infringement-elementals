#include "PixelShader.h"


CPixelShader::CPixelShader(ID3D11PixelShader * pixShaderIn, std::string nameIn)
{
	m_pShader = pixShaderIn;
	m_cName = nameIn;
}


CPixelShader::~CPixelShader(void)
{
}
