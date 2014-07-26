/***********************************************
* Filename:  		Utilities.cpp
* Date:      		7/14/2014
* Mod. Date: 		7/14/2014
* Mod. Initials:	MB
* Author:    		Michael Boodle
* Purpose:   		A place for functions who functionality can be used in
*					multiple places.
************************************************/


#include "Utilities.h"

namespace Utilities
{
	bool PathIsValid(std::wstring & szPath, bool bIsDir)
	{
		DWORD fileAttributes = GetFileAttributes(szPath.c_str());
		if(fileAttributes != INVALID_FILE_ATTRIBUTES)
		{
			//It exists
			if(bIsDir && !(fileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				//It's not a directory
				return false;
			}

			return true;
		}
		return false;
	}

	TCHAR * GetCommonFolder(const KNOWNFOLDERID &rfid)
	{
		TCHAR * szReturnPath = nullptr;
		HRESULT hr = SHGetKnownFolderPath(rfid, 0, 0, &szReturnPath);
		if(SUCCEEDED(hr))
		{
			return szReturnPath;
		}
		return nullptr;
	}
}