#include "gtest_configreader.h"

constexpr auto CONFIG{ "test.json" };

using ::testing::AtLeast;

namespace gtest_configreader {

    TEST_F(GTest_configreader, test_readConfig)
    {
        QJsonObject _objReference{{"General",QJsonObject{{"LogLevel",1}}}};

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
