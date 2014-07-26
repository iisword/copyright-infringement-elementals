/***********************************************
* Filename:  		IListener.h
* Date:      		7/16/2014
* Mod. Date: 		7/16/2014
* Mod. Initials:	RT
* Author:    		Roman Taran
* Purpose:   		This is a base class for the Event System, the derived classes must override
					the ProcessEvent() 
************************************************/


#ifndef _ILISTENER_H
#define _ILISTENER_H

class CEvent;

class IListener
{
public:
	IListener(void) {}
	virtual ~IListener(void) {}

	// this function must be written for all derived classes to handle events
	virtual void ProcessEvent(CEvent * Event) = 0;
};
#endif
