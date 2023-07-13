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

set_target_properties(calc.v2 PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(calc.v2)
endif()
