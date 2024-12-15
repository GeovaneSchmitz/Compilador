# Define variables
BUILD_DIR = bin
CMAKE = cmake
BUILD_TYPE = Debug
CC = clang 
CXX = clang++
NPROC = $(shell nproc)
NPROC_PLUS_ONE = $(shell expr $(NPROC) + 1)
# Default target
all: configure compile

# Configure the project using CMake
configure:
	mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && CC=$(CC) CXX=$(CXX) $(CMAKE) -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) ..

# Build the project
compile:
	$(CMAKE) --build $(BUILD_DIR) -j$(NPROC_PLUS_ONE) --target main

# Clean the build directory
clean:
	rm -rf $(BUILD_DIR)

# Rebuild the project
rebuild: clean all
