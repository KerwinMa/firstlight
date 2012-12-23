#ifndef FLT_SHARE_PTR_H
#define FLT_SHARE_PTR_H

#include "common.h"

namespace flt
{

//! Shared smart pointer for BaseObject and its sub classes.
template<typename T>
class SharePtr
{
public:
	
	// construction and destruction
	SharePtr();	
	SharePtr(T* p);	
	SharePtr(const SharePtr<T>& ptr);	
	~SharePtr();

	// assignment
	void operator=(T* p);
	void operator=(const SharePtr<T>& ptr);
	
	// comparisons
	bool operator==(const T* p) const;
	bool operator!=(const T* p) const;

	bool operator==(const SharePtr<T>& ptr) const;
	bool operator!=(const SharePtr<T>& ptr) const;		
	
	// safe -> operator
	T* operator->() const;
	// safe dereference operator
	T& operator*() const;
	// safe pointer cast operator
	operator T*() const;
	// return raw pointer 
	T* get() const;
	// check if pointer is valid
	bool isValid() const;
	// release raw pointer
	void reset();
	// get raw pointer to sub class, so can use = to get a sub class share ptr
	template<typename SubClassType> SubClassType* toSubClass();	
	
private:
	T* m_p;
};

/////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////
// construction and destruction

template<typename T>
SharePtr<T>::SharePtr() : m_p(0)
{
}


template<typename T>
SharePtr<T>::SharePtr(T* p) : m_p(p)
{
	if (m_p!=0)
	{
		m_p->grab();
	}
}

template<typename T>
SharePtr<T>::SharePtr(const SharePtr<T>& p) : m_p(p.m_p)
{
	if (m_p!=0)
	{
		m_p->grab();
	}
}


template<typename T>
SharePtr<T>::~SharePtr()
{
	if (m_p!=0)
	{
		m_p->drop();
		m_p=0;
	}
}

/////////////////////////////////////////////////////////////////////////////////
// assignment

template<typename T>
void SharePtr<T>::operator=(T* p)
{
	if (m_p != p)
	{
		if(p!=0)
		{
			p->grab();
		}

		if(m_p!=0)
		{
			m_p->drop();
		}

		m_p = p;		
	}
}

template<typename T>
void SharePtr<T>::operator=(const SharePtr<T>& ptr)
{
	if (m_p != ptr.m_p)
	{
		if(ptr.m_p!=0)
		{
			ptr.m_p->grab();
		}

		if(m_p!=0)
		{
			m_p->drop();
		}

		m_p = ptr.m_p;		
	}
}


/////////////////////////////////////////////////////////////////////////////////
// comparisons

template<typename T>
bool SharePtr<T>::operator==(const T* p) const
{
	return (m_p == p);
}

template<typename T>
bool SharePtr<T>::operator!=(const T* p) const
{
	return (m_p != p);
}

template<typename T>
bool SharePtr<T>::operator==(const SharePtr<T>& ptr) const
{
	return (m_p == ptr.m_p);
}

template<typename T>
bool SharePtr<T>::operator!=(const SharePtr<T>& ptr) const
{
	return (m_p != ptr.m_p);
}


/////////////////////////////////////////////////////////////////////////////////

template<typename T>
T* SharePtr<T>::operator->() const
{
	FLT_ASSERT(m_p!=0 && "NULL pointer access in SharePtr::operator->()!");
	return m_p;
}

template<typename T>
T& SharePtr<T>::operator*() const
{
	FLT_ASSERT(m_p!=0 && "NULL pointer access in SharePtr::operator*()!");
	return *m_p;
}

template<typename T>
SharePtr<T>::operator T*() const
{
	FLT_ASSERT(m_p!=0 && "NULL pointer access in SharePtr::operator T*()!");
	return m_p;
}

template<typename T>
T* SharePtr<T>::get() const
{
	FLT_ASSERT(m_p!=0 && "NULL pointer access in SharePtr::get()!");
	return m_p;
}

template<typename T>
bool SharePtr<T>::isValid() const
{
	return (m_p!=0);
}

template<typename T>
void SharePtr<T>::reset()
{
	if(m_p!=0)
	{
		m_p->drop();
	}

	m_p = 0;
}

template<typename T>
template<typename SubClassType> 
SubClassType* SharePtr<T>::toSubClass()
{		
	return static_cast<SubClassType*>(m_p);
};

/////////////////////////////////////////////////////////////////////////////////
	
} //end namespace flt

#endif //FLT_SHARE_PTR_H