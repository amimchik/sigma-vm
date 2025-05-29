# Compiler and flags
CXX := clang++
CXXFLAGS := -Wall -Wextra -std=c++17 -Iinclude

# Directories
SRC_DIR := src
BUILD_DIR := build
OBJ_DIR := $(BUILD_DIR)/obj
BIN_DIR := $(BUILD_DIR)/bin
BIN := $(BIN_DIR)/target

# Source and object files
SRCS := $(shell find $(SRC_DIR) -name '*.cpp')
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# Default target
all: dirs $(BIN)

# Link all object files into the final binary
$(BIN): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Compile each .cpp to corresponding .o in build/
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run the binary
run: dirs $(BIN)
	./$(BIN)

dirs:
	mkdir -p $(OBJ_DIR) $(BIN_DIR)

# Remove binary and object files
clean:
	rm -rf $(BUILD_DIR)

# Full rebuild
rebuild: clean all

.PHONY: all clean rebuild run

