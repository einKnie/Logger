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

TEST_SRCS   = $(SRC_DIR)/main.cpp
TEST_OBJ    = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(TEST_SRCS))
TEST_DEPS   = $(patsubst %,$(INC_DIR)/%.h, *)

XMPL_SRCS   = $(wildcard $(SRC_DIR)/example*)
XMPL_OBJ    = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(XMPL_SRCS))
XMPL_DEPS   = $(patsubst %,$(INC_DIR)/%.h, *)

SRCS        = $(filter-out $(XMPL_SRCS) $(TEST_SRCS),$(wildcard $(SRC_DIR)/*.cpp))
OBJ         = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))
DEPS        = $(patsubst %,$(INC_DIR)/%.h, *)
EXAMPLES    = $(patsubst $(SRC_DIR)/%.cpp, $(TARGET_DIR)/%, $(XMPL_SRCS))

.PHONY: all clean lib test examples doc
.PHONY: $(EXAMPLES)

$(EXAMPLES): $(XMPL_OBJ) $(OBJ)
	$(foreach i, $(patsubst $(OBJ_DIR)/%.o,$(TARGET_DIR)/%,$(XMPL_OBJ)), $(CC) -o $(i) $(OBJ) $(CFLAGS) $(LIB_FLAGS) $(LIBS) $(patsubst $(TARGET_DIR)/%,$(SRC_DIR)/%.cpp,$(i));)

# make object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(DEPS) | $(OBJ_DIR)
	$(CC) -c -o $@ $< $(CFLAGS)

all: lib test examples doc

clean:
	rm -frd $(OBJ_DIR) $(TARGET_DIR)/$(TARGET) ../$(HEADER) $(TEST_TARGET_DIR)/$(TEST_TARGET) $(EXAMPLES)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

lib: $(OBJ)
	ar rcs $(TARGET_DIR)/$(TARGET) $^
	cp $(INC_DIR)/$(HEADER) $(LIBS_DIR)
	cp $(TARGET_DIR)/$(TARGET) $(LIBS_DIR)/bin

test: $(OBJ) $(TEST_OBJ)
	$(CC) -o $(TEST_TARGET_DIR)/$(TEST_TARGET) $^ $(CFLAGS) $(LIB_FLAGS) $(LIBS)

examples: $(EXAMPLES)

doc:
	doxygen doxyfile
