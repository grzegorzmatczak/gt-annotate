#include "gtest_view.h"
#include <QDebug>

constexpr auto CONFIG{ "test.json" };

using ::testing::AtLeast;


namespace gtest_view
{
    TEST_F(GTest_view, test_view_configure)
    {
        QApplication app();

        QHash<QString, QColor> m_colorHash;
        QHash<QString, int> m_colorInthash;

        //ConfigReader * _configreader = new ConfigReader();
        std::shared_ptr<ConfigReader> _configreader = std::make_shared<ConfigReader>();
        QJsonObject a_config{};
        _configreader->readConfig(CONFIG, a_config);
        app.exec()
        //MainWindow window{ config };
        //window.show();
        app.exec();

        //View* m_view = new View(a_config);
        std::shared_ptr<View> _view = std::make_shared<View>(a_config, nullptr);

        //EXPECT_EQ (obj, _objReference);
    }

    TEST_F(GTest_view, test2)
    {

    }

}  // namespace gtest_view
