#Compiler, Compiler flags, Linker flags
CXX = g++
CXXFLAGS = -std=c++23 -Wall -Wextra -g -O2
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXrandr -lXi

#Directories
BUILD_DIR = ./bin
SRC_DIR = ./src
OBJ_DIR = ./obj
DEP_DIR = ./obj/deps
SHADER_SCRIPT = ./resources/scripts/compile_shaders.sh


#source files
SRC_CPP = $(wildcard $(SRC_DIR)/**/*.cpp)
SRC_HPP = $(wildcard $(SRC_DIR)/**/*.hpp)
SRC_ALL = $(SRC_CPP) $(SRC_HPP)

#object files
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_CPP))

#executable output
TARGET = $(BUILD_DIR)/engineTEST

#build the target with the build instructions below
all: $(TARGET)

#build instructions for linking the objects into the executable output
$(TARGET): $(OBJS)
	@echo "linking $@..."
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

#build instructions for compiling the source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@echo "compiling $< to $@..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

#run shader compile script
compileShaders:
	$(SHADER_SCRIPT)

#clean project directories to remove executable output and object files
clean:
	@echo "cleaning project directories..."
	rm -rf $(OBJ_DIR)/*
	rm -f $(TARGET)

#run the executable output
run: $(TARGET)
	@echo "running application..."
	$(TARGET)

#do every command specified
.PHONY: clean compileShaders all run