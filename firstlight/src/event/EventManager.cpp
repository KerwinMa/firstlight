#include "EventManager.h"
#include "IEventReceiver.h"

namespace flt
{

EventManager::EventManager()	
{
	clearAllEvents();
}

EventManager::~EventManager()
{
}

bool EventManager::registerReceiver(IEvent::UID_TYPE eventID, IEventReceiver* receiver)
{
	EventQueues::iterator map_iter = m_events.find(eventID);

	if(map_iter == m_events.end())
	{
		m_events[eventID].push_back(receiver);
	}
	else
	{
		for(ReceiverList::iterator list_iter = map_iter->second.begin(); list_iter!=map_iter->second.end(); ++list_iter)
		{
			if((*list_iter) == receiver)
			{
				return false;
			}

			map_iter->second.push_back(receiver);
		}
	}

	return true;
}

bool EventManager::unRegisterReceiver(IEvent::UID_TYPE eventID, IEventReceiver* receiver)
{
	EventQueues::iterator map_iter = m_events.find(eventID);

	if(map_iter == m_events.end())
	{
		return false;
	}

	for(ReceiverList::iterator list_iter = map_iter->second.begin(); list_iter!=map_iter->second.end(); ++list_iter)
	{
		if((*list_iter) == receiver)
		{
			map_iter->second.erase(list_iter);
			if(map_iter->second.empty())
			{
				m_events.erase(eventID);
			}

			return true;
		}
	}

	return false;
}

void EventManager::unRegisterAllReceivers()
{
	m_events.clear();	
}

void EventManager::raiseEvent(const IEvent& event)
{
	int size = ((IEvent*)(&event))->getSize();

	FLT_ASSERT(size<k_max_event_size);

	memcpy( (m_eventBuffer+m_bufferAddPos), &event, size);
	m_bufferAddPos += k_max_event_size;

	if(m_bufferAddPos >= k_event_buffer_size)
	{
		m_bufferAddPos = 0;
	}
}

void EventManager::clearAllEvents()
{
	memset(m_eventBuffer, 0, k_event_buffer_size);

	m_bufferCurPos = 0;
	m_bufferAddPos = 0;	
}

void EventManager::update()
{
	while(m_bufferCurPos != m_bufferAddPos)
	{
		const IEvent* evt = (IEvent*)(m_eventBuffer+m_bufferCurPos);
		FLT_ASSERT(evt!=0);

		EventQueues::iterator map_iter = m_events.find(evt->getUID());
		if(map_iter!=m_events.end())
		{
			const ReceiverList& receivers = map_iter->second;
			for(ReceiverList::const_iterator list_iter=receivers.begin(); list_iter!=receivers.end(); ++list_iter)
			{
				FLT_ASSERT((*list_iter)!=0);
				if((*list_iter)->onEvent(*evt))
					break;
			}
		}

		m_bufferCurPos += k_max_event_size;
		if(m_bufferCurPos >= k_event_buffer_size)
		{
			m_bufferCurPos = 0;
		}
	}
}


}//end namespace flt