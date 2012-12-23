#ifndef FLT_UI_MANAGER_H
#define FLT_UI_MANAGER_H

#include "utils/ISingleton.h"
#include "ui/UILayer.h"

namespace flt
{
namespace ui
{
	class UIManager: public utils::ISingleton<UIManager>
	{
	public:
		UIManager();
		virtual ~UIManager();

		UILayer* loadUILayerFromXML(const char* xmlFile);

		void onScreenSizeChanged(u32 newWidth, u32 newHeight);

		void onDeviceOrientationChanged(EDeviceOrientation newOrientation);

	private:
		u16 m_layerNextID;

		//TODO: manager all ui layers in this class

	};

}//ui
}//flt

#endif //FLT_UI_MANAGER_H
