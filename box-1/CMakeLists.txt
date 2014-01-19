cmake_minimum_required (VERSION 2.8.7)
include(ExternalProject)

project (box)

include_directories(../../nebula/src ../../glutpp/src ../../galaxy/src ../../math/src)
link_directories(../../nebula ../../glutpp ../../galaxy ../../math)


add_executable(box src/main.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -D_DEBUG -std=c++0x -Wall -Wno-unknown-pragmas -Wno-unused-local-typedefs -rdynamic -pthread -pg")


#find_library(nebula NAME nebula PATHS ../../nebula)

find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

target_link_libraries(box nebula)
target_link_libraries(box glutpp)
target_link_libraries(box galaxy)
target_link_libraries(box math)

target_link_libraries(box GL GLU)
target_link_libraries(box glfw3)
target_link_libraries(box GLEW)
target_link_libraries(box freetype)
target_link_libraries(box png)
target_link_libraries(box tinyxml2)
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



