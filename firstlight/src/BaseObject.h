#ifndef FLT_BASE_OBJECT_H
#define FLT_BASE_OBJECT_H

#include "common.h"
#include "sharePtr.h"

namespace flt
{

//! Base class of most objects of the FirstLight Engine.

FLT_FORWARD_PTR(BaseObject)

class BaseObject
{
public:
	BaseObject();
	virtual ~BaseObject();

	void grab() const;

	bool drop() const;

	s32 getRefCount() const;

	void setName(const stringc& name);
	
	const stringc& getName() const;

protected:
	virtual void onDelete();
	
	stringc m_name;	

private:
	mutable s32 m_refCount;	
};

} //end namespace flt

#endif //FLT_BASE_OBJECT_H