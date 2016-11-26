cmake_minimum_required(VERSION 2.8)
project(tic-tac-toe)

set(source
	src/display.cpp
	src/game.cpp
	src/initialization.cpp
	src/main.cpp
	)

set(includes
	src/coord.h
	src/color.h
	src/display.h
	src/game.h
	src/initialization.h
	)

include_directories(
	SYSTEM inc
	)

cmake_policy(SET CMP0015 NEW)

link_directories(
	)

add_definitions(-std=c++11 -g)

add_executable(tic-tac-toe ${source} ${includes})

target_link_libraries(tic-tac-toe
    #GLEW
    glfw3
    GL
    X11
    Xi
    Xrandr
    Xxf86vm
    Xinerama
    Xcursor
    rt
    m
    pthread
    )

if (MSVC)
	set(warnings "/W3")
else()
	set(warnings "-Wall -Wcast-align -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wdisabled-optimization -Wextra -Wformat=2 -Winit-self -Wmissing-declarations -Wmissing-include-dirs -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-overflow=5 -Wswitch-default -Wundef -Wunreachable-code -pedantic")
	if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
		set(warnings "${warnings} -Wlogical-op -Wnoexcept -Wstrict-null-sentinel")
	endif()
endif()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${warnings}")
