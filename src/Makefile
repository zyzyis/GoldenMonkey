# Makefile for the Golden Monkey renderer
# Author: S.Yin <zyzyis@gmail.com>
#
# The primary targets in this files are:
#
# gm        Generate the renderer
# core      libcore.a
# sampler   libsampler.a
# camera    libcamera.a
#

# Targets
.PHONY: all main clean core sampler camera gm spu jobber util accel integ
########################################################
# Variables for compiling tools
########################################################
OS       := $(shell uname -s)
PROC     := $(shell uname -p)
LEX      := flex
YACC     := bison
CXX      := g++
CX       := gcc
EMBEDED  := embedspu32.sh
AR       := ar
LD       := ld
INCXML   := $(shell xml2-config --cflags)
LIBXML   := $(shell xml2-config --libs)
MATH	 := -lm

########################################################
# Debug flags
# Options: -DRAY_PRINT -DJOB_PRINT -DGRIDJOB_PRINT 
#	   -DSTATUS_PRINT -DTIME_MEASURE -DPARSEPRINT
#	   -DSPECULAR_ENABLED -DREFLECTION_ENABLED
########################################################
DEBUG	:= -DBLANK -DPROFILE -DLEFT_HAND -DREFLECTION_ENABLED -DSPECULAR_ENABLED
#	-DSTATUS_PRINT -DSPECULAR_ENABLED

########################################################
# Options for compiler
########################################################
OPT     := -Wall -Wno-non-virtual-dtor -O3
COMPILE := -c

########################################################
# Paths
########################################################
TOP := ..
BIN := $(TOP)/bin
LIB := $(TOP)/lib
SCALP := ../../scalp

########################################################
# CELL specific option
########################################################
ifeq ($(PROC), Cell Broadband Engine, altivec supported)
  GORDON  := $(SCALP)/cell/gordon/libgordon.a $(SCALP)/util/util.o
  OPT     += -DCBE #-maltivec -mabi=altivec
  SPE_LIB := -lspe2
  SPUCXX  := spu-elf-gcc
else 
  GORDON := $(LIB)/libgordon.fake.a 
  SPUCXX := gcc
endif

########################################################
# Libs
########################################################
ifeq ($(OS), Linux)
  THREAD := -pthread
endif
openEXR := -lIlmImf

LIBS = $(THREAD) $(openEXR) $(SPE_LIB) $(GORDON) $(LIBXML) $(MATH)

########################################################
# Includes
########################################################
INCLUDE     := -I. -I/usr/include/OpenEXR $(INCXML)
SPU_INCLUDE := -I/usr/local/spu/include

########################################################
# Compiling command
########################################################
CC    = $(CXX) $(INCLUDE) $(OPT) $(DEBUG) $(WARN)
CO    = $(CXX) $(COMPILE) $(WARN) $(INCLUDE) $(DEBUG) $(OPT)
SPUCO = $(SPUCXX) $(COMPILE) $(WARN) $(SPU_INCLUDE)

########################################################
# Function defined
########################################################
# compile : compile a component as a static component
# $1 - the name of this component
# $2 - the path name
# $3 - the component name
# $4 - source names
# $5 - file format
#
define compile
	$(if $5, $(eval compiler := $(SPUCO)) \
		 $(eval format := .c), \
		 $(eval format := .cpp) \
		 $(eval compiler := $(CO)))
        $(eval cppfiles := $(addprefix $2/, $(addsuffix $(format), $4)))
        $(eval objfiles := $(subst $(format),.o,$(cppfiles)))
        @echo -e "\e[0;34;1mStart to compile $1 component\e[0m";	\
        $(foreach f,$(cppfiles),    \
            echo -n -e 'Compiling $f...';  \
            if $(compiler) -o $(subst $(format),.o,$f) $f; \
            then     \
                echo -e "\e[0;32;1mOK\e[0m";   \
            fi;)
        @$(AR) rcs $(LIB)/$3.a $(objfiles)
        @rm -f $(objfiles)
endef     

########################################################
# GM Render
########################################################
TARGET_BIN    := gm
GM_COMPONENTS := util accel jobber core camera sampler integ gm
GM_LIBS	      := $(addprefix $(LIB)/lib, $(addsuffix .a, $(GM_COMPONENTS)))

all: $(GM_COMPONENTS) gordon main
	@echo -e "\e[0;32;1mCompilation completed with the following DEBUG options: \e[5;35;40m$(DEBUG)."
	@echo -e "\e[0;32;1mPlease use gm [input file] [output file] to render your image.\e[0m"

main :		
	@echo -n -e "\e[0;34;1mLinking the binary file...\e[31;1m"
	@$(CC) -o $(TARGET_BIN) $< main.cpp parseXML.cpp $(GM_LIBS) $(LIBS)
	@echo -e "\e[0;32;1mOK\e[0m"

gm :
	$(call compile,GM,.,libgm, \
	  gm)

core:
	$(call compile,CORE,core,lib$@, \
	matrix vector point ray bbox thread sphere plane)

sampler:
	$(call compile,SAMPLER,sampler,lib$@, \
	stratifiedsampler)

jobber:
	$(call compile,JOBBER,jobber,lib$@, \
	celljobber simplejob vectorjob job)

camera:
	$(call compile,CAMERA,camera,lib$@, \
	camera orthocamera perspective)

accel:
	$(call compile,ACCELERATOR,accelerator,lib$@, \
	accelerator simpleaccel)

util:
	$(call compile,UTIL,util,lib$@, \
	global)

integ:
	$(call compile,INTEGRATOR,integrator,lib$@, \
	whittedintegrator simpleintegrator integrator reduceintegrator,)

spu:
	$(call compile,SPU,spu,lib$@, \
	intersect getnormal shading,.c)

########################################################
# Gordon component
########################################################
gordon: spu jobber/gordon.c jobber/gordon.h
	@echo -e "\e[0;34;1mPreparing the gordon component...\e[0;36;40m"
ifeq ($(PROC), Cell Broadband Engine, altivec supported)
	@make -B -s -C $(SCALP)/cell/gordon/ libgordon.a
else
	@$(CX) $(COMPILE) $<
	@$(AR) rcs $(LIB)/libgordon.fake.a gordon.o
	@rm -f gordon.o
endif	
	@echo -e "\e[32;1mgordon is assembled.\e[0m"

########################################################
# XML Parser
########################################################
xml: testXML.cpp
	@gcc testXML.cpp -o testXML $(shell xml2-config --cflags) $(shell xml2-config --libs)

########################################################
# Clean your table
########################################################
clean:
	@rm -f $(LIB)/* $(BIN)/* gm *~
	@echo "craps are gone."
