#include "EventSystem.h"


CEventSystem::CEventSystem(void)
{
}
CEventSystem::~CEventSystem(void)
{
}
/**********************************
*  FunctionName:       DispatchEvent
*
*  In:                 CEvent *
*
*  Out:                N/A
*
*  Return:             void
*
*  Mod. Date:          7/16/2014
*  Mod. Initials:      RT

*  This will dispatch an event notification to all registered IListeners
**********************************/
void CEventSystem::DispatchEvent( CEvent* pEvent )
{
	pair<multimap<EVENTID, IListener*>::iterator,
		multimap<EVENTID, IListener*>::iterator> range;

	range = m_mapListeners.equal_range(pEvent->getID());
	for(multimap<EVENTID, IListener*>::iterator Iter = range.first;
			Iter != range.second; Iter++)
	{

		if((*Iter).second != nullptr)    //this checks if the listener has been removed
		{
			(*Iter).second->ProcessEvent(pEvent);
		}
	}
}
/**********************************
*  FunctionName:       IsRegistered
*
*  In:                 EVENTID , IListener*
*
*  Out:                N/A
*
*  Return:             bool
*
*  Mod. Date:          7/16/2014
*  Mod. Initials:      RT
*  This will check for duplicate registration of IListeners
**********************************/
bool CEventSystem::IsRegistered( EVENTID ID, IListener* pListener )
{
	
	pair<multimap<EVENTID,IListener*>::iterator,
		multimap<EVENTID,IListener*>::iterator> range;
	
	range = m_mapListeners.equal_range(ID);
	multimap<EVENTID,IListener*>::iterator Iter = range.first;

	//check if the pListener is already registered to receive this event
	for(;Iter != range.second;Iter++)
	{
		if((*Iter).second == pListener)
			return true;
	}


	return false;
}
/**********************************
*  FunctionName:       RegisterForEvent
*
*  In:                 EVENTID -- which event to register for
					   IListener* -- Who is registering
*
*  Out:                N/A
*
*  Return:             void
*
*  Mod. Date:          7/16/2014
*  Mod. Initials:      RT
*  This function must be called by Objects that wish to receive Event notifications
**********************************/
void CEventSystem::RegisterForEvent( EVENTID ID, IListener* pListener )
{
	
	m_qWaitingToJoin.push(std::make_pair(ID,pListener));
	
}
/**********************************
*  FunctionName:       UnregisterFromEvent
*
*  In:                 EVENTID - event to unregister from
					   IListener* - Who is unregistering from the event
*
*  Out:                N/A
*
*  Return:             void
*
*  Mod. Date:          7/17/2014
*  Mod. Initials:      RT
*  This function will unregister the listener from a specific event
**********************************/
void CEventSystem::UnregisterFromEvent( EVENTID ID, IListener* pListener )
{
	m_vInactive.push_back(std::make_pair(ID,pListener));
}
/**********************************
*  FunctionName:       UnregisterFromAll
*
*  In:                 IListener* -- which client 
*
*  Out:                Add parameters here
*
*  Return:             void
*
*  Mod. Date:          7/16/2014
*  Mod. Initials:      RT
*  This will Unregister the listener from all Events
**********************************/
void CEventSystem::UnregisterFromALL( IListener* pListener )
{
	m_vInactive.push_back(std::make_pair(UNREGISTER_EVENT_INTERNAL_DO_NOT_USE,pListener));

}
/**********************************
*  FunctionName:       SendEvent
*
*  In:                 EVENTID  - the id of the event
*					   EVENTPRIORITY - priority of the event
					   void* - pDATA == optional pointer to pass data
					   void* - pReciepient == optional pointer to designate a particular recipient
					   void* - pSender == optional pointer to who sent the event
*
*  Out:                Add parameters here
*
*  Return:             void
*
*  Mod. Date:          7/16/2014
*  Mod. Initials:      RT
*  This function is called through singleton functionality by the sender, adds the event to the queue
**********************************/
void CEventSystem::SendEvent( EVENTID eID, EVENTPRIORITY ePriority, void* pData /*= nullptr*/, void* pRecipient/*= nullptr*/, void* pSender /*=nullptr */ )
{
	CEvent event(eID,ePriority,pData,pRecipient,pSender);

	m_EventQueue.push(event);
}
/**********************************
*  FunctionName:       ProcessEvents()
*
*  In:                 N/A
*
*  Out:                N/A
*
*  Return:             void
*
*  Mod. Date:          7/17/2014
*  Mod. Initials:      RT
*  This will process all events waiting in the queue
**********************************/
void CEventSystem::ProcessEvents()
{
	while(!m_EventQueue.empty())
	{
		RegisterListeners(); //this will add all players waiting to register
		UnregisterListeners(); // this will remove all players waiting to unregister
		DispatchEvent(&m_EventQueue.top());
		m_EventQueue.pop();
	}
}
/**********************************
*  FunctionName:       Terminate
*
*  In:                 N/A
*
*  Out:                N/A
*
*  Return:             void
*
*  Mod. Date:          7/17/2014
*  Mod. Initials:      RT
*	This will clear all events waiting and clear out the map of listeners
**********************************/
void CEventSystem::Terminate()
{
	//m_EventQueue.
	while(!m_EventQueue.empty())
	{
		m_EventQueue.pop();
	}
	m_mapListeners.clear();
	m_vInactive.clear();

}
/**********************************
*  FunctionName:       UnregisterListeners()
*
*  In:                 void
*
*  Out:                void
*
*  Return:             void
*
*  Mod. Date:          7/19/2014
*  Mod. Initials:      RT
*  This function will unregister all clients, this prevents from outside users 
*  messing with internal map memory
**********************************/
void CEventSystem::UnregisterListeners( void )
{
	//this is a redundancy loop to clean any bad pointers
	multimap<EVENTID,IListener*> :: iterator mapIter = m_mapListeners.begin();
	while(mapIter != m_mapListeners.end())
	{
		if((*mapIter).second == nullptr)
		{
			mapIter = m_mapListeners.erase(mapIter);
		}
		else
		{
			mapIter++;
		}
	}


	for(size_t i = 0; i<m_vInactive.size(); ++i)
	{
		//this chunk of code will unregister a listener from all events/ or if the listener already been deleted
		// wil clean up the map;

		if(m_vInactive[i].first == UNREGISTER_EVENT_INTERNAL_DO_NOT_USE)
		{
			multimap<EVENTID,IListener*> :: iterator Iter = m_mapListeners.begin();
			 
			 	while(Iter != m_mapListeners.end() )
			 	{
			 		if((*Iter).second == m_vInactive[i].second )
			 		{
			 			Iter = m_mapListeners.erase(Iter);
			 		}
			 		else
			 		{
			 			Iter++;
			 		}
			 	} //end While()
		} // end if(UNREGISTER_EVENT_INTERNAL_DO_NOT_USE )

		else
		{
			pair<multimap<EVENTID,IListener*>::iterator,
						multimap<EVENTID,IListener*>::iterator> range;
				
					range = m_mapListeners.equal_range(m_vInactive[i].first);
				
					multimap<EVENTID,IListener*>::iterator Iter = range.first;
				
					for(;Iter != range.second;Iter++)
					{
						if((*Iter).second ==m_vInactive[i].second )
						{
							Iter = m_mapListeners.erase(Iter);
							break;
						}
					}
		}
	}//end for()

	//after all inactive Listeners have been cleared out of the map, clear the vector;
	m_vInactive.clear();
}

/**********************************
*  FunctionName:       RegisterListeners()
*
*  In:                 void
*
*  Out:                void
*
*  Return:             void
*
*  Mod. Date:          7/19/2014
*  Mod. Initials:      RT
*  This function will register all clients, this function prevents end user from messing with 
*  internal map memory
**********************************/
void CEventSystem::RegisterListeners( void )
{
	while(!m_qWaitingToJoin.empty())
	{
		//check if pListener is valid or already registered
		if(m_qWaitingToJoin.front().second!=nullptr ||
			IsRegistered(m_qWaitingToJoin.front().first,m_qWaitingToJoin.front().second)==false)
		{
			m_mapListeners.insert( m_qWaitingToJoin.front());
		}
		
		m_qWaitingToJoin.pop();
	}
}



