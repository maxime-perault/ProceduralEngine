cmake_minimum_required (VERSION 2.6)

project (ProceduralEngine)

SET(ProceduralEngine_SOURCES
        main.cpp
        Core/CoreTest.cpp)

SET(ProceduralEngine_INCLUDES
        Core/CoreTest.hpp)

add_executable(ProceduralEngine ${ProceduralEngine_SOURCES} ${ProceduralEngine_INCLUDES})