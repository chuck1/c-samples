
cpp_files = main.cpp 
cpp_files += ../Quad/Quadrotor.cpp
cpp_files += ../Quad/Brain.cpp
cpp_files += ../Quad/Attitude.cpp
cpp_files += ../Quad/FDA.cpp
cpp_files += ../Quad/Position.cpp
cpp_files += ../Quad/Plant.cpp
cpp_files += ../Quad/Command.cpp
cpp_files += ../Quad/Telem.cpp




all:
	g++ ${cpp_files} -g -o quad -I. -I../ -I../../../math/src -L../../../math -lmath -Wall -std=c++0x





