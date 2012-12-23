#ifndef FLT_EVENTS_DEF_H
#define FLT_EVENTS_DEF_H

#include "common.h"

namespace flt
{

namespace events_id
{
	enum
	{
		EVT_KEYBOARD = 0,
		EVT_MOUSE_MOVE,
		EVT_MOUSE_BUTTON,
		EVT_MOUSE_WHEEL,
		EVT_TOUCH,
		EVT_UI_BUTTON_TAPPED,
		EVT_UI_BUTTON_SWITCHED,
		EVT_UI_SLIDE,
		EVT_UI_SLIDE_X,
		EVT_UI_SLIDE_Y,
		EVT_UI_HOLD,
		EVT_GAME,

		MAX_BUILTIN_EVT_ID, //user defined IDs should start from MAX_BUILTIN_EVT_ID
	};

}//end namespace events_id


}//end namespace flt

#endif //FLT_EVENTS_DEF_H