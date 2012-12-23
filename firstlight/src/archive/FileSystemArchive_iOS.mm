/*
 *  FileSystemArchive_iOS.mm
 *  firstlight
 *
 *  Created by liang.wu on 10-12-15.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#if defined(FLT_DEVICE_IOS)

#include <CoreFoundation/CoreFoundation.h>
#include <Foundation/Foundation.h>
#include "firstlight.h"

namespace flt
{
	
	stringc FileSystemArchive::getRootPath()
	{
		char path[PATH_MAX];
		CFBundleRef mainBundle = CFBundleGetMainBundle();
		FLT_ASSERT(mainBundle);
		
		CFURLRef mainBundleURL = CFBundleCopyBundleURL(mainBundle);
		FLT_ASSERT(mainBundleURL);
		
		CFStringRef cfStringRef = CFURLCopyFileSystemPath( mainBundleURL, kCFURLPOSIXPathStyle);
		FLT_ASSERT(cfStringRef);
		
		CFStringGetCString(cfStringRef, path, PATH_MAX, kCFStringEncodingASCII);
		
		CFRelease(mainBundleURL);
		CFRelease(cfStringRef);
		
		return stringc(path) + '/';
	}

}

#endif