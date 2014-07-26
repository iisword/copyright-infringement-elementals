#include "Event.h"

//IEvent::IEvent()
//{
//    id = 0;
//}
//
//EntityRenderEvent::EntityRenderEvent(Entity* e)
//{
//    id = EVENT_ENTITYRENDER;
//    entity = e;
//}
//
//EntityUpdateEvent::EntityUpdateEvent(Entity* e)
//{
//    id = EVENT_ENTITYUPDATE;
//    entity = e;
//}
//
//TimerEvent::TimerEvent(int tid)
//{
//    id = EVENT_TIMER;
//    timerid = tid;
//}
//
//
//KeyPressEvent::KeyPressEvent(int key)
//{
//    id = EVENT_KEYPRESS;
//    keycode = key;
//}
//
//KeyReleaseEvent::KeyReleaseEvent(int key)
//{
//    id = EVENT_KEYRELEASE;
//    keycode = key;
//}
//
//MouseClickEvent::MouseClickEvent(int btn)
//{
//    id = EVENT_MOUSECLICK;
//    button = btn;
//}
//
//MouseMotionEvent::MouseMotionEvent( int dx, int dy )
//{
//    id = EVENT_MOUSEMOTION;
//    deltax = dx;
//    deltay = dy;
//}
//
//MouseWheelEvent::MouseWheelEvent( int whl )
//{
//    id = EVENT_MOUSEWHEEL;
//    wheel = whl;
//}
//
//MouseMoveEvent::MouseMoveEvent( int px, int py )
//{
//    id = EVENT_MOUSEMOVE;
//    posx = px;
//    posy = py;
//}



CEvent::CEvent( EVENTID nID, EVENTPRIORITY Priority,void* DATA /*= nullptr*/,void* Sender /*=nullptr*/, void* Recepient /*= nullptr*/ )
{
	m_eID = nID;
	m_ePriority = Priority;
	m_pDATA = DATA;
	m_pRecipient = Recepient;
	m_pSender = Sender;
}

CEvent::~CEvent()
{

}
