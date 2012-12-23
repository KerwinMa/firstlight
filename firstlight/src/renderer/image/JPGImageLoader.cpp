#include "JPGImageLoader.h"
#include "Image.h"
#include "stream/FileStream.h"
#include "utils/Logger.h"
#include "utils/ColorConverter.h"
#include <cstring>

namespace flt
{
namespace renderer
{	

JPGImageLoader::JPGImageLoader()
{
	LOG("engine","JPGImageLoader created.");
}

JPGImageLoader::~JPGImageLoader()
{
	LOG("engine","JPGImageLoader released.");
}

bool JPGImageLoader::isFileExtensionSupport(const char* fileName) const
{
	return strstr(fileName, ".jpg") != 0 || strstr(fileName, ".JPG") != 0;
}

#ifdef FLT_COMPILE_WITH_LIBJPEG_

// struct for handling jpeg errors
struct irr_jpeg_error_mgr
{
    // public jpeg error fields
    struct jpeg_error_mgr pub;

    // for longjmp, to return to caller on a fatal error
    jmp_buf setjmp_buffer;
};

void JPGImageLoader::init_source (j_decompress_ptr cinfo)
{
	// DO NOTHING
}



boolean JPGImageLoader::fill_input_buffer (j_decompress_ptr cinfo)
{
	// DO NOTHING
	return 1;
}



void JPGImageLoader::skip_input_data (j_decompress_ptr cinfo, long count)
{
	jpeg_source_mgr * src = cinfo->src;
	if(count > 0)
	{
		src->bytes_in_buffer -= count;
		src->next_input_byte += count;
	}
}



void JPGImageLoader::term_source (j_decompress_ptr cinfo)
{
	// DO NOTHING
}


void JPGImageLoader::error_exit (j_common_ptr cinfo)
{
	// unfortunately we need to use a goto rather than throwing an exception
	// as gcc crashes under linux crashes when using throw from within
	// extern c code

	// Always display the message
	(*cinfo->err->output_message) (cinfo);

	// cinfo->err really points to a irr_error_mgr struct
	irr_jpeg_error_mgr *myerr = (irr_jpeg_error_mgr*) cinfo->err;

	longjmp(myerr->setjmp_buffer, 1);
}


void JPGImageLoader::output_message(j_common_ptr cinfo)
{
	// display the error message.
	c8 temp1[JMSG_LENGTH_MAX];
	(*cinfo->err->format_message)(cinfo, temp1);
	LOG("engine", "JPEG FATAL ERROR",temp1);
}

#endif // FLT_COMPILE_WITH_LIBJPEG_

bool JPGImageLoader::isFileDataSupport(FileStream* file) const
{
	
#ifndef FLT_COMPILE_WITH_LIBJPEG_
	return false;
#else

	if(file==0)
		return false;	

	if(file->Size() < 6)
		return false;

	s32 jfif = 0;
	file->Seek(6, io::beg);
	file->Read(&jfif, sizeof(s32));
	return (jfif == 0x4a464946 || jfif == 0x4649464a);

	#endif
}

Image* JPGImageLoader::loadImage(FileStream* file) const
{
#ifndef FLT_COMPILE_WITH_LIBJPEG_
	return 0;
#else

	u8 **rowPtr=0;
	u8* input = new u8[file->Size()];
	file->Read(input, file->Size());

	// allocate and initialize JPEG decompression object
	struct jpeg_decompress_struct cinfo;
	struct irr_jpeg_error_mgr jerr;

	//We have to set up the error handler first, in case the initialization
	//step fails.  (Unlikely, but it could happen if you are out of memory.)
	//This routine fills in the contents of struct jerr, and returns jerr's
	//address which we place into the link field in cinfo.

	cinfo.err = jpeg_std_error(&jerr.pub);
	cinfo.err->error_exit = error_exit;
	cinfo.err->output_message = output_message;

	// compatibility fudge:
	// we need to use setjmp/longjmp for error handling as gcc-linux
	// crashes when throwing within external c code
	if (setjmp(jerr.setjmp_buffer))
	{
		// If we get here, the JPEG code has signaled an error.
		// We need to clean up the JPEG object and return.

		jpeg_destroy_decompress(&cinfo);

		delete [] input;
		// if the row pointer was created, we delete it.
		if (rowPtr)
			delete [] rowPtr;

		// return null pointer
		return 0;
	}

	// Now we can initialize the JPEG decompression object.
	jpeg_create_decompress(&cinfo);

	// specify data source
	jpeg_source_mgr jsrc;

	// Set up data pointer
	jsrc.bytes_in_buffer = file->Size();
	jsrc.next_input_byte = (JOCTET*)input;
	cinfo.src = &jsrc;

	jsrc.init_source = init_source;
	jsrc.fill_input_buffer = fill_input_buffer;
	jsrc.skip_input_data = skip_input_data;
	jsrc.resync_to_restart = jpeg_resync_to_restart;
	jsrc.term_source = term_source;

	// Decodes JPG input from whatever source
	// Does everything AFTER jpeg_create_decompress
	// and BEFORE jpeg_destroy_decompress
	// Caller is responsible for arranging these + setting up cinfo

	// read file parameters with jpeg_read_header()
	jpeg_read_header(&cinfo, TRUE);

	bool useCMYK=false;
	if (cinfo.jpeg_color_space==JCS_CMYK)
	{
		cinfo.out_color_space=JCS_CMYK;
		cinfo.out_color_components=4;
		useCMYK=true;
	}
	else
	{
		cinfo.out_color_space=JCS_RGB;
		cinfo.out_color_components=3;
	}
	cinfo.do_fancy_upsampling=FALSE;

	// Start decompressor
	jpeg_start_decompress(&cinfo);

	// Get image data
	u16 rowspan = cinfo.image_width * cinfo.out_color_components;
	u32 width = cinfo.image_width;
	u32 height = cinfo.image_height;

	// Allocate memory for buffer
	u8* output = new u8[rowspan * height];

	// Here we use the library's state variable cinfo.output_scanline as the
	// loop counter, so that we don't have to keep track ourselves.
	// Create array of row pointers for lib
	rowPtr = new u8* [height];

	for( u32 i = 0; i < height; i++ )
		rowPtr[i] = &output[ i * rowspan ];

	u32 rowsRead = 0;

	while( cinfo.output_scanline < cinfo.output_height )
		rowsRead += jpeg_read_scanlines( &cinfo, &rowPtr[rowsRead], cinfo.output_height - rowsRead );

	delete [] rowPtr;
	// Finish decompression

	jpeg_finish_decompress(&cinfo);

	// Release JPEG decompression object
	// This is an important step since it will release a good deal of memory.
	jpeg_destroy_decompress(&cinfo);

	// convert image
	Image* image = 0;
	if (useCMYK)
	{
		image = new Image(IPF_RGB_888, width, height);
		const u32 size = 3*width*height;
		u8* data = (u8*)image->lock();
		if (data)
		{
			for (u32 i=0,j=0; i<size; i+=3, j+=4)
			{
				// Also works without K, but has more contrast with K multiplied in
//				data[i+0] = output[j+2];
//				data[i+1] = output[j+1];
//				data[i+2] = output[j+0];
				data[i+0] = (char)(output[j+2]*(output[j+3]/255.f));
				data[i+1] = (char)(output[j+1]*(output[j+3]/255.f));
				data[i+2] = (char)(output[j+0]*(output[j+3]/255.f));
			}
		}
		image->unlock();
		delete [] output;
	}
	else
		image = new Image(IPF_RGB_888, width, height, output, true);

	delete [] input;

	return image;

#endif //FLT_COMPILE_WITH_LIBJPEG_

}

} //end namespace renderer
} //end namespace flt


