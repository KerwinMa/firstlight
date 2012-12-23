#ifndef FLT_FIRST_LIGHT_H
#define FLT_FIRST_LIGHT_H

#include "common.h"
#include "BaseObject.h"
#include "sharePtr.h"
#include "app/App.h"

//core
#include "core/fltMath.h"
#include "core/aabbox3d.h"
#include "core/dimension2d.h"
#include "core/line2d.h"
#include "core/line3d.h"
#include "core/matrix4.h"
#include "core/plane3d.h"
#include "core/quaternion.h"
#include "core/rect.h"
#include "core/triangle3d.h"
#include "core/vector2d.h"
#include "core/vector3d.h"
#include "core/position2d.h"

//utils
#include "utils/ISingleton.h"
#include "utils/Logger.h"
#include "utils/StringUtil.h"

//stream
#include "stream/Stream.h"
#include "stream/Reader.h"
#include "stream/DataStream.h"
#include "stream/BufferStream.h"
#include "stream/FileStream.h"

//archive
#include "archive/ArchiveTypes.h"
#include "archive/ArchiveManager.h"
#include "archive/IArchive.h"
#include "archive/FileSystemArchive.h"

//resource
#include "ResourceManager.h"

//device
#include "device/IDevice.h"
#include "device/DeviceDefs.h"

//renderer
#include "renderer/IRenderer.h"
#include "renderer/RendererDefs.h"
#include "renderer/Color.h"
#include "renderer/VertexArray.h"
#include "renderer/StubRenderer.h"
#include "renderer/MaterialFactory.h"
//#include "renderer/opengl/OpenGLES1Renderer.h"

//image
#include "renderer/image/Image.h"
#include "renderer/image/IImageLoader.h"

//material
#include "renderer/material/MaterialMgr.h"
#include "renderer/material/TextureMgr.h"

//animation
#include "animation/animSetManager.h"

//event
#include "event/IEvent.h"
#include "event/IEventReceiver.h"
#include "event/EventManager.h"
#include "event/EventsDef.h"
#include "event/EvtKeyboard.h"
#include "event/EvtMouse.h"
#include "event/EvtTouch.h"
#include "event/EvtUI.h"
#include "event/EvtGame.h"

//xlib2d
#include "xlib2d/XSprite.h"
#include "xlib2d/XAnimObject.h"
#include "xlib2d/XFont.h"
#include "xlib2d/XSpriteMgr.h"
#include "xlib2d/XFontMgr.h"

//phy2d
#include "phy2d/phy2dWorld.h"
#include "phy2d/phy2dActor.h"

//ui
#include "ui/UILayer.h"
#include "ui/UIPane.h"
#include "ui/UIButton.h"
#include "ui/UISwitchButton.h"
#include "ui/UIManager.h"
#include "ui/UITouchZone.h"


//3rd party libs
#include "externs/tinyxml/tinyxml.h"
#include "externs/jsoncpp/json.h"
#include "externs/Box2D/Box2D.h"

//sound
#include "sound/soundPlay.h"

namespace flt
{

void initEngine();

void uninitEngine();

device::IDevicePtr createDevice(const device::RenderDeviceAttribute& renderDeviceAttr);

FLT_FORWARD_PTR(Engine)

class Engine: public BaseObject
{
public:
	Engine();
	~Engine();
};

#define FLT_IMPLEMENT_ENGINE flt::EnginePtr engine = new flt::Engine();

} //end namespace flt

#endif //FLT_FIRST_LIGHT_H