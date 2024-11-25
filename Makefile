CXX = clang++
SELF_DIR := $(shell pwd)

# Directories containing source code
SRC_DIRECTORIES = src src/gaussian_integer src/utility
INC_DIRECTORIES = include include/gaussian_integer include/utility

# Include paths for project headers
INC = $(addprefix -I$(SELF_DIR)/include/,$(INC_DIRECTORIES)) $(addprefix -I$(SELF_DIR)/,$(INC_DIRECTORIES)) -I$(SELF_DIR)/.

# List of source files
SOURCES = $(wildcard $(addsuffix /*.cpp,$(SRC_DIRECTORIES))) $(wildcard *.cpp)
MODULES = $(wildcard $(addsuffix /*.cppm,$(SRC_DIRECTORIES))) $(wildcard *.cppm)
MODULE_PCMS = $(patsubst %.cppm,$(BUILD_DIR)/%.pcm,${MODULES})

# System includes
SYSTEM_INCLUDES = -I/usr/include/c++/14.2.1 \
                 -I/usr/include

# Common flags
COMMON_FLAGS = -Wall \
               -g \
               -O2 \
               -Werror=vla \
               ${INC} \
               ${SYSTEM_INCLUDES} \
               -std=c++23 \
               -stdlib=libstdc++

# Debug flags
CXXFLAGS = ${COMMON_FLAGS} \
           -D_MODULES \
           -DDEBUG \
           -fsanitize=address \
           -fmodules \
           -fimplicit-modules \
           -fmodules-cache-path=$(BUILD_DIR)/modules_cache \
           -fprebuilt-module-path=$(BUILD_DIR)/modules_cache

# Linking flags
LDFLAGS = -fsanitize=address \
          -lm -lpthread -ldl -lrt -lX11

BUILD_DIR = bin
OBJECTS = $(patsubst %.cpp, $(BUILD_DIR)/%.o, ${SOURCES})
MODULE_OBJECTS = $(patsubst %.cppm, $(BUILD_DIR)/%.o, ${MODULES})
DEPENDS = $(OBJECTS:.o=.d)
EXEC = gaussian-factorise

# Create necessary directories
$(shell mkdir -p $(BUILD_DIR))
$(shell mkdir -p $(BUILD_DIR)/src)
$(shell mkdir -p $(BUILD_DIR)/src/gaussian_integer)
$(shell mkdir -p $(BUILD_DIR)/src/utility)
$(shell mkdir -p $(BUILD_DIR)/modules_cache)

# Target to build the executable
${EXEC}: ${MODULE_OBJECTS} ${OBJECTS}
	${CXX} ${CXXFLAGS} ${MODULE_OBJECTS} ${OBJECTS} -o ${EXEC} $(LDFLAGS)

# Function to get module name from file path
module_name = $(shell basename $(1) .cppm | tr '[:upper:]' '[:lower:]')

# Rule to compile module PCM files
$(BUILD_DIR)/%.pcm: %.cppm
	@mkdir -p $(dir $@)
	${CXX} ${COMMON_FLAGS} -fmodules -x c++-module --precompile $< -o $@

# Rule to compile .cppm (module interface) files
$(BUILD_DIR)/%.o: %.cppm $(BUILD_DIR)/%.pcm
	@mkdir -p $(dir $@)
	${CXX} ${CXXFLAGS} -c $< -o $@

# Rule to build object files from source (.cpp files)
$(BUILD_DIR)/%.o: %.cpp ${MODULE_PCMS}
	@mkdir -p $(dir $@)
	${CXX} ${CXXFLAGS} $(foreach pcm,${MODULE_PCMS},-fmodule-file=$(call module_name,$(patsubst $(BUILD_DIR)/%.pcm,%.cppm,$(pcm)))=$(pcm)) -c $< -o $@

# Print module information for debugging
.PHONY: print-modules
print-modules:
	@echo "Module PCMs: ${MODULE_PCMS}"
	@echo "Module Objects: ${MODULE_OBJECTS}"

# Include dependency files
-include $(DEPENDS)

.PHONY: clean

# Clean the build directory and executable
clean:
	rm -rf $(BUILD_DIR) ${EXEC}