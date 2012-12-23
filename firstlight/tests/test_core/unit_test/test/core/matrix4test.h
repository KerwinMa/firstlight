#ifndef TEST_FLT_MATRIX_H
#define TEST_FLT_MATRIX_H

#include "common.h"
#include "core/matrix4.h"
#include <gtest/gtest.h> 

namespace flt_test
{
	using namespace flt;

	class CMatrix4Test : public testing::Test
	{
	protected:   
		CMatrix4Test() {   
			// You can do set-up work for each test here.   
		}   
		virtual ~CMatrix4Test() {   
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

		core::CMatrix4<f32> matrix;
	};

	TEST_F(CMatrix4Test, IsIdentity) {   
		EXPECT_EQ(true, matrix.isIdentity());
	}   

}
#endif