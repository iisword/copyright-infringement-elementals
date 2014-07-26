#pragma once
#include "XTime.h"
/* BGM 7-12-2014
This is the time keeping singleton.  This keeps more than one class calling the Signal func in XTime.

Need to add teh singleton funcs.  Make the constructor  private.  Add the XTime.h and any accesor functionality.
*/

class CTimer
{
	XTime time;

	CTimer(void);
public:

	static CTimer * GetInstance();
	~CTimer(void);
	double Delta(){ return time.Delta();}
	void Signal(){ time.Signal();};
};

