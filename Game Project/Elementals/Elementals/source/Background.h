#pragma once

#include "Renderer.h"

class Background
{

	unsigned int texture;
	XMFLOAT4 coords;  
	XMFLOAT4 imgPart;
	float delta;
	XTime xtime;

public:
	Background(void);
	~Background(void);
	void Render();
	void Update();
};

