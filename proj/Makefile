SRC_DIR     = ../src
INC_DIR     = ../inc
TARGET_DIR  = ../bin
OBJ_DIR     = ../bin/obj
LIBS_DIR    = ../../
TARGET      = libcpplogging.a
HEADER      = cpp_log.h

TEST_TARGET = out
TEST_TARGET_DIR = $(TARGET_DIR)

CC          = g++
CFLAGS      = -Wall -std=c++11 -pedantic -g -D_POSIX_C_SOURCE=200809L -I$(INC_DIR) -I$(LIBS_DIR)
LIBS        = -lutils
LIB_FLAGS   = -L$(LIBS_DIR)/bin

SRCS       := $(wildcard $(SRC_DIR)/*.cpp)
OBJ         = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))
DEPS        = $(patsubst %,$(INC_DIR)/%.h, *)

TEST_SRCS   = $(SRC_DIR)/main.cpp
TEST_OBJ    = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(TEST_SRCS))
TEST_DEPS   = $(patsubst %,$(INC_DIR)/%.h, *)

.PHONY: all clean lib test

# make object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(DEPS) | $(OBJ_DIR)
	$(CC) -c -o $@ $< $(CFLAGS)

all: lib test

clean:
	rm -frd $(OBJ_DIR) $(TARGET_DIR)/$(TARGET) ../$(HEADER) $(TEST_TARGET_DIR)/$(TEST_TARGET)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

lib: $(OBJ)
	ar rcs $(TARGET_DIR)/$(TARGET) $^
	cp $(INC_DIR)/$(HEADER) $(LIBS_DIR)

test: $(OBJ) $(TEST_OBJ)
	$(CC) -o $(TEST_TARGET_DIR)/$(TEST_TARGET) $^ $(CFLAGS) $(LIB_FLAGS) $(LIBS)
