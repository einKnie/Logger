BASE        = ..
SRC_DIR     = $(BASE)/src
INC_DIR     = $(BASE)/inc
TARGET_DIR  = $(BASE)/bin
DOC_DIR     = $(BASE)/doc
OBJ_DIR     = $(BASE)/bin/obj
TARGET      = libcpplogging.a
HEADER      = log.h

TEST_TARGET = out
TEST_TARGET_DIR = $(TARGET_DIR)

CC          = g++
CFLAGS      = -Wall -std=c++11 -pedantic -g -I$(INC_DIR)
LIBS        =
LIB_FLAGS   =

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

all: lib test examples doc

$(EXAMPLES): $(XMPL_OBJ) $(OBJ)
	$(foreach i, $(patsubst $(OBJ_DIR)/%.o,$(TARGET_DIR)/%,$(XMPL_OBJ)), $(CC) -o $(i) $(OBJ) $(CFLAGS) $(LIB_FLAGS) $(LIBS) $(patsubst $(TARGET_DIR)/%,$(SRC_DIR)/%.cpp,$(i));)

# make object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(DEPS) | $(OBJ_DIR)
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -frd $(OBJ_DIR) $(TARGET_DIR)/$(TARGET) $(TEST_TARGET_DIR)/$(TEST_TARGET) $(EXAMPLES) $(DOC_DIR)/*

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

lib: $(OBJ)
	ar rcs $(TARGET_DIR)/$(TARGET) $^

test: $(OBJ) $(TEST_OBJ)
	$(CC) -o $(TEST_TARGET_DIR)/$(TEST_TARGET) $^ $(CFLAGS) $(LIB_FLAGS) $(LIBS)

examples: $(EXAMPLES)

doc:
	doxygen Doxyfile
	ln -sf $(DOC_DIR)/html/index.html $(DOC_DIR)/Documentation
