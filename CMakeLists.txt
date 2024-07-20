cmake_minimum_required(VERSION 3.10)

project(PeakDetector LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

# Измените эту строку
set(GTEST_ROOT "/usr/src/gtest")
find_package(GTest REQUIRED)
find_package(Qt5Core REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Библиотека PeakDetector
add_library(peak_detector STATIC
    peak_detector.h
    peak_detector.cpp
)
target_link_libraries(peak_detector Qt5::Core)

# Основной исполняемый файл
add_executable(main main.cpp)
target_link_libraries(main peak_detector Qt5::Core)

enable_testing()
# Тестовый исполняемый файл
add_executable(peak_detector_test test_main.cpp)
target_link_libraries(peak_detector_test
    PRIVATE
    peak_detector
    ${GTEST_LIBRARIES}
    pthread
)


add_test(NAME PeakDetectorTests COMMAND peak_detector_test)



