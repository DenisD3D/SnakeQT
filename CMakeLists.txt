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
        Network
        REQUIRED)

add_subdirectory(minizip-ng)
link_directories(minizip-ng)

qt5_add_resources(RESOURCE_ADDED resources.qrc)

add_executable(SnakeQT ${RESOURCE_ADDED}
        src/main.cpp
        src/jeu.cpp
        src/map.cpp
        src/constants.hpp
        src/inmemoryzip.cpp
        src/network_manager.cpp
        src/screens/snakewindow.cpp
        src/screens/mainmenu.hpp
        src/screens/gamescreen.cpp
        src/screens/editorscreen.cpp
        src/screens/snakewindow.cpp
        src/screens/gamearea.cpp
        src/screens/browsemapscreen.cpp
)

target_link_libraries(SnakeQT
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Xml
        Qt5::Network
        minizip
)
