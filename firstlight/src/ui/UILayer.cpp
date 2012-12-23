#include "UILayer.h"
#include "UIPane.h"
#include "UIButton.h"
#include "UISwitchButton.h"
#include "UITouchZone.h"
#include "event/EvtTouch.h"
#include "event/EventManager.h"
//#include "animation/animSetManager.h"

#include <algorithm>

namespace flt
{
namespace ui
{
	UILayer::UILayer()
		:m_layerID(0),m_layerObj(0),
		 m_pAnimObject(0),
		 m_bActive(false)
	{		
		
	}

	UILayer::~UILayer()
	{		
		releaseAll();
	}

	void UILayer::releaseAll()
	{
		SAFE_DEL(m_layerObj);

		for(int i=0, is=(int)m_uiObjs.size(); i<is; i++)
		{
			delete m_uiObjs[i];
		}

		m_uiObjs.clear();		
		m_xmlFileName.clear();

		SAFE_DEL(m_pAnimObject);
	}

	UIObject* UILayer::createUIObjectByType(const stringc& type)
	{
		UIObject* uio = 0;

		if(type=="UIPane")
			uio = new UIPane();
		else if(type=="UIButton")
			uio = new UIButton();
		else if(type=="UISwitchButton")
			uio = new UISwitchButton();
		else if(type=="UITouchZone")
			uio = new UITouchZone();

		return uio;
	}

	bool UILayer::loadFromXMLFile(const char* xmlFile)
	{
		TiXmlDocument doc(xmlFile);
		if (!doc.LoadFile()) 
			return false;

		TiXmlHandle hDoc(&doc);
		
		TiXmlElement* pDocRoot = hDoc.FirstChildElement().Element();
		// should always have a valid root but handle gracefully if it does
		if (!pDocRoot) 
			return false;	

		stringc docRootName=pDocRoot->Value();
		if(docRootName != "FLTResXML")
			return false;

		TiXmlHandle hDocRoot(pDocRoot);
		TiXmlElement* pXMLNodeUILayer = hDocRoot.FirstChild("UILayer").Element();

		bool bLoaded = loadFromXML(pXMLNodeUILayer);

		if(bLoaded)
		{
			m_xmlFileName = xmlFile;

			//load animation data
			//m_animSet = anim::AnimSetManager::getInstance().getRes(m_xmlFileName,anim::ANIM_SET_XML);	
			m_animSet = new anim::AnimationSet();
			if(!m_animSet->loadFromXMLFile(xmlFile))
			{
				m_animSet.reset();
			}
		}

		return bLoaded;
	}

	void UILayer::saveXMLFile()
	{
		saveToXMLFile(m_xmlFileName.c_str());
	}

	void UILayer::saveToXMLFile(const char* otherXMLFile)
	{
		TiXmlDocument doc;  		

		TiXmlElement * pDocRoot = new TiXmlElement( "FLTResXML" );  
		doc.LinkEndChild( pDocRoot ); 

		TiXmlElement * pXmlNodeUILayer = new TiXmlElement( "UILayer" );  
		pDocRoot->LinkEndChild( pXmlNodeUILayer ); 

		saveToXML(pXmlNodeUILayer);

		///----------------
		/// save animation data
		if(m_animSet.isValid())
		{
			TiXmlElement * pXmlNodeAS = new TiXmlElement( "AnimationSet" );  
			pDocRoot->LinkEndChild( pXmlNodeAS );
			m_animSet->saveToXML(pXmlNodeAS);
		}
		///--------------

		doc.SaveFile(otherXMLFile);
	}

	bool UILayer::loadFromXML(TiXmlElement *xmlNodeUILayer)
	{
		releaseAll();				
		
		TiXmlHandle hXmlNode=TiXmlHandle(xmlNodeUILayer);

		// load m_layerObject
		// Note: this is a trick! Use UILayer xml node as the layer obj xml node (a UIObject xml node)
		m_layerObj = new UIObject();
		m_layerObj->loadFromXML(xmlNodeUILayer);
		m_layerObj->setRotationCenterType(ER_RECT_CENTER);

		u16 curObjID = 0;

		// load ui objs
		TiXmlElement* pObjNode = hXmlNode.FirstChild( "UIObjects" ).FirstChild("UIObject").Element();
		for( ; pObjNode; pObjNode=pObjNode->NextSiblingElement())
		{
			stringc type;
			pObjNode->QueryStringAttribute("type", &type);
			UIObject* uio = createUIObjectByType(type);			
			if(uio->loadFromXML(pObjNode))
			{
				uio->init();
				uio->attachToParent(m_layerObj);
				uio->setOwnerLayerID(m_layerID);
				uio->setObjID(curObjID++);
				m_uiObjs.push_back(uio);
			}
		}
				
		return true;
	}

