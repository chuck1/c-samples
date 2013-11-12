here      := $(shell pwd)
rootname  := C++/
root      := $(shell expr $(here) : '\(.*$(rootname)\)')

mode := bin
os   := lin64


hpaths    :=
hpaths    += $(root)JSL/inc
hpaths    += $(root)SIG/inc


lpaths    :=
lpaths    += $(root)SIG/lib/lin64
lpaths    += $(root)JSL/lib/lin64


defines   := 


libraries :=
libraries += SIG
libraries += JSL



-include $(root)../make/Makefile.Cpp.mk
