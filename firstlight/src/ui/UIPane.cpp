#include "UIPane.h"
#include "renderer/Color.h"
#include "renderer/IRenderer.h"
#include "xlib2d/XSprite.h"
#include "xlib2d/XSpriteMgr.h"
#include "xlib2d/XAnimObject.h"
#include "utils/Logger.h"

namespace flt
{
namespace ui
{
	UIPane::UIPane()
		:m_cxAnimObj(0),
		 m_animID(0)
	{
	}	

	UIPane::~UIPane()
	{
		SAFE_DEL(m_cxAnimObj);
	}

	void UIPane::saveToXML(TiXmlElement *xmlUIObjNode)
	{
		UIObject::saveToXML(xmlUIObjNode);

		xmlUIObjNode->SetAttribute("type","UIPane");

		TiXmlElement * paneNode;
		paneNode = new TiXmlElement( "pane" );  
		xmlUIObjNode->LinkEndChild( paneNode );  		
		paneNode->SetAttribute("sprite", m_spriteName.c_str());
		paneNode->SetAttribute("animID", m_animID);
	}

	bool UIPane::loadFromXML(TiXmlElement *xmlUIObjNode)
	{
		if(UIObject::loadFromXML(xmlUIObjNode))
		{
			TiXmlHandle hXmlNode(xmlUIObjNode);

			TiXmlElement * paneNode = hXmlNode.FirstChild("pane").Element();
			if(paneNode==0)
			{
				DBG("engineDbg_UI","Fail to load UIPane, missing pane node!");
				return false;			
			}
			
			paneNode->QueryStringAttribute("sprite", &m_spriteName);
			paneNode->QueryIntAttribute("animID",&m_animID);

			return true;
		}
		
		return false;
	}

	void UIPane::init()
	{
		m_sprite = XSpriteManager::getInstance().getRes(m_spriteName.c_str());
		if(m_alpha!=1.0f || m_r!=1.0f || m_g!=1.0f || m_b!=1.0f)
			m_sprite->SetMaterial2D(renderer::BIMT_2D_TEXTURE_ALPHA_BLEND_MODULATE_COLOR);
		m_cxAnimObj = new CXAnimObject();
		m_cxAnimObj->SetSprite(m_sprite);
		m_cxAnimObj->SetAnim(m_animID,true);		

		//auto calc local rect if not valid in layer file
		if(m_localRect.getWidth()<=0.0f || m_localRect.getHeight()<=0.0f)
		{
			//Note: in UIObject, scale,rotation already taken by local xform
			//m_cxAnimObj->SetScale(m_scaleX, m_scaleY);
			//m_cxAnimObj->SetRotation(m_rotation);
			m_cxAnimObj->CalcAnimFrameRect(m_animID, 0, m_localRect);
		}

		updateFinalRect();

		m_color.set(255*m_alpha, 255*m_r, 255*m_g, 255*m_b);
	}
	
	void UIPane::update(f32 dt)
	{
		UIObject::update(dt);

		m_cxAnimObj->UpdateAnim(dt);				
	}

	void UIPane::draw(const renderer::IRendererPtr& renderer)
	{	
		UIObject::draw(renderer);

		m_cxAnimObj->SetPos(m_pos);
		m_cxAnimObj->SetScale(m_scaleX, m_scaleY);
		m_cxAnimObj->SetRotation(m_rotation);

		if(m_parentObj==0)
		{
			m_sprite->SetColor(m_color);
			m_cxAnimObj->Draw(renderer);
		}
		else
		{
			renderer::Color color(m_color);
			color.A *= m_parentObj->getAlpha();
			color.R *= m_parentObj->getRed();
			color.G *= m_parentObj->getGreen();
			color.B *= m_parentObj->getBlue();
			m_sprite->SetColor(color);

			m_cxAnimObj->Draw(renderer, &m_parentObj->getLocalXForm());
		}						

		m_sprite->UnsetColor();		
	}

	void UIPane::notifyChangePrameter()
	{
		UIObject::notifyChangePrameter();

		m_color.set(255*m_alpha, 255*m_r, 255*m_g, 255*m_b);
	}

}//ui
}//flt
