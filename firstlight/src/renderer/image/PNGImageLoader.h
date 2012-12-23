#ifndef FLT_PNG_IMAGELOADER_H
#define FLT_PNG_IMAGELOADER_H

#include "IImageLoader.h"

namespace flt
{
namespace renderer
{	

class PNGImageLoader: public IImageLoader
{
public:	
	PNGImageLoader();

	virtual ~PNGImageLoader();

	virtual bool isFileExtensionSupport(const char* fileName) const;

	virtual bool isFileDataSupport(FileStream* file) const;

	virtual Image* loadImage(FileStream* file) const;
};


} //end namespace renderer
} //end namespace flt

#endif //FLT_PNG_IMAGELOADER_H

