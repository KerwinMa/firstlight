#ifndef FLT_LITE_ARRAY_H
#define FLT_LITE_ARRAY_H

#include "common.h"

namespace flt
{
namespace core
{

template<typename T>
class LiteArray 
{
public:

	inline const T& operator[](s32 idx) const 
	{
		FLT_DEBUG_BREAK_IF(idx < 0 || idx >= m_size);
		return m_ptr[idx];
	}

	inline T& operator[](s32 idx) 
	{
		FLT_DEBUG_BREAK_IF(idx < 0 || idx >= m_size);
		return m_ptr[idx];
	}

	inline const T* operator->() const 
	{
		return m_ptr;
	}

	inline T* operator->() 
	{
		return m_ptr;
	}

	inline const T* ptr() const 
	{
		return m_ptr;
	}

	inline T* ptr() 
	{
		return m_ptr;
	}

	inline const T& ref() const 
	{
		return *ptr(); 
	}

	inline T& ref() 
	{
		return *ptr(); 
	}

	inline bool isValid() const 
	{
		return m_ptr != 0;
	}

	inline bool operator==(const LiteArray& a) const 
	{ 
		return ptr() == a.ptr(); 
	}

	inline bool operator!=(const LiteArray& a) const 
	{ 
		return ptr() != a.ptr();
	}  

	inline s32 size() const { return m_size; }

private:
	
	s32 m_size;
	
	union 
	{
		int m_offset;
		T* m_ptr;
	};
};

} //end namespace core
} //end namespace flt

#endif //FLT_LITE_ARRAY_H
