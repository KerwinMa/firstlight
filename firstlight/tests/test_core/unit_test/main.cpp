#include <iostream>

#include <gtest/gtest.h>
#include "test/testcase.h"

#ifdef FLT_DEBUG
#include "vld.h"
#endif

FLT_IMPLEMENT_ENGINE

int main(int argc, char **argv)
{
	std::cout << "FLT Engine unit tests\n";

	testing::InitGoogleTest(&argc, argv);	

	int ret = RUN_ALL_TESTS();

	getchar();

	return ret;
}