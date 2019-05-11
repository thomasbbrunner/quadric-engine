
## Variables
# Paths
INC_PATH = ./inc
LIB_PATH = ./lib
BIN_PATH = ./bin
CPP_PATH = ./study.cpp $(INC_PATH)/thing.cpp $(INC_PATH)/buffer.cpp $(LIB_PATH)/glad/glad.c
INCS = -I $(INC_PATH) -I $(LIB_PATH) -I ./lib/glm -I ./lib/eigen -I ./lib/eigen/unsupported

# Compilers
CC = g++
WCC = em++

## Compile
all: bin

# OpenGL
bin: main

main:
	$ $(CC) -Wall --std=c++11 -DOPENGL $(INCS) $(CPP_PATH) -lglfw -lrt -lm -ldl -o $(BIN_PATH)/study$(FILE_NUM)

dbg:
	$ $(CC) -g -Wall --std=c++11 -DOPENGL $(INCS) $(CPP_PATH) -lglfw -lrt -lm -ldl -o $(BIN_PATH)/study$(FILE_NUM)
	$ gdb $(BIN_PATH)/study$(FILE_NUM)

# WebGL
web:
	$ $(WCC) -Wall --std=c++11 -s USE_WEBGL2=1 -s FULL_ES3=1 -DOPENGL_ES -s USE_GLFW=3 --preload-file shaders -I $(INCLUDE_PATH) $(FILE_PATH) ./include/glad.c -lglfw -lrt -lm -ldl -o ./web/study$(FILE_NUM).html

## Run

run: rb

rb:
	$ $(BIN_PATH)/study$(FILE_NUM)

rw:
	$ firefox ./web/study$(FILE_NUM).html

## Tools
pkg-conf:
	$ pkg-config --static --libs glfw3




