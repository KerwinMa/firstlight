#ifndef TEST_FLT_RES_MGR_H
#define TEST_FLT_RES_MGR_H

#include "DummyResMgr.h"
#include <gtest/gtest.h> 


namespace flt_test
{
	using namespace flt;

	class ResMgrTest : public testing::Test
	{
	protected:   
		ResMgrTest() {   
			// You can do set-up work for each test here.   
		}   
		virtual ~ResMgrTest() {   
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

	TEST_F(ResMgrTest, MainTest) 
	{
		DummyResMgr::newInstance();
		DummyResMgr& mgr = DummyResMgr::getInstance();

		//ger res, not found, so create it in getRes
		DummyPtr dummy = mgr.getRes("test");
		EXPECT_EQ(dummy->getRefCount(),2);
		EXPECT_EQ(mgr.getResCount(),1);
		EXPECT_EQ(dummy->getName(),"test");
		EXPECT_EQ(dummy->idata,123);
		EXPECT_EQ(mgr.getResCount(),1);
		

		//clone res, modify and put into mgr as a new res
		DummyPtr cloned = mgr.cloneRes(dummy,"cloned");
		cloned->idata = 456;
		mgr.addRes(cloned);
		EXPECT_EQ(cloned->getRefCount(),2);
		EXPECT_EQ(mgr.getResCount(),2);
		EXPECT_EQ(cloned->getName(),"cloned");
		EXPECT_EQ(cloned->idata,456);
		EXPECT_EQ(mgr.getResCount(),2);//added in clone

		//cloned user drop it, but mgr hold it
		cloned.reset();
		EXPECT_EQ(mgr.getResCount(),2);

		//test if used "cloned,456"
		EXPECT_EQ(mgr.isResUsed("cloned"),false);
		//mgr clear unused res - the "cloned,456"
		mgr.removeAllUnusedRes();
		EXPECT_EQ(mgr.getResCount(),1);

		//get res, share
		DummyPtr dummy2 = mgr.getRes("test");
		EXPECT_TRUE(dummy2==dummy);
		EXPECT_EQ(dummy2->getRefCount(),3);
		EXPECT_EQ(mgr.getResCount(),1);
		
		//remove test, all user's ref count -1, and mgr now has 0 res
		mgr.removeRes("test");
		EXPECT_EQ(dummy->getRefCount(),2);
		EXPECT_EQ(dummy2->getRefCount(),2);
		EXPECT_EQ(mgr.getResCount(),0);

		DummyResMgr::deleteInstance();				
	}   

}

#endif //TEST_FLT_RES_MGR_H