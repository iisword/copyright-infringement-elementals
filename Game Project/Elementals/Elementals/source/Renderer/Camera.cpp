#include "Camera.h"


CCamera::CCamera()
{
	cameraMatrix.SetMatrixToIdentity();
	input = CInput::GetInstance();
	m_pTimer = CTimer::GetInstance();
	focus = 15.0f;
}


CCamera::~CCamera(void)
{

}

void CCamera::Init()
{
	cameraMatrix.GetCMatrix()->TranslateLocal(XMFLOAT3(0, 0, 0));

}

void CCamera::Input(XMFLOAT4X4 * mat, float rot)
{
	int back = -15;
	int up = 10;
	XMFLOAT3 lookDir = XMFLOAT3(mat->_31*back, mat->_32*back, mat->_33*back);
	lookDir.x += mat->_21*up;
	lookDir.y += mat->_22*up;
	lookDir.z += mat->_23*up;
	XMFLOAT3 lookAt(mat->_41+(mat->_31*focus), mat->_42+(mat->_32*focus), mat->_43+(mat->_33*focus));
	XMStoreFloat4x4(cameraMatrix.GetMatrix(), DirectX::XMMatrixLookAtLH(
		XMVectorSet(mat->_41+lookDir.x, mat->_42+lookDir.y, mat->_43+lookDir.z, 1), XMVectorSet(lookAt.x, lookAt.y, lookAt.z, 0), XMVectorSet(mat->_21, mat->_22, mat->_23, 0)));
}

void CCamera::AdjustFocus(float focusIn)
{ 
	focus += focusIn; 
	if(focus > 20.0f)
		focus = 20.0f;
	else if(focus < 5.0f)
		focus = 5.0f;

};




