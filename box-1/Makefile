here      := $(shell pwd)
rootname  := C++/
root      := $(shell expr $(here) : '\(.*$(rootname)\)')

mode := bin
os   := lin64


hpaths    :=
hpaths    += $(root)NEB/inc
hpaths    += $(root)GRU/inc
hpaths    += $(root)GUL/inc

hpaths    += $(root)JSL/inc
hpaths    += $(root)SIG/inc
hpaths    += $(root)external
hpaths    += $(root)external/Maths/inc

hpaths    += $(root)external/PhysX/Include


lpaths    :=
lpaths    += $(root)jess/lib/lin64
lpaths    += $(root)NEB/lib/lin64
lpaths    += $(root)GRU/lib/lin64
lpaths    += $(root)GUL/lib/lin64

lpaths    += $(root)JSL/lib/lin64
lpaths    += $(root)SIG/lib/lin64
lpaths    += $(root)external/tinyxml
lpaths    += $(root)external/glew/lib
lpaths    += $(root)external/PhysX/Lib/linux64
lpaths    += $(root)external/Maths/lib/lin64


defines   := 
defines   += _DEBUG


libraries :=
libraries += rt
libraries += NEB
libraries += GRU
libraries += GUL
libraries += glee
libraries += Maths

libraries += JSL
libraries += SIG
libraries += tinyxml
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



-include $(root)../make/Makefile.Cpp.mk
