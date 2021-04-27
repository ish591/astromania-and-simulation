# Directories
SRC_DIR ?= ./src ../logic
BUILD_DIR ?= ./build
INC_DIRS := $(shell find $(SRC_DIR) -type d)

# Files list
SRCS := $(shell find $(SRC_DIR) -name *.cpp)
OBJS := $(SRCS:%.cpp=$(BUILD_DIR)/%.o)
DEPENDENCIES := $(OBJS:.o=.d)

# Compiler Flags
CC = g++
INC_FLAGS := $(addprefix -I,$(INC_DIRS))
CPPFLAGS ?= -std=c++11 $(INC_FLAGS) -MMD -MP
SDL ?= -l SDL2-2.0.0 -l sdl2_mixer -l sdl2_image -l sdl2_net -l sdl2_ttf # Set path to SDL here
LDFLAGS ?= $(SDL)

# Target Executable
TARGET_EXECUTABLE ?= game

$(BUILD_DIR)/$(TARGET_EXECUTABLE): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# c++ source
$(BUILD_DIR)/%.o: %.cpp
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) -c $< -o $@

-include $(DEPENDENCIES)

game:
	./build/game

.PHONY: clean

clean:
	rm -rf $(BUILD_DIR)
