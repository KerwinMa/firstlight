#ifndef FLT_LITE_STRING_H
#define FLT_LITE_STRING_H

#include "common.h"

namespace flt
{
namespace core
{

template<typename T>
struct Data 
{
	T* ptr;

	size_t getSize() const 
	{
		size_t s = *(reinterpret_cast<const size_t*>(ptr) - 1);
		return s;
	}

	operator T*() { return ptr; }
	operator const T*() const { return ptr; }
};

struct LiteString
	: public Data<char>
{
	bool
		operator ==(const LiteString &str2) const
	{
		return (strcmp(ptr, str2.ptr) == 0);
	}

	bool
		operator ==(const char *str2) const
	{
		return (strcmp(ptr, str2) == 0);
	}
};

} //end namespace core
} //end namespace flt

#endif //FLT_LITE_STRING_H
