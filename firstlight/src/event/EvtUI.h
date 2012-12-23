#ifndef FLT_EVT_UI_H
#define FLT_EVT_UI_H

#include "EventsDef.h"
#include "IEvent.h"

namespace flt
{

// button tap event, for normal button
class EvtUIButtonTapped: public IEvent
{
public:	
	u16 LayerID;
	u16 ButtonID;

public:
	EvtUIButtonTapped(u16 layerID, u16 buttonID)
		:IEvent(events_id::EVT_UI_BUTTON_TAPPED),
		 LayerID(layerID),ButtonID(buttonID)
	{
	}

	virtual ~EvtUIButtonTapped(){}

	virtual int getSize() const
	{
		return sizeof(EvtUIButtonTapped);
	}
};

// button switch event, for switch button
class EvtUIButtonSwitched: public IEvent
{
public:	
	u16 LayerID;
	u16 ButtonID;
	bool ButtonIsOn;

public:
	EvtUIButtonSwitched(u16 layerID, u16 buttonID, bool isOn)
		:IEvent(events_id::EVT_UI_BUTTON_SWITCHED),
		 LayerID(layerID),ButtonID(buttonID),ButtonIsOn(isOn)
	{
	}

	virtual ~EvtUIButtonSwitched(){}

	virtual int getSize() const
	{
		return sizeof(EvtUIButtonSwitched);
	}
};

class EvtUISlide: public IEvent
{
public:	
	u16 LayerID;
	u16 ButtonID;	
	core::vector2df Direction;
	//Speed is normailzed to 0~1
	f32 Speed;

public:
	EvtUISlide(u16 layerID, u16 buttonID, const core::vector2df& direction, f32 speed)
		:IEvent(events_id::EVT_UI_SLIDE),
		 LayerID(layerID),ButtonID(buttonID),Direction(direction),Speed(speed)
	{
	}

	virtual ~EvtUISlide(){}

	virtual int getSize() const
	{
		return sizeof(EvtUISlide);
	}
};

class EvtUISlideX: public IEvent
{
public:	
	u16 LayerID;
	u16 ButtonID;
	//Speed is normailzed to 0~1
	f32 Speed;

public:
	EvtUISlideX(u16 layerID, u16 buttonID, f32 speed)
		:IEvent(events_id::EVT_UI_SLIDE_X),
		 LayerID(layerID),ButtonID(buttonID),Speed(speed)
	{
	}

	virtual ~EvtUISlideX(){}

	virtual int getSize() const
	{
		return sizeof(EvtUISlideX);
	}
};

class EvtUISlideY: public IEvent
{
public:	
	u16 LayerID;
	u16 ButtonID;
	//Speed is normailzed to 0~1
	f32 Speed;

public:
	EvtUISlideY(u16 layerID, u16 buttonID, f32 speed)
		:IEvent(events_id::EVT_UI_SLIDE_Y),
		 LayerID(layerID),ButtonID(buttonID),Speed(speed)
	{
	}

	virtual ~EvtUISlideY(){}

	virtual int getSize() const
	{
		return sizeof(EvtUISlideY);
	}
};

class EvtUIHold: public IEvent
{
public:	
	u16 LayerID;
	u16 ButtonID;
	u32 HoldTime;

public:
	EvtUIHold(u16 layerID, u16 buttonID, u32 holdTime)
		:IEvent(events_id::EVT_UI_HOLD),
		 LayerID(layerID),ButtonID(buttonID),HoldTime(holdTime)
	{
	}

	virtual ~EvtUIHold(){}

	virtual int getSize() const
	{
		return sizeof(EvtUIHold);
	}
};

}//end namespace flt

#endif //FLT_EVT_UI_H
