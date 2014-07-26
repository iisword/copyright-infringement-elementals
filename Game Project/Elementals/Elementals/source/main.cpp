/***********************************************
* Filename:  		main.h
* Date:      		6/27/2014
* Mod. Date: 		6/27/2014
* Mod. Initials:	MB
* Author:    		Michael Boodle
* Purpose:   		Starting point for the server/client applications.
************************************************/

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <shellapi.h>
#include <ShlObj.h>
#include <iostream>
#include <string>
#include <assert.h>
#include <fcntl.h>
#include <io.h>
using namespace std;

#include <vld.h>

#include "State\Game.h"
#include "Input\Input.h"
#include "Utilities\DebugConsole.h"
#include "Utilities\Utilities.h"
#include "Utilities\Timer.h"
using namespace Utilities;
#ifdef SERVER_BUILD
//Server includes
#include "Server\ServerFactory.h"
#include "Server\Server.h"

//Server defines
#define SERVER_CLASS_NAME					L"Elemental Server App"


#define SERVER_DEF_CONFIG_FILE				L"\\assests\\defaults\\config.con"

//Server commands
#define SERVER_COMMAND_CONFIG_FILE			L"c"
#define SERVER_COMMAND_BASE					L"b"
#define SERVER_COMMAND_DEATHMATH			L"d"

//Server only functions
wstring GetConfigFilePath();
#else
//Client includes
#include "Audio\AudioManager.h"
#include "Audio\AudioSource.h"
//Client defines
#define CLIENT_CLASS_NAME					L"Elementals App"
#define CLIENT_WINDOW_NAME					L"Elementals"
#define CLIENT_WINDOW_WIDTH					800
#define CLIENT_WINDOW_HEIGHT				600


#endif

class CServer;

//int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine,	int nCmdShow );						   
LRESULT CALLBACK WndProc(HWND hwnd,	UINT message, WPARAM wparam, LPARAM lparam );

CInput * input;

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
#ifdef SERVER_BUILD
	//Put server code here
	CServer * pcServer = nullptr;
	CDebugConsole cConsole;
	//TODO: Open a console window
	cConsole.ShowConsoleWindow();
	
	//TODO: Use command line to configure server
	//Get the command line and converts it to an argument style
	LPWSTR * szCommandLine;
	int nArgCount;

	szCommandLine = CommandLineToArgvW(GetCommandLineW(), &nArgCount);

	if(nArgCount > 1)
	{
		wstring szArg = szCommandLine[1];
		if(szArg == SERVER_COMMAND_CONFIG_FILE)
		{
			wstring szConfigFile;
			if(nArgCount > 2)
			{
				//a path to a config file has been specificied
				szConfigFile = szCommandLine[2];
				if(!PathIsValid(szConfigFile, false))
				{
					cout << "ERROR: Path specified is not valid." << endl;
					return 1;
				}
			}
			else
			{
				//use default config file
				//szConfigFile = GetConfigFilePath();
			}

			//TODO: Start a server from the config
		}
		else if(szArg == SERVER_COMMAND_BASE)
		{
			//TODO: Start a server with no win condition
		}
		else if(szArg == SERVER_COMMAND_DEATHMATH)
		{
			//TODO: Start a deathmatch server
		}
		
		//TODO: Display help
		cout << "Elementals Server Help" << endl;
		cout << "CMD			Desc" << endl;
		cout << "c [path]		Starts a server base on the config file" << endl;
		cout << "b [players]		Starts a base server" << endl;
		cout << "d [players]		Starts a deathmatch server" << endl;
	}
	else
	{
		//TODO: Start a server based on the default server config
		//wstring szConfigFile = GetConfigFilePath();

		pcServer = new CServer();

	}
	
	if(pcServer && pcServer->Initialize())
	{
		if(pcServer->LoadCharacterStats("assets\\defaults\\CI_XML_BasicStats.xml"))
		{
			CTimer::GetInstance()->Signal();
			while (CDebugConsole::ShouldShutdown() == false)
			{
				CTimer::GetInstance()->Signal();
				pcServer->Update((float)CTimer::GetInstance()->Delta());
			}
		}
	}
	
	cConsole.DestroyConsoleWindow();
	pcServer->Shutdown();
	delete pcServer;
