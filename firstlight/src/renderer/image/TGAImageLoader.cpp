#include "TGAImageLoader.h"
#include "Image.h"
#include "stream/FileStream.h"
#include "utils/Logger.h"
#include "utils/ColorConverter.h"
#include <cstring>

namespace flt
{
namespace renderer
{	

TGAImageLoader::TGAImageLoader()
{
	LOG("engine","TGAImageLoader created.");
}

TGAImageLoader::~TGAImageLoader()
{
	LOG("engine","TGAImageLoader released.");
}

bool TGAImageLoader::isFileExtensionSupport(const char* fileName) const
{
	return strstr(fileName, ".tga") != 0 || strstr(fileName, ".TGA") != 0;
}

bool TGAImageLoader::isFileDataSupport(FileStream* file) const
{
	if(file==0)
		return false;

	TGAFooter footer;
	
	if(file->Size() < sizeof(TGAFooter))
		return false;

	memset(&footer, 0, sizeof(TGAFooter));

	file->Seek(file->Size()-sizeof(TGAFooter), io::beg);
	file->Read(&footer, sizeof(TGAFooter));

	return (!strcmp(footer.Signature,"TRUEVISION-XFILE.")); // very old tgas are refused.

}

Image* TGAImageLoader::loadImage(FileStream* file) const
{
	TGAHeader header;
	u8* colorMap = 0;

	file->Read(&header, sizeof(TGAHeader));

#ifdef __BIG_ENDIAN__
	header.ColorMapLength = byteswap(header.ColorMapLength);
	header.ImageWidth = byteswap(header.ImageWidth);
	header.ImageHeight = byteswap(header.ImageHeight);
#endif

	// skip image identification field
	if (header.IdLength)
		file->Seek(header.IdLength, io::cur);

	if (header.ColorMapType)
	{
		// read color map
		colorMap = new u8[header.ColorMapEntrySize/8 * header.ColorMapLength];
		file->Read(colorMap,header.ColorMapEntrySize/8 * header.ColorMapLength);
	}

	// read image

	u8* data = 0;

	if (header.ImageType == 2)
	{
		const s32 imageSize = header.ImageHeight * header.ImageWidth * header.PixelDepth/8;
		data = new u8[imageSize];
		file->Read(data, imageSize);
	}
	else if(header.ImageType == 10)
	{
		data = loadCompressedImage(file, header);
	}
	else
	{
		LOG("engine","Unsupported TGA file type: %s", file->GetFileName().c_str());
		if (colorMap)
			delete [] colorMap;
		return 0;
	}

	Image* image = 0;

	switch(header.PixelDepth)
	{
	case 16:
		{
			image = new Image(IPF_RGBA_5551, header.ImageWidth, header.ImageHeight);
			if (image)
			{
				ColorConverter::convert16BitTo16Bit((s16*)data,
				(s16*)image->lock(), header.ImageWidth,	header.ImageHeight, 0, (header.ImageDescriptor&0x20)==0);
			}
		}
		break;
	case 24:
		{
			image = new Image(IPF_RGB_888, header.ImageWidth, header.ImageHeight);				
			if (image)
			{
				ColorConverter::convert24BitTo24Bit(
				(u8*)data, (u8*)image->lock(), header.ImageWidth, header.ImageHeight, 0, (header.ImageDescriptor&0x20)==0, true);
			}
		}
		break;
	case 32:
		{
			image = new Image(IPF_ARGB_8888, header.ImageWidth, header.ImageHeight);						
			if (image)
			{
				ColorConverter::convert32BitTo32Bit((s32*)data,
				(s32*)image->lock(), header.ImageWidth, header.ImageHeight, 0, (header.ImageDescriptor&0x20)==0);
			}
		}
		break;
	default:
		LOG("engine","Unsupported TGA format: %s", file->GetFileName().c_str());
		break;
	}
	if (image)
		image->unlock();
	delete [] data;
	if (colorMap)
		delete [] colorMap;

	return image;

}

//! loads a compressed tga.
u8 * TGAImageLoader::loadCompressedImage(FileStream *file, const TGAHeader& header) const
{
	// This was written and sent in by Jon Pry, thank you very much!
	// I only changed the formatting a little bit.

	s32 bytesPerPixel = header.PixelDepth/8;
	s32 imageSize =  header.ImageHeight * header.ImageWidth * bytesPerPixel;
	u8* data = new u8[imageSize];
	s32 currentByte = 0;

	while(currentByte < imageSize)
	{
		u8 chunkheader = 0;
		file->Read(&chunkheader, sizeof(u8)); // Read The Chunk's Header

		if(chunkheader < 128) // If The Chunk Is A 'RAW' Chunk
		{
			chunkheader++; // Add 1 To The Value To Get Total Number Of Raw Pixels

			file->Read(&data[currentByte], bytesPerPixel * chunkheader);
			currentByte += bytesPerPixel * chunkheader;
		}
		else
		{
			// thnx to neojzs for some fixes with this code

			// If It's An RLE Header
			chunkheader -= 127; // Subtract 127 To Get Rid Of The ID Bit

			s32 dataOffset = currentByte;
			file->Read(&data[dataOffset], bytesPerPixel);

			currentByte += bytesPerPixel;

			for(s32 counter = 1; counter < chunkheader; counter++)
			{
				for(s32 elementCounter=0; elementCounter < bytesPerPixel; elementCounter++)
					data[currentByte + elementCounter] = data[dataOffset + elementCounter];

				currentByte += bytesPerPixel;
			}
		}
	}

	return data;
}



} //end namespace renderer
} //end namespace flt