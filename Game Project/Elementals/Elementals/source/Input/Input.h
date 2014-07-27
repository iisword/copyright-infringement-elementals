#pragma once
#include <windows.h>
class CInput
{
    
	char keys[256];
	BYTE prevKeys[256];
	POINTS mouse, prevPosition;
	bool mousePressedL, mousePressedR;
	CInput();
	float wheelDelta;


public:

	static CInput * GetInstance(void);

	~CInput(void);

	void Update(HWND hWnd, UINT message, LPARAM lParam );
	char * GetKeys(){ return keys; };
	BYTE * GetPrevKeys(){ return prevKeys; };
	bool IsKey(int keyIn);
	bool IsKeyOnce(int keyIn){ return (keys[keyIn] & 0x80 && !(prevKeys[keyIn] & 0x80));};
	void SetMousePos(POINTS pt){ mouse = pt; };
	void ChangeWheelDelta(float wheel){ wheelDelta = wheel; };
	float GetWheelDelta(){ return wheelDelta / 1200.0f; };

	short GetMouseX(){ return mouse.x; };
	short GetMouseY(){ return mouse.y; };
	POINTS GetMouse(){ return mouse; };
	POINTS * GetPrevPos(){ return &prevPosition; };
	void SetPrevPosX(short x){ prevPosition.x = x;};
	void SetPrevPosY(short y){ prevPosition.y = y;};
	void SetPrevPos(){ prevPosition = mouse; };
	void SetMousePressedL(bool pressed){ mousePressedL = pressed; };
	void SetMousePressedR(bool pressed){ mousePressedR = pressed; };
	bool GetMousePressedL(){ return mousePressedL; };
	bool GetMousePressedR(){ return mousePressedR; };
	void Reset();

};

