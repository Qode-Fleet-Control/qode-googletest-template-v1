# GoogleTest wired in the way the official "Quickstart: Building with CMake"
# teaches: FetchContent pulls a pinned GoogleTest release, enable_testing()
# + gtest_discover_tests() registers every TEST() with CTest.
cmake_minimum_required(VERSION 3.16)

project(calc VERSION 0.1 LANGUAGES CXX)

# GoogleTest requires at least C++14; this project uses C++17.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
  URL_HASH SHA256=40d4ec942217dcc84a9ebe2a68584ada7d4a33a8ee958755763278ea1c5e18ff
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# --- the library under test ---------------------------------------------------
add_library(calc src/calc.cpp)
target_include_directories(calc PUBLIC include)

# --- its tests ------------------------------------------------------------------
enable_testing()

add_executable(calc_test tests/calc_test.cpp)
target_link_libraries(calc_test PRIVATE calc GTest::gtest_main GTest::gmock)

include(GoogleTest)
gtest_discover_tests(calc_test)
