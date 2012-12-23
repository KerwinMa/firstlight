#ifndef FLT_COMMON_H
#define FLT_COMMON_H

#include "config.h"

//sys includes
#include <string>
#include <vector>
#include <list>
#include <map>
#include <cassert>

namespace flt
{
//basic types
#if defined (FLT_DEVICE_WIN32)

	typedef char				c8;
	typedef unsigned char		u8;
	typedef signed char			s8;
	typedef unsigned short		u16;
	typedef signed short		s16;
	typedef unsigned int		u32;
	typedef signed int			s32;
	typedef unsigned __int64	u64;
	typedef signed __int64		s64;
	typedef float				f32;
	typedef double				f64;	

#elif defined (FLT_DEVICE_IOS)

	typedef char				c8;
	typedef unsigned char		u8;
	typedef signed char			s8;
	typedef unsigned short		u16;
	typedef signed short		s16;
	typedef unsigned int		u32;
	typedef signed int			s32;
	typedef unsigned long long	u64;
	typedef signed long long	s64;
	typedef float				f32;
	typedef double				f64;

#endif

//string types
typedef std::string stringc;
typedef std::wstring stringw;

//container types
#define array_t		std::vector
#define list_t		std::list
#define map_t		std::map


//orientation enum, used by device and renderer
enum EDeviceOrientation 
{
	EDO_Unknown,
	EDO_Portrait,
	EDO_PortraitUpsideDown,
	EDO_LandscapeLeft,
	EDO_LandscapeRight,
	EDO_FaceUp,
	EDO_FaceDown,
};

} //end namespace flt

#define ForEach(_type, _container, _it) for (_type::iterator _it = _container.begin(), _end = _container.end(); _it != _end; ++_it)
#define ForEachC(_type, _container, _it) for (_type::const_iterator _it = _container.begin(), _end = _container.end(); _it != _end; ++_it)

//debug mode define
#if defined (FLT_DEVICE_WIN32) && defined(_DEBUG)
	#define FLT_DEBUG
#endif

#ifdef FLT_DEBUG
	#define FLT_DEBUG_BREAK_IF(condition) assert(!(condition))
	#define FLT_ASSERT(condition) assert(condition)
#else
	#define FLT_DEBUG_BREAK_IF(condition)
	#define FLT_ASSERT(condition)
#endif

#define SAFE_DEL(p)					do{ if (p) { delete (p); p = NULL; } }while(false)
#define SAFE_DEL_ARRAY(p)			do{ if (p) { delete[] (p); p = NULL; } }while(false)


/// Use this define to forward declare a class and its smart pointer.
#define FLT_FORWARD_PTR(className)                        \
	class className;                                      \
	typedef SharePtr< className > className##Ptr;              \
	typedef SharePtr< const className > className##ConstPtr;

#if defined (FLT_DEVICE_WIN32)
	#define DBG(group, fmt, ...) flt::utils::Logger::getInstance().debugOut(group, fmt, __VA_ARGS__);
	#define DBGL(group, fmt, ...) flt::utils::Logger::getInstance().debugOutCodeLine(__FILE__, __LINE__, group, fmt, __VA_ARGS__);
	#define LOG(group, fmt, ...) flt::utils::Logger::getInstance().logOut(group, fmt, __VA_ARGS__);
	#define LOGL(group, fmt, ...) flt::utils::Logger::getInstance().logOutCodeLine(__FILE__, __LINE__, group, fmt, __VA_ARGS__);
#else
	#define DBG(group, fmt, args...) flt::utils::Logger::getInstance().debugOut(group, fmt, ##args);
	#define DBGL(group, fmt, args...) flt::utils::Logger::getInstance().debugOutCodeLine(__FILE__, __LINE__, group, fmt, ##args);
	#define LOG(group, fmt, args...) flt::utils::Logger::getInstance().logOut(group, fmt, ##args);
	#define LOGL(group, fmt, args...) flt::utils::Logger::getInstance().logOutCodeLine(__FILE__, __LINE__, group, fmt, ##args);
#endif


#endif //FLT_COMMON_H
