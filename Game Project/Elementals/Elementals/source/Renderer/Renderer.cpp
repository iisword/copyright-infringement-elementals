/***********************************************
 * Filename:  		TextureManager.cpp
 * Date:      		06/09/2014
 * Mod. Date: 		06/09/2014
 * Mod. Initials:	BGM
 * Author:    		Benjamin G. Miles
 * Purpose:   		This is the DirectX manager.  
					It handles logging textures and verts for rendering.  
					Called for draw calls
					Used for Rotation, Translation, Scaling of objects due to its reference ownership of models
 ************************************************/

#include "Renderer.h"
//#include "MasterClass.h"
//#include <string>


/*****************************************************************
 * Init():  Instantiates DirectX device and swap chain.  Defined and registers vertex/pixel shaders.
 *
 * Ins:					hinst - Directory and file name of the texture
 *						proc -	width of the image file
 * Returns:				unsigned int - location of the object in the vector for render reference
 *
 * Mod. Date:		      06/09/2014
 * Mod. Initials:	      BGM
 *****************************************************************/
void CRenderer::Init(HINSTANCE hinst, WNDPROC proc)
{

	application = hinst; 
	appWndProc = proc; 

	WNDCLASSEX  wndClass;
    ZeroMemory( &wndClass, sizeof( wndClass ) );
    wndClass.cbSize         = sizeof( WNDCLASSEX );             
    wndClass.lpfnWndProc    = appWndProc;						
    wndClass.lpszClassName  = L"Elementals";            
	wndClass.hInstance      = application;		               
    wndClass.hCursor        = LoadCursor( NULL, IDC_ARROW );    
    wndClass.hbrBackground  = ( HBRUSH )( COLOR_WINDOWFRAME ); 
	//wndClass.hIcon			= LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_FSICON));
    RegisterClassEx( &wndClass );

	RECT window_size = { 0, 0, BACKBUFFER_WIDTH, BACKBUFFER_HEIGHT };
	AdjustWindowRect(&window_size, WS_OVERLAPPEDWINDOW, false);

	window = CreateWindow(	L"Elementals", L"Elementals",	WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME|WS_MAXIMIZEBOX), 
							CW_USEDEFAULT, CW_USEDEFAULT, window_size.right-window_size.left, window_size.bottom-window_size.top,					
							NULL, NULL,	application, this );												
	
	ShowWindow( window, SW_SHOW );

	buffer.BufferCount = 1;
	buffer.BufferDesc.Width = BACKBUFFER_WIDTH;
	buffer.BufferDesc.Height = BACKBUFFER_HEIGHT;
	buffer.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	buffer.BufferDesc.RefreshRate.Numerator = 0;
	buffer.BufferDesc.RefreshRate.Denominator = 1;
	buffer.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	buffer.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	buffer.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	buffer.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	buffer.OutputWindow = window;
	buffer.SampleDesc.Count = 1;
	buffer.SampleDesc.Quality = 0;
	buffer.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	buffer.Windowed = true;

	D3D_FEATURE_LEVEL fLevel = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL * TEMPFEATURE = nullptr;

	m_pd3dswapChain = nullptr;
	m_pd3ddevice = nullptr;
	m_pd3dcontext = nullptr;	
	hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_DEBUG, &fLevel, 1, D3D11_SDK_VERSION, 
	&buffer, &m_pd3dswapChain, &m_pd3ddevice, TEMPFEATURE, &m_pd3dcontext); 
	
	ID3D11Resource * pBB;
	m_pd3dswapChain->GetBuffer(0, _uuidof(pBB), reinterpret_cast<void**>(&pBB));
	m_pd3ddevice->CreateRenderTargetView(	pBB, NULL, &m_pd3drenderTarget);
	
	vPort.Height = BACKBUFFER_HEIGHT;
	vPort.Width  = BACKBUFFER_WIDTH;
	vPort.MaxDepth = 1;
	vPort.MinDepth = 0;
	vPort.TopLeftX = 0;
	vPort.TopLeftY = 0;

	XMStoreFloat4x4(&scene.projectionMatrix, DirectX::XMMatrixPerspectiveFovLH(XMConvertToRadians(45), (float)BACKBUFFER_WIDTH / (float)BACKBUFFER_HEIGHT, 0.1f, 1000.0f));
	XMStoreFloat4x4(&scene.viewMatrix, DirectX::XMMatrixLookAtLH(XMVectorSet(0, 15, -15, 1), XMVectorSet(0, 0, 3, 0), XMVectorSet(0, 1, 0, 0)));
	XMStoreFloat4x4(&object.worldMatrix, XMMatrixIdentity());
	
	scene.screenDimensions = XMFLOAT2(vPort.Width, vPort.Height);

	D3D11_BUFFER_DESC objectDesc;
	objectDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	objectDesc.ByteWidth = sizeof(TOBJECT);
	objectDesc.MiscFlags = 0;
	objectDesc.CPUAccessFlags =  D3D11_CPU_ACCESS_WRITE;
	objectDesc.Usage = D3D11_USAGE_DYNAMIC;
 
	D3D11_SUBRESOURCE_DATA pDataObject;
	pDataObject.pSysMem = &object;
	hr = m_pd3ddevice->CreateBuffer( &objectDesc, &pDataObject, &objectBuffer);

	D3D11_BUFFER_DESC colorDesc;
	colorDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	colorDesc.ByteWidth = sizeof(XMFLOAT4);
	colorDesc.MiscFlags = 0;
	colorDesc.CPUAccessFlags =  D3D11_CPU_ACCESS_WRITE;
	colorDesc.Usage = D3D11_USAGE_DYNAMIC;
	
	XMFLOAT4 color = XMFLOAT4(0, 0, 0, 0);
	D3D11_SUBRESOURCE_DATA pDataColor;
	pDataColor.pSysMem = &color;
	hr = m_pd3ddevice->CreateBuffer( &colorDesc, &pDataColor, &colorBuffer);

	D3D11_BUFFER_DESC sceneDesc;
	sceneDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	sceneDesc.ByteWidth = sizeof(TSCENE);
	sceneDesc.MiscFlags = 0;
	sceneDesc.CPUAccessFlags =  D3D11_CPU_ACCESS_WRITE;
	sceneDesc.Usage = D3D11_USAGE_DYNAMIC;
 
	D3D11_SUBRESOURCE_DATA pDataScene;
	pDataScene.pSysMem = &scene;
	hr = m_pd3ddevice->CreateBuffer( &sceneDesc, &pDataScene, &sceneBuffer);
	
	hr = m_pd3ddevice->CreatePixelShader( PShader, sizeof(PShader), NULL,		&m_pd3dtexPShader);
	hr = m_pd3ddevice->CreatePixelShader( OBJPShader, sizeof(OBJPShader), NULL, &m_pd3dobjPShader);
	hr = m_pd3ddevice->CreateVertexShader(VShader, sizeof(VShader), NULL,		&m_pd3dtexVShader);
	hr = m_pd3ddevice->CreateVertexShader(OBJVShader, sizeof(OBJVShader), NULL, &m_pd3dobjVShader);

	hr = m_pd3ddevice->CreatePixelShader( OBJNoTexPShader, sizeof(OBJNoTexPShader), NULL, &m_pd3dobjNoTexPShader);
	hr = m_pd3ddevice->CreateVertexShader(OBJNoTexVShader, sizeof(OBJNoTexVShader), NULL, &m_pd3dobjNoTexVShader);

	D3D11_INPUT_ELEMENT_DESC layoutTex[] =  {{ 
		 "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{    "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}};	
	hr = m_pd3ddevice->CreateInputLayout( layoutTex, 2, VShader, sizeof(VShader), &texLayout);

	D3D11_INPUT_ELEMENT_DESC layoutOBJ[] =  {{ 
		 "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{    "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}, 
	{	 "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}};	
	hr = m_pd3ddevice->CreateInputLayout( layoutOBJ, 3, OBJVShader, sizeof(OBJVShader), &OBJLayout);
	
	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MinLOD = -FLT_MAX;
	samplerDesc.MaxLOD =  FLT_MAX;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.BorderColor[0] = 0.0f;
	samplerDesc.BorderColor[1] = 0.0f;
	samplerDesc.BorderColor[2] = 0.0f;
	samplerDesc.BorderColor[3] = 0.0f;
	hr = m_pd3ddevice->CreateSamplerState(&samplerDesc, &sampler);

	m_nNumLights = 1;
	light.color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	light.pos = XMFLOAT3(-1, 1.5, 0);
	light.intensity = 3.0f;

	D3D11_BUFFER_DESC lightDesc;
	lightDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightDesc.ByteWidth = sizeof(TLIGHT);
	lightDesc.MiscFlags = 0;
	lightDesc.CPUAccessFlags =  D3D11_CPU_ACCESS_WRITE;
	lightDesc.Usage = D3D11_USAGE_DYNAMIC;
	D3D11_SUBRESOURCE_DATA pDatalight;
	pDatalight.pSysMem = &light;
	hr = m_pd3ddevice->CreateBuffer( &lightDesc, &pDatalight, &lightBuffer);

	D3D11_BUFFER_DESC variantDesc;
	variantDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	variantDesc.ByteWidth = sizeof(XMFLOAT4);
	variantDesc.MiscFlags = 0;
	variantDesc.CPUAccessFlags =  D3D11_CPU_ACCESS_WRITE;
	variantDesc.Usage = D3D11_USAGE_DYNAMIC;
	variant = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	D3D11_SUBRESOURCE_DATA pDataVariant;
	pDataVariant.pSysMem = &variant;
	hr = m_pd3ddevice->CreateBuffer( &variantDesc, &pDataVariant, &variantBuffer);

	D3D11_TEXTURE2D_DESC RTTtdesc;
	ZeroMemory(&RTTtdesc, sizeof(D3D11_TEXTURE2D_DESC));
	RTTtdesc.Width = buffer.BufferDesc.Width;
	RTTtdesc.Height = buffer.BufferDesc.Height;
	RTTtdesc.MipLevels = 1;
	RTTtdesc.ArraySize = 1;
	RTTtdesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	RTTtdesc.Usage = D3D11_USAGE_DEFAULT;
	RTTtdesc.SampleDesc = buffer.SampleDesc;
	RTTtdesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	RTTtdesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
	m_pd3ddevice->CreateTexture2D(&RTTtdesc, NULL, &RTTexture);

	m_pd3ddevice->CreateRenderTargetView((ID3D11Resource*)RTTexture, NULL, &renderTargetTexture);
	m_pd3ddevice->CreateShaderResourceView((ID3D11Resource*)RTTexture, NULL, &RTTshaderSRV);
//	m_pd3dcontext->GenerateMips(RTTshaderSRV);

	renderTargetD2D = AddTexture((ID3D11Texture2D*)renderTargetTexture, BACKBUFFER_WIDTH, BACKBUFFER_HEIGHT);

	ID3D11Texture2D * stencilResource;
	D3D11_TEXTURE2D_DESC stencilDesc;
	ZeroMemory(&stencilDesc, sizeof(D3D11_TEXTURE2D_DESC));
	stencilDesc.Width = buffer.BufferDesc.Width;
	stencilDesc.Height = buffer.BufferDesc.Height;
	stencilDesc.MipLevels = 0;
	stencilDesc.ArraySize = 1;
	stencilDesc.Format = DXGI_FORMAT_D32_FLOAT;
	stencilDesc.Usage = D3D11_USAGE_DEFAULT;
	stencilDesc.SampleDesc = buffer.SampleDesc;
	stencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	hr = m_pd3ddevice->CreateTexture2D(&stencilDesc, 0, &stencilResource);
	hr = m_pd3ddevice->CreateDepthStencilView(stencilResource, NULL, &m_pd3dstencilView);

	screenHeight = BACKBUFFER_HEIGHT;
	screenWidth = BACKBUFFER_WIDTH;

	hr = m_pd3ddevice->CreatePixelShader( FlameShaderNEW, sizeof(FlameShaderNEW), NULL, &m_pd3dFlamePShader);
	hr = m_pd3ddevice->CreatePixelShader( LavaPShaderNEW, sizeof(LavaPShaderNEW), NULL, &m_pd3dLavaPShader);
	hr = m_pd3ddevice->CreatePixelShader( WaterShader, sizeof(WaterShader), NULL, &m_pd3dWaterPShader);

	pixelShaders.push_back(CPixelShader(m_pd3dtexPShader,		"TexPShader"));
	pixelShaders.push_back(CPixelShader(m_pd3dobjPShader,		"OBJPShader"));
	pixelShaders.push_back(CPixelShader(m_pd3dobjNoTexPShader,	"OBJNoTexPShader"));
	pixelShaders.push_back(CPixelShader(m_pd3dFlamePShader,		"FlamePShaderNEW"));
	pixelShaders.push_back(CPixelShader(m_pd3dLavaPShader,		"LavaPShaderNEW"));
	pixelShaders.push_back(CPixelShader(m_pd3dWaterPShader,		"WaterPShader"));
//	pixelShaders.push_back(CPixelShader(m_pd3FlamePShader,		"FlamePShader"));

	D3D11_BLEND_DESC blendDesc;
	blendDesc.AlphaToCoverageEnable = TRUE;
	D3D11_RENDER_TARGET_BLEND_DESC rtBlendDesc;
//	rtBlendDesc.BlendEnable = TRUE;
//	rtBlendDesc.BlendOp = D3D11_BLEND_OP_ADD;
//	rtBlendDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
//	rtBlendDesc.DestBlend = D3D11_BLEND_SRC_COLOR;
//	rtBlendDesc.DestBlendAlpha = D3D11_BLEND_SRC_COLOR;
////	rtBlendDesc.RenderTargetWriteMask = 
//	rtBlendDesc.SrcBlend = D3D11_BLEND_SRC_COLOR;
//	rtBlendDesc.SrcBlendAlpha = D3D11_BLEND_SRC_COLOR;
//	blendDesc.RenderTarget[0] = rtBlendDesc;
	rtBlendDesc.BlendEnable = TRUE;
    rtBlendDesc.SrcBlend = D3D11_BLEND_SRC_ALPHA;
    rtBlendDesc.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	rtBlendDesc.BlendOp = D3D11_BLEND_OP_ADD;
    rtBlendDesc.SrcBlendAlpha = D3D11_BLEND_ZERO;
    rtBlendDesc.DestBlendAlpha = D3D11_BLEND_ZERO;
    rtBlendDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
    rtBlendDesc.RenderTargetWriteMask = 0x0F;	
	blendDesc.RenderTarget[0] = rtBlendDesc;
	blendDesc.RenderTarget[1] = rtBlendDesc;
	m_pd3ddevice->CreateBlendState(&blendDesc, &m_pd3dBlendState);


	m_eRType = NONE;
	AnimationInit();
}

/*****************************************************************
 * AddTexture():  Adds a texture to the manager for rendering onto a model or other geometry
 *
 * Ins:					textureLoc	- Directory and file name of the texture
						imgWidth	- width of the image file
						imgHeight	- height of the image
 *      		      
 *
 *
 * Returns:		      unsigned int - location of the object in the vector for render reference
 *
 * Mod. Date:		      06/09/2014
 * Mod. Initials:	      BGM
 *****************************************************************/

/*///
A copy paste of the AddTexture func.  Made it overloaded.
*/
D2DObject * CRenderer::AddTexture(ID3D11Texture2D * texture, float imgWidth, float imgHeight)
{
	D2DObject * testObj = new D2DObject(texture, imgWidth, imgHeight);
	AddD2D(testObj);
	return testObj;
}

void CRenderer::AddD2D(D2DObject * testObj)
{
	D3D11_SUBRESOURCE_DATA pData;
	pData.pSysMem = testObj->GetQuadLoc();
	hr = m_pd3ddevice->CreateBuffer(&testObj->GetDataBufferDesc(), &pData, testObj->GetDataBuffer());

	D3D11_SUBRESOURCE_DATA pDataLowQuad;
	pDataLowQuad.pSysMem = testObj->GetQuadLoc();
	m_pd3ddevice->CreateBuffer(&testObj->GetDataBufferDesc(), &pDataLowQuad, testObj->GetDataBuffer());
		
	hr = CreateDDSTextureFromFile(m_pd3ddevice, testObj->GetTextureLocation(), (ID3D11Resource**)(testObj->GetTexture()), testObj->GetSRV());
	hr = m_pd3ddevice->CreateSamplerState(testObj->GetSamplerDesc(), testObj->GetSampler());

	D3D11_BUFFER_DESC sizeBufferDesc;
	sizeBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	sizeBufferDesc.ByteWidth = sizeof(DISPLAY_SIZE);
	sizeBufferDesc.MiscFlags = 0;
	sizeBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	sizeBufferDesc.Usage = D3D11_USAGE_DYNAMIC;

	D3D11_SUBRESOURCE_DATA sizeData;
	sizeData.pSysMem = testObj->GetSize();
	hr = m_pd3ddevice->CreateBuffer( &sizeBufferDesc, &sizeData, &sizeBuffer);
}

/*****************************************************************
 * AddModel():  Adds a 3D Model to the manager for rendering
 *
 * Ins:					OBJFileLoc - Directory and file name of the model
						modelTexLoc -	Directory and file name of the models texture
 *      		      
 *
 *
 * Returns:		      unsigned int - location of the object in the vector for render reference
 *
 * Mod. Date:		      06/09/2014
 * Mod. Initials:	      BGM
 *****************************************************************/
D3DObject * CRenderer::AddModel(D3DObject * d3dobj, const char * OBJFileLoc)
{
	d3dobj = new D3DObject;
	d3dobj->FileReader(OBJFileLoc);
	hr = m_pd3ddevice->CreateBuffer(d3dobj->GetObjLoader()->GetBufferDesc(), d3dobj->GetObjLoader()->GetData(), d3dobj->GetBuffer());

	return d3dobj;
}

/*****************************************************************
 * PreRender():		Set render target, viewport, clearscreen color, and 
					CLEARS THE RENDERTARGETVIEW to prep for rendering
 * Returns:				void
 * Mod. Date:		      06/09/2014
 * Mod. Initials:	      BGM
 *****************************************************************/
void CRenderer::PreRender()
{
	m_pd3dcontext->RSSetViewports(1, &vPort);
	FLOAT x[] = {1, .6f, .5f, .5f};
	m_pd3dcontext->ClearRenderTargetView(renderTargetTexture, x);
	m_pd3dcontext->ClearDepthStencilView(m_pd3dstencilView, D3D11_CLEAR_DEPTH, 1, 0);
	m_pd3dcontext->OMSetRenderTargets(1, &renderTargetTexture, m_pd3dstencilView);	
//	m_pd3dcontext->OMSetBlendState(m_pd3dBlendState, NULL, 0xFFFFFF);
}

/*****************************************************************
 * Render2D():  Render a 2D image
 *
 * Ins:					vectorLoc - location in the 2D container of the image
						imgCoords -	screen space for the image to appear
 *      				imgPart - portion of the image to show
 *
 *
 * Returns:				void
 *
 * Mod. Date:		      06/09/2014
 * Mod. Initials:	      BGM
 *****************************************************************/
void CRenderer::Render2D(D2DObject * texture, XMFLOAT4 imgCoords, XMFLOAT4 imgPart)
{
		texture->ChangeCoords(imgCoords, imgPart);
		D3D11_MAPPED_SUBRESOURCE edit;
		m_pd3dcontext->Map(sizeBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &edit);
		memcpy(edit.pData, texture->GetSize(), sizeof(DISPLAY_SIZE));
		m_pd3dcontext->Unmap(sizeBuffer, 0);

		unsigned int sizeT = sizeof(V_IN);
		unsigned int offsetT = 0;
		m_pd3dcontext->PSSetShaderResources(0, 1, texture->GetSRV());
		m_pd3dcontext->PSSetSamplers(0, 1, texture->GetSampler());
		m_pd3dcontext->IASetVertexBuffers(0, 1, texture->GetDataBuffer(), &sizeT, &offsetT);
		m_pd3dcontext->PSSetConstantBuffers(		  0, 0, nullptr);
	if(m_eRType != TWOD)
	{	
		m_eRType = TWOD;
		ID3D11Buffer * buffers[] = {sizeBuffer, sceneBuffer};
		m_pd3dcontext->VSSetConstantBuffers(0, 2, buffers);
		m_pd3dcontext->PSSetShader(m_pd3dtexPShader, NULL, 0);
		m_pd3dcontext->VSSetShader(m_pd3dtexVShader, NULL, 0);
		m_pd3dcontext->IASetInputLayout(texLayout);
		m_pd3dcontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}
	m_pd3dcontext->Draw(6, 0);

	m_pd3dcontext->ClearDepthStencilView(m_pd3dstencilView, D3D11_CLEAR_DEPTH, 1, 0);
}


/*****************************************************************
 * Render3D():  Render a 3D image with or without a texture
 *
 * Ins:					vectorLoc - location in the 2D container of the image
						imgCoords -	screen space for the image to appear
 *      				imgPart - portion of the image to show
 *						color - color output for untextured models
 *
 * Returns:				void
 *
 * Mod. Date:		      06/09/2014
 * Mod. Initials:	      BGM
 *****************************************************************/
void CRenderer::Render3D(D3DObject * mesh, D2DObject * texture, XMFLOAT4 color)
{

	unsigned int offsetT = 0;
	unsigned int sizeT = sizeof(commonObject);

	object.worldMatrix = *mesh->GetMatrix();
	D3D11_MAPPED_SUBRESOURCE edit;
	m_pd3dcontext->Map(objectBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &edit);
	memcpy(edit.pData, &object.worldMatrix, sizeof(TOBJECT));
	m_pd3dcontext->Unmap(objectBuffer, 0);

	ID3D11Buffer * buffers[] = {objectBuffer,  sceneBuffer, colorBuffer};

	if(texture == nullptr)
	{
		D3D11_MAPPED_SUBRESOURCE editColor;
		m_pd3dcontext->Map(colorBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &editColor);
		memcpy(editColor.pData, &color, sizeof(XMFLOAT4));
		m_pd3dcontext->PSSetShaderResources(0, 0, nullptr);
		m_pd3dcontext->Unmap(colorBuffer, 0);
		m_pd3dcontext->IASetVertexBuffers(0, 1, mesh->GetBuffer(), &sizeT, &offsetT);
		m_pd3dcontext->PSSetSamplers( 0, 0, nullptr);
	}
	else
	{
		m_pd3dcontext->PSSetConstantBuffers(0, 1, &variantBuffer);
		m_pd3dcontext->PSSetShaderResources(0, 1, texture->GetSRV() );
		m_pd3dcontext->PSSetSamplers(		  0, 1, texture->GetSampler());

		m_pd3dcontext->IASetVertexBuffers(0, 1, mesh->GetBuffer(), &sizeT, &offsetT);
	}

	if(m_eRType != OBJNOTEX && texture == nullptr)
	{
		m_eRType = OBJNOTEX;
		m_pd3dcontext->VSSetShader(m_pd3dobjNoTexVShader, NULL, 0);
		m_pd3dcontext->VSSetConstantBuffers(0, 3, buffers);
		m_pd3dcontext->PSSetShader(m_pd3dobjNoTexPShader, NULL, 0);
		m_pd3dcontext->PSSetConstantBuffers(0, 1, &lightBuffer);
		m_pd3dcontext->IASetInputLayout(OBJLayout);
		m_pd3dcontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);	
	}
	else if(m_eRType != OBJTEX && texture)
	{
		m_eRType = OBJTEX;
		m_pd3dcontext->VSSetShader(m_pd3dobjVShader, NULL, 0);
		m_pd3dcontext->VSSetConstantBuffers(0, 2, buffers);
		m_pd3dcontext->IASetInputLayout(OBJLayout);
		m_pd3dcontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);		
		if(mesh->GetShader())
			m_pd3dcontext->PSSetShader(mesh->GetShader(), NULL, 0);
		else
			m_pd3dcontext->PSSetShader(m_pd3dobjPShader, NULL, 0);
	}

	m_pd3dcontext->Draw(mesh->GetObjLoader()->Size(), 0);
}

