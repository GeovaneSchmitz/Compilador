# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude

# Source directories
SRC_DIR = src
OBJ_DIR = build
BIN_DIR = bin

# Output executable name
TARGET = $(BIN_DIR)/compilador

# Find all .cpp files in the src/ directory
SRCS = $(shell find $(SRC_DIR) -name '*.cpp')

# Generate .o files in the build/ directory, mirroring src/ structure
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# Default target
all: $(TARGET)

# Link the final executable
$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile each .cpp file to .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Clean up
clean:
	@rm -rf $(OBJ_DIR) $(BIN_DIR)

# Rebuild everything
rebuild: clean all

# Declare phony targets
.PHONY: all clean rebuild
