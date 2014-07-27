// DX11 Wrappers
// Author: Benjamin Miles
#pragma once


using namespace std;
#include <D3D11.h>

#pragma comment(lib,"d3d11.lib")

#include <DirectXMath.h>
#include <DirectXPackedVector.h> 
#include <DirectXColors.h> 
#include <DirectXCollision.h>
using namespace DirectX;

#include <vector>
#include "PixelShader.h"

#include "OBJLoader.h"
#include "D2Dobjects.h"
#include "DDSTextureLoader.h"
#include "commonObject.h"
#include "D3DObjects.h"
#include "D3DAnimObject.h"

#include "VShader.csh"
#include "PShader.csh"
#include "OBJVShader.csh"
#include "OBJPShader.csh"
#include "OBJNoTexVShader.csh"
#include "OBJNoTexPShader.csh"
#include "FlameShaderNEW.csh"
#include "LavaPShaderNEW.csh"
#include "WaterShader.csh"
#include "AnimVertShader.csh"

#include "../Utilities/XTime.h"


#define BACKBUFFER_WIDTH	1280
#define BACKBUFFER_HEIGHT	768

struct TSCENE { 
	XMFLOAT4X4 viewMatrix;
	XMFLOAT4X4 projectionMatrix;
	XMFLOAT2   screenDimensions;
	XMFLOAT2	padding;
};

struct TOBJECT { XMFLOAT4X4 worldMatrix; };

struct TLIGHT { 
	XMFLOAT4 color;
	XMFLOAT3 pos;
	float intensity;
};

enum RENDERTYPE{ NONE, OBJTEX, OBJNOTEX, ANIMATION, TWOD};

class CRenderer
{	
	HINSTANCE						application;
	WNDPROC							appWndProc;
	HWND							window;

	ID3D11Device *				m_pd3ddevice;
	ID3D11DeviceContext *		m_pd3dcontext;
	ID3D11RenderTargetView *	m_pd3drenderTarget;
	IDXGISwapChain *			m_pd3dswapChain;
	ID3D11DepthStencilView *	m_pd3dstencilView;
	DXGI_SWAP_CHAIN_DESC		buffer;
	CD3D11_VIEWPORT				vPort;
	HRESULT						hr;
	
	ID3D11InputLayout * texLayout;	
	ID3D11InputLayout * OBJLayout;
	ID3D11InputLayout * AnimLayout;

	ID3D11VertexShader * m_pd3dtexVShader;
	ID3D11PixelShader  * m_pd3dtexPShader;
	ID3D11VertexShader * m_pd3dobjVShader;
	ID3D11PixelShader  * m_pd3dobjPShader;
	ID3D11VertexShader * m_pd3dobjNoTexVShader;
	ID3D11PixelShader  * m_pd3dobjNoTexPShader;
	ID3D11PixelShader  * m_pd3dFlamePShader;
	ID3D11PixelShader  * m_pd3dLavaPShader;
	ID3D11PixelShader  * m_pd3dWaterPShader;
	
	ID3D11VertexShader * m_pd3dAnimationShader;

	TOBJECT			object;
	ID3D11Buffer *	objectBuffer;
	ID3D11Buffer *	colorBuffer;
	TSCENE			scene;
	ID3D11Buffer *	sceneBuffer;
	ID3D11Buffer *	sizeBuffer;
	ID3D11Buffer *	OBJBuffer;
	ID3D11Buffer *	hitBoxBuffer;

	ID3D11SamplerState * sampler;

	XMFLOAT4 variant;
	ID3D11Buffer * variantBuffer;
	XTime xtime;

	ID3D11Texture2D * RTTexture;
	ID3D11RenderTargetView * renderTargetTexture;
	D2DObject * renderTargetD2D;
	ID3D11ShaderResourceView * RTTshaderSRV;
	unsigned int renderTextureInt, screenWidth, screenHeight;

	std::vector<CPixelShader> pixelShaders;
	RENDERTYPE m_eRType;

	unsigned int m_nNumLights;
	TLIGHT light;
	ID3D11Buffer * lightBuffer;

	ID3D11BlendState * m_pd3dBlendState;
	
	void AnimationInit();
	D2DObject* BullshiTexture;
public:
	
	ID3D11Device* GetDevice() {return m_pd3ddevice;}
	void Init(HINSTANCE hinst, WNDPROC proc);
	void Update(void);
	void PreRender();
	void PostRender();
	void Render2D(D2DObject * texture, XMFLOAT4 imgCoords, XMFLOAT4 imgPart);
	void Render3D(D3DObject * mesh, D2DObject * texture, XMFLOAT4 color);
	bool ShutDown();
	D3DObject * AddModel(D3DObject * d3dobject, const char * OBJFileLoc);
	D2DObject * AddTexture(ID3D11Texture2D * texture, float imgWidth, float imgHeight);
	void AddD2D(D2DObject * texture);
	void MoveCamera(XMFLOAT4X4 * camMatrix){ scene.viewMatrix = *camMatrix; };
	XMFLOAT4X4 GetCamera(){ return scene.viewMatrix; };
	unsigned int GetWindowWidth(){ return screenWidth; };
	unsigned int GetWindowHeight(){ return screenHeight; };

	ID3D11PixelShader * GetPShader(string name);

	void RenderAnim3D(D3DAnimObject * mesh, D2DObject * texture, XMFLOAT4 color);
};

