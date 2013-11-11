here      := $(shell pwd)
rootname  := C++/
root      := $(shell expr $(here) : '\(.*$(rootname)\)')

mode := bin
os   := lin64


hpaths    :=
hpaths    += $(root)kernel/inc
hpaths    += $(root)GRU/inc
hpaths    += $(root)external/PhysX/Include

lpaths    :=
lpaths    += $(root)jess/lib/lin64
lpaths    += $(root)nebula/lib/lin64
lpaths    += $(root)GRU/lib/lin64
lpaths    += $(root)external/glew/lib
lpaths    += $(root)external/PhysX/Lib/linux64

defines   := 

libraries :=
libraries += rt
libraries += GRU
libraries += GL
libraries += GLU
libraries += glut
libraries += pthread
libraries += PhysX3
libraries += SimulationController
libraries += LowLevel
libraries += LowLevelCloth
libraries += PhysXProfileSDK
libraries += PhysX3Extensions
libraries += SceneQuery
libraries += PhysX3Common
libraries += PhysX3CharacterKinematic
libraries += PhysX3Vehicle
libraries += PhysX3Cooking
libraries += PvdRuntime
libraries += RepX3
libraries += PxTask
libraries += X11



-include $(root)make/Makefile.Cpp.mk
