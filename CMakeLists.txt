cmake_minimum_required(VERSION 3.5)

project(calc.v2 VERSION 2.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS  ${CMAKE_CXX_FLAGS} "-g -fdiagnostics-color=always")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "/Wall")
endif()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(MODEL_SOURCES 
    src/model/translator.cc)

set(CONTROLLER_SOURCES 
        )

set(VIEW_SOURCES 
        src/view/mainwindow.cpp
        src/view/mainwindow.h
        src/view/mainwindow.ui
)

set(PROJECT_SOURCES
        main.cpp
        ${VIEW_SOURCES}
        ${MODEL_SOURCES}
)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

if (WIN32)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif (WIN32)

FetchContent_MakeAvailable(googletest)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(calc.v2
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    add_executable(calc.v2
        ${PROJECT_SOURCES})
endif()

target_link_libraries(calc.v2 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# add_custom_target(test )

set_target_properties(calc.v2 PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

enable_testing()

add_executable(
  TranslationTest
  tests/TranslationTest.cc
  ${MODEL_SOURCES}
)

target_link_libraries(
  TranslationTest
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(TranslationTest)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(calc.v2)
endif()
