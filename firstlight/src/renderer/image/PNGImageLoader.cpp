#include "PNGImageLoader.h"
#include "Image.h"
#include "stream/FileStream.h"
#include "utils/Logger.h"
#include "utils/ColorConverter.h"
#include <cstring>
#include "config.h"

#ifdef FLT_COMPILE_WITH_LIBPNG_	
	#include "externs/libpng/png.h"
#endif // FLT_COMPILE_WITH_LIBPNG_

namespace flt
{
namespace renderer
{	

PNGImageLoader::PNGImageLoader()
{
	LOG("engine","PNGImageLoader created.");
}

PNGImageLoader::~PNGImageLoader()
{
	LOG("engine","PNGImageLoader released.");
}

#ifdef FLT_COMPILE_WITH_LIBPNG_
// PNG function for error handling
static void png_cpexcept_error(png_structp png_ptr, png_const_charp msg)
{
	LOG("engine", "PNG FATAL ERROR", msg);
	longjmp(png_ptr->jmpbuf, 1);
}

// PNG function for file reading
void PNGAPI user_read_data_fcn(png_structp png_ptr, png_bytep data, png_size_t length)
{
	png_size_t check;

	// changed by zola {
	FileStream* file=(FileStream*)png_ptr->io_ptr;
	check=(png_size_t) file->Read((void*)data,length);
	// }

	if (check != length)
		png_error(png_ptr, "Read Error");
}
#endif // FLT_COMPILE_WITH_LIBPNG_

bool PNGImageLoader::isFileExtensionSupport(const char* fileName) const
{
	const c8* ext = strrchr(fileName, '.');
	if (ext == 0)
		return false;

	return strstr(fileName, ".png") != 0 || strstr(fileName, ".PNG") != 0;
}

bool PNGImageLoader::isFileDataSupport(FileStream* file) const
{
#ifdef FLT_COMPILE_WITH_LIBPNG_
	if(file==0)
		return false;

	png_byte buffer[8];
	// Read the first few bytes of the PNG file
	if (file->Read(buffer, 8) != 8)
		return false;

	// Check if it really is a PNG file
	return !png_sig_cmp(buffer, 0, 8);
#else
	return false;
#endif // FLT_COMPILE_WITH_LIBPNG_
}

Image* PNGImageLoader::loadImage(FileStream* file) const
{
#ifdef FLT_COMPILE_WITH_LIBPNG_
	if (file==0)
		return 0;
	Image* image = 0;
	//Used to point to image rows
	u8** RowPointers = 0;

	png_byte buffer[8];
	// Read the first few bytes of the PNG file
	if( file->Read(buffer, 8) != 8 )
	{
		LOG("engine", "LOAD PNG: can't read file\n", file->GetFileName().c_str());
		return 0;
	}
	// Check if it really is a PNG file
	if( png_sig_cmp(buffer, 0, 8) )
	{
		LOG("engine", "LOAD PNG: not really a png\n", file->GetFileName().c_str());
		return 0;
	}
	
	// Allocate the png read struct
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
		NULL, (png_error_ptr)png_cpexcept_error, NULL);


	if (!png_ptr)
	{
		LOG("engine", "LOAD PNG: Internal PNG create read struct failure\n", file->GetFileName().c_str());
		return 0;
	}


	// Allocate the png info struct
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
	{
		LOG("engine", "LOAD PNG: Internal PNG create info struct failure\n", file->GetFileName().c_str());
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		return 0;
	}
	// for proper error handling
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		if (RowPointers)
			delete [] RowPointers;
		return 0;
	}
	// changed by zola so we don't need to have public FILE pointers
	png_set_read_fn(png_ptr, file, user_read_data_fcn);

	png_set_sig_bytes(png_ptr, 8); // Tell png that we read the signature

	png_read_info(png_ptr, info_ptr); // Read the info section of the png file

