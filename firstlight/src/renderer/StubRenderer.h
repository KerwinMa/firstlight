#ifndef FLT_STUB_RENDERER_H
#define FLT_STUB_RENDERER_H

#include "renderer/IRenderer.h"
#include "core/dimension2d.h"
#include "core/rect.h"

namespace flt
{	
namespace renderer
{

FLT_FORWARD_PTR(IImageLoader)
FLT_FORWARD_PTR(TextureManager)

//useful for profile and common functions, also can be used for background render

class StubRenderer: public IRenderer
{
public:	
	explicit StubRenderer(const core::dimension2di& screenSize);

	virtual ~StubRenderer();

	virtual void onSizeChange(u32 width, u32 height);

	virtual bool makeCurrent();

	virtual void beginRender();

	virtual void endRender();

	virtual void setCamera(const CameraPtr& camera);

	virtual CameraPtr getCamera();

	virtual void clearBuffer(int bufferMask = FB_COLOR|FB_DEPTH);	

	virtual bool swapBuffer();

	virtual void setClearColor(const Color& color);

	virtual Color getClearColor() const;

	virtual void setClearDepth(f32 depth);

	virtual f32 getClearDepth() const;

	virtual u32 getMaxTextureUnitNum() const;

	virtual Image* createImageFromFile(const char* filePath);

	virtual Image* createImageFromStream(FileStream* file);	

	virtual void setClip2D(const core::rectf& clipRect);

	virtual void resetClip2D();

	virtual void setOrientation(EDeviceOrientation newOrientation);	

	virtual EDeviceOrientation getOrientation() const;

protected:

	CameraPtr m_camera3D;

	Color m_clearColor;
	f32 m_clearDepth;
	core::recti m_viewPort;
	core::dimension2di m_screenSize;

	core::rectf m_clipRect2D;
	bool m_hasClip2D;

	RENDER_SCREEN_MODE m_curRenderScreenMode;

	EDeviceOrientation m_orientation;

	array_t<IImageLoaderPtr> m_imageLoaders;	

	//uvArray: 0~3: u0, v0, u1, v1
	//return true if the quard is totally clipped
	bool clipQuard2D(const core::rectf& clipRect, core::rectf& quardRect, f32* uvArray, Color* colorArray);
};

} //end namespace renderer
} //end namespace flt

#endif //FLT_STUB_RENDERER_H