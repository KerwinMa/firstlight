#ifndef FLT_I_IMAGELOADER_H
#define FLT_I_IMAGELOADER_H

#include "common.h"
#include "BaseObject.h"
#include "sharePtr.h"

namespace flt
{

class FileStream;

namespace renderer
{	

class Image;

FLT_FORWARD_PTR(IImageLoader)

class IImageLoader: public BaseObject
{
public:	
	virtual ~IImageLoader(){};

	virtual bool isFileExtensionSupport(const char* fileName) const = 0;

	virtual bool isFileDataSupport(FileStream* file) const = 0;

	virtual Image* loadImage(FileStream* file) const = 0;
		
};


} //end namespace renderer
} //end namespace flt

#endif //FLT_I_IMAGELOADER_H