#else
	//Put client code here
//	HWND hwnd;
//	const TCHAR * szWindowTitle = CLIENT_WINDOW_NAME;
//	const TCHAR * szWindowClassName = CLIENT_CLASS_NAME;
//	const int nWindowWidth = CLIENT_WINDOW_WIDTH;
//	const int nWindowHeight = CLIENT_WINDOW_HEIGHT;
//	const bool bIsWindowed = TRUE;
//
////	WNDPROC appWndProc; ////////EDITED BY BEN
//
//	//Register class
//	
//	WNDCLASSEX tWCEX;
//	ZeroMemory(&tWCEX, sizeof(tWCEX));
//	tWCEX.cbSize = sizeof(tWCEX);
//	tWCEX.lpfnWndProc = WndProc;				//TODO: Decide if WNDPROC is in a class EDITED BY BEN
//	tWCEX.lpszClassName = szWindowClassName;
//	tWCEX.hInstance = hInstance;
//	tWCEX.hCursor = LoadCursor(NULL, IDC_ARROW);
//	tWCEX.hbrBackground = (HBRUSH)(COLOR_WINDOWFRAME);
//	tWCEX.hIcon = NULL;
//	tWCEX.lpszMenuName = NULL;
//	tWCEX.hIconSm = NULL;
//
//	if(RegisterClassEx(&tWCEX) == 0)
//	{
//		return 1;		
//	}
//
//	//Create Window
//	RECT tWindowSize = {0, 0, nWindowWidth, nWindowHeight};
//	AdjustWindowRect(&tWindowSize, WS_OVERLAPPEDWINDOW, false);
//
//	hwnd = CreateWindow(szWindowClassName,
//						szWindowTitle, 
//						WS_OVERLAPPEDWINDOW, 
//						CW_USEDEFAULT, 
//						CW_USEDEFAULT, 
//						tWindowSize.right - tWindowSize.left, 
//						tWindowSize.bottom - tWindowSize.top,
//						NULL,
//						NULL,
//						hInstance,
//						NULL);
	CAudioManager * cManager = CAudioManager::GetInstance();
	cManager->Initialize();
	int nMusic = 0;
	//cManager->LoadMusic("Xion's Final Battle (Piano Solo).mp3", &nMusic);
	cManager->LoadXML("assets\\sounds\\CI_XML_soundresources.xml");
	CAudioSource cAudioSource;
	cAudioSource.LoadSlot("XionMusic", true, 0);
	//cAudioSource.SetPosition(200.0f);
	//cAudioSource.Play();
	CGame gameState(hInstance,(WNDPROC)WndProc); ///// EDITED BY BEN
	input = CInput::GetInstance();

#ifdef _DEBUG
	//CDebugConsole cConsole;
	//cConsole.ShowConsoleWindow();
#endif
	//if(!hwnd)
	//{
	//	return 1;
	//}

	//Show window
	//ShowWindow(hwnd, nCmdShow);
	//UpdateWindow(hwnd);

	//TODO: Initialize application
	MSG msg; ZeroMemory( &msg, sizeof( msg ) );
    while ( msg.message != WM_QUIT )
    {	
		CTimer::GetInstance()->Signal();
		CAudioManager::GetInstance()->Update();
		//bool bIsEnded = false;
		
		/*if(cAudioSource.GetSongStatus(WORKING_SLOT, &bIsEnded))
		{
			if(bIsEnded)
			{
				cAudioSource.RestoreChannel();
				cAudioSource.Play();
			}
		}*/
		cManager->Update();
		gameState.Run();
		if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
        { 
            TranslateMessage( &msg );
            DispatchMessage( &msg ); 
        }
    }
	
	cManager->Shutdown();
#ifdef _DEBUG
	//cConsole.DestroyConsoleWindow();
#endif

#endif
	return 0;
}

LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam ) //Put this in CInput as a static method
{
	if(GetAsyncKeyState(VK_F1))
		message = WM_DESTROY;

//	while(ShowCursor(false) > 0){};		
	input->Update(hWnd, message, lParam);
 
    switch ( message )
    {    
		case ( WM_DESTROY ): { PostQuitMessage( 0 ); }
        break;
    }
    return DefWindowProc( hWnd, message, wParam, lParam );
}