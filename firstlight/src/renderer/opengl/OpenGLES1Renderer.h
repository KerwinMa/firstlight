#ifndef FLT_OPENGLES1_RENDERER_H
#define FLT_OPENGLES1_RENDERER_H

#include "config.h"

#ifdef FLT_COMPILE_WITH_OPENGLES1

#include "renderer/StubRenderer.h"
#include "renderer/material/RenderStates.h"

#if defined(FLT_DEVICE_WIN32)
	//#include "libs/win32/gles1/egl.h"
	//#include "libs/win32/gles1/gl.h"
	#include <GLES/gl.h>
	#include <GLES/glext.h>
	#include <GLES/egl.h>
#elif defined(FLT_DEVICE_IOS)
	#include <OpenGLES/ES1/gl.h>
	#include <OpenGLES/ES1/glext.h>
	#import <OpenGLES/EAGL.h>
	#import <QuartzCore/QuartzCore.h>
	#import <OpenGLES/EAGLDrawable.h>
#endif

namespace flt
{	
namespace renderer
{

FLT_FORWARD_PTR(OpenGLES1Renderer)

//OpenGL ES 1.1 Common profile

class OpenGLES1Renderer: public StubRenderer
{
public:	
	OpenGLES1Renderer(const core::dimension2di& screenSize);

	virtual ~OpenGLES1Renderer();

	virtual void onSizeChange(u32 width, u32 height);

	virtual bool makeCurrent();

	virtual void beginRender();

	virtual void endRender();

	virtual void begin2DMode();

	virtual void end2DMode();

	virtual void clearBuffer(int bufferMask = FB_COLOR|FB_DEPTH);	

	virtual bool swapBuffer();	

	virtual void setRenderScreenMode(RENDER_SCREEN_MODE mode);

	virtual u32 getMaxTextureUnitNum() const;

	virtual TexturePtr createTexture(const char* filePath);

	virtual TexturePtr createTexture(FileStream* file);

	virtual TexturePtr createTextureFromImage(Image* image);

	virtual TexturePtr createDeviceNativeTexture(const char* filePath);

	virtual TexturePtr createDeviceNativeTexture(FileStream* file);
	
	//--set global states--

	virtual void enableLighting(bool enable);

	virtual void setSceneAmbientLightColor(const Colorf& color);

	//---------------------	

	virtual void setViewTransform(const core::matrix4& viewTransform);

	virtual void setWorldTransform(const core::matrix4& worldTransform);

	virtual void setMaterial(const MaterialPtr& material);

	virtual void drawPrimitives(const VertexArray& vertexArray);

	virtual void drawBatchBuffer(const BatchBuffer& batchBuffer);

	virtual void set2DMaterial(BUILT_IN_MATERIAL_TYPE materialType);
	
	virtual void set2DMaterialTextureWrapModeU(TEX_WRAP_MODE wrapModeU);

	virtual void set2DMaterialTextureWrapModeV(TEX_WRAP_MODE wrapModeV);

	virtual void set2DMaterialTextureMinFilter(TEX_FILTER_MODE minFilter);

	virtual void set2DMaterialTextureMagFilter(TEX_FILTER_MODE magFilter);

	virtual void drawImage2D(TexturePtr texture, s32 x, s32 y, const core::rectf* const srcRect, const Color* const colorArray=0, core::matrix4* pMatrix=0);

	virtual void drawImage2D(TexturePtr texture, s32 x, s32 y, const Color* const colorArray=0, core::matrix4* pMatrix=0);

	virtual void drawRect2D(const core::rectf& rect, const Color& color);

	virtual void drawRect2D(const core::rectf& rect, const Color* const colorArray=0, bool singleColor=false);

	virtual void drawBox2D(const core::rectf& rect, const Color& color = renderer::ColorWhite);

public:

#if defined(FLT_DEVICE_WIN32)
	bool init(HWND hWnd, void* shareRender);
#elif defined(FLT_DEVICE_IOS)
	bool init();
	bool resizeFromLayer(CAEAGLLayer *layer);
#endif

private:

	void initRenderer();

	void uninitRenderer();

#if defined(FLT_DEVICE_WIN32)
	EGLDisplay	m_eglDisplay;
	EGLConfig	m_eglConfig;
	EGLContext	m_eglContext;
	EGLSurface	m_eglWindowSurface;

