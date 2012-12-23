#ifndef FLT_IMAGE_H
#define FLT_IMAGE_H

#include "common.h"

namespace flt
{
namespace renderer
{	

enum IMG_PIXEL_FORMAT
{	
	IPF_RGBA_5551 = 0,	//16 bits
	IPF_RGBA_4444,		//16 bits
	IPF_RGB_565,		//16 bits
	IPF_RGB_888,		//24 bits
	IPF_RGBA_8888,		//32 bits
	IPF_ARGB_8888,		//32 bits

	IPF_UNKNOWN,
};

class Image
{
public:
	Image(IMG_PIXEL_FORMAT format, s32 width, s32 height);
	Image(IMG_PIXEL_FORMAT format, s32 width, s32 height, void* data, bool useOutBuffer);
	Image(Image* source, bool makePowerOf2);

	~Image();

	void* lock();
	void unlock();

	static u32 getBppFromFormat(IMG_PIXEL_FORMAT format);

	IMG_PIXEL_FORMAT getFormat() { return m_pixelFormat; }
	s32 getWidth() { return m_width; }
	s32 getHeight() { return m_height; }
	s32 getOriginalWidth() { return m_originalWidth; }
	s32 getOriginalHeight() { return m_originalHeight; }
	s32 getPitch() { return m_pitch; }
	bool hasAlphaChannel();

	//! copy image data to the target, if target is bigger or smaller, the data only fits target(no scale)
	// The pixel format must match.
	// This is very slow, don't call it each frame.
	void copyTo(Image* target);

	//! return false if width or height is not power of 2
	bool isSizePowerOf2();

private:
	void * m_data;
	s32 m_width;
	s32 m_height;
	s32 m_originalWidth;
	s32 m_originalHeight;
	s32 m_pitch;
	IMG_PIXEL_FORMAT m_pixelFormat;	
};


} //end namespace renderer
} //end namespace flt

#endif //FLT_IMAGE_H