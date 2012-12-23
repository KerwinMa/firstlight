#ifndef FLT_TGA_IMAGELOADER_H
#define FLT_TGA_IMAGELOADER_H

#include "IImageLoader.h"

namespace flt
{
namespace renderer
{	

// byte-align structures
#include "BeginPack.h"

struct TGAHeader
{
	u8 IdLength;
	u8 ColorMapType;
	u8 ImageType;
	u8 FirstEntryIndex[2];
	u16 ColorMapLength;
	u8 ColorMapEntrySize;
	u8 XOrigin[2];
	u8 YOrigin[2];
	u16 ImageWidth;
	u16 ImageHeight;
	u8 PixelDepth;
	u8 ImageDescriptor;
} PACK_STRUCT;

struct TGAFooter
{
	u32 ExtensionOffset;
	u32 DeveloperOffset;
	c8  Signature[18];
} PACK_STRUCT;

// Default alignment
#include "EndPack.h"

class TGAImageLoader: public IImageLoader
{
public:	
	TGAImageLoader();

	virtual ~TGAImageLoader();

	virtual bool isFileExtensionSupport(const char* fileName) const;

	virtual bool isFileDataSupport(FileStream* file) const;

	virtual Image* loadImage(FileStream* file) const;

private:
	u8* loadCompressedImage(FileStream *file, const TGAHeader& header) const;
};


} //end namespace renderer
} //end namespace flt

#endif //FLT_TGA_IMAGELOADER_H