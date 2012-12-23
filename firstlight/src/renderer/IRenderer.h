#ifndef FLT_I_RENDERER_H
#define FLT_I_RENDERER_H

#include "BaseObject.h"
#include "sharePtr.h"
#include "renderer/RendererDefs.h"
#include "renderer/Color.h"
#include "renderer/VertexArray.h"
#include "renderer/BatchBuffer.h"
#include "renderer/material/Material.h"
#include "core/matrix4.h"

namespace flt
{

class FileStream;

namespace renderer
{

FLT_FORWARD_PTR(IRenderer)

FLT_FORWARD_PTR(Texture)

FLT_FORWARD_PTR(TextureManager)

FLT_FORWARD_PTR(Camera)

class Image;

class IRenderer: public BaseObject
{
public:		
	virtual ~IRenderer(){}	

	virtual void onSizeChange(u32 width, u32 height) = 0;

	virtual bool makeCurrent() = 0;

	virtual void beginRender() = 0;

	virtual void endRender() = 0;

	virtual void setCamera(const CameraPtr& camera) = 0;

	virtual CameraPtr getCamera() = 0;

	virtual void begin2DMode() = 0;

	virtual void end2DMode() = 0;

	virtual void clearBuffer(int bufferMask = FB_COLOR|FB_DEPTH) = 0;	

	virtual bool swapBuffer() = 0;

	virtual void setClearColor(const Color& color) = 0;

	virtual Color getClearColor() const = 0;

	virtual void setClearDepth(f32 depth) = 0;

	virtual f32 getClearDepth() const = 0;	

	virtual void setRenderScreenMode(RENDER_SCREEN_MODE mode) = 0;

	virtual u32 getMaxTextureUnitNum() const = 0;	

	virtual Image* createImageFromFile(const char* filePath) = 0;

	virtual Image* createImageFromStream(FileStream* file) = 0;

	virtual TexturePtr createTexture(const char* filePath) = 0;

	virtual TexturePtr createTexture(FileStream* file) = 0;

	virtual TexturePtr createTextureFromImage(Image* image) = 0;

	virtual TexturePtr createDeviceNativeTexture(const char* filePath) = 0;

	virtual TexturePtr createDeviceNativeTexture(FileStream* file) = 0;	
	

	//--set global states--

	virtual void enableLighting(bool enable) = 0;

	virtual void setSceneAmbientLightColor(const Colorf& color) = 0;

	//---------------------

	//NOTE: projection transform is set via setRenderScreenMode internlly by engine, 
	//maybe later engine will provide a interface to set custom projection transform.
	//But now there is no need.


	virtual void setViewTransform(const core::matrix4& viewTransform) = 0;

	virtual void setWorldTransform(const core::matrix4& worldTransform) = 0;

	virtual void setMaterial(const MaterialPtr& material) = 0;

	virtual void drawPrimitives(const VertexArray & vertexArray) = 0;

	virtual void drawBatchBuffer(const BatchBuffer& batchBuffer) = 0;

	//TODO:
	//-- 2d drawing --
	//Engine provides some pro-defined built in materials user can choose from, using set2DMaterial. And the texture is set via drawImage2D.
	//Advanced material setting should directly call setMaterial by passing material created mannully or load from file.	
	virtual void set2DMaterial(BUILT_IN_MATERIAL_TYPE materialType) = 0;

	//Set some of the shared 2d material's texture state, this method will modify the texture state of shared 2d material.
	//but these states are almost common to 2d materials, so it will not conflit with pre-defined 2d material most of the time, but still need take care.
	virtual void set2DMaterialTextureWrapModeU(TEX_WRAP_MODE wrapModeU) = 0;
	virtual void set2DMaterialTextureWrapModeV(TEX_WRAP_MODE wrapModeV) = 0;
	virtual void set2DMaterialTextureMinFilter(TEX_FILTER_MODE minFilter) = 0;
	virtual void set2DMaterialTextureMagFilter(TEX_FILTER_MODE magFilter) = 0;

	//set a screen space 2d clip rect
	//2d clip rect is used when drawImage2D, drawRect2D, the drawing will be clipped and limited in this rect.
	virtual void setClip2D(const core::rectf& clipRect) = 0;

	virtual void resetClip2D() = 0;
	
	//these drawing APIs use a shared 2d material, set by set2DMaterial
	//TODO: support custom material 2d drawing by adding additional drawing APIs
	//scrRect is used to draw a part of image, used for modules.
	//colors is for vertex color modulate with texture color, how to use vertex color is defined in material, including pre-defined 2d material.
	//pMatrix is for transform image's quard vertices
	virtual void drawImage2D(TexturePtr texture, s32 x, s32 y, const core::rectf* const srcRect, const Color* const colorArray=0, core::matrix4* pMatrix=0) = 0;

	virtual void drawImage2D(TexturePtr texture, s32 x, s32 y, const Color* const colorArray=0, core::matrix4* pMatrix=0) = 0;

	virtual void drawRect2D(const core::rectf& rect, const Color& color) = 0;

	virtual void drawRect2D(const core::rectf& rect, const Color* const colorArray=0, bool singleColor=false) = 0;

	virtual void drawBox2D(const core::rectf& rect, const Color& color = renderer::ColorWhite) = 0;

	virtual void setOrientation(EDeviceOrientation orientation) = 0;

	virtual EDeviceOrientation getOrientation() const = 0;

};

} //end namespace renderer
} //end namespace flt

#endif //FLT_I_RENDERER_H