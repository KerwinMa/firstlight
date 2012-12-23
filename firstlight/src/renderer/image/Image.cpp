#include "Image.h"

namespace flt
{
namespace renderer
{	

u32 enlargeToPower2(u32 n)
{
	while( (n&(n-1))!=0 )
	{
		n |= n-1;
		n++;
	}
	return n;
}

u32 Image::getBppFromFormat(IMG_PIXEL_FORMAT format)
{
	u32 bpp = 0;

	switch(format)
	{
		case IPF_RGBA_5551:
		case IPF_RGBA_4444:
		case IPF_RGB_565:
			bpp = 16;
			break;

		case IPF_RGB_888:
			bpp = 24;
			break;

		case IPF_RGBA_8888:
		case IPF_ARGB_8888:
			bpp = 32;
			break;
	}	

	return bpp;
}

Image::Image(IMG_PIXEL_FORMAT format, s32 width, s32 height)
	:m_pixelFormat(format),m_width(width),m_height(height),m_originalWidth(width),m_originalHeight(height),m_data(0)
{
	
	u32 bpp = getBppFromFormat(m_pixelFormat);		
	m_pitch = bpp * m_width / 8;
	//TODO: align pitch

	m_data = new s8[m_height * m_pitch];
}

Image::Image(IMG_PIXEL_FORMAT format, s32 width, s32 height, void* data, bool useOutBuffer)
	:m_pixelFormat(format),m_width(width),m_height(height),m_originalWidth(width),m_originalHeight(height),m_data(0)
{
	u32 bpp = getBppFromFormat(m_pixelFormat);		
	m_pitch = bpp * m_width / 8;
	//TODO: align pitch

	if(useOutBuffer)
	{
		m_data = data;
	}
	else
	{
		m_data = new s8[m_height * m_pitch];
		memcpy(m_data, data, m_height * m_pitch);
	}
}

Image::Image(Image* source, bool makePowerOf2)
{
	FLT_ASSERT(source!=0);

	m_originalWidth = source->m_width;
	m_originalHeight = source->m_height;
	m_width = makePowerOf2 ? enlargeToPower2(m_originalWidth) : m_originalWidth;
	m_height = makePowerOf2 ? enlargeToPower2(m_originalHeight) : m_originalHeight;
	m_pixelFormat = source->m_pixelFormat;

	u32 bpp = getBppFromFormat(m_pixelFormat);		
	m_pitch = bpp * m_width / 8;
	//TODO: align pitch

	m_data = new s8[m_height * m_pitch];
	source->copyTo(this);
}

Image::~Image()
{
	delete[] (s8*)m_data;
}

void* Image::lock()
{
	return m_data;
}

void Image::unlock()
{

}

bool Image::hasAlphaChannel()
{
	switch(m_pixelFormat)
	{
	case IPF_RGB_565:
	case IPF_RGB_888:
		return false;

	case IPF_RGBA_5551:
	case IPF_RGBA_4444:
	case IPF_RGBA_8888:
	case IPF_ARGB_8888:
		return true;

	default:
		return false;
	}
}

bool Image::isSizePowerOf2()
{
	return (m_width&(m_width-1))==0 && (m_height&(m_height-1))==0;		
}

void Image::copyTo(Image* target)
{
	FLT_ASSERT(target->m_pixelFormat==m_pixelFormat && "Image::copyTo: pixel format not match!");	

	s32 destWidth = (m_width < target->m_width) ? m_width : target->m_width;
	s32 destHeight = (m_height < target->m_height) ? m_height : target->m_height;

	u32 bpp = getBppFromFormat(m_pixelFormat);		
	s32 srcPitch = bpp * m_width / 8;
	s32 destPitch = bpp * destWidth / 8;
	s32 targetPitch = bpp * target->m_width / 8;

	u32 targetDataSize = target->m_width * target->m_height * bpp/8;;
	memset(target->m_data, 0, targetDataSize);

	if(m_width==target->m_width && m_height==target->m_height)
	{
		memcpy(target->m_data, m_data, srcPitch*m_height);
	}
	else
	{
		u8* _src = (u8*)m_data;
		u8* _dest = (u8*)target->m_data;
		for(int i=0; i<destHeight; ++i)
		{
			memcpy(_dest, _src, destPitch);
			_src += srcPitch;
			_dest += targetPitch;
		}
	}
}


} //end namespace renderer
} //end namespace flt