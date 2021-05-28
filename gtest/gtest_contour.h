#include "gtest/gtest.h"
#include "gmock/gmock.h" 

#include <QDebug>




using ::testing::AtLeast;

namespace gtest_contour 
{
	class GTest_contour : public ::testing::Test 
	{
		public:
			void testCannyImage(std::string str1, std::string str2);
			void testFindContours(std::string str, QJsonArray array);
			void testCrateRois(std::string str, QJsonArray array);
		protected:
			GTest_contour(){}
			~GTest_contour() override {}
			void SetUp() override{}
			void TearDown() override {}
	};
}  // namespace gtest_contour
