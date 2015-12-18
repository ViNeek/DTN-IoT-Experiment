CC=gcc
FIT_TOOLCHAIN=./toolchain/gcc-arm-none-eabi-4_8-2014q1/bin
FIT_CC=$(FIT_TOOLCHAIN)/arm-none-eabi-gcc
SRC_DIR=src
BIN_DIR=bin
OBJ_DIR=obj
LIB_DIR=lib
INC_DIR=include
OBJS=main.o dtn.o util.o networking.o packet.o

# arch
TARGET=NATIVE

# role
ROLE=CLIENT

CFLAGS=-I$(INC_DIR) -D$(TARGET) -D$(ROLE) -c -g -Wall 
# Windows require this for native UDP
#LDFLAGS=-L$(LIB_DIR) -lws2_32
LDFLAGS=-L$(LIB_DIR)
EXECUTABLE_NAME=run

PHONY = main move

all: main move print

native: main move print

testbed: CC=$(FIT_CC)
testbed: main move print

move:
	@mv -f ./*.o $(OBJ_DIR)

print: 
	@echo "\n\n$(ROLE) executable is in the 'bin' directory\n\n"

main: $(OBJS)
	echo $(CC)
	@mkdir -p $(LIB_DIR)
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(BIN_DIR)
	@mkdir -p fit
	@$(CC) $(OBJS) $(LDFLAGS) -o $(BIN_DIR)/$(EXECUTABLE_NAME)
	@echo "	LD 		$(OBJS)"

main.o: $(SRC_DIR)/main.c
	@$(CC) $(CFLAGS) $< 
	@echo "	CC 		$<"

dtn.o: $(SRC_DIR)/dtn.c
	@$(CC) $(CFLAGS) $< 
	@echo "	CC 		$<"

util.o: $(SRC_DIR)/util.c
	@$(CC) $(CFLAGS) $< 
	@echo "	CC 		$<"

networking.o: $(SRC_DIR)/networking.c
	@$(CC) $(CFLAGS) $< 
	@echo "	CC 		$<"

packet.o: $(SRC_DIR)/packet.c
	@$(CC) $(CFLAGS) $< 
	@echo "	CC 		$<"

clean:
	rm $(OBJ_DIR)/*o $(BIN_DIR)/$(EXECUTABLE_NAME)