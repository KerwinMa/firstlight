#ifndef FLT_IEVENT_RECEIVER_H
#define FLT_IEVENT_RECEIVER_H

#include "common.h"

namespace flt
{

class IEvent;

class IEventReceiver
{
public:
	virtual ~IEventReceiver(){}

	//!if return true, the event is absorbed by the receiver
	virtual bool onEvent(const IEvent& event) = 0;
};

}//end namespace flt

#endif //FLT_IEVENT_RECEIVER_H