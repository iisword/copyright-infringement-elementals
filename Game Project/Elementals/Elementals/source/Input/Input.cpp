#include "Input.h"


CInput * CInput::GetInstance(void)
{
	// Static local variable
	static CInput s_Instance;

	// Return the ADDRESS
	return &s_Instance;
}

CInput::CInput()
{
	mouse.x = 0;
	mouse.y = 0;
	mousePressedL = false;
	mousePressedR = false;
	wheelDelta = 0.0f;
}

CInput::~CInput(void)
{

}

void CInput::Update(HWND hWnd, UINT message,LPARAM lParam )
{
    RECT rcClient;                 // client area rectangle 
    POINT ptClientUL;              // client upper left corner 
    POINT ptClientLR;              // client lower right corner 
    //poll state of the keyboard
	switch(message)
	{
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
		{
			GetKeyboardState((PBYTE)GetKeys());
		}
		break;
		case WM_LBUTTONDOWN:  
		case WM_RBUTTONDOWN:  
        case WM_MOUSEMOVE: 
            // Capture mouse input.  
			SetCapture(hWnd);

			if(message == WM_LBUTTONDOWN)
				SetMousePressedL(true);
			if(message == WM_RBUTTONDOWN)
				SetMousePressedR(true);

            // Retrieve the screen coordinates of the client area, 
            // and convert them into client coordinates.  
            GetClientRect(hWnd, &rcClient); 
            ptClientUL.x = rcClient.left; 
            ptClientUL.y = rcClient.top; 
 
            // Add one to the right and bottom sides, because the 
            // coordinates retrieved by GetClientRect do not 
            // include the far left and lowermost pixels.  
            ptClientLR.x = rcClient.right + 1; 
            ptClientLR.y = rcClient.bottom + 1; 
            ClientToScreen(hWnd, &ptClientUL); 
            ClientToScreen(hWnd, &ptClientLR); 
 
            // Copy the client coordinates of the client area 
            // to the rcClient structure. Confine the mouse cursor 
            // to the client area by passing the rcClient structure 
            // to the ClipCursor function.  
            SetRect(&rcClient, ptClientUL.x, ptClientUL.y, 
                ptClientLR.x, ptClientLR.y); 
            //ClipCursor(&rcClient); 
            // Convert the cursor coordinates into a POINTS 
            // structure, which defines the beginning point of the 
            // line drawn during a WM_MOUSEMOVE message. 
 
//			SetCursorPos(ptClientLR.x/2, ptClientLR.y/2);
			SetPrevPos();
			SetMousePos( MAKEPOINTS(lParam));
			
//            return 0; 
 //           break; 
		case WM_LBUTTONUP: 
		case WM_RBUTTONUP:  
			ReleaseCapture(); 
			if(message == WM_LBUTTONUP)
				SetMousePressedL(false);
			if(message == WM_RBUTTONUP)
				SetMousePressedR(false);

			return; 
	}

}
    
void CInput::Reset()
{
	memcpy(prevKeys, keys, sizeof(bool) * 256);
}

bool CInput::IsKey(int keyIn)
{ 
	if(keys[keyIn] & 0x80) 
		return true;
	return false;
};


