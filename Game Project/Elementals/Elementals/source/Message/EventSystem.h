/***********************************************
* Filename:  		CEVENTSystem.h
* Date:      		7/16/2014
* Mod. Date: 		7/16/2014
* Mod. Initials:	RT
* Author:    		Roman Taran
* Purpose:   		This is a Message/Event system hub, which will control all intermodule communication, Classes derived from
					IListener interface will register here to receive event notifications.
					This Will Be a SINGLETON
************************************************/

#ifndef _CEVENT_SYSTEM_H_
#define _CEVENT_SYSTEM_H_

#include "Event.h"
#include "Listener.h"

#include <map>
#include <vector>
#include <queue>
using std::priority_queue;
using std::multimap;
using std::pair;
using std::vector;


// This class is for PriorityQueue to prioritize Events in the queue
class CompareEvents {
public:
	bool operator()(CEvent& E1, CEvent& E2) // Returns true if E1 priority is greater than E2 priority
	{
		if (E1.getPriority() > E2.getPriority()) 
			return true;
		
		return false;
	}
};


class CEventSystem
{
private:
	//Storage for all active listeners
	multimap<EVENTID, IListener*> m_mapListeners;

	//Queue of all events waiting to be processed, prioritized
	priority_queue<CEvent, vector<CEvent>,CompareEvents> m_EventQueue;
	
	vector<pair<EVENTID,IListener*>> m_vInactive; // all unregisterng Listeners are added here

	std::queue<pair<EVENTID, IListener*>> m_qWaitingToJoin; //all registering Listeners get added here

	//Must be private for singleton functionality
	CEventSystem(void);
	~CEventSystem(void);
	CEventSystem(const CEventSystem&);
	CEventSystem& operator = (const CEventSystem&); 

	void DispatchEvent(CEvent* pEvent);
	bool IsRegistered(EVENTID ID, IListener* pListener);

	//This functions is only called internaly to prevent heap corruptions
	void RegisterListeners(void);
	void UnregisterListeners(void);

public:
	
	static CEventSystem* GetInstance()
	{
		static CEventSystem instance;
		return &instance;
	}


	void RegisterForEvent(EVENTID ID, IListener* pListener);
	void UnregisterFromEvent(EVENTID ID, IListener* pListener);
	void UnregisterFromALL(IListener* pListener);
	void ProcessEvents();
	void Terminate();
	//Called by the senders through singleton 
	void SendEvent(EVENTID nID, EVENTPRIORITY ePriority, void* pData = nullptr, void* pRecipient= nullptr, void* pSender =nullptr );
};

#endif