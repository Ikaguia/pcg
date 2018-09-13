CC=g++
RMDIR = rm -rf
RM = rm -f

# DEP_FLAGS = 
DEP_FLAGS = -MT $@ -MMD -MP -MF $(DEP_PATH)/$*.d
DIRECTIVES = -std=c++14 -Wall -Wextra -I $(HEADER_PATH) -O2 -D DATA_PATH=\"$(DATA_PATH)\" -Wno-unused-result -Wno-unused-parameter
LIBS = -L$(LIB_PATH) -lGL -lGLU -lglut -lGLEW -L/usr/lib64

SRC_PATH = src
BIN_PATH = bin
DEP_PATH = dep
LIB_PATH = lib
DATA_PATH = data
HEADER_PATH = $(SRC_PATH)/headers

CPP_FILES = src/main.cpp src/GLmanager.cpp src/shader.cpp src/dataFile.cpp src/mesh.cpp
# CPP_FILES = $(wildcard $(SRC_PATH)/*.cpp)
OBJ_FILES = $(addprefix $(BIN_PATH)/,$(notdir $(CPP_FILES:.cpp=.o)))
DEP_FILES = $(wildcard $(DEP_PATH)/*.d)

EXEC = batata


all: $(EXEC)
$(EXEC): $(OBJ_FILES)
	$(CC) -o $@ $^ $(LIBS)
run: clear all
	./$(EXEC)

$(BIN_PATH)/%.o: $(SRC_PATH)/%.cpp
	@mkdir -p $(DEP_PATH) $(BIN_PATH)
	$(CC) $(DEP_FLAGS) -c -o $@ $< $(DIRECTIVES)
-include $(DEP_FILES)

clean:
	$(RMDIR) $(BIN_PATH) $(DEP_PATH)
	$(RM) $(EXEC)	

clear:
	clear && clear

debug: DIRECTIVES += -ggdb -O0 -D DEBUG
debug: run

remake: clear clean all

print-% : ; @echo $* = $($*)
comp-% : ; $(CC) $* $(DIRECTIVES) $(LIBS)

help :
	@echo all : creates the executable file $(EXEC)
	@echo run : creates the executable file $(EXEC) and runs it
	@echo clean : removes all temporary files
	@echo clear : clears the screen
	@echo debug : creates the executable file $(EXEC) with debug obtions
	@echo remake : removes all temporary files and then creates the executable file $(EXEC)
	@echo help : displays this message

.PRECIOUS: $(DEP_PATH)/%.d
.PHONY: all run clean clear debug remake help
