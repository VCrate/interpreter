##############################################
#                     SRC                    #
##############################################
# All directories to build (inside src/)
DIR := Interpreter Program Sandbox Sandbox/Memory Instruction Instruction/Instruction
# Files to exclude
EXCLUDED_FILE := 
# Directories with src/ prefix
SRC_DIR := $(addprefix src/,$(DIR))
# All .cpp files
SRC := $(filter-out $(EXCLUDED_FILE),$(foreach d,$(SRC_DIR),$(wildcard $(d)/*.cpp)))

##############################################
#                    MAIN                    #
##############################################
# Main directories
MAIN_DIR := src/
# Main file 
# Pick one : main.cpp test.cpp write_test_file.cpp
MAIN := main.cpp
# Main path
MAIN_PATH := $(MAIN_DIR)/$(MAIN)
# Main .o
MAIN_OBJ := $(patsubst %.cpp,build/main/%.o,$(MAIN))

##############################################
#                  BUILD/.o                  #
##############################################
# Name of the executable
DEST := build/main_app
# Build Directories
BUILD_DIR := build build/main build/src $(addprefix build/src/,$(SRC_DIR)) $(addprefix build/deps/,$(SRC_DIR)) build/lib build/shared $(addprefix build/shared/,$(SRC_DIR))
# .o files
OBJ := $(patsubst %.cpp,build/src/%.o,$(SRC))
# .o already build
LIB_PATH := lib/bytecode-description/
LIB_OBJ_PATH := $(LIB_PATH)/build/src
# Include folders
INCLUDE_FOLDER := include/

##############################################
#                    DEPS                    #
##############################################
# All .d files corresponding to each .cpp
DEPS := $(patsubst %.cpp,build/deps/%.d,$(SRC))
# Deps folder
DEPS_FOLDER := build/deps
# Flags
DEPS_FLAGS := -MMD -MP

##############################################
#                    FLAGS                   #
##############################################
# C++ optimsation level
OPTIM := -O2
# C++ flags
FLAGS := -std=c++17 -g3 -Wall -Wextra -Wno-pmf-conversions
# C++ librairy
LIBS :=
# Header include folder
INCLUDE := -I ./include -I $(LIB_PATH)/include
# Makefile flags
MAKEFLAGS += --no-print-directory

all: $(DEST)

# Main build task
# Compile each file and link them
$(DEST): $(BUILD_DIR) $(LIB_OBJ_PATH) $(OBJ) $(MAIN_OBJ)
	@echo -e "\033[32m\033[1m:: Linking of all objects\033[0m"
	@g++ $(INCLUDE) $(FLAGS) $(OBJ) $(shell find $(LIB_OBJ_PATH) -name '*.o') $(MAIN_OBJ) -o $(DEST) $(LIBS)
	@echo -e -n "\033[34m"
	@echo -e "---------------"
	@echo -e "Build finished!"
	@echo -e "---------------"
	@echo -e -n "\033[0m"

# Compile a file into a object
build/src/%.o: %.cpp
	@echo -e "\033[1m:: Building" "$<" "\033[0m"
	@g++ -c $(INCLUDE) $(OPTIM) $(FLAGS) $(DEPS_FLAGS) -o "$@" "$<"

# Compile a file into a object
$(MAIN_OBJ): $(MAIN_PATH)
	@echo -e "\033[1m:: Building Main" "\033[0m"
	@g++ -c $(INCLUDE) $(OPTIM) $(FLAGS) -o "$@" "$<"

# Make build folders
$(BUILD_DIR):
	@mkdir -p $@

# Build lib
$(LIB_OBJ_PATH):
	cd $(LIB_PATH) && make

# Clean every build files by destroying the build/ folder.
clean:
	@echo -e "Removing build folder..."
	@rm -rf build
	@echo -e -n "\033[34m"
	@echo -e "----------------"
	@echo -e "Project  Cleaned"
	@echo -e "----------------"
	@echo -e -n "\033[0m"

# Run the program
run: $(DEST)
	@echo -e -n "\033[34m"
	@echo -e "----------------"
	@echo -e "      Run       "
	@echo -e "----------------"
	@echo -e -n "\033[0m"
	@$(DEST)
	@echo -e -n "\033[34m"
	@echo -e "----------------"
	@echo -e "      Stop      "
	@echo -e "----------------"
	@echo -e -n "\033[0m"

# Equivalent of make clean and make run
again:
	@make clean
	@make run

valgrind: $(DEST)
	@echo -e -n "\033[34m"
	@echo -e "----------------"
	@echo -e "  Run Valgrind  "
	@echo -e "----------------"
	@echo -e -n "\033[0m"
	@valgrind --leak-check=full --show-leak-kinds=all $(DEST)
	@echo -e -n "\033[34m"
	@echo -e "----------------"
	@echo -e "      Stop      "
	@echo -e "----------------"
	@echo -e -n "\033[0m"

-include $(DEPS)