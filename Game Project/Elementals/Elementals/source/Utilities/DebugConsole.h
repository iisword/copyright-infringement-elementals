/***********************************************
* Filename:  		DebugConsole.h
* Date:      		7/15/2014
* Mod. Date: 		7/15/2014
* Mod. Initials:	MB
* Author:    		Michael Boodle
* Purpose:   		Supplies a debug console to the application.
************************************************/

#ifndef DEBUGCONSOLE_HEADER
#define DEBUGCONSOLE_HEADER

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>

class CDebugConsole
{
	static HANDLE m_hOutputHandle;
	static HANDLE m_hInputHandle;
	static HANDLE m_hErrorHandle;
	static CONSOLE_SCREEN_BUFFER_INFO m_tBufferInfo;
	static bool m_bShouldClose;

	COORD m_tCursorPosition;

	static BOOL WINAPI ConsoleHandler(DWORD CEvent);
public:
	CDebugConsole();
	~CDebugConsole();
#pragma region Console Window Fuctions
	/**********************************
	*	ShowConsoleWindow:			Brings up a console window.
	*
	*	Return:						void
	*
	*	Mod. Date:					7/17/2014
	*	Mod. Initials:				MB
	**********************************/
	void ShowConsoleWindow();

	/**********************************
	*	DestroyConsoleWindow:		Frees the console window.
	*
	*	Return:						void
	*
	*	Mod. Date:					7/17/2014
	*	Mod. Initials:				MB
	**********************************/
	void DestroyConsoleWindow();
#pragma endregion

#pragma region Output Manipulation
	/**********************************
	*	ClearConsole:			Clears the console window
	*
	*	Return:					void
	*
	*	Mod. Date:				7/17/2014
	*	Mod. Initials:			MB
	**********************************/
	void ClearConsole();

	/**********************************
	*  UseCurrentPosition:			Forces the console to the current position of the instance. 
	*
	*  Return:						void
	*
	*  Mod. Date:					7/17/2014
	*  Mod. Initials:				MB
	**********************************/
	void UseCurrentPosition();
#pragma endregion

	/////////////
	//Accessors//
	/////////////
	static bool ShouldShutdown();

	/////////////
	//Mutators //
	/////////////
	void SetCursorPosition(short sX, short sY);
	void SetAttributes(unsigned short sAttributes);
};



#endif