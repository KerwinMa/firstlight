#ifndef FLT_EVT_GAME_H
#define FLT_EVT_GAME_H

#include "EventsDef.h"
#include "IEvent.h"

namespace flt
{

//!用来表示通用的游戏事件

class EvtGame: public IEvent
{
public:

	//! 游戏事件的类型，由游戏定义
	s32 EventType;

	//! 32位参数1
	s32 Param1;

	//! 32位参数2
	s32 Param2;
	
public:
	EvtGame(s32 type, s32 param1, s32 param2)
		:IEvent(events_id::EVT_GAME),
		 EventType(type),Param1(param1),Param2(param2)
	{
	}

	virtual ~EvtGame(){}

	virtual int getSize() const
	{
		return sizeof(EvtGame);
	}
};



}//end namespace flt

#endif //FLT_EVT_GAME_H