cmake_minimum_required(VERSION 3.11 FATAL_ERROR)

include(FetchContent)




FetchContent_Declare(
  gtest
  GIT_REPOSITORY https://github.com/google/googletest
  GIT_TAG        f8d7d77
)

FetchContent_MakeAvailable(gtest)



add_executable(SubSamplerTest SubSamplerTest.cpp)
target_link_libraries(SubSamplerTest PRIVATE GTest::gtest_main filters)
add_test(NAME SubSamplerTest COMMAND SubSamplerTest)
