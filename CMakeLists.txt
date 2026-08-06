cmake_minimum_required(VERSION 3.20)

project(SFMLProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH "C:/msys64/ucrt64")

find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System)

add_executable(SFMLProject main.cpp
        main.cpp
        DEFINITIONS.hpp
        MainMenuState.cpp
        State.hpp
        Statemachine.cpp
        Statemachine.hpp)

target_link_libraries(SFMLProject PRIVATE
        SFML::Window
        SFML::System
        SFML::Graphics
)