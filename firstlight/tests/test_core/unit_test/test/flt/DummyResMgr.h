#ifndef DUMMY_RES_MGR_H
#define DUMMY_RES_MGR_H

#include "firstlight.h"

using namespace flt;

namespace flt_test
{


FLT_FORWARD_PTR(Dummy)

class Dummy: public BaseObject
{
public:
	explicit Dummy(int i):idata(i){}

	Dummy(const Dummy& rhs)
	{
		idata = rhs.idata;
	}

	int idata;
};


class DummyResMgr: public ResourceManager<DummyPtr>, public utils::ISingleton<DummyResMgr>
{
public:
	DummyResMgr(){};
	virtual ~DummyResMgr(){};

	virtual DummyPtr createRes(const stringc& name, int flag);

	virtual DummyPtr cloneRes(const DummyPtr& rhs, const stringc& newResName);

};

} //namespace flt_test

#endif //DUMMY_RES_MGR_H