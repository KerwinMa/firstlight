#ifndef FLT_EVT_KEYBOARD_H
#define FLT_EVT_KEYBOARD_H

#include "EventsDef.h"
#include "IEvent.h"
#include "KeyCodes.h"

namespace flt
{

class EvtKeyboard: public IEvent
{
public:
	//! Character corresponding to the key (0, if not a character)
	wchar_t Char;

	//! Key which has been pressed or released
	u32 KeyCode;

	//! If not true, then the key was left up
	bool Pressed;

	//! True if shift was also pressed
	bool Shift;

	//! True if ctrl was also pressed
	bool Control;

	//! True if alt was also pressed
	bool Alt;

public:	
	EvtKeyboard(wchar_t c=0, u32 keyCode=0, bool pressed=false, bool shift=false, bool ctrl=false, bool alt=false)
		:IEvent(events_id::EVT_KEYBOARD),
		 Char(c),
		 KeyCode(keyCode),
		 Pressed(pressed),
		 Shift(shift),
		 Control(ctrl),
		 Alt(alt)
	{
	}

	virtual ~EvtKeyboard(){}

	virtual int getSize() const
	{
		return sizeof(EvtKeyboard);
	}
};



}//end namespace flt

#endif //FLT_EVT_KEYBOARD_H