	EGLint m_majorVersion;
	EGLint m_minorVersion;

	bool m_initialized;
	
#elif defined(FLT_DEVICE_IOS)
	EAGLContext *context;
	
    // The pixel dimensions of the CAEAGLLayer
    GLint backingWidth;
    GLint backingHeight;
	
    // The OpenGL ES names for the framebuffer and renderbuffer used to render to this view
    GLuint defaultFramebuffer, colorRenderbuffer;
#endif

	u32 m_maxTextureUnits;

private:

	//transform management
	core::matrix4 m_viewMatrix;
	core::matrix4 m_worldMatrix;

	u32 m_transformDirtyFlag;

	enum
	{		
		TDF_VIEW = 1,
		TDF_WORLD = TDF_VIEW<<1,

		TDF_ALL = TDF_VIEW | TDF_WORLD
	};

	void applyTransforms(bool forceApply=false);

	bool m_useBatch;

	// Array states only set/unset around draw primitives
	// so will not break batching
	// used inside renderer
	struct ArrayStates
	{				
		bool VertexArrayEnable;
		bool NormalArrayEnable;
		bool ColorArrayEnable;
		bool UVArrayEnable[FLT_MAX_TEXTURE_UNITS];

		ArrayStates()
			:VertexArrayEnable(false),
			 NormalArrayEnable(false),
			 ColorArrayEnable(false)
		{
			for(u32 i=0; i<FLT_MAX_TEXTURE_UNITS; ++i)
			{
				UVArrayEnable[i] = false;
			}
		}

	};


	inline void enableVertexArray(bool enable) { m_curArrayStates.VertexArrayEnable = enable; }
	inline void enableNormalArray(bool enable) { m_curArrayStates.NormalArrayEnable = enable; }
	inline void enableColorArray(bool enable) { m_curArrayStates.ColorArrayEnable = enable; }
	inline void enableUVArray(u32 unit, bool enable) { m_curArrayStates.UVArrayEnable[unit] = enable; }
	
	void applyRenderPass(const RenderPass& pass);

	void applyRenderStates(const RenderStates& renderState, bool forceApply=false);

	//-- texture --

	void setActiveTextureUnit(u32 unit, bool forceApply=false);

	void setClientActiveTextureUnit(u32 unit, bool forceApply=false);

	void enableTexture(bool enable, bool forceApply=false);

	bool setTexture(TexturePtr texture);

	void applyTextureStates(const TextureState& textureState, bool forceApply=false);

	// ------------

	void applyGlobalStates(bool forceApply=false);	

	void applyArrayStates(bool forceApply=false);

	void flushBatch();

	void drawPrimitivesInternal(const VertexArray& vertexArray);
	
	MaterialPtr m_currentMaterial;

	//GlobalStates set by renderer interface
	GlobalStates m_curGlobalStates;
	
	//ArrayStates set internal when rendering
	ArrayStates m_curArrayStates;	

	struct ShadowStates
	{
		//RenderStates set by material->renderPass	
		RenderStates m_renderStates;

		//TextureStates set by material->renderPass->TextureLayers
		TextureState m_textureStates[FLT_MAX_TEXTURE_UNITS];

		u32 m_activeTexture;
		u32 m_clientActiveTexture;

		bool m_textureEnable[FLT_MAX_TEXTURE_UNITS];

		//GlobalStates set by renderer interface		
		GlobalStates m_globalStates;

		//ArrayStates set internal when rendering	
		ArrayStates m_arrayStates;
	};

	ShadowStates m_lastStates;

	BatchBuffer* m_pBatchBuffer2DImage; //batch buffer for 2d image

	/// 2d image batch buffer size, count in quard, so the vertex count is quard*4
	static const int k_maxBatch2DImgQuardNum = 200;
	
	VertexArray m_vertexArray2DQuard; //temp vertex array for 2d quard
	VertexArray m_vertexArray2DImage; //temp vertex array for 2d image	

	MaterialPtr m_sharedMaterial2D; //material shared by standard 2d drawing APIs

	BUILT_IN_MATERIAL_TYPE m_sharedMaterial2DType;

	MaterialPtr m_defaultMaterial; //Engine default material
};

} //end namespace renderer
} //end namespace flt

#endif //FLT_COMPILE_WITH_OPENGLES1

#endif //FLT_OPENGLES1_RENDERER_H