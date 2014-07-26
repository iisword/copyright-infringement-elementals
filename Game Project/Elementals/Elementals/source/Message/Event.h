/***********************************************
* Filename:  		Event.h
* Date:      		7/16/2014
* Mod. Date: 		7/16/2014
* Mod. Initials:	RT
* Author:    		Roman Taran
* Purpose:   		This is an Event class, it will be passed to registered listeners(Derived from IListener);
					Each Event should have a EVENTID, and Priority FLAG. DATA can be passed with an optional void *
************************************************/

#ifndef _CEVENT_H_
#define _CEVENT_H_


enum PRIORITY {LOW,MEDIUM,HIGH};
enum EVENTTYPE {INPUT_EVENT,CHANGE_STATE,SFX,VFX,MUSIC,NETWORK, UN_PAUSE, UNREGISTER_EVENT_INTERNAL_DO_NOT_USE};
typedef EVENTTYPE EVENTID;
typedef PRIORITY EVENTPRIORITY;

class CEvent 
{
private:
	EVENTID m_eID;   
	EVENTPRIORITY m_ePriority; 

	void* m_pDATA; 
	void* m_pSender;
	void* m_pRecipient;

public:
	CEvent(EVENTID nID, EVENTPRIORITY Priority,void* DATA = nullptr,void* Sender =nullptr, void* Recepient = nullptr);
	~CEvent();


	//Accessors
	inline EVENTID getID() { return m_eID; }
	inline EVENTPRIORITY getPriority () {return m_ePriority;}
	inline void* GetData() {return m_pDATA;}
	inline void* GetSender() {return m_pSender;}
	inline void* GetRecepient() {return m_pRecipient;}
};







#endif


   
//
//    enum eventtype {
//        EVENT_TIMER = 10,
//        EVENT_KEYPRESS = 20,
//        EVENT_KEYRELEASE = 30,
//        EVENT_MOUSECLICK = 40,
//        EVENT_MOUSEMOTION = 50,
//        EVENT_MOUSEWHEEL = 60,
//        EVENT_MOUSEMOVE = 70,
//        EVENT_ENTITYUPDATE = 80,
//        EVENT_ENTITYRENDER = 90,
//    };
//
//    class TimerEvent : public IEvent {
//    public:
//        int timerid;
//        TimerEvent(int tid);
//    };
//
//    class KeyPressEvent : public IEvent {
//    public:
//        int keycode;
//        KeyPressEvent( int key );
//    };
//
//    class KeyReleaseEvent : public IEvent {
//    public:
//        int keycode;
//        KeyReleaseEvent( int key );
//    };
//
//    class MouseClickEvent : public IEvent {
//    public:
//        int button;
//        MouseClickEvent( int btn );
//    };
//
//    class MouseMotionEvent : public IEvent {
//    public:
//        int deltax,deltay;
//        MouseMotionEvent( int dx, int dy );
//    };
//
//    class MouseWheelEvent : public IEvent {
//    public:
//        int wheel;
//        MouseWheelEvent( int wheel );
//    };
//
//    class MouseMoveEvent : public IEvent {
//    public:
//        int posx, posy;
//        MouseMoveEvent( int px, int py );
//    };
//
//    class EntityRenderEvent : public IEvent {
//    public:
//        Entity* entity;
//        EntityRenderEvent(Entity* e);
//    };
//
//    class EntityUpdateEvent : public IEvent {
//    public:
//        Entity* entity;
//        EntityUpdateEvent(Entity* e);
//    };
//
