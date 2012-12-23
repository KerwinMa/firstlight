#ifndef FLT_OPENGLES_TEXTURE_H
#define FLT_OPENGLES_TEXTURE_H

#include "config.h"
#include "renderer/material/TextureLayer.h"
#include "renderer/image/Image.h"

#ifdef FLT_COMPILE_WITH_OPENGLES1

#if defined(FLT_DEVICE_WIN32)
	//#include "libs/win32/gles1/egl.h"
	//#include "libs/win32/gles1/gl.h"
	#include <GLES/gl.h>
	#include <GLES/glext.h>
	#include <GLES/egl.h>
#elif defined(FLT_DEVICE_IOS)
	#include <OpenGLES/ES1/gl.h>
	#include <OpenGLES/ES1/glext.h>
#endif

namespace flt
{	
namespace renderer
{

FLT_FORWARD_PTR(OpenGLESTexture)
class Image;

class OpenGLESTexture: public Texture
{
public:

	OpenGLESTexture(Image* image);
	virtual ~OpenGLESTexture();

	virtual void create(bool holdImage=false);

	virtual bool isValid();	

	GLuint getGLID() { return m_glID; }	

private:
	GLuint m_glID;
	Image* m_image;

	void processOpenGLFormat(IMG_PIXEL_FORMAT format, 
		GLint& InternalFormat, 
		GLenum& PixelFormat, 
		GLenum& PixelType);

	void loadImageToTexture(Image* image, bool useMipMap);


};
	

} //end namespace renderer
} //end namespace flt

#endif //FLT_COMPILE_WITH_OPENGLES1

#endif //FLT_OPENGLES_TEXTURE_H