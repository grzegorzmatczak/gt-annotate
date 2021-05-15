#include "gtest_view.h"
#include <QDebug>

constexpr auto CONFIG{ "test.json" };
constexpr auto BLACK{ "Black" };
constexpr auto WHITE{ "White" };
constexpr auto G{ "G" };


using ::testing::AtLeast;


namespace gtest_view
{
    TEST_F(GTest_view, test_paintersettings_configure)
    {
        QHash<QString, QColor> m_colorHash;
        QHash<QString, int> m_colorInthash;
        std::shared_ptr<ConfigReader> _configreader = std::make_shared<ConfigReader>();
        QJsonObject a_config{};
        _configreader->readConfig(CONFIG, a_config);
        std::shared_ptr<PainterSettings> _paintersettings = std::make_shared<PainterSettings>();
        _paintersettings->configureColors(a_config);
        EXPECT_EQ (_paintersettings->m_colors[0], BLACK);
        EXPECT_EQ (_paintersettings->m_colorInthash[WHITE], 255);
    }

    TEST_F(GTest_view, test2)
    {

    }

}  // namespace gtest_view
