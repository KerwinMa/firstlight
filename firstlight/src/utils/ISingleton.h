#ifndef FLT_I_SINGLETON_H
#define FLT_I_SINGLETON_H

#include "common.h"

namespace flt
{
namespace utils
{

template <typename T> class ISingleton
{	
public:		
	virtual ~ISingleton()
	{  
		FLT_ASSERT( ms_singleton!=0 );  
		ms_singleton = 0;  
	}

	static void newInstance()
	{
		new T();
	}

	static void deleteInstance()
	{
		delete ms_singleton;
	}

	static T& getInstance()
	{  
		FLT_ASSERT( ms_singleton!=0 );  
		return ( *ms_singleton );  
	}

	static T* getInstancePtr()
	{  
		FLT_ASSERT( ms_singleton!=0 );
		return ( ms_singleton );  
	}

protected:
	ISingleton()
	{
		FLT_ASSERT(ms_singleton==0);
#if defined( _MSC_VER ) && _MSC_VER < 1200	 		
		u64 offset = (u64)(T*)1 - (u64)(ISingleton <T>*)(T*)1;	//using u64 to avoid warning 4311,lyo
		ms_singleton = (T*)((u64)this + offset);
#else
		ms_singleton = static_cast< T* >( this );
#endif		
	}

private:
	static T* ms_singleton;
};

template <typename T> T* ISingleton <T>::ms_singleton = 0;

} //end namespace utils
} //end namespace flt

#endif //FLT_I_SINGLETON_H
