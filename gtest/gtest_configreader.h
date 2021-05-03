#include "gtest/gtest.h"
#include "gmock/gmock.h" 

#include <QDebug>

#include "../include/configreader.h"

constexpr auto CONFIG{ "config.json" };

using ::testing::AtLeast;

namespace gtest_configreader {
		class GTest_configreader : public ::testing::Test {
		protected:
			GTest_configreader(){}
			~GTest_configreader() override {}
			void SetUp() override{}
			void TearDown() override {}
		};

		TEST_F(GTest_configreader, test_readConfig)
		{
			QJsonObject _objReference{{"General",QJsonObject{{"LogLevel",0}}}};

			ConfigReader * _configreader = new ConfigReader();
			QJsonObject obj{};
			_configreader->readConfig(CONFIG, obj);
			qDebug() << "obj:" << obj;
			qDebug() << "_objReference:" << _objReference;
			EXPECT_EQ (obj, _objReference);
		}
		
		TEST_F(GTest_configreader, test2)
		{
			
		}
}  // namespace gtest_configreader