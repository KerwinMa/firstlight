#include "UISwitchButton.h"
#include "renderer/Color.h"
#include "renderer/IRenderer.h"
#include "xlib2d/XSprite.h"
#include "xlib2d/XSpriteMgr.h"
#include "xlib2d/XAnimObject.h"
#include "xlib2d/XFontMgr.h"
#include "utils/Logger.h"
#include "event/EvtTouch.h"
#include "event/EvtUI.h"
#include "event/EventManager.h"

namespace flt
{
namespace ui
{
	UISwitchButton::UISwitchButton()
		:m_offAnimID(-1),m_offPressedAnimID(-1),
		 m_isOn(true)		 
	{			
	}	

	UISwitchButton::~UISwitchButton()
	{				
	}

	bool UISwitchButton::loadFromXML(TiXmlElement *xmlUIObjNode)
	{
		if(UIButton::loadFromXML(xmlUIObjNode))
		{
			TiXmlHandle hXmlNode(xmlUIObjNode);

			TiXmlElement * switchNode = hXmlNode.FirstChild("switch").Element();
			if(switchNode!=0)
			{				
				switchNode->QueryIntAttribute("offAnimID",&m_offAnimID);							
				switchNode->QueryIntAttribute("offPressedAnimID",&m_offPressedAnimID);
				switchNode->QueryStringAttribute("offText",&m_offText);
			}
			else
			{
				DBG("engineDbg_UI","Fail to load switch button from xml, miss switch node.");					
			}

			//FLT_ASSERT(m_animID>=0 && m_offAnimID>=0);
			FLT_ASSERT(m_animID>=0);

			if(m_offAnimID<0)
				m_offAnimID = m_animID;

			return true;
		}
		
		return false;		
	}

	void UISwitchButton::saveToXML(TiXmlElement *xmlUIObjNode)
	{
		UIButton::saveToXML(xmlUIObjNode);

		xmlUIObjNode->SetAttribute("type","UISwitchButton");

		TiXmlElement * switchNode;
		switchNode = new TiXmlElement( "switch" );  
		xmlUIObjNode->LinkEndChild( switchNode );  		
		switchNode->SetAttribute("offAnimID", m_offAnimID);		
		switchNode->SetAttribute("offPressedAnimID", m_offPressedAnimID);
		switchNode->SetAttribute("offText", m_offText.c_str());
	}

	void UISwitchButton::init()
	{
		UIButton::init();				
	}

	void UISwitchButton::update(f32 dt)
	{
		UIPane::update(dt);		
	}

	void UISwitchButton::draw(const renderer::IRendererPtr& renderer)
	{
		UIPane::draw(renderer);

		if(m_font.isValid())
		{
			if(m_isOn && m_text.length()>0)
				drawButtonText(renderer, m_text);
			else if(!m_isOn && m_offText.length()>0)
				drawButtonText(renderer, m_offText);
		}		

		//Note: Now drawing text is poor, for all buttons, we prefer to use anim instead of text.
	}

	bool UISwitchButton::processEvent(const IEvent& event)
	{
		if(event.getUID()==events_id::EVT_TOUCH)
		{
			EvtTouch* evtTouch = (EvtTouch*)&event;
			core::position2df pos(evtTouch->X,evtTouch->Y);
			if(m_finalRect.isPointInside(pos))
			{
				if(m_isHolding && evtTouch->Phase==Touch_Ended)
				{					
					m_isHolding = false;

					m_isOn = !m_isOn;

					setAnimState();					

					EvtUIButtonSwitched evt(m_ownerLayerID, m_objID, m_isOn);
					EventManager::getInstance().raiseEvent(evt);
					DBG("test", "raise button switched event, button is now %s.", m_isOn?"on":"off");					
				}
				else if(evtTouch->Phase==Touch_Cancelled)
				{					
					m_isHolding = false;
					setAnimState();
				}
				else if(evtTouch->Phase==Touch_Began)
				{
					m_isHolding = true;

					if(m_isOn)
					{
						if(m_pressedAnimID>=0)
						{
							m_cxAnimObj->SetAnimOnce(m_pressedAnimID,true);
						}
					}
					else
					{
						if(m_offPressedAnimID>=0)
						{	
							m_cxAnimObj->SetAnimOnce(m_offPressedAnimID,true);
						}
					}
					
				}

				return true;//absorb this touch event, so UIObjects can't overlap, else only one can receive events
			}
			else if(m_isHolding)
			{				
				m_isHolding = false;				
				setAnimState();
			}
		}
		return false;
	}

	void UISwitchButton::setAnimState()
	{		
		FLT_ASSERT(m_animID>=0 && m_offAnimID>=0);

		m_cxAnimObj->SetAnimOnce(m_isOn?m_animID:m_offAnimID, true);		
	}

}//ui
}//flt
