//
//  OpenGLES1Renderer_iOS.mm
//  firstlight
//
//  Created by wu hao on 10-9-6.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#include "renderer/opengl/OpenGLES1Renderer.h"


#if defined(FLT_COMPILE_WITH_OPENGLES1) && defined(FLT_DEVICE_IOS)

#include "utils/Logger.h"
#include "stream/FileStream.h"

namespace flt
{
	namespace renderer
	{
		
		OpenGLES1Renderer::OpenGLES1Renderer(const core::dimension2di& screenSize)
		:StubRenderer(screenSize)
		{
			
			
		}
		
		OpenGLES1Renderer::~OpenGLES1Renderer()
		{
			// Tear down GL
			if (defaultFramebuffer)
			{
				glDeleteFramebuffersOES(1, &defaultFramebuffer);
				defaultFramebuffer = 0;
			}
			
			if (colorRenderbuffer)
			{
				glDeleteRenderbuffersOES(1, &colorRenderbuffer);
				colorRenderbuffer = 0;
			}
			
			// Tear down context
			if ([EAGLContext currentContext] == context)
				[EAGLContext setCurrentContext:nil];
			
			[context release];
			context = nil;
			
		}
		
		bool OpenGLES1Renderer::init()
		{
			context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
			
			if (!context || ![EAGLContext setCurrentContext:context])
			{
				return false;
			}
			
			// Create default framebuffer object. The backing will be allocated for the current layer in -resizeFromLayer
			glGenFramebuffersOES(1, &defaultFramebuffer);
			glGenRenderbuffersOES(1, &colorRenderbuffer);
			glBindFramebufferOES(GL_FRAMEBUFFER_OES, defaultFramebuffer);
			glBindRenderbufferOES(GL_RENDERBUFFER_OES, colorRenderbuffer);
			glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, colorRenderbuffer);
			
			initRenderer();
			
			return true;
		}
		
		bool OpenGLES1Renderer::resizeFromLayer(CAEAGLLayer *layer)
		{
			// Allocate color buffer backing based on the current layer size
			glBindRenderbufferOES(GL_RENDERBUFFER_OES, colorRenderbuffer);
			[context renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:layer];
			glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_WIDTH_OES, &backingWidth);
			glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_HEIGHT_OES, &backingHeight);
			
			if (glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES) != GL_FRAMEBUFFER_COMPLETE_OES)
			{
				NSLog(@"Failed to make complete framebuffer object %x", glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES));
				return false;
			}
			
			return true;
		}
		
		bool OpenGLES1Renderer::makeCurrent()
		{
			// This application only creates a single context which is already set current at this point.
			// This call is redundant, but needed if dealing with multiple contexts.
			[EAGLContext setCurrentContext:context];
			
			// This application only creates a single default framebuffer which is already bound at this point.
			// This call is redundant, but needed if dealing with multiple framebuffers.
			glBindFramebufferOES(GL_FRAMEBUFFER_OES, defaultFramebuffer);
		}
		
		bool OpenGLES1Renderer::swapBuffer()
		{	
			// This application only creates a single color renderbuffer which is already bound at this point.
			// This call is redundant, but needed if dealing with multiple renderbuffers.
			glBindRenderbufferOES(GL_RENDERBUFFER_OES, colorRenderbuffer);
			[context presentRenderbuffer:GL_RENDERBUFFER_OES];
		}	
		
		IRendererPtr createOpenGLES1Renderer(const core::dimension2di& screenSize)
		{
			OpenGLES1RendererPtr renderer = new OpenGLES1Renderer(screenSize);
			
			if(!renderer->init())
			{
				renderer.reset();
			}	
			
			if(renderer.isValid())
				return IRendererPtr(renderer);		
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
