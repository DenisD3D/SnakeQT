cmake_minimum_required(VERSION 3.22)
project(SnakeQT)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        Xml
        REQUIRED)

add_subdirectory(minizip-ng)
link_directories(minizip-ng)

qt5_add_resources(RESOURCE_ADDED resources.qrc)

add_executable(SnakeQT src/main.cpp src/jeu.cpp src/snakewindow.cpp src/zip.cpp ${RESOURCE_ADDED}
        src/map.cpp
        src/map.hpp)
target_link_libraries(SnakeQT
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Xml
        minizip
)
