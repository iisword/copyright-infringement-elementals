#include "Background.h"
#include "commonObject.h"


Background::Background(void)
{
	cRenderer * pTM = cRenderer::GetInstance();
	texture = pTM->AddTexture(L"Bgrounds/generic1.dds", 2500.0f, 2500.0f);

	coords  = XMFLOAT4(00, 00, 1280, 768);
	imgPart = XMFLOAT4(1500, 1750, 1000, 750); // Upper left then width and height...
}


Background::~Background(void)
{
}

void Background::Render()
{
	cRenderer * pTM = cRenderer::GetInstance();
	pTM->Render2D(texture, coords, imgPart);

}

void Background::Update()
{
	xtime.Signal();
	delta = (float)xtime.Delta();

//	imgPart.x += .1;
	imgPart.y -= 10*delta;
}


