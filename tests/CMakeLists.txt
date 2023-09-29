cmake_minimum_required(VERSION 3.11 FATAL_ERROR)

include(FetchContent)

FetchContent_Declare(
  ut
  GIT_REPOSITORY https://github.com/boost-ext/ut
  GIT_TAG        cd12498
)

FetchContent_MakeAvailable(ut)



add_executable(SubSamplerTest SubSamplerTest.cpp)
target_link_libraries(SubSamplerTest PRIVATE Boost::ut filters)
add_test(SubSamplerTest SubSamplerTest)
