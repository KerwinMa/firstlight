#ifndef FLT_CONFIG_H
#define FLT_CONFIG_H

/////////////////////////////////////////////////////////////////////
//device defines
// FLT_DEVICE_WIN32
// FLT_DEVICE_IOS
// FLT_DEVICE_PSP

#ifdef WIN32

#define FLT_DEVICE_WIN32

#pragma warning (disable : 4996)  //_CRT_SECURE_NO_WARNINGS

#elif defined(TARGET_OS_IPHONE) || defined(TARGET_IPHONE_SIMULATOR) //Note:these are defined in a header file, not by compile, so when used in a project not include header file, you should define FLT_DEVICE_IOS in build setting.

#define FLT_DEVICE_IOS

#endif

/////////////////////////////////////////////////////////////////////
//gfx API support
// FLT_COMPILE_WITH_OPENGLES1
// FLT_COMPILE_WITH_OPENGLES2
// FLT_COMPILE_WITH_OPENGL

#if defined(FLT_DEVICE_WIN32)

#define FLT_COMPILE_WITH_OPENGLES1

#elif defined(FLT_DEVICE_IOS)

#define FLT_COMPILE_WITH_OPENGLES1

#endif

///////////////////////////////////////////////////////////////////////

//native texture support
//#if defined(FLT_DEVICE_WIN32)
//
//#define FLT_COMPILE_WITH_PVRT_TEXTURE
//
//#endif


#define FLT_COMPILE_WITH_LIBPNG_

#define FLT_TEXTURE_ENABLE_THREAD_LOAD 0


#endif //FLT_CONFIG_H