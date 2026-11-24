cmake_minimum_required(VERSION 3.16)
project(DataMining_Log_Detector VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include_directories(include)

add_library(detector_lib src/AnomalyDetector.cpp)

add_executable(log_detector src/main.cpp)
target_link_libraries(log_detector PRIVATE detector_lib)

enable_testing()
add_executable(run_tests tests/test_detectors.cpp)
target_link_libraries(run_tests PRIVATE detector_lib GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(run_tests)