	void UILayer::saveToXML(TiXmlElement *xmlNodeUILayer)
	{			
		m_layerObj->saveToXML(xmlNodeUILayer);

		TiXmlElement *objsNode = new TiXmlElement("UIObjects");
		xmlNodeUILayer->LinkEndChild(objsNode);

		int objNum =  (int)m_uiObjs.size();

		for(int i=0; i<objNum; i++)
		{
			TiXmlElement * node = new TiXmlElement("UIObject");
			m_uiObjs[i]->saveToXML(node);
			objsNode->LinkEndChild(node);
		}		
	}

	void UILayer::init()
	{
		if(m_animSet.isValid())
		{
			m_pAnimObject = new anim::AnimObject();
			m_pAnimObject->init(this, m_animSet);
		}

		setActive(true);
	}

	bool UILayer::onEvent(const IEvent& event)
	{
		if(!m_bActive)
			return false;

		for(int i=0, is=(int)m_uiObjs.size(); i<is; i++)
		{
			if(m_uiObjs[i]->processEvent(event))
				return true;
		}

		return false;
	}

	void UILayer::update(f32 dt)
	{
		if(m_pAnimObject!=0)
			m_pAnimObject->update(dt);

		m_layerObj->update(dt);

		for(int i=0, is=(int)m_uiObjs.size(); i<is; i++)
		{
			m_uiObjs[i]->update(dt);
		}
	}

	void UILayer::draw(const renderer::IRendererPtr& renderer)
	{
		m_layerObj->draw(renderer);

		for(int i=0, is=(int)m_uiObjs.size(); i<is; i++)
		{
			m_uiObjs[i]->draw(renderer);
		}
	}

	void UILayer::setActive(bool active)
	{
		m_bActive = active;
		
		if(m_bActive)
		{
			EventManager::getInstance().registerReceiver(events_id::EVT_TOUCH, this);
		}
		else
		{
			EventManager::getInstance().unRegisterReceiver(events_id::EVT_TOUCH, this);
		}
	}


	int UILayer::getAnimableNum()
	{
		return int(m_uiObjs.size()+1);
	}

	anim::IAnimable* UILayer::getAnimable(int index)
	{
		anim::IAnimable* animable = 0;
		
		if(index<(int)m_uiObjs.size())
			animable = (anim::IAnimable*)m_uiObjs[index];
		else
			animable = (anim::IAnimable*)m_layerObj;

		return animable;
	}

	void UILayer::playAnimation(const stringc& animName)
	{
		if(m_pAnimObject!=0)
		{
			m_pAnimObject->playAnimation(animName);
			m_currentAnimName = animName;
		}
	}

	bool UILayer::isCurrentAnimationOver()
	{
		if(m_pAnimObject!=0)
			return m_pAnimObject->isAnimationOver();
		else
			return true;
	}

	/////////////////////////////////////////////////////////////////////


	void UILayer::outputLayerDefs(stringc& output)
	{
		output.append("//Auto-generated UIObject IDs.\n\n");		

		stringc tempStr;
		stringc space(" ");				

		for(int i=0, is=(int)m_uiObjs.size(); i<is; i++)
		{
			char buf[512];
			tempStr = m_layerObj->getName() + "_" + m_uiObjs[i]->getName();
			std::transform(tempStr.begin(), tempStr.end(), tempStr.begin(), ::toupper);
			tempStr.replace(tempStr.find(space), space.size(), "_");			
			tempStr.replace(tempStr.find(space), space.size(), "_");//not a good way...			
			
			sprintf(buf,"static const int UIID_%s = %d;\n", tempStr.c_str(), m_uiObjs[i]->getObjID());
						
			output.append(buf);
		}		
	}

}//ui
}//flt
