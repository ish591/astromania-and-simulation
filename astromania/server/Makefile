# Directories
SRC_DIR ?= ./src ../logic
BUILD_DIR ?= ./build/server
INC_DIRS := $(shell find $(SRC_DIR) -type d)

# Files list
SRCS := $(shell find $(SRC_DIR) -name *.cpp)
OBJS := $(SRCS:%.cpp=$(BUILD_DIR)/%.o)
DEPENDENCIES := $(OBJS:.o=.d)

# Compiler Flags
CC = g++
INC_FLAGS := $(addprefix -I,$(INC_DIRS))
CPPFLAGS ?= -std=c++11 $(INC_FLAGS) -MMD -MP
SDL ?= -l SDL2 -l SDL2_mixer -l SDL2_image -l SDL2_net -l SDL2_ttf # Set path to SDL here
LDFLAGS ?= $(SDL)

# Target Executable
TARGET_EXECUTABLE ?= ../server.out

$(BUILD_DIR)/$(TARGET_EXECUTABLE): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# c++ source
$(BUILD_DIR)/%.o: %.cpp
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) -c $< -o $@

-include $(DEPENDENCIES)
	
server:
	./build/server.out

.PHONY: clean server

clean:
	rm -rf $(BUILD_DIR)

free:
	kill  $(shell lsof -ti:1234)
