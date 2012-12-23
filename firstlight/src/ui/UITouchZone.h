#ifndef FLT_UI_TOUCH_ZONE_H
#define FLT_UI_TOUCH_ZONE_H

#include "ui/UIObject.h"
#include "renderer/Color.h"

namespace flt
{

static const int k_touchZone_cached_points_num = 10; 

//how many pixels at least to touch and move to emit a slide event
static const int k_touchZone_slide_min_dis_quard = 5*5;

//Max pixel per deltaTimeMS to clamp to max speed (1.0)
static const f32 k_touchZone_max_pixel_speed = 1.5f;

//For single direction slide (x or y), it is the min dis in another direction.
static const int k_touchZone_single_dir_min_dis = 20;

namespace ui
{		
	/**
		Raise slide or hold UI event
	*/

	class UITouchZone: public UIObject
	{
	public:
		UITouchZone();
		virtual ~UITouchZone();
		
		virtual bool loadFromXML(TiXmlElement *xmlUIObjNode);
		virtual void saveToXML(TiXmlElement *xmlUIObjNode);		
		virtual void init();
		virtual void update(f32 dt);				

		//! return true to absorb the event
		virtual bool processEvent(const IEvent& event);

		bool isHolding() const { return m_holdTime>50.0f; }

		f32 getHoldTime() const { return m_holdTime; }

	protected:

		bool m_bFullScreenTouch;

		//! Which type of slide event will produce, can be "EvtUISlideX", "EvtUISlideY" or "EvtUISlide"
		stringc m_eventType; 
		
		core::position2df m_startPoint;

		core::position2df m_touchedPoints[k_touchZone_cached_points_num];

		f64 m_touchedTimes[k_touchZone_cached_points_num];
		
		s32 m_touchedArrayIndex;

		u32 m_touchedPointsNum;
		
		core::position2df m_lastDir;

		f32 m_holdTime;

		bool m_isPressed;
	};

}//ui
}//flt

#endif //FLT_UI_TOUCH_ZONE_H
