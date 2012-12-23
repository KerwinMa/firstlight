#ifndef FLT_IEVENT_H
#define FLT_IEVENT_H

#include "common.h"

namespace flt
{

class IEvent
{
public:
	typedef u32 UID_TYPE;

	IEvent(UID_TYPE uid):m_uid(uid)
	{
	}

	virtual ~IEvent()
	{
	}

	virtual UID_TYPE getUID() const 
	{
		return m_uid;
	}

	virtual int getSize() const = 0;

protected:
	UID_TYPE m_uid; //unique id

};


}//end namespace flt

#endif //FLT_IEVENT_H