void CRenderer::PostRender()
{
//	m_pd3dcontext->OMSetBlendState(nullptr, NULL, NULL);
	m_pd3dcontext->GenerateMips(RTTshaderSRV);
	FLOAT clearScreen[] = {0, 0, 0, 1};
	m_pd3dcontext->ClearRenderTargetView(m_pd3drenderTarget, clearScreen);
	m_pd3dcontext->ClearDepthStencilView(m_pd3dstencilView, D3D11_CLEAR_DEPTH, 1, 0);
	m_pd3dcontext->OMSetRenderTargets(1, &m_pd3drenderTarget, m_pd3dstencilView);

	renderTargetD2D->ChangeCoords(XMFLOAT4(0, 0, BACKBUFFER_WIDTH, BACKBUFFER_HEIGHT), XMFLOAT4(0, 0, BACKBUFFER_WIDTH, BACKBUFFER_HEIGHT));

	D3D11_MAPPED_SUBRESOURCE edit;
	m_pd3dcontext->Map(sizeBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &edit);
	memcpy(edit.pData, renderTargetD2D->GetSize(), sizeof(DISPLAY_SIZE));
	m_pd3dcontext->Unmap(sizeBuffer, 0);

	unsigned int sizeT = sizeof(V_IN);
	unsigned int offsetT = 0;
	ID3D11Buffer * buffers[] = {sizeBuffer, sceneBuffer};
	m_pd3dcontext->VSSetConstantBuffers(0, 2, buffers);
	m_pd3dcontext->PSSetShaderResources(0, 1, &RTTshaderSRV );//D2DObjects[vectorLoc]->GetSRV());
	m_pd3dcontext->PSSetShader(m_pd3dtexPShader, NULL, 0);
	m_pd3dcontext->PSSetConstantBuffers(0, 1, &variantBuffer);
	m_pd3dcontext->PSSetSamplers(0, 1, renderTargetD2D->GetSampler());
	m_pd3dcontext->IASetVertexBuffers(0, 1, renderTargetD2D->GetDataBuffer(), &sizeT, &offsetT);
	m_pd3dcontext->VSSetShader(m_pd3dtexVShader, NULL, 0);
	m_pd3dcontext->IASetInputLayout(texLayout);
	m_pd3dcontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pd3dcontext->Draw(6, 0);

	m_pd3dswapChain->Present(0, 0);
	m_eRType = NONE;
}

