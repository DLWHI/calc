cmake_minimum_required(VERSION 3.5)

project(calc.v2 VERSION 2.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(DEFAULT_UNIX_FLAGS -Wall -Werror -Wextra -g)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  add_compile_options(${DEFAULT_UNIX_FLAGS} -fdiagnostics-color=always)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
  add_compile_options(${DEFAULT_UNIX_FLAGS} -fdiagnostics-color=always)
endif()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets PrintSupport)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets PrintSupport)

set(MODEL_SOURCES 
    src/model/Tokenizer.cc
    src/model/Translator.cc
    src/model/Calculator.cc
)

set(CONTROLLER_SOURCES 
        )

set(VIEW_SOURCES 
        src/view/mainwindow.cpp
        src/view/mainwindow.ui
        src/view/assets.qrc
        src/view/qcustomplot.cpp
)

set(PROJECT_SOURCES
        main.cpp
        ${VIEW_SOURCES}
        ${CONTROLLER_SOURCES}
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

add_library(model ${MODEL_SOURCES})

target_include_directories(calc.v2 PRIVATE src/view)
target_link_libraries(calc.v2 PRIVATE model Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::PrintSupport)

set_target_properties(calc.v2 PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

enable_testing()

add_executable(
  tokenizer_test
  tests/TokenizerTest.cc
)

target_link_libraries(
  tokenizer_test
  model
  GTest::gtest_main
)

add_executable(
  model_integration
  tests/ModelTest.cc
)

target_link_libraries(
  model_integration
  model
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(tokenizer_test)
gtest_discover_tests(model_integration)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(calc.v2)
endif()
