#include <iostream>
#include <sstream>
#include <string.h>
#include <gtest/gtest.h>

using namespace std;

namespace {
	class BasicTest : public ::testing::Test {
		virtual void SetUp() {
		}

		virtual void TearDown() {
		}
	};

	TEST_F(BasicTest, 01_put_get) {
		int a = 1;
		ASSERT_EQ(1, a);
	}
}
int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
