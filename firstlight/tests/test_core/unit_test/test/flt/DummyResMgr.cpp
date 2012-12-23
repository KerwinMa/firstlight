#include "DummyResMgr.h"

namespace flt_test
{

DummyPtr DummyResMgr::createRes(const stringc& name, int flag)
{
	DummyPtr dummy = new Dummy(123);
	dummy->setName(name);
	return dummy;
}

DummyPtr DummyResMgr::cloneRes(const DummyPtr& rhs, const stringc& newResName)
{
	DummyPtr dummyNew = new Dummy(*rhs.get());
	dummyNew->setName(newResName);	
	return dummyNew;
}

} //namespace flt_test