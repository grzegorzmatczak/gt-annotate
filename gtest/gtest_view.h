#include "gtest/gtest.h"
#include "gmock/gmock.h" 

#include <QDebug>

#include "includespdlog.h"
#include "configreader.h"
#include "widgets/view.h"
#include "mainwindow.h"

namespace gtest_view
{
    class GTest_view : public ::testing::Test {
    protected:
    GTest_view(){}
    ~GTest_view() override {}
    void SetUp() override{}
    void TearDown() override {}
    };
}  // namespace gtest_view
