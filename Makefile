CC=g++
CFLAGS=-Wall -g
INC=-I/usr/include -Iinc
LIB=-lglfw -lGLEW -lGL

SRC_DIR=src
OBJ_DIR=obj
PRJ_DIR=graphics

SRCS = $(wildcard $(SRC_DIR)/$(PRJ_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/$(PRJ_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

.SILENT:

rebuild: begin clean main end
build: begin main end

begin:
	@date

end:
	@date

main : $(OBJS)
	$(CC) $(CFLAGS) $(INC) src/main.cpp -o bin/main $^ $(LIB)
	@echo bin/main

$(OBJ_DIR)/%.o : $(SRC_DIR)/$(PRJ_DIR)/%.cpp
	@echo building $@
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo cleaning build
	@rm -f $(OBJ_DIR)/*.o
	@rm -f bin/*
