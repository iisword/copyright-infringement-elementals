/***********************************************
* Filename:  		Utilities.h
* Date:      		7/14/2014
* Mod. Date: 		7/14/2014
* Mod. Initials:	MB
* Author:    		Michael Boodle
* Purpose:   		A place for functions who functionality can be used in
*					multiple places.
************************************************/

#ifndef UTILITIES_HEADER
#define UTILITIES_HEADER

#include <ShlObj.h>
#include <string>

namespace Utilities
{
	bool PathIsValid(std::wstring & szPath, bool bIsDir);
	TCHAR * GetCommonFolder(const KNOWNFOLDERID &rfid);
}

#endif