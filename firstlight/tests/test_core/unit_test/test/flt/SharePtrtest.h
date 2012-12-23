#ifndef TEST_FLT_SHARE_PTR_H
#define TEST_FLT_SHARE_PTR_H

#include "common.h"
#include "BaseObject.h"
#include "sharePtr.h"
#include <gtest/gtest.h> 

namespace flt
{
	FLT_FORWARD_PTR(BaseObject)
}

namespace flt_test
{
	using namespace flt;

	FLT_FORWARD_PTR(TestObj)

	class TestObj: public BaseObject
	{
	public:

		void test()
		{
			std::cout<<"TestObj::test(),name="<<m_name<<std::endl;
		}
	};
		
	class SharePtrTest : public testing::Test
	{
	protected:   
		SharePtrTest() {   
			// You can do set-up work for each test here.   
		}   
		virtual ~SharePtrTest() {   
			// You can do clean-up work that doesn't throw exceptions here.   
		}   
  
		virtual void SetUp() {   
			// Code here will be called immediately after the constructor (right   
			// before each test).   
		}   
		virtual void TearDown() {   
			// Code here will be called immediately after each test (right   
			// before the destructor).   
		}   

		
	};

	TEST_F(SharePtrTest, BasicTest) 
	{
		TestObjPtr objPtr = new TestObj;
		BaseObjectPtr ptr = objPtr;

		BaseObjectPtr ptr2 = ptr;
		ptr2->setName("obj1");
		ptr2.reset();

		TestObjPtr objPtr2 = ptr.toSubClass<TestObj>();
		objPtr2->test();
		objPtr2 = 0;

		TestObj* tp = objPtr;
		tp->test();

		EXPECT_EQ(true,objPtr.get()==tp);
		EXPECT_EQ(true,(objPtr==ptr && objPtr!=objPtr2 && ptr2!=ptr));

		EXPECT_EQ(2,ptr->getRefCount());
		EXPECT_EQ("obj1",ptr->getName());

		//std::cout<<ptr->getRefCount()<<" : "<<ptr->getName()<<std::endl;

		BaseObjectConstPtr bcp = ptr;
		bcp->getName();
		//bcp->setName("obj2"); //should compile error

		BaseObjectPtr pp = const_cast<flt::BaseObject*>(bcp.get());
		pp->setName("oo");
		
		//std::cout<<ptr->getRefCount()<<" : "<<ptr->getName()<<std::endl;

		EXPECT_EQ(4, ptr->getRefCount());
		EXPECT_EQ("oo", ptr->getName());
	}   

}
#endif