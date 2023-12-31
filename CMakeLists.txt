cmake_minimum_required(VERSION 3.5)

project(SmartCalc_v2 VERSION 2.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INSTALL_PREFIX ~/)

set(DEFAULT_UNIX_FLAGS -Wall -Werror -Wextra -g)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  add_compile_options(${DEFAULT_UNIX_FLAGS} -fdiagnostics-color=always)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
  add_compile_options(${DEFAULT_UNIX_FLAGS} -fdiagnostics-color=always -fsanitize=address)
  add_link_options(-fsanitize=address)
endif()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets PrintSupport)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets PrintSupport)

set(MODEL_SOURCES 
    src/model/tokenizer.cc
    src/model/translator.cc
    src/model/calculator.cc
)

set(CONTROLLER_SOURCES 
        )

set(VIEW_SOURCES 
    src/view/mainwindow.cc
    src/view/mainwindow.ui
    src/view/assets.qrc
)

set(PROJECT_SOURCES
        main.cc
        ${VIEW_SOURCES}
        ${CONTROLLER_SOURCES}
        src/external/plot/qcustomplot.cc
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
    qt_add_executable(SmartCalc_v2
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        assets/icon.png
    )
else()
    add_executable(SmartCalc_v2
        ${PROJECT_SOURCES} assets/icon.png)
endif()

include_directories(src/external/containers src/external/plot)

add_library(model ${MODEL_SOURCES})

target_link_libraries(SmartCalc_v2 PRIVATE model Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::PrintSupport)

set_target_properties(SmartCalc_v2 PROPERTIES
    MACOSX_BUNDLE_BUNDLE_NAME SmartCalc_v2
    MACOSX_BUNDLE_BUNDLE_VERSION 2.0
    MACOSX_BUNDLE_GUI_IDENTIFIER dlwhisoftware.calc.v2
    MACOSX_BUNDLE_ICON_FILE ../Resources/icon.png
    RESOURCE assets/icon.png
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

enable_testing()

add_executable(
  tokenizer_test
  tests/tokenizertest.cc
)

target_link_libraries(
  tokenizer_test
  model
  GTest::gtest_main
)

add_executable(
  model_integration
  tests/modeltest.cc
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
    qt_finalize_executable(SmartCalc_v2)
endif()
