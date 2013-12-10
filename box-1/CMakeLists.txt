cmake_minimum_required (VERSION 2.6)
project (box)

add_executable(box src/main.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_DEBUG -std=c++0x")



#include_directories("$ENV{HOME}/usr/include")

target_link_libraries(box
	galaxy
	neb
	math
	glutpp
	GL glut GLEW
	png tinyxml
	rt
	pthread
	PhysX3 
	SimulationController
	LowLevel
	LowLevelCloth
	PhysXProfileSDK
	PhysX3Extensions
	SceneQuery
	PhysX3Common
	PhysX3CharacterKinematic
	PhysX3Vehicle
	PhysX3Cooking 
	PvdRuntime 
	RepX3 
	PxTask 
	X11 
)
