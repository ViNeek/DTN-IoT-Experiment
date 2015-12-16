CC=gcc
SRC_DIR=src
BIN_DIR=bin
OBJ_DIR=obj
LIB_DIR=lib
INC_DIR=include
OBJS=main.o dtn.o util.o networking.o packet.o
TARGET=NATIVE
CFLAGS=-I$(INC_DIR) -D$(TARGET) -c -g -Wall 
LDFLAGS=-L$(LIB_DIR) -lws2_32
EXECUTABLE_NAME=run

PHONY = main move

all: main move

native: main move

testbed: main move

move:
	mv -f ./*.o $(OBJ_DIR)

main: $(OBJS)
	mkdir -p $(OBJ_DIR)
	$(CC) $(OBJS) $(LDFLAGS) -o $(BIN_DIR)/$(EXECUTABLE_NAME)

main.o: $(SRC_DIR)/main.c $(SRC_DIR)/common.h
	$(CC) $(CFLAGS) $(SRC_DIR)/main.c

dtn.o: $(SRC_DIR)/dtn.c $(SRC_DIR)/dtn.h
	$(CC) $(CFLAGS) $(SRC_DIR)/dtn.c

util.o: $(SRC_DIR)/util.c $(SRC_DIR)/util.h
	$(CC) $(CFLAGS) $(SRC_DIR)/util.c

networking.o: $(SRC_DIR)/networking.c $(SRC_DIR)/networking.h
	$(CC) $(CFLAGS) $(SRC_DIR)/networking.c

packet.o: $(SRC_DIR)/packet.c $(SRC_DIR)/packet.h
	$(CC) $(CFLAGS) $(SRC_DIR)/packet.c

clean:
	rm $(OBJ_DIR)/*o $(BIN_DIR)/$(EXECUTABLE_NAME)