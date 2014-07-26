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
	bool IsW(){return (keys[0x57] & 0x80 && !(prevKeys[0x57] & 0x80));};
	bool IsA(){return (keys[0x41] & 0x80 && !(prevKeys[0x41] & 0x80));};
	bool IsS(){return (keys[0x53] & 0x80 && !(prevKeys[0x53] & 0x80));};
	bool IsD(){return (keys[0x44] & 0x80 && !(prevKeys[0x44] & 0x80));};
	bool IsEnter(){return (keys[VK_RETURN] & 0x80 && !(prevKeys[VK_RETURN] & 0x80));};
	bool IsEsc() { return (keys[VK_ESCAPE] & 0x80 && !(prevKeys[VK_ESCAPE] & 0x80));};
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

