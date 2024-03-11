cmake_minimum_required(VERSION 3.28)
project(ProjetPOOA)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

qt5_add_resources(RESOURCE_ADDED resources.qrc)

add_executable(ProjetPOOA main.cpp jeu.cpp snakewindow.cpp ${RESOURCE_ADDED})
target_link_libraries(ProjetPOOA
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
)

