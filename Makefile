TARGET=DTN_NATIVE
ROLE=DTN_CLIENT
SRC_DIR=src
ARM_DIR=ARM
BIN_DIR=bin
OBJ_DIR=obj
LIB_DIR=lib
INC_DIR=src
CONTIKI=fit/iot-lab/parts/contiki
CONTIKI_PROJECT = dtn_experiment
PROJECTDIRS = ./src/ARM
PROJECT_SOURCEFILES += util.c dtn.c networking.c

all: dtn_experiment

#UIP_CONF_IPV6=1

ifneq (,$(filter $(TARGET),iotlab-m3 native minimal-net))
  CC=/Users/nickvitsas/Documents/Projects/IOT/DTN-IoT-Experiment/toolchain/gcc-arm-none-eabi-4_8-2014q1/bin/arm-none-eabi-gcc
  CFLAGS=-I$(INC_DIR) -DDTN_IOTLAB -D$(ROLE)
  ifeq ($(TARGET),native)
    CFLAGS=-I$(INC_DIR) -DDTN_IOTLAB_X86 -D$(ROLE)
    CC=gcc
  endif
  ifeq ($(TARGET),minimal-net)
    CC=gcc
  endif
  include $(CONTIKI)/Makefile.include
else
  CFLAGS=-I$(INC_DIR) -D$(TARGET) -D$(ROLE) -c -g -Wall 
  include ./Makefile.native
endif
