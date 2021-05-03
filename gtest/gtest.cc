#include "gtest/gtest.h"
#include "gmock/gmock.h" 
#include "gtest_configreader.h"

using ::testing::AtLeast;


namespace gtest {
		class TestAll : public ::testing::Test {
		protected:
			TestAll(){}

			~TestAll() override {}

			void SetUp() override{}

			void TearDown() override 
			{
			}
		};

		TEST_F(TestAll, test1)
		{
			
		}
		
		TEST_F(TestAll, test2)
		{
			
		}
}  // namespace gtest

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}