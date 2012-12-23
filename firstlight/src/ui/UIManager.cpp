#include "UIManager.h"

namespace flt
{
namespace ui
{

	UIManager::UIManager()
		:m_layerNextID(0)
	{

	}

	UIManager::~UIManager()
	{

	}

	UILayer* UIManager::loadUILayerFromXML(const char* xmlFile)
	{
		UILayer* layer = new UILayer();
		layer->setLayerID(m_layerNextID);
		layer->loadFromXMLFile(xmlFile);		
		layer->init();

		m_layerNextID ++;

		return layer;
	}

	void UIManager::onScreenSizeChanged(u32 newWidth, u32 newHeight)
	{
		//TODO: manager all UI need to konw screen size (full screen or relative size)
	}

	void UIManager::onDeviceOrientationChanged(EDeviceOrientation newOrientation)
	{
		//TODO: UI may auto-adjust for orientation
	}

	
}//ui
}//flt