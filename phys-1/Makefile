here      := $(shell pwd)
rootname  := C++/
root      := $(shell expr $(here) : '\(.*$(rootname)\)')

mode := bin
os   := lin64

hpaths := $(root)NEB/inc
hpaths += $(root)external/PhysX/Include

lpaths := $(root)NEB/lib/lin64
lapths += $(root)external/PhysX/Lib/linux64

libraries := NEB

libraries += PhysX3
libraries += SimulationController
libraries += LowLevel
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

defines := _DEBUG

-include $(root)make/Makefile.Cpp.mk

