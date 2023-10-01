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

qt_add_executable(CombatLogTest CombatLogTest.cpp "test-data/CombatLog/data.qrc")
qt_add_resources(CombatLogTest "test-data/CombatLog/data.qrc" PREFIX "/")
target_link_libraries(CombatLogTest PRIVATE GTest::gtest_main CombatLog Qt6::Core)
add_test(NAME CombatLogTest COMMAND CombatLogTest)
