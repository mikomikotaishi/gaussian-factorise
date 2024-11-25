CXX = g++
SELF_DIR := $(shell pwd)
DIRECTORIES = include src src/gaussian_integer src/utility
INC = $(addprefix -I$(SELF_DIR)/,$(DIRECTORIES)) -I$(SELF_DIR)/.

# Find all source files
SOURCES = $(shell find src -name "*.cpp") # This will recursively find all .cpp files in src directory
HEADERS = $(shell find src -name "*.h") $(shell find src -name "*.hpp")

# Debug flags
CXXFLAGS = -std=c++23 -DDEBUG -fsanitize=address -I/usr/include -Wall -g -MMD -O2 -Werror=vla ${INC}
LDFLAGS = -fsanitize=address -lm -lpthread -ldl -lrt -lX11

BUILD_DIR = bin
OBJECTS = $(SOURCES:%.cpp=$(BUILD_DIR)/%.o)
DEPENDS = $(OBJECTS:.o=.d)
EXEC = gaussian-factoriser

# ANSI color codes
GREEN := $(shell echo -e "\033[32m")
RESET := $(shell echo -e "\033[0m")

# Progress bar variables
TOTAL_FILES := $(words $(SOURCES))
CURRENT_FILE = 0
BAR_LENGTH = 50

# Store start time in nanoseconds
START_TIME := $(shell date +%s.%N)

# Function to calculate elapsed time in seconds and milliseconds
define calc_elapsed
$(shell echo "$(shell date +%s.%N) - $(START_TIME)" | bc)
endef

# Function to format time with milliseconds
define format_time
$(shell echo "$1" | awk '{mins=int($$1/60); secs=int($$1%60); ms=int(($$1*1000)%1000); printf "%02d:%02d.%03d", mins, secs, ms}')
endef

# Function to draw progress bar and compilation status
define draw_progress_bar
	$(eval CURRENT_FILE=$(shell echo $$(($(CURRENT_FILE) + 1))))
	$(eval PROGRESS=$(shell echo "scale=2; $(CURRENT_FILE) * 100 / $(TOTAL_FILES)" | bc))
	$(eval FILLED=$(shell echo "scale=0; $(PROGRESS) * $(BAR_LENGTH) / 100" | bc))
	$(eval EMPTY=$(shell echo "$(BAR_LENGTH) - $(FILLED)" | bc))
	$(eval ELAPSED=$(call calc_elapsed))
	$(eval TIME_STR=$(call format_time,$(ELAPSED)))
	@echo '$(GREEN)Compiling$(RESET) $<'
	@if [ $(CURRENT_FILE) -lt $(TOTAL_FILES) ]; then \
		printf '[%-*s%*s] %3.1f%% (%d/%d files) [%s]\n' \
			'$(FILLED)' \
			'$(shell printf '%*s' '$(FILLED)' | tr " " "#")' \
			'$(EMPTY)' \
			'' \
			'$(PROGRESS)' \
			'$(CURRENT_FILE)' \
			'$(TOTAL_FILES)' \
			'$(TIME_STR)'; \
	else \
		printf '[%*s] 100.0%% (%d/%d files) [%s]\n' \
			'$(BAR_LENGTH)' \
			'$(shell printf '%*s' '$(BAR_LENGTH)' | tr " " "#")' \
			'$(TOTAL_FILES)' \
			'$(TOTAL_FILES)' \
			'$(TIME_STR)'; \
	fi
endef

# Create build directories
$(shell mkdir -p $(sort $(dir $(OBJECTS))))

# Default target
all: ${EXEC}

# Target to build the executable
${EXEC}: ${OBJECTS}
	@echo -e "\n$(GREEN)Linking$(RESET) executable..."
	@${CXX} ${OBJECTS} -o ${EXEC} ${LDFLAGS}
	@echo -e "\n$(GREEN)Build complete:$(RESET) ${EXEC} [Total time: $(call format_time,$(call calc_elapsed))]"

# Rule to build object files from source
$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@$(call draw_progress_bar)
	@${CXX} ${CXXFLAGS} -c $< -o $@

# Print sources for debugging
print-sources:
	@echo "Sources found: "
	@echo "${SOURCES}"
	@echo "Objects to build: "
	@echo "${OBJECTS}"

# Include dependency files
-include $(DEPENDS)

.PHONY: clean print-sources all

# Clean the build directory and executable
clean:
	@echo "$(GREEN)Cleaning$(RESET) build files..."
	@rm -rf $(BUILD_DIR) ${EXEC}
	@echo "Clean complete"