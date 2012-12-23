#include "UITouchZone.h"
#include "event/EvtTouch.h"
#include "event/EvtUI.h"
#include "event/EventManager.h"
#include "utils/Logger.h"
#include "app/App.h"
#include "device/IDevice.h"

namespace flt
{
namespace ui
{
	UITouchZone::UITouchZone()		
		:m_bFullScreenTouch(true),m_eventType("EvtUISlide"),m_touchedPointsNum(0),m_holdTime(0.0f),m_isPressed(false)
	{
	}	

	UITouchZone::~UITouchZone()
	{		
	}

	void UITouchZone::saveToXML(TiXmlElement *xmlUIObjNode)
	{
		UIObject::saveToXML(xmlUIObjNode);

		xmlUIObjNode->SetAttribute("type","UITouchZone");

		TiXmlElement * thisNode;
		thisNode = new TiXmlElement( "touchZone" );  
		xmlUIObjNode->LinkEndChild( thisNode );  		
		
		thisNode->SetAttribute("eventType", m_eventType.c_str());
		thisNode->SetAttribute("fullScreen", m_bFullScreenTouch);
	}

	bool UITouchZone::loadFromXML(TiXmlElement *xmlUIObjNode)
	{
		if(UIObject::loadFromXML(xmlUIObjNode))
		{
			TiXmlHandle hXmlNode(xmlUIObjNode);

			TiXmlElement * thisNode = hXmlNode.FirstChild("touchZone").Element();
			if(thisNode==0)
			{
				DBG("engineDbg_UI","Fail to load UITouchZone, missing touchZone node!");
				return false;			
			}
			
			thisNode->QueryStringAttribute("eventType", &m_eventType);
			int tmp;
			thisNode->QueryIntAttribute("fullScreen",&tmp);
			m_bFullScreenTouch = (tmp==1);

			return true;
		}
		
		return false;
	}

	void UITouchZone::init()
	{		
		updateFinalRect();		
	}
	
	void UITouchZone::update(f32 dt)
	{
		UIObject::update(dt);	

		if(m_isPressed)
		{
			m_holdTime += dt;
		}
	}
	
	bool UITouchZone::processEvent(const IEvent& event)
	{
		if(event.getUID()==events_id::EVT_TOUCH)
		{
			EvtTouch* evtTouch = (EvtTouch*)&event;
			core::position2df currentPoint(evtTouch->X,evtTouch->Y);
			
			if(m_bFullScreenTouch || m_finalRect.isPointInside(currentPoint))
			{	
				//DBG("engineDbg_UI", "event touch phase=%d",evtTouch->Phase);					

				if(evtTouch->Phase==Touch_Began)
				{
					m_startPoint = currentPoint;

					m_touchedArrayIndex = 0;
					m_touchedPoints[m_touchedArrayIndex] = currentPoint;
					m_touchedTimes[m_touchedArrayIndex] = App::TheApp->getDevice()->getSystemTime();
					m_touchedArrayIndex++;	

					m_touchedPointsNum = 1;

					m_holdTime = 0.0f;
					m_isPressed = true;
					
				}	
				else if(evtTouch->Phase==Touch_Moved)
				{					
					s32 lastIndex = m_touchedArrayIndex-1;
					if(lastIndex<0)
						lastIndex = k_touchZone_cached_points_num-1;

					m_lastDir = currentPoint-m_touchedPoints[lastIndex];	

					if(fabs(m_lastDir.X)>2 || fabs(m_lastDir.Y)>2)
						m_holdTime = 0.0f;
					
					m_touchedPoints[m_touchedArrayIndex] = currentPoint;
					m_touchedTimes[m_touchedArrayIndex] = App::TheApp->getDevice()->getSystemTime();
					m_touchedArrayIndex++;	
					if(m_touchedArrayIndex>=k_touchZone_cached_points_num)
						m_touchedArrayIndex = 0;

					m_touchedPointsNum++;
					if(m_touchedPointsNum>k_touchZone_cached_points_num)
						m_touchedPointsNum = k_touchZone_cached_points_num;
					
				}
				else if(evtTouch->Phase==Touch_Ended)
				{	
					if(m_touchedPointsNum<2)
						return false;

					f32 wholeDeltaX = currentPoint.X - m_startPoint.X;
					f32 wholeDeltaY = currentPoint.Y - m_startPoint.Y;
					f32 wholeDeltaDisQ = wholeDeltaX*wholeDeltaX+wholeDeltaY*wholeDeltaY;

					if(wholeDeltaDisQ>k_touchZone_slide_min_dis_quard)
					{
						int deltaNum = m_touchedPointsNum-1;
						f32 totalDis = 0;
						f32 totalDeltaTime = 0;

						int last_array_index = m_touchedArrayIndex-1;
						if(last_array_index<0)
							last_array_index = k_touchZone_cached_points_num-1;
						
						for(int i=last_array_index, n=0; n<deltaNum; ++n)
						{
							int pre_i = i-1;
							if(pre_i<0)
								pre_i = k_touchZone_cached_points_num-1;

							totalDis += (m_touchedPoints[i]-m_touchedPoints[pre_i]).getLength();
							totalDeltaTime += m_touchedTimes[i]-m_touchedTimes[pre_i];

							i = pre_i;
						}

						//DBG("engineDbg_UI", "m_touchedPointsNum=%d, m_touchedArrayIndex=%d, totalDis=%f, totalDeltaTime=%f",m_touchedPointsNum,m_touchedArrayIndex,totalDis,totalDeltaTime);
																	
						f32 deltaDis = totalDis/deltaNum;
						f32 deltaTimeMS = totalDeltaTime/deltaNum;
						if(deltaTimeMS<=0)//There must be something wrong!
							return false;

						core::vector2df direction = m_lastDir;						
												

						f32 pixelSpeed = deltaDis/deltaTimeMS;

						f32 speed = 0.0f;

						if(pixelSpeed>=k_touchZone_max_pixel_speed)
						{
							speed = 1.0f;
						}
						else
						{
							speed = pixelSpeed/k_touchZone_max_pixel_speed;
						}
					
						if(m_eventType=="EvtUISlide")
						{
							direction.normalize();

							EvtUISlide evt(m_ownerLayerID, m_objID, direction, speed);
											
							EventManager::getInstance().raiseEvent(evt);
							DBG("engineDbg_UI", "raise slide event:dir=(%f,%f),speed=%f",direction.X,direction.Y,speed);					
						}

						m_holdTime = 0.0f;
						m_isPressed = false;
					}										
				}

				return true;//absorb this touch event, so UIObjects can't overlap, else only one can receive events
			}			
		}
		return false;
	}

}//ui
}//flt
