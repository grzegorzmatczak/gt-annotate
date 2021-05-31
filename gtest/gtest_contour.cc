#include "gtest_contour.h"

constexpr auto TEST1{ "test_images/test1.png" };

constexpr auto TEST1_0_CANNY{ "test_images/TEST1/0_CrateRois.png" };
constexpr auto TEST1_0_CANNY_OUTPUT{ "test_images/TEST1/0_canny_output.png" };
constexpr auto TEST1_1_CANNY{ "test_images/TEST1/1_CrateRois.png" };
constexpr auto TEST1_1_CANNY_OUTPUT{ "test_images/TEST1/1_canny_output.png" };
constexpr auto TEST1_2_CANNY{ "test_images/TEST1/2_CrateRois.png" };
constexpr auto TEST1_2_CANNY_OUTPUT{ "test_images/TEST1/2_canny_output.png" };
constexpr auto TEST1_3_CANNY{ "test_images/TEST1/3_CrateRois.png" };
constexpr auto TEST1_3_CANNY_OUTPUT{ "test_images/TEST1/3_canny_output.png" };

constexpr auto TEST_DATA{ "TestData" };
constexpr auto CONTOUR{ "Contour" };

constexpr auto NAME{ "Name" };
constexpr auto WIDTH{ "Width" };
constexpr auto HEIGHT{ "Height" };
constexpr auto X{ "x" };
constexpr auto Y{ "y" };

#include "utils/configreader.h"
#include "utils/includespdlog.h"
#include "imageprocessing/contour.h"

using ::testing::AtLeast;


namespace gtest_contour {

	TEST_F(GTest_contour, test_CreateCannyImage1)
	{
		
		testCannyImage(TEST1_0_CANNY, TEST1_0_CANNY_OUTPUT);
		testCannyImage(TEST1_1_CANNY, TEST1_1_CANNY_OUTPUT);
		testCannyImage(TEST1_2_CANNY, TEST1_2_CANNY_OUTPUT);
		testCannyImage(TEST1_3_CANNY, TEST1_3_CANNY_OUTPUT);
	}

	TEST_F(GTest_contour, test_FindContours0)
	{
		QString name = "0";
		QJsonObject obj{{NAME,name}, {HEIGHT,10}, {WIDTH,40}, {X,229}, {Y,29}};
		QJsonArray jarray_ref{};
		jarray_ref.append(obj);
		
		testFindContours(TEST1_0_CANNY_OUTPUT, jarray_ref, name);
	}

	TEST_F(GTest_contour, test_FindContours1)
	{
		QJsonArray jarray_ref{};
		QString name = "0";
		jarray_ref.append(QJsonObject({{NAME,name}, {HEIGHT,20}, {WIDTH,21}, {X,135}, {Y,105}}));
		jarray_ref.append(QJsonObject({{NAME,name}, {HEIGHT,20}, {WIDTH,18}, {X,137}, {Y,33}}));
		jarray_ref.append(QJsonObject({{NAME,name}, {HEIGHT,18}, {WIDTH,21}, {X,43}, {Y,32}}));
		
		testFindContours(TEST1_1_CANNY_OUTPUT, jarray_ref, name);
	}

	TEST_F(GTest_contour, test_FindContours2)
	{
		QString name = "0";
		QJsonArray jarray_ref{};
		jarray_ref.append(QJsonObject({{NAME,name}, {HEIGHT,19}, {WIDTH,21}, {X,12}, {Y,79}}));
		
		testFindContours(TEST1_2_CANNY_OUTPUT, jarray_ref, name);
	}

	TEST_F(GTest_contour, test_FindContours3)
	{
		QString name = "0";
		QJsonArray jarray_ref{};
		jarray_ref.append(QJsonObject({{NAME,name}, {HEIGHT,9}, {WIDTH,10}, {X,41}, {Y,118}}));
		
		testFindContours(TEST1_3_CANNY_OUTPUT, jarray_ref, name);
	}

	TEST_F(GTest_contour, test_CrateRois0)
	{
		QString name = "0";
		QJsonArray jarray_ref{};
		jarray_ref.append(QJsonObject({{NAME,name}, {HEIGHT,10}, {WIDTH,40}, {X,229}, {Y,29}}));

		testCrateRois(TEST1_0_CANNY, jarray_ref);

	}

	void GTest_contour::testCrateRois(std::string str, QJsonArray array)
	{
		Logger->info("testCrateRois({}, QJsonArray array)", str.c_str());
		Contour _contour;
		cv::Mat test = cv::imread(str, cv::IMREAD_GRAYSCALE);
		QJsonArray jarray{};
		QString i = "0";
		_contour.CrateRois(test, i, jarray);
		EXPECT_EQ (jarray, array);
	}

	void GTest_contour::testFindContours(std::string str, QJsonArray array, QString name)
	{
		
		Contour _contour;
		Logger->info("testFindContours({}, QJsonArray array)", str.c_str());
		cv::Mat testImage = cv::imread(str, cv::IMREAD_GRAYSCALE);
		QJsonArray jarray{};
		_contour.FindContours(testImage, jarray, name);
		EXPECT_EQ (jarray, array);
		
	}

	void GTest_contour::testCannyImage(std::string str1, std::string str2)
	{
		Contour _contour;
		Logger->info("testCannyImage({}, {})", str1.c_str(), str2.c_str());
		cv::Mat test1_canny_ref = cv::imread(str1, cv::IMREAD_GRAYSCALE);
		cv::Mat test1_canny_output_ref = cv::imread(str2, cv::IMREAD_GRAYSCALE);
		cv::Mat test1_canny_output = test1_canny_ref.clone();

		_contour.CreateCannyImage(test1_canny_ref, test1_canny_output);

		for (int i = 0; i < test1_canny_ref.cols; i++)
		{
			for (int j = 0; j < test1_canny_ref.rows; j++)
			{
				EXPECT_EQ (test1_canny_output_ref.at<unsigned char>(j,i) , test1_canny_output.at<unsigned char>(j,i));
			}
		}
	}
}  // namespace gtest_configreader
