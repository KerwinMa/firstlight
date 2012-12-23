#ifndef FLT_EVT_TOUCH_H
#define FLT_EVT_TOUCH_H

#include "EventsDef.h"
#include "IEvent.h"

namespace flt
{

enum ETouchPhase
{
	Touch_Began,
	Touch_Moved,
	Touch_Ended,
	Touch_Cancelled,
};

class EvtTouch: public IEvent
{
public:
	u64 ID;
	s16 X, Y;
	u8 Phase;

public:
	EvtTouch(u64 id, u8 phase, s16 x, s16 y)
		:IEvent(events_id::EVT_TOUCH),
		 ID(id),Phase(phase),X(x),Y(y)
	{
	}

	virtual ~EvtTouch(){}

	virtual int getSize() const
	{
		return sizeof(EvtTouch);
	}
};

}//end namespace flt

#endif //FLT_EVT_TOUCH_H