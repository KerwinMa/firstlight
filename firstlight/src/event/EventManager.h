#ifndef FLT_EVENT_MANAGER_H
#define FLT_EVENT_MANAGER_H

#include "common.h"
#include "utils/ISingleton.h"
#include "IEvent.h"

namespace flt
{

class IEventReceiver;

static const int k_max_event_size = 32;
static const int k_event_buffer_size = 1024;

class EventManager: public utils::ISingleton<EventManager>
{
public:
	EventManager();
	~EventManager();

	bool registerReceiver(IEvent::UID_TYPE eventID, IEventReceiver* receiver);
	bool unRegisterReceiver(IEvent::UID_TYPE eventID, IEventReceiver* receiver);
	void unRegisterAllReceivers();

	void raiseEvent(const IEvent& event);

	void clearAllEvents();

	void update();

private:
	typedef list_t<IEventReceiver*> ReceiverList;
	typedef map_t<IEvent::UID_TYPE, ReceiverList> EventQueues;

	EventQueues	m_events;
	u8 m_eventBuffer[k_event_buffer_size];
	int m_bufferCurPos;
	int m_bufferAddPos;
	

	
};

}//end namespace flt

#endif //FLT_EVENT_MANAGER_H