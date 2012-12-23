#ifndef FLT_EVT_MOUSE_H
#define FLT_EVT_MOUSE_H

#include "EventsDef.h"
#include "IEvent.h"

namespace flt
{

//! Enumeration for all mouse input events
enum EMOUSE_INPUT_EVENT
{
	//! Left mouse button was pressed down.
	EMIE_LMOUSE_PRESSED_DOWN = 0,

	//! Right mouse button was pressed down.
	EMIE_RMOUSE_PRESSED_DOWN,

	//! Middle mouse button was pressed down.
	EMIE_MMOUSE_PRESSED_DOWN,

	//! Left mouse button was left up.
	EMIE_LMOUSE_LEFT_UP,

	//! Right mouse button was left up.
	EMIE_RMOUSE_LEFT_UP,

	//! Middle mouse button was left up.
	EMIE_MMOUSE_LEFT_UP,

	//! The mouse cursor changed its position.
	EMIE_MOUSE_MOVED,

	//! The mouse wheel was moved. Use Wheel value in event data to find out
	//! in what direction and how fast.
	EMIE_MOUSE_WHEEL,

	//! Left mouse button double click.
	//! This event is generated after the second EMIE_LMOUSE_PRESSED_DOWN event.
	EMIE_LMOUSE_DOUBLE_CLICK,

	//! Right mouse button double click.
	//! This event is generated after the second EMIE_RMOUSE_PRESSED_DOWN event.
	EMIE_RMOUSE_DOUBLE_CLICK,

	//! Middle mouse button double click.
	//! This event is generated after the second EMIE_MMOUSE_PRESSED_DOWN event.
	EMIE_MMOUSE_DOUBLE_CLICK,

	//! Left mouse button triple click.
	//! This event is generated after the third EMIE_LMOUSE_PRESSED_DOWN event.
	EMIE_LMOUSE_TRIPLE_CLICK,

	//! Right mouse button triple click.
	//! This event is generated after the third EMIE_RMOUSE_PRESSED_DOWN event.
	EMIE_RMOUSE_TRIPLE_CLICK,

	//! Middle mouse button triple click.
	//! This event is generated after the third EMIE_MMOUSE_PRESSED_DOWN event.
	EMIE_MMOUSE_TRIPLE_CLICK,

	//! No real event. Just for convenience to get number of events
	EMIE_COUNT
};

//! Masks for mouse button states
enum E_MOUSE_BUTTON_STATE_MASK
{
	EMBSM_LEFT    = 0x01,
	EMBSM_RIGHT   = 0x02,
	EMBSM_MIDDLE  = 0x04,

	//! currently only on windows
	EMBSM_EXTRA1  = 0x08,

	//! currently only on windows
	EMBSM_EXTRA2  = 0x10,

	EMBSM_FORCE_32_BIT = 0x7fffffff
};

class EvtMouseMove: public IEvent
{
public:
	u16 X, Y;
	bool Shift:1;
	bool Ctrl:1;

public:
	EvtMouseMove(u16 x, u16 y, bool shift=false, bool ctrl=false)
		:IEvent(events_id::EVT_MOUSE_MOVE),
		 X(x),Y(y),Shift(shift),Ctrl(ctrl)
	{
	}

	virtual ~EvtMouseMove(){}

	virtual int getSize() const
	{
		return sizeof(EvtMouseMove);
	}
};

class EvtMouseButton: public IEvent
{
public:
	enum MOUSE_BUTTON
	{
		MOUSE_BTN_LEFT = 0,
		MOUSE_BTN_RIGHT,
		MOUSE_BTN_MIDDLE,	
	};

	u8 Button;
	u16 X,Y;
	bool Pressed;	

public:
	EvtMouseButton(u8 button, bool pressed, u16 x, u16 y)
		:IEvent(events_id::EVT_MOUSE_BUTTON),
		 Button(button),
		 X(x),Y(y),
		 Pressed(pressed)
	{
	}

	virtual ~EvtMouseButton(){}

	virtual int getSize() const
	{
		return sizeof(EvtMouseButton);
	}
};

class EvtMouseWheel: public IEvent
{
public:
	f32 Delta;

public:
	EvtMouseWheel(f32 delta)
		:IEvent(events_id::EVT_MOUSE_WHEEL),
		 Delta(delta)
	{
	}

	virtual ~EvtMouseWheel(){}

	virtual int getSize() const
	{
		return sizeof(EvtMouseWheel);
	}
};


}//end namespace flt

#endif //FLT_EVT_MOUSE_H