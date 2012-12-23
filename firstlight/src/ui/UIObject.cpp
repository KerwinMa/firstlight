#include "UIObject.h"
#include "renderer/IRenderer.h"
#include "utils/Logger.h"

namespace flt
{
namespace ui
{

	UIObject::UIObject()
		:m_pos(20.0f,20.0f),
		 m_localRect(-10.0f,-10.0f,20.0f,20.0f),
		 m_scaleX(1.0f), m_scaleY(1.0f),
		 m_rotation(0.0f),
		 m_rotationCenter(ER_LEFT_TOP),
		 m_alpha(1.0f),
		 m_r(1.0f), m_g(1.0f), m_b(1.0f),
		 m_ownerLayerID(0),
		 m_objID(0),
		 m_parentObj(0),
		 m_isLocalXformDirty(true)
#ifdef FLT_DEBUG
		,m_bShowDebug(false)
#endif
	{
		
	}

	bool UIObject::loadFromXML(TiXmlElement *xmlUIObjNode)
	{
		TiXmlHandle hXmlNode(xmlUIObjNode);

		xmlUIObjNode->QueryStringAttribute("name",&m_name);

		TiXmlElement * basicNode = hXmlNode.FirstChild("basic").Element();
		if(basicNode==0)
		{
			DBG("engineDbg_UI","Fail to load UIObject, missing basic node!");
			return false;
		}

		basicNode->QueryFloatAttribute("x", &m_pos.X); // If this fails, original value is left as-is
		basicNode->QueryFloatAttribute("y", &m_pos.Y);

		f32 rl, rt, rw, rh;
		basicNode->QueryFloatAttribute("rl", &rl);
		basicNode->QueryFloatAttribute("rt", &rt);
		basicNode->QueryFloatAttribute("rw", &rw);
		basicNode->QueryFloatAttribute("rh", &rh);
		m_localRect.set(rl, rt, rw, rh);

		basicNode->QueryFloatAttribute("scaleX", &m_scaleX);
		basicNode->QueryFloatAttribute("scaleY", &m_scaleY);
		basicNode->QueryFloatAttribute("rotation", &m_rotation);
		basicNode->QueryFloatAttribute("alpha", &m_alpha);
		basicNode->QueryFloatAttribute("r", &m_r);
		basicNode->QueryFloatAttribute("g", &m_g);
		basicNode->QueryFloatAttribute("b", &m_b);		

		return true;
	}

	void UIObject::saveToXML(TiXmlElement *xmlUIObjNode)
	{
		xmlUIObjNode->SetAttribute("name",m_name.c_str());

		TiXmlElement * basicNode;
		basicNode = new TiXmlElement( "basic" );  
		xmlUIObjNode->LinkEndChild( basicNode );  		
		basicNode->SetDoubleAttribute("x", m_pos.X);
		basicNode->SetDoubleAttribute("y", m_pos.Y);
		basicNode->SetDoubleAttribute("rl", m_localRect.UpperLeftCorner.X);
		basicNode->SetDoubleAttribute("rt", m_localRect.UpperLeftCorner.Y);
		basicNode->SetDoubleAttribute("rw", m_localRect.getWidth());
		basicNode->SetDoubleAttribute("rh", m_localRect.getHeight());
		basicNode->SetDoubleAttribute("scaleX", m_scaleX);
		basicNode->SetDoubleAttribute("scaleY", m_scaleY);
		basicNode->SetDoubleAttribute("rotation", m_rotation);
		basicNode->SetDoubleAttribute("alpha", m_alpha);
		basicNode->SetDoubleAttribute("r", m_r);
		basicNode->SetDoubleAttribute("g", m_g);
		basicNode->SetDoubleAttribute("b", m_b);
	}	

	void UIObject::updateLocalXForm()
	{
		if(m_isLocalXformDirty)
		{
			core::matrix4 matT, matRot;

			m_localXform.makeIdentity();
			
			matT.setTranslation(core::vector3df(m_pos.X, m_pos.Y, 0.0f));
			m_localXform.setScale(core::vector3df(m_scaleX,m_scaleY,1.0f));
			//TODO: need to study and modify the 2d transform system
			matRot.setRotationDegrees(core::vector3df(0.0f,0.0f,-m_rotation));

			if(m_rotationCenter==ER_RECT_CENTER)
			{
				core::matrix4 matCenterOff, matCenterOff2;
				core::vector3df rectCenterOff(m_pos.X+m_localRect.getWidth()/2, m_pos.Y+m_localRect.getHeight()/2, 0);
				matCenterOff.setTranslation(-rectCenterOff);
				matCenterOff2.setTranslation(rectCenterOff);
				m_localXform = matCenterOff2*matT*m_localXform*matRot*matCenterOff;
			}
			else
			{			
				m_localXform = matT*m_localXform*matRot;
			}			

			m_isLocalXformDirty = false;
		}
	}

	void UIObject::updateFinalRect()
	{		
		core::matrix4 mat = getLocalXForm();
		if(m_parentObj!=0)
		{
			mat = m_parentObj->getLocalXForm()*mat;
		}

		core::aabbox3df aabb(m_localRect.UpperLeftCorner.X, m_localRect.UpperLeftCorner.Y, 0, m_localRect.LowerRightCorner.X, m_localRect.LowerRightCorner.Y, 0);
		mat.transformBoxEx(aabb);
		m_finalRect.UpperLeftCorner.X = aabb.MinEdge.X;
		m_finalRect.UpperLeftCorner.Y = aabb.MinEdge.Y;
		m_finalRect.LowerRightCorner.X = aabb.MaxEdge.X;
		m_finalRect.LowerRightCorner.Y = aabb.MaxEdge.Y;
	}

	void UIObject::update(f32 dt)
	{
		//updateFinalRect();
	}

	void UIObject::draw(const renderer::IRendererPtr& renderer)
	{
#ifdef FLT_DEBUG		
		//debug ui, show touch rect, should switched by debugger on runtime.
		//if(m_bShowDebug)
			renderer->drawBox2D(m_finalRect, renderer::Color(128,255,0,255));
#endif
	}

	void* UIObject::getTarget(const stringc& name)
	{
		if(name=="pos2d")
		{
			return &m_pos;
		}
		else if(name=="scaleX")
		{
			return &m_scaleX;
		}
		else if(name=="scaleY")
		{
			return &m_scaleY;
		}
		else if(name=="rot2d")
		{
			return &m_rotation;
		}
		else if(name=="alpha")
		{
			return &m_alpha;
		}
		else if(name=="red")
		{
			return &m_r;
		}
		else if(name=="green")
		{
			return &m_g;
		}
		else if(name=="blue")
		{
			return &m_b;
		}

		return 0;
	}

	void UIObject::notifyTargetModified()
	{
		notifyChangePrameter();
	}
}//ui
}//flt
