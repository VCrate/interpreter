##############################################
#                     SRC                    #
##############################################
# All directories to build (inside src/)
DIR := 
# Directories with src/ prefix
SRC_DIR := $(addprefix src/,$(DIR))
# All .cpp files
SRC := $(foreach d,$(SRC_DIR),$(wildcard $(d)/*.cpp))

##############################################
#                    MAIN                    #
##############################################
# Main directories
MAIN_DIR := src/
# Main file 
# Pick one : main.cpp
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
LIBS := -lGL -lGLU -lGLEW -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor 
# Header include folder
INCLUDE := -I ./include
# Makefile flags
MAKEFLAGS += --no-print-directory

all: $(DEST)

# Main build task
# Compile each file and link them
$(DEST): $(BUILD_DIR) $(OBJ) $(MAIN_OBJ)
	@echo "\033[32m\033[1m:: Linking of all objects\033[0m"
	@g++ $(INCLUDE) $(FLAGS) $(OBJ) $(MAIN_OBJ) -o $(DEST) $(LIBS)
	@echo -n "\033[34m"
	@echo "---------------"
	@echo "Build finished!"
	@echo "---------------"
	@echo -n "\033[0m"

# Compile a file into a object
build/src/%.o: %.cpp
	@echo "\033[1m:: Building" "$<" "\033[0m"
	@g++ -c $(INCLUDE) $(OPTIM) $(FLAGS) $(DEPS_FLAGS) -o "$@" "$<"

# Compile a file into a object
$(MAIN_OBJ): $(MAIN_PATH)
	@echo "\033[1m:: Building Main" "\033[0m"
	@g++ -c $(INCLUDE) $(OPTIM) $(FLAGS) -o "$@" "$<"

# Make build folders
$(BUILD_DIR):
	@mkdir -p $@

# Clean every build files by destroying the build/ folder.
clean:
	@echo "Removing build folder..."
	@rm -rf build
	@echo -n "\033[34m"
	@echo "----------------"
	@echo "Project  Cleaned"
	@echo "----------------"
	@echo -n "\033[0m"

# Run the program
run: $(DEST)
	@echo -n "\033[34m"
	@echo "----------------"
	@echo "      Run       "
	@echo "----------------"
	@echo -n "\033[0m"
	@$(DEST)
	@echo -n "\033[34m"
	@echo "----------------"
	@echo "      Stop      "
	@echo "----------------"
	@echo -n "\033[0m"

# Equivalent of make clean and make run
again:
	@make clean
	@make run

valgrind: $(DEST)
	@echo -n "\033[34m"
	@echo "----------------"
	@echo "  Run Valgrind  "
	@echo "----------------"
	@echo -n "\033[0m"
	@valgrind --leak-check=full --show-leak-kinds=all $(DEST)
	@echo -n "\033[34m"
	@echo "----------------"
	@echo "      Stop      "
	@echo "----------------"
	@echo -n "\033[0m"

-include $(DEPS)