#include "DebugConsole.h"

#include <fcntl.h>
#include <io.h>
#include <iostream>

HANDLE CDebugConsole::m_hOutputHandle = nullptr;
HANDLE CDebugConsole::m_hInputHandle = nullptr;
HANDLE CDebugConsole::m_hErrorHandle = nullptr;
CONSOLE_SCREEN_BUFFER_INFO CDebugConsole::m_tBufferInfo = {};
bool CDebugConsole::m_bShouldClose = nullptr;
CDebugConsole::CDebugConsole()
{
	
}

CDebugConsole::~CDebugConsole()
{

}

void CDebugConsole::ShowConsoleWindow()
{
	if(AllocConsole())
	{
		//output buffer
		m_hOutputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		int hCrt = _open_osfhandle((long)m_hOutputHandle, _O_TEXT);
		FILE *hFout = _fdopen(hCrt, "w");
		setvbuf(hFout, NULL, _IONBF, 0);
		*stdout = *hFout;

		//input buffer
		m_hInputHandle = GetStdHandle(STD_INPUT_HANDLE);
		hCrt = _open_osfhandle((long)m_hInputHandle, _O_TEXT);
		FILE * hFIn = _fdopen(hCrt, "r");
		setvbuf(hFIn, NULL, _IONBF, 0);
		*stdin = *hFIn;

		//error buffer
		m_hErrorHandle = GetStdHandle(STD_ERROR_HANDLE);
		hCrt = _open_osfhandle((long)m_hErrorHandle, _O_TEXT);
		FILE * hFErr = _fdopen(hCrt, "w");
		setvbuf(hFErr, NULL, _IONBF, 0);
		*stderr = *hFErr;

		std::ios::sync_with_stdio();

		GetConsoleScreenBufferInfo(m_hOutputHandle, &m_tBufferInfo);

		SetConsoleCtrlHandler(&CDebugConsole::ConsoleHandler, true);
	}
}

void CDebugConsole::DestroyConsoleWindow()
{
	FreeConsole();
	SetConsoleCtrlHandler(&CDebugConsole::ConsoleHandler, false);
	m_hOutputHandle = nullptr;
	m_hErrorHandle = nullptr;
	m_hInputHandle = nullptr;
}

BOOL WINAPI CDebugConsole::ConsoleHandler(DWORD CEvent)
{
	if(CEvent == CTRL_CLOSE_EVENT || CEvent == CTRL_LOGOFF_EVENT || CEvent == CTRL_SHUTDOWN_EVENT)
	{
		printf("Closing application from console.");
		m_bShouldClose = true;
	}
	return TRUE;
}

void CDebugConsole::ClearConsole()
{
	if(m_hOutputHandle != nullptr)
	{
		DWORD nCharWritten = 0;
		DWORD nConsoleArea = m_tBufferInfo.dwSize.X * m_tBufferInfo.dwSize.Y;
	
		m_tCursorPosition.X = 0;
		m_tCursorPosition.Y = 0;
	
		FillConsoleOutputCharacter(m_hOutputHandle, (TCHAR)' ', nConsoleArea, m_tCursorPosition, &nCharWritten);

		GetConsoleScreenBufferInfo(m_hOutputHandle, &m_tBufferInfo);

		FillConsoleOutputAttribute(m_hOutputHandle, m_tBufferInfo.wAttributes, nConsoleArea, m_tCursorPosition, &nCharWritten);

		SetConsoleCursorPosition(m_hOutputHandle, m_tCursorPosition);
	}
}

void CDebugConsole::UseCurrentPosition()
{
	if(m_hOutputHandle)
	{
		SetConsoleCursorPosition(m_hOutputHandle, m_tCursorPosition);
	}
}

void CDebugConsole::SetCursorPosition(short sX, short sY)
{
	m_tCursorPosition.X = sX;
	m_tCursorPosition.Y = sY;
	if(m_hOutputHandle)
	{
		SetConsoleCursorPosition(m_hOutputHandle, m_tCursorPosition);
	}
}

void CDebugConsole::SetAttributes(unsigned short sAttributes)
{
	if(m_hOutputHandle)
	{
		SetConsoleTextAttribute(m_hOutputHandle, sAttributes);
	}
}

bool CDebugConsole::ShouldShutdown()
{
	return m_bShouldClose;
}