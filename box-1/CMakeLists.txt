cmake_minimum_required (VERSION 2.6)
include(ExternalProject)

project (box)

add_executable(box src/main.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_DEBUG -std=c++0x -Wall -Wno-unknown-pragmas -rdynamic -pthread")

#include_directories("$ENV{HOME}/usr/include")

find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

target_link_libraries(box neb)
target_link_libraries(box glutpp)
target_link_libraries(box galaxy)
target_link_libraries(box math)

target_link_libraries(box GL)
target_link_libraries(box GLU)
target_link_libraries(box glfw3)
target_link_libraries(box GLEW)
target_link_libraries(box png)
target_link_libraries(box tinyxml)
target_link_libraries(box rt)
target_link_libraries(box pthread)
target_link_libraries(box X11)
target_link_libraries(box Xrandr)
target_link_libraries(box X11)
target_link_libraries(box ${GLFW_STATIC_LIBRARIES})

target_link_libraries(box PhysX3)
target_link_libraries(box SimulationController)
target_link_libraries(box LowLevel)
target_link_libraries(box LowLevelCloth)
target_link_libraries(box PhysXProfileSDK)
target_link_libraries(box PhysX3Extensions)
target_link_libraries(box SceneQuery)
target_link_libraries(box PhysX3Common)
target_link_libraries(box PhysX3CharacterKinematic)
target_link_libraries(box PhysX3Vehicle)
target_link_libraries(box PhysX3Cooking)
target_link_libraries(box PvdRuntime)
target_link_libraries(box RepX3)
target_link_libraries(box PxTask)
target_link_libraries(box PhysX3)
target_link_libraries(box SimulationController)
target_link_libraries(box LowLevel)
target_link_libraries(box LowLevelCloth)
target_link_libraries(box PhysXProfileSDK)
target_link_libraries(box PhysX3Extensions)
target_link_libraries(box SceneQuery)
target_link_libraries(box PhysX3Common)
target_link_libraries(box PhysX3CharacterKinematic)
target_link_libraries(box PhysX3Vehicle)
target_link_libraries(box PhysX3Cooking)
target_link_libraries(box PvdRuntime)
target_link_libraries(box RepX3)
target_link_libraries(box PxTask)

add_dependencies(box nebula glutpp galaxy math)

ExternalProject_Add(
	galaxy
	PREFIX "$ENV{HOME}/Programming/C++/galaxy"
	SOURCE_DIR "$ENV{HOME}/Programming/C++/galaxy"
	BUILD_IN_SOURCE 1
	BUIDL_COMMAND make install
)
ExternalProject_Add(
	math
	PREFIX "$ENV{HOME}/Programming/C++/math"
	SOURCE_DIR "$ENV{HOME}/Programming/C++/math"
	BUILD_IN_SOURCE 1
	BUIDL_COMMAND make install
)
ExternalProject_Add(
	nebula
	PREFIX "$ENV{HOME}/Programming/C++/neb"
	SOURCE_DIR "$ENV{HOME}/Programming/C++/neb"
	BUILD_IN_SOURCE 1
	BUIDL_COMMAND make install
)
ExternalProject_Add(
        glutpp
        PREFIX "$ENV{HOME}/Programming/C++/glutpp"
        SOURCE_DIR "$ENV{HOME}/Programming/C++/glutpp"
        BUILD_IN_SOURCE 1
        BUILD_COMMAND make install
)



