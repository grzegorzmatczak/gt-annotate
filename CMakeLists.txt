cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
project(gt-annotate  VERSION 3.0  DESCRIPTION "gt-annotate v3.0 Windows/Linux"  LANGUAGES C CXX)

set(DEFAULT_BUILD_TYPE "Debug")
set(CMAKE_BUILD_TYPE "Debug")
set(DEFAULT_BUILD_TYPE "Release")
set(CMAKE_BUILD_TYPE "Release")
set(TARGET_NAME "gt-annotate")

#STNDARD STUFF
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FLAGS_COMMON}")
if (UNIX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -lX11 -Wall" )
endif (UNIX)

if (WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP /O2 /MD" )
endif (WIN32)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}")
file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/config")
file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/test_images")
file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/platforms")
file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/gtest_files")
file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/gtest_files/test_dataset1")
file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/gtest_files/test_dataset2")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

macro(copy_json jsonname)
	configure_file(${jsonname} ${CMAKE_BINARY_DIR}/${jsonname})
	configure_file(${jsonname} ${CMAKE_BUILD_TYPE}/${jsonname})
	configure_file(${jsonname} ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${jsonname})
endmacro()

copy_json(gtest/test.json)
copy_json(gtest/test.json)
copy_json(gtest_files/test_dataset1.mp4)
copy_json(gtest_files/test_dataset1/test1.json)
copy_json(gtest_files/test_dataset2/test2.json)
copy_json(config/config.json)
copy_json(config/postprocess.json)


file(COPY test_images DESTINATION ${CMAKE_BINARY_DIR})

include_directories( "include" )
include_directories( "Utils/include" )
include_directories( "Dlib/include" )
include_directories( "DataManagement/include" )
include_directories( "PostProcessingModules/include" )
include_directories( "ProcessingModules/include" )
include_directories( "ProcessingGraph/include" )

if (WIN32)
	set(QT5_PATH  "C:\\Qt\\5.15.2\\msvc2019_64")
	list(APPEND CMAKE_PREFIX_PATH ${QT5_PATH})
	list(APPEND CMAKE_PREFIX_PATH ${QT5_PATH}"\\bin\\")
	list(APPEND CMAKE_PREFIX_PATH "C:\\repo\\opencv_build")
endif (WIN32)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(OpenCV REQUIRED)
find_package(Qt5Test REQUIRED)
find_package(Qt5Widgets REQUIRED)
#find_package(Qt5Network REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
find_package(Qt5OpenGL)

enable_testing()

macro(clone_build libname)
	configure_file(clone_build/${libname}.in ${libname}-download/CMakeLists.txt)
	execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/${libname}-download")
	execute_process(COMMAND "${CMAKE_COMMAND}" --build .
	WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/${libname}-download")
	add_subdirectory("${CMAKE_BINARY_DIR}/${libname}-src"
                 "${CMAKE_BINARY_DIR}/${libname}-build")
endmacro()

clone_build(googletest)
# Prevent GoogleTest from overriding our compiler/linker options
# when building with Visual Studio
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

clone_build(spdlog)
clone_build(dlib)

file(GLOB_RECURSE SRC_DLIB Dlib/src/*.cc)
file(GLOB_RECURSE HEADERS_DLIB Dlib/include/*.h)

file(GLOB_RECURSE SRC_UTILS Utils/src/*.cc)
file(GLOB_RECURSE HEADERS_UTILS Utils/include/*.h)

file(GLOB_RECURSE SRC_DATA DataManagement/src/*.cc)
file(GLOB_RECURSE HEADERS_DATA DataManagement/include/*.h)

file(GLOB_RECURSE SRC_PROCESSING ProcessingModules/src/*.cc)
file(GLOB_RECURSE HEADERS_PROCESSING ProcessingModules/include/*.h)

file(GLOB_RECURSE SRC_POST_PROCESSING PostProcessingModules/src/*.cc)
file(GLOB_RECURSE HEADERS_POST_PROCESSING PostProcessingModules/include/*.h)

set(SRC_GRAPH ProcessingGraph/src/graph.cc ProcessingGraph/src/case.cc)
set(HEADERS_GRAPH ProcessingGraph/include/graph.h ProcessingGraph/include/case.h)

set(SRC_MAIN src/main.cc)

set(SRC
  src/mainwindow.cc)

set(HEADERS
  include/mainwindow.h)

file(GLOB_RECURSE SRC_WIDGETS_VIEW src/widgets/view/*.cc)
file(GLOB_RECURSE HEADERS_WIDGETS_VIEW include/widgets/view/*.h)

file(GLOB_RECURSE SRC_WIDGETS_DATA src/widgets/data/*.cc)
file(GLOB_RECURSE HEADERS_WIDGETS_DATA include/widgets/data/*.h)

file(GLOB_RECURSE SRC_IMAGE_PROCESSING src/imageprocessing/*.cc)
file(GLOB_RECURSE HEADERS_IMAGE_PROCESSING include/imageprocessing/*.h)

add_library(${PROJECT_NAME}_lib STATIC
	${SRC} ${HEADERS} 
	${SRC_WIDGETS_VIEW} ${HEADERS_WIDGETS_VIEW}
	${SRC_WIDGETS_DATA} ${HEADERS_WIDGETS_DATA}
	${SRC_IMAGE_PROCESSING} ${HEADERS_IMAGE_PROCESSING})

add_library(utils_lib STATIC
	${SRC_UTILS} ${HEADERS_UTILS})

add_library(dlib_lib STATIC
	${SRC_DLIB} ${HEADERS_DLIB})

add_library(data_lib STATIC
	${SRC_DATA} ${HEADERS_DATA})

add_library(graph_lib STATIC
	${SRC_GRAPH} ${HEADERS_GRAPH})

add_library(processing_lib STATIC
	${SRC_PROCESSING} ${HEADERS_PROCESSING})

add_library(postprocessing_lib STATIC
	${SRC_POST_PROCESSING} ${HEADERS_POST_PROCESSING})

add_executable(${PROJECT_NAME}
    ${SRC_MAIN})


file(GLOB_RECURSE SRC_HEADERS_TEST gtest/*.cc gtest/*.h)
file(GLOB_RECURSE SRC_HEADERS_DATA_TEST DataManagement/gtest/*.cc DataManagement/gtest/*.h)

add_executable(g_test ${SRC_HEADERS_TEST})
add_executable(g_test_data ${SRC_HEADERS_DATA_TEST})

target_link_libraries(utils_lib ${QT_LIBRARIES} spdlog Qt5::Core Threads::Threads)
target_link_libraries(graph_lib ${QT_LIBRARIES} spdlog Qt5::Core ${OpenCV_LIBS} Threads::Threads utils_lib processing_lib postprocessing_lib)
target_link_libraries(data_lib ${QT_LIBRARIES} spdlog Qt5::Core Threads::Threads)
target_link_libraries(processing_lib ${QT_LIBRARIES} spdlog Qt5::Core ${OpenCV_LIBS} Threads::Threads)
target_link_libraries(postprocessing_lib ${QT_LIBRARIES} spdlog Qt5::Core ${OpenCV_LIBS} Threads::Threads)

target_link_libraries(dlib_lib ${QT_LIBRARIES} spdlog Qt5::Core Threads::Threads dlib::dlib processing_lib postprocessing_lib graph_lib)

target_link_libraries(${PROJECT_NAME}_lib ${QT_LIBRARIES} ${OpenCV_LIBS} Threads::Threads utils_lib data_lib dlib_lib)
target_link_libraries(${PROJECT_NAME}_lib Qt5::Core Qt5::Widgets Qt5::OpenGL)

target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_lib)

target_link_libraries(g_test ${PROJECT_NAME}_lib gtest gmock utils_lib)
target_link_libraries(g_test_data ${PROJECT_NAME}_lib gtest gmock utils_lib data_lib)

message("CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE} ")

if (CMAKE_BUILD_TYPE STREQUAL "Debug" AND WIN32)
	set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
	set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Release" AND WIN32)
	set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
	set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
endif()


if (WIN323)
    get_target_property(QT5_QMAKE_EXECUTABLE Qt5::qmake IMPORTED_LOCATION)
    get_filename_component(QT5_WINDEPLOYQT_EXECUTABLE ${QT5_QMAKE_EXECUTABLE} PATH)
    set(QT5_WINDEPLOYQT_EXECUTABLE "${QT5_WINDEPLOYQT_EXECUTABLE}/windeployqt.exe")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
       COMMAND ${QT5_WINDEPLOYQT_EXECUTABLE} --qmldir ${CMAKE_SOURCE_DIR} $<TARGET_FILE_DIR:${PROJECT_NAME}>)
endif(WIN323)

if (WIN32)
	set(OPENCV_PREFIX_BUILD_TYPE "")
	message("Copy OpenCV file:" )
	set(OPENCV_VERSION_WINDOWS "453")
	if(${CMAKE_BUILD_TYPE} STREQUAL "Release")
		message("OPENCV_PREFIX_BUILD_TYPE: Release" )
		set(OPENCV_PREFIX_BUILD_TYPE "")
	endif()
	if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
		message("OPENCV_PREFIX_BUILD_TYPE: Debug" )
		set(OPENCV_PREFIX_BUILD_TYPE "d")
	endif()


	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${QT5_PATH}/bin/Qt5Test${OPENCV_PREFIX_BUILD_TYPE}.dll   ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/
		COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${QT5_PATH}/bin/Qt5Gui${OPENCV_PREFIX_BUILD_TYPE}.dll   ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/
		COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${QT5_PATH}/bin/Qt5Core${OPENCV_PREFIX_BUILD_TYPE}.dll   ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/
		COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${QT5_PATH}/bin/Qt5Widgets${OPENCV_PREFIX_BUILD_TYPE}.dll   ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/
		
		COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${QT5_PATH}/plugins/platforms/qwindows${OPENCV_PREFIX_BUILD_TYPE}.dll   ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/platforms/
		

		COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_stitching${OPENCV_VERSION_WINDOWS}${OPENCV_PREFIX_BUILD_TYPE}.dll   ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/
		COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_bgsegm${OPENCV_VERSION_WINDOWS}${OPENCV_PREFIX_BUILD_TYPE}.dll   ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/
		COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_tracking${OPENCV_VERSION_WINDOWS}${OPENCV_PREFIX_BUILD_TYPE}.dll   ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/
		COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_videoio${OPENCV_VERSION_WINDOWS}${OPENCV_PREFIX_BUILD_TYPE}.dll   ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/
		COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_video${OPENCV_VERSION_WINDOWS}${OPENCV_PREFIX_BUILD_TYPE}.dll   ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/
		COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_imgcodecs${OPENCV_VERSION_WINDOWS}${OPENCV_PREFIX_BUILD_TYPE}.dll   ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/
		COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_imgproc${OPENCV_VERSION_WINDOWS}${OPENCV_PREFIX_BUILD_TYPE}.dll   ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/
		COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_core${OPENCV_VERSION_WINDOWS}${OPENCV_PREFIX_BUILD_TYPE}.dll   ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/
		COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_calib3d${OPENCV_VERSION_WINDOWS}${OPENCV_PREFIX_BUILD_TYPE}.dll   ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/
		COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_features2d${OPENCV_VERSION_WINDOWS}${OPENCV_PREFIX_BUILD_TYPE}.dll   ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/
		COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_flann${OPENCV_VERSION_WINDOWS}${OPENCV_PREFIX_BUILD_TYPE}.dll   ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/
		COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_dnn${OPENCV_VERSION_WINDOWS}${OPENCV_PREFIX_BUILD_TYPE}.dll   ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/
		COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_highgui${OPENCV_VERSION_WINDOWS}${OPENCV_PREFIX_BUILD_TYPE}.dll   ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/
		COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_highgui${OPENCV_VERSION_WINDOWS}${OPENCV_PREFIX_BUILD_TYPE}.dll   ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/
		COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_gapi${OPENCV_VERSION_WINDOWS}${OPENCV_PREFIX_BUILD_TYPE}.dll   ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/

		COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_videoio_ffmpeg${OPENCV_VERSION_WINDOWS}_64.dll   ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/
		COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_cudev${OPENCV_VERSION_WINDOWS}${OPENCV_PREFIX_BUILD_TYPE}.dll   ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/
		COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_structured_light${OPENCV_VERSION_WINDOWS}${OPENCV_PREFIX_BUILD_TYPE}.dll   ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/
			
	)
endif(WIN32)