	u32 Width;
	u32 Height;
	s32 BitDepth;
	s32 ColorType;
	{
		// Use temporary variables to avoid passing casted pointers
		png_uint_32 w,h;
		// Extract info
		png_get_IHDR(png_ptr, info_ptr,
			&w, &h,
			(int*)&BitDepth, (int*)&ColorType, NULL, NULL, NULL);
		Width=w;
		Height=h;
	}
	// Convert palette color to true color
	if (ColorType==PNG_COLOR_TYPE_PALETTE)
		png_set_palette_to_rgb(png_ptr);

	// Convert low bit colors to 8 bit colors
	if (BitDepth < 8)
	{
		if (ColorType==PNG_COLOR_TYPE_GRAY || ColorType==PNG_COLOR_TYPE_GRAY_ALPHA)
			png_set_gray_1_2_4_to_8(png_ptr);
		else
			png_set_packing(png_ptr);
	}

	if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
		png_set_tRNS_to_alpha(png_ptr);

	// Convert high bit colors to 8 bit colors
	if (BitDepth == 16)
		png_set_strip_16(png_ptr);

	// Convert gray color to true color
	if (ColorType==PNG_COLOR_TYPE_GRAY || ColorType==PNG_COLOR_TYPE_GRAY_ALPHA)
		png_set_gray_to_rgb(png_ptr);

	// Update the changes
	png_read_update_info(png_ptr, info_ptr);
	png_get_IHDR(png_ptr, info_ptr,
		(png_uint_32*)&Width, (png_uint_32*)&Height,
		(int*)&BitDepth, (int*)&ColorType, NULL, NULL, NULL);

	// Convert RGBA to BGRA
	if (ColorType==PNG_COLOR_TYPE_RGB_ALPHA)
	{
#ifdef __BIG_ENDIAN__
		png_set_swap_alpha(png_ptr);
#else
		png_set_bgr(png_ptr);
#endif
	}

	// Update the changes
	png_get_IHDR(png_ptr, info_ptr,
		(png_uint_32*)&Width, (png_uint_32*)&Height,
		&BitDepth, &ColorType, NULL, NULL, NULL);

	// Create the image structure to be filled by png data
    IMG_PIXEL_FORMAT colorformat;
	if (ColorType==PNG_COLOR_TYPE_RGB_ALPHA)
        colorformat = IPF_ARGB_8888;
	else
        colorformat = IPF_RGB_888;
//#ifdef _IRR_COMPILE_WITH_PSGL_
    //if(colorformat==ECF_A8R8G8B8) colorformat = ECF_R8G8B8A8;
//#endif
    image = new Image(colorformat, Width, Height);
	if (!image)
	{
		LOG("engine", "LOAD PNG: Internal PNG create image struct failure\n", file->GetFileName().c_str());
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		return 0;
	}

	// Create array of pointers to rows in image data
	RowPointers = new png_bytep[Height];
	if (!RowPointers)
	{
		LOG("engine","LOAD PNG: Internal PNG create row pointers failure\n", file->GetFileName().c_str());
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		delete image;
		return 0;
	}
	// Fill array of pointers to rows in image data
	unsigned char* data = (unsigned char*)image->lock();
	for (u32 i=0; i<Height; ++i)
	{
		RowPointers[i]=data;
		data += image->getPitch();
	}

	// for proper error handling
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		delete [] RowPointers;
		image->unlock();
		delete [] image;
		return 0;
	}
	// Read data using the library function that handles all transformations including interlacing
	png_read_image(png_ptr, RowPointers);
	png_read_end(png_ptr, NULL);
	delete [] RowPointers;
	image->unlock();
	png_destroy_read_struct(&png_ptr,&info_ptr, 0); // Clean up memory
#ifdef _IRR_COMPILE_WITH_PSGL_
    if(colorformat==ECF_R8G8B8A8)
    {
        void* data = image->lock();
        CColorConverter::convert_A8R8G8B8toR8G8B8A8(data, image->getImageDataSizeInBytes()/4, data);
        image->unlock();
    }
#endif
	return image;
#else
	return 0;
#endif // FLT_COMPILE_WITH_LIBPNG_

}

} //end namespace renderer
} //end namespace flt
