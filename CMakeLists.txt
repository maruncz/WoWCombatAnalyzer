cmake_minimum_required(VERSION 3.5)

project(WoWCombatAnalyzer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Charts)

option(BUILD_TESTS OFF)



add_library(CombatLog STATIC
    CombatLog/LogLine.h CombatLog/LogLine.cpp
    CombatLog/defs.h
    CombatLog/SubEvents.h CombatLog/SubEvents.cpp
    CombatLog/LineParser.h CombatLog/LineParser.cpp
    CombatLog/exceptions.h CombatLog/exceptions.cpp
    CombatLog/CombatLog.h CombatLog/CombatLog.cpp
)

target_link_libraries(CombatLog PUBLIC Qt6::Core)

add_library(DataModels STATIC
    DataModels/healdatamodel.h DataModels/healdatamodel.cpp
    DataModels/ObjectListModel.h DataModels/ObjectListModel.cpp
)

target_link_libraries(DataModels
    PUBLIC Qt6::Core
    PRIVATE CombatLog)

add_library(filters STATIC
    filters/SubSampler.h filters/SubSampler.cpp
)

target_link_libraries(filters PUBLIC Qt6::Core)

add_library(forms STATIC
    forms/DamagePerSecond.h forms/DamagePerSecond.cpp forms/DamagePerSecond.ui
    forms/SourceSelector.h forms/SourceSelector.cpp forms/SourceSelector.ui
)

target_link_libraries(forms PUBLIC Qt6::Widgets Qt6::Charts
    PRIVATE DataModels filters)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
)


qt_add_executable(WoWCombatAnalyzer
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)

target_link_libraries(WoWCombatAnalyzer PRIVATE Qt6::Widgets Qt6::Charts CombatLog DataModels filters forms)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if("${QT_VERSION}" VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.WoWCombatAnalyzer)
endif()
set_target_properties(WoWCombatAnalyzer PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS WoWCombatAnalyzer
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_finalize_executable(WoWCombatAnalyzer)

if("${BUILD_TESTS}")
    enable_testing()
    add_subdirectory(tests)
endif()
