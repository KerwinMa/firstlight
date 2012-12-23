#include "UIButton.h"
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
	UIButton::UIButton()
		:m_pressedAnimID(-1),m_doneAnimID(-1),
		 m_isHolding(false)
	{			
	}	

	UIButton::~UIButton()
	{				
	}	

	bool UIButton::loadFromXML(TiXmlElement *xmlUIObjNode)
	{
		if(UIPane::loadFromXML(xmlUIObjNode))
		{
			TiXmlHandle hXmlNode(xmlUIObjNode);

			TiXmlElement * buttonNode = hXmlNode.FirstChild("button").Element();
			if(buttonNode!=0)
			{
				buttonNode->QueryStringAttribute("font", &m_fontName);
				buttonNode->QueryIntAttribute("pressedAnimID",&m_pressedAnimID);
				buttonNode->QueryIntAttribute("doneAnimID",&m_doneAnimID);				
				buttonNode->QueryStringAttribute("text",&m_text);
			}
			else
			{
				DBG("engineDbg_UI","Fail to load button from xml, miss button node.");					
			}

			return true;
		}
		
		return false;		
	}

	void UIButton::saveToXML(TiXmlElement *xmlUIObjNode)
	{
		UIPane::saveToXML(xmlUIObjNode);

		xmlUIObjNode->SetAttribute("type","UIButton");

		TiXmlElement * buttonNode;
		buttonNode = new TiXmlElement( "button" );  
		xmlUIObjNode->LinkEndChild( buttonNode );  
		buttonNode->SetAttribute("font", m_fontName.c_str());
		buttonNode->SetAttribute("pressedAnimID", m_pressedAnimID);
		buttonNode->SetAttribute("doneAnimID", m_doneAnimID);
		buttonNode->SetAttribute("text", m_text.c_str());
	}

	void UIButton::init()
	{
		UIPane::init();	

		if(m_fontName.length()>0)
			m_font = XFontManager::getInstance().getRes(m_fontName);		
	}

	void UIButton::update(f32 dt)
	{
		UIPane::update(dt);

		if(m_cxAnimObj->GetCurAnim()==m_doneAnimID)
		{
			if(m_cxAnimObj->TestFlag(AO_FLAG_ANIM_OVER))
			{
				m_cxAnimObj->SetAnim(m_animID,true);
			}
		}
	}

	void UIButton::draw(const renderer::IRendererPtr& renderer)
	{
		UIPane::draw(renderer);		

		drawButtonText(renderer, m_text);		
	}

	void UIButton::drawButtonText(const renderer::IRendererPtr& renderer, const stringc& text)
	{
		if(m_font.isValid() && text.length()>0)
		{
			if(m_parentObj==0)
			{
				m_font->DrawTextLine(renderer, m_text.c_str(), m_pos.X, m_pos.Y);
			}
			else
			{
				//TODO: note, now button text don't support color
				renderer::Color color(255,255,255,255);
				color.A *= m_parentObj->getAlpha();
				color.R *= m_parentObj->getRed();
				color.G *= m_parentObj->getGreen();
				color.B *= m_parentObj->getBlue();
				m_font->SetColor(color);

				m_font->SetObjectMatrix(m_parentObj->getLocalXForm());
				m_font->DrawTextLine(renderer, text.c_str(), m_pos.X, m_pos.Y);
				m_font->SetObjectMatrix(core::IdentityMatrix);

				m_font->UnsetColor();
			}
		}
	}

	bool UIButton::processEvent(const IEvent& event)
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

					if(m_doneAnimID>=0)
					{
						m_cxAnimObj->SetAnimOnce(m_doneAnimID,false);
					}
					else
					{
						m_cxAnimObj->SetAnimOnce(m_animID,true);
					}

					EvtUIButtonTapped evt(m_ownerLayerID, m_objID);
					EventManager::getInstance().raiseEvent(evt);
					DBG("engineDbg_UI", "raise button tapped event");					
				}
				else if(evtTouch->Phase==Touch_Cancelled)
				{					
					m_isHolding = false;
					m_cxAnimObj->SetAnimOnce(m_animID,true);
				}
				else if(evtTouch->Phase==Touch_Began)
				{
					m_isHolding = true;
					if(m_pressedAnimID>=0)
					{
						m_cxAnimObj->SetAnimOnce(m_pressedAnimID,true);
					}
				}

				return true;//absorb this touch event, so UIObjects can't overlap, else only one can receive events
			}
			else if(m_isHolding)
			{				
				m_isHolding = false;				
				m_cxAnimObj->SetAnimOnce(m_animID,true);
			}
		}
		return false;
	}

}//ui
}//flt
