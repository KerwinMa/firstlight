#include "renderer/opengl/OpenGLES1Renderer.h"
#include "utils/Logger.h"
#include "stream/FileStream.h"

#if defined(FLT_COMPILE_WITH_OPENGLES1) && defined(FLT_DEVICE_WIN32)

namespace flt
{
namespace renderer
{

OpenGLES1Renderer::OpenGLES1Renderer(const core::dimension2di& screenSize)
	:StubRenderer(screenSize)
{
	m_eglDisplay = EGL_NO_DISPLAY;
	m_eglConfig = NULL;
	m_eglWindowSurface = EGL_NO_SURFACE;
	m_eglContext = EGL_NO_CONTEXT;
	m_initialized = false;

}

OpenGLES1Renderer::~OpenGLES1Renderer()
{
	uninitRenderer();

	if(m_initialized)
	{
		eglMakeCurrent(m_eglDisplay, NULL, NULL, NULL);
		eglDestroyContext(m_eglDisplay, m_eglContext);
		eglDestroySurface(m_eglDisplay, m_eglWindowSurface);
		eglTerminate(m_eglDisplay);

		m_initialized = false;

		LOG("engine","OpenGLES1 renderer shut down.");
	}
}

bool OpenGLES1Renderer::init(HWND hWnd, void* shareRender)
{
	NativeWindowType eglWindow = hWnd;
	EGLint numConfigs;

	m_eglDisplay = eglGetDisplay(NativeDisplayType(GetDC(hWnd)));
	if(m_eglDisplay == EGL_NO_DISPLAY)
		m_eglDisplay = eglGetDisplay((NativeDisplayType) EGL_DEFAULT_DISPLAY);
	if(!m_eglDisplay)
		return false;

	if( eglInitialize(m_eglDisplay, &m_majorVersion, &m_minorVersion) == EGL_FALSE)
		return false;

	eglGetConfigs(m_eglDisplay, NULL, 0, &numConfigs);

	/*const EGLint configAttribs[] =
	{
	EGL_RED_SIZE,		8,
	EGL_GREEN_SIZE, 	8,
	EGL_BLUE_SIZE,		8,
	EGL_ALPHA_SIZE, 	EGL_DONT_CARE,
	EGL_DEPTH_SIZE, 	16,
	EGL_STENCIL_SIZE,	EGL_DONT_CARE,
	EGL_SURFACE_TYPE,	EGL_WINDOW_BIT,
	EGL_NONE
	};*/

	EGLint	pi32ConfigAttribs[128];
	int i = 0;
	pi32ConfigAttribs[i++] = EGL_RED_SIZE;
	pi32ConfigAttribs[i++] = 8;
	pi32ConfigAttribs[i++] = EGL_GREEN_SIZE;
	pi32ConfigAttribs[i++] = 8;
	pi32ConfigAttribs[i++] = EGL_BLUE_SIZE;
	pi32ConfigAttribs[i++] = 8;
	pi32ConfigAttribs[i++] = EGL_ALPHA_SIZE;
	pi32ConfigAttribs[i++] = 8;
	pi32ConfigAttribs[i++] = EGL_SURFACE_TYPE;
	pi32ConfigAttribs[i++] = EGL_WINDOW_BIT;
	pi32ConfigAttribs[i++] = EGL_NONE;


	if( eglChooseConfig(m_eglDisplay, pi32ConfigAttribs, &m_eglConfig, 1, &numConfigs) == EGL_FALSE || numConfigs==0)
		return false;

	m_eglWindowSurface = eglCreateWindowSurface(m_eglDisplay, m_eglConfig, eglWindow, NULL);
	if(m_eglWindowSurface==EGL_NO_SURFACE)
		return false;

	EGLContext shareContext = NULL;
	if(shareRender) 
	{
		OpenGLES1Renderer *render = static_cast<OpenGLES1Renderer *>(shareRender);
		shareContext = render->m_eglContext;
	}

	m_eglContext = eglCreateContext(m_eglDisplay, m_eglConfig, shareContext, NULL);

	eglMakeCurrent(m_eglDisplay, m_eglWindowSurface, m_eglWindowSurface, m_eglContext);

	m_initialized = true;

	initRenderer();

	return true;
}

bool OpenGLES1Renderer::makeCurrent()
{
	return (eglMakeCurrent(m_eglDisplay, m_eglWindowSurface, m_eglWindowSurface, m_eglContext) == EGL_TRUE);
}

bool OpenGLES1Renderer::swapBuffer()
{
	EGLint err=0;

	int r = eglSwapBuffers(m_eglDisplay, m_eglWindowSurface);
	if(r==EGL_FALSE)
		err = eglGetError();

	return ( r== EGL_TRUE);
}	

IRendererPtr createOpenGLES1Renderer(const core::dimension2di& screenSize, HWND hWnd, void* shareRender)
{
	FLT_ASSERT(hWnd!=0 && "hWnd should not 0!");

	OpenGLES1RendererPtr renderer = new OpenGLES1Renderer(screenSize);

	if(!renderer->init(hWnd, shareRender))
	{
		renderer.reset();
	}	

	if(renderer.isValid())
		return IRendererPtr(renderer);		//warning C4927, more than one user-defined conversion, 改成显示转换
	else
		return IRendererPtr();	
}

TexturePtr OpenGLES1Renderer::createDeviceNativeTexture(const char* filePath)
{
	FileStream file(filePath);
	return createDeviceNativeTexture(&file);
}

TexturePtr OpenGLES1Renderer::createDeviceNativeTexture(FileStream* stream)
{
	TexturePtr texture = new Texture();
	return texture;
}

}// end namespace renderer
}// end namespace flt

#endif //FLT_COMPILE_WITH_OPENGLES1 && FLT_DEVICE_WIN32