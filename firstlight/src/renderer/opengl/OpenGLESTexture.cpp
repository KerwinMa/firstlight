#include "OpenGLESTexture.h"

#ifdef FLT_COMPILE_WITH_OPENGLES1

#include "renderer/image/Image.h"

namespace flt
{
namespace renderer
{

OpenGLESTexture::OpenGLESTexture(Image* image)
:m_glID(-1),m_image(0)
{
	FLT_ASSERT(image);
	
	m_width = image->getWidth();
	m_height = image->getHeight();
	m_originalWidth = image->getOriginalWidth();
	m_originalHeight = image->getOriginalHeight();
	m_hasAlphaChannel = image->hasAlphaChannel();
	m_hasMipMaps = false;//TODO: mipmaps should set by material
	m_image = image;
	
	IMG_PIXEL_FORMAT format = image->getFormat();

	glGenTextures(1,&m_glID);			
}

OpenGLESTexture::~OpenGLESTexture()
{
	if(m_image)
	{
		delete m_image;
	}

	if(m_glID!=-1)
	{
		glDeleteTextures(1,&m_glID);
		m_glID = -1;
	}
}

void OpenGLESTexture::create(bool holdImage/*=false*/)
{
	loadImageToTexture(m_image, m_hasMipMaps);
	
	if(!holdImage)	//TODO:也许以后image改成share的
		m_image = 0; 
}

void OpenGLESTexture::processOpenGLFormat(IMG_PIXEL_FORMAT format, 
						 GLint& InternalFormat, 
						 GLenum& PixelFormat, 
						 GLenum& PixelType)
{
	switch(format)
	{
	case IPF_RGBA_5551:
		InternalFormat=GL_RGBA;
		PixelFormat=GL_RGBA;
		PixelType=GL_UNSIGNED_SHORT_5_5_5_1;
		break;
	case IPF_RGBA_4444:
		InternalFormat=GL_RGBA;
		PixelFormat=GL_RGBA;
		PixelType=GL_UNSIGNED_SHORT_4_4_4_4;
		break;
	case IPF_RGB_565:
		InternalFormat=GL_RGB;
		PixelFormat=GL_RGB;
		PixelType=GL_UNSIGNED_SHORT_5_6_5;
		break;
	case IPF_RGB_888:
		InternalFormat=GL_RGB;
		PixelFormat=GL_RGB;
		PixelType=GL_UNSIGNED_BYTE;
		break;
	case IPF_RGBA_8888:
		InternalFormat=GL_RGBA;
		PixelFormat=GL_RGBA;
		PixelType=GL_UNSIGNED_BYTE;
		break;

	case IPF_ARGB_8888:	
		//FLT_ASSERT(false && "GLES not support IPF_ARGB_8888");
		//Note: now use GL_BGRA_EXT glext in powervr es1.1 windows sdk
		//should test in other platform
#if defined (FLT_DEVICE_WIN32)			
		InternalFormat=GL_BGRA_EXT;
		PixelFormat=GL_BGRA_EXT;
#elif defined (FLT_DEVICE_IOS)
		InternalFormat=GL_RGBA;
		PixelFormat=GL_BGRA;
#endif			
		PixelType=GL_UNSIGNED_BYTE;
		break;

	default:
		FLT_ASSERT(false && "not support unknown format");
		break;
	}
}

void OpenGLESTexture::loadImageToTexture(Image* image, bool useMipMap)
{	
	if(useMipMap)
	{
		// automatically generate and update mipmaps
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	}

	GLint internalFormat;
	GLenum pixelFormat; 
	GLenum pixelType;
	
	processOpenGLFormat(image->getFormat(), internalFormat, pixelFormat, pixelType);

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, image->getWidth(), image->getHeight(), 0, pixelFormat, pixelType, image->lock());
	image->unlock();
}

bool OpenGLESTexture::isValid()
{
	return m_glID>=0;
}

}// end namespace renderer
}// end namespace flt

#endif //FLT_COMPILE_WITH_OPENGLES1