/*****************************************************************
 * ShutDown():  Releases D3D device, and swapChain, cleans up dynamic memory
 *
 * Returns:				bool
 *
 * Mod. Date:		      06/09/2014
 * Mod. Initials:	      BGM
 *****************************************************************/
bool CRenderer::ShutDown()
{
	delete renderTargetD2D;
	renderTargetD2D = nullptr;

	m_pd3dcontext->ClearState();
	m_pd3ddevice->Release();
	m_pd3dswapChain->Release();


	if(BullshiTexture)
		delete BullshiTexture;
	UnregisterClass( L"DirectXApplication", application ); 
	return true;
}

ID3D11PixelShader * CRenderer::GetPShader(string name)
{
	for(unsigned int i = 0; i < pixelShaders.size(); i++)
		if(name == pixelShaders[i].GetName())
			return pixelShaders[i].GetShader();
	return nullptr;
}

void CRenderer::Update(void)
{
	/////////// Updating delta time
	xtime.Signal();
	variant.x += (float)xtime.Delta()*.1f;
	variant.y += (float)xtime.Delta()/1.0f;
	variant.z += (float)xtime.Delta()/1.0f;
	D3D11_MAPPED_SUBRESOURCE edit;
	m_pd3dcontext->Map(variantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &edit);
	memcpy(edit.pData, &variant, sizeof(XMFLOAT4));
	m_pd3dcontext->Unmap(variantBuffer, 0);
	///////////////////////////////////
	
	// Map Unmap camera view and projection matrices as well as dimensions of the screen
	m_pd3dcontext->Map(sceneBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &edit);
	memcpy(edit.pData, &scene, sizeof(TSCENE));
	m_pd3dcontext->Unmap(sceneBuffer, 0);

	m_pd3dcontext->Map(lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &edit);
	memcpy(edit.pData, &light, sizeof(TLIGHT)*m_nNumLights);
	m_pd3dcontext->Unmap(lightBuffer, 0);

}

/*****************************************************************
 * AnimationInit():  Private func called from Init() for Romans Animations Initializations
 *
 * Returns:				void
 *
 * Mod. Date:		      06/09/2014
 * Mod. Initials:	      RT
 *****************************************************************/
void CRenderer::AnimationInit()
{

	BullshiTexture = new D2DObject(L"assets/TestCube.dds",1024.0f,1024.0f);
	this->AddD2D(BullshiTexture);


	hr = m_pd3ddevice->CreateVertexShader(AnimVertShader, sizeof(AnimVertShader), NULL, &m_pd3dAnimationShader);

	//Layout DESC
	// position
	// normal
	// UV
	// Bone Index x4
	// Bone Weight x4

	D3D11_INPUT_ELEMENT_DESC layoutAnim[] =  
	{
		{	 "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{	 "NORMAL", 0,	DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{    "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{    "BONEIDS",0,	DXGI_FORMAT_R32G32B32A32_UINT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
		{    "BONEWEIGHTS",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0}
		
	};	
	hr = m_pd3ddevice->CreateInputLayout( layoutAnim, 5, AnimVertShader, sizeof(AnimVertShader), &AnimLayout);
}

/*****************************************************************
 * Render3D():  Overloaded Render3D unc for animation rendering
 *
 * Returns:				void
 *
 * Mod. Date:		      07/22/2014
 * Mod. Initials:	      RT
 *****************************************************************/
void CRenderer::RenderAnim3D(D3DAnimObject * AnimObject, D2DObject * texture, XMFLOAT4 color)
{
	if(m_eRType != ANIMATION) //DONT FORGET SOME THINGS STILL NEED TO BE CONTEXT SWITCHED (BUFERS AND SHIT)
	{
		m_eRType = ANIMATION;
	}

	unsigned int offsetT = 0;
	unsigned int sizeT = sizeof(tAnimVert);

	//MATRIX BUFFER
	object.worldMatrix = *AnimObject->GetMatrix();
	D3D11_MAPPED_SUBRESOURCE edit;
	m_pd3dcontext->Map(objectBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &edit);
	memcpy(edit.pData, &object.worldMatrix, sizeof(TOBJECT));
	m_pd3dcontext->Unmap(objectBuffer, 0);

	//MODEL VIEW PROJECTION
	m_pd3dcontext->Map(sceneBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &edit);
	memcpy(edit.pData, &scene, sizeof(TSCENE));
	m_pd3dcontext->Unmap(sceneBuffer, 0);

	//bones
	m_pd3dcontext->Map(*(AnimObject->GetBoneBuffer()), 0, D3D11_MAP_WRITE_DISCARD, 0, &edit);
		memcpy(edit.pData, AnimObject->GetBoneData(), (sizeof(XMFLOAT4X4) * AnimObject->GetNumBones()));
	m_pd3dcontext->Unmap(*(AnimObject->GetBoneBuffer()), 0);


	ID3D11Buffer * buffers[] = {objectBuffer,  sceneBuffer, colorBuffer,*AnimObject->GetBoneBuffer()};
	texture = BullshiTexture;
 	if(texture == nullptr)
 	{
 		D3D11_MAPPED_SUBRESOURCE editColor;
 		m_pd3dcontext->Map(colorBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &editColor);
 		memcpy(editColor.pData, &color, sizeof(XMFLOAT4));
 		m_pd3dcontext->Unmap(colorBuffer, 0);
 		m_pd3dcontext->IASetVertexBuffers(0, 1, AnimObject->GetVertBuffer(), &sizeT, &offsetT);
 	}
 	else
 	{
 		m_pd3dcontext->PSSetConstantBuffers(0, 1, &variantBuffer);
 		m_pd3dcontext->PSSetShaderResources(0, 1, texture->GetSRV() );
 		m_pd3dcontext->PSSetSamplers(		  0, 1, texture->GetSampler());
  		m_pd3dcontext->IASetVertexBuffers(0, 1, AnimObject->GetVertBuffer(), &sizeT, &offsetT);
 	}
 
	
	m_pd3dcontext->VSSetConstantBuffers(0,4,buffers);
	m_pd3dcontext->IASetIndexBuffer(*AnimObject->GetIndexBuffer(),DXGI_FORMAT_R32_UINT,0);
	m_pd3dcontext->IASetInputLayout(AnimLayout);

	m_pd3dcontext->PSSetShader(m_pd3dobjPShader,NULL,NULL);
	m_pd3dcontext->VSSetShader(m_pd3dAnimationShader,NULL,NULL);
	m_pd3dcontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_pd3dcontext->DrawIndexed(AnimObject->GetMeshData()->GetNumTris()*3,0,0);


}

