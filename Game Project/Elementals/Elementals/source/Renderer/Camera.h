#pragma once

#include "Renderer.h"
#include "../Input/Input.h"
#include "D3DObjects.h"
#include "../Utilities/Timer.h"

class CCamera
{
//	XMFLOAT4X4 cameraMatrix;
	D3DObject cameraMatrix;
//	POINTS prevPosition;
	CInput * input;
	CTimer * m_pTimer;
	float focus;


public:

	CCamera();
	~CCamera(void);

	void Input(XMFLOAT4X4 * mat, float rot);
	void Init();
	XMFLOAT4X4 * GetCameraMatrix(){ return cameraMatrix.GetMatrix(); };
	void SetCameraMatrix(XMFLOAT4X4 inMat){ cameraMatrix.SetMatrix(inMat); };
	D3DObject * GetMesh(){ return &cameraMatrix; };
	//POINTS * GetPrevPos(){ return &prevPosition; };
	//void SetPrevPosX(short x){ prevPosition.x = x;};
	//void SetPrevPosY(short y){ prevPosition.y = y;};
	void AdjustFocus(float focusIn); 
};

