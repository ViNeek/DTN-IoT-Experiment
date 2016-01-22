TARGET=native
IOT_SERVER=2
IOT_CLIENT=1
IOT_PLATFORM_SKY=1
IOT_PLATFORM_M3=2
IOT_PLATFORM_NATIVE=3
CPPFLAGS= -DIOT_PLATFORM_NATIVE=3 -DIOT_PLATFORM_M3=3 -DIOT_PLATFORM_SKY=1 -DIOT_SERVER=2 -DIOT_CLIENT=1
ROLE=client
SRC_DIR=src
ARM_DIR=ARM
BIN_DIR=bin
OBJ_DIR=obj
LIB_DIR=lib
INC_DIR=src
CONTIKI=fit/iot-lab/parts/contiki
CONTIKI_PROJECT = dtn_experiment
PROJECTDIRS = ./src/ARM
PROJECT_SOURCEFILES += util.c dtn.c networking.c cache.c packet.c mjson.c

all: dtn_experiment

#UIP_CONF_IPV6=1

ifneq (,$(filter $(TARGET),sky iotlab-m3 native minimal-net))
  
  ifeq ($(TARGET),sky)
  	ifeq ($(ROLE),server)
    	CFLAGS=-std=gnu99 -I$(INC_DIR) $(CPPFLAGS) -DDTN_IOTLAB -DROLE=IOT_SERVER -DTARGET=IOT_PLATFORM_SKY
  	endif
  	ifeq ($(ROLE),client)
    	CFLAGS=-std=gnu99 -I$(INC_DIR) $(CPPFLAGS) -DDTN_IOTLAB -DROLE=IOT_CLIENT -DTARGET=IOT_PLATFORM_SKY
  	endif
  endif
  ifeq ($(TARGET),iotlab-m3)
    CC=/Users/nickvitsas/Documents/Projects/IOT/DTN-IoT-Experiment/toolchain/gcc-arm-none-eabi-4_8-2014q1/bin/arm-none-eabi-gcc
    ifeq ($(ROLE),server)
    	CFLAGS=-std=gnu99 -I$(INC_DIR) $(CPPFLAGS) -DROLE=IOT_SERVER -DTARGET=IOT_PLATFORM_M3
  	endif
  	ifeq ($(ROLE),client)
    	CFLAGS=-std=gnu99 -I$(INC_DIR) $(CPPFLAGS) -DROLE=IOT_CLIENT -DTARGET=IOT_PLATFORM_M3
  	endif
  endif
  ifeq ($(TARGET),native)
    CC=gcc
    ifeq ($(ROLE),server)
    	CFLAGS=-std=gnu99 -I$(INC_DIR) $(CPPFLAGS) -DDTN_IOTLAB_X86 -DROLE=IOT_SERVER -DTARGET=IOT_PLATFORM_NATIVE
  	endif
  	ifeq ($(ROLE),client)
    	CFLAGS=-std=gnu99 -I$(INC_DIR) $(CPPFLAGS) -DDTN_IOTLAB_X86 -DROLE=IOT_CLIENT -DTARGET=IOT_PLATFORM_NATIVE
  	endif
  endif
  ifeq ($(TARGET),minimal-net)
    CC=gcc
  endif
  include $(CONTIKI)/Makefile.include
else
  CFLAGS=-I$(INC_DIR) $(CPPFLAGS) -D$(TARGET) -D$(ROLE) -c -g -Wall 
  include ./Makefile.native
endif
