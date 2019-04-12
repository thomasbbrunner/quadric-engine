
# Variables
FILE_NUM = 1
FILE_PATH = ./study$(FILE_NUM).cpp
INC_PATH = ./inc
INCS = -I $(INC_PATH) -I $(LIB_PATH)
LIB_PATH = ./lib
BIN_PATH = ./bin
CC = g++
WCC = em++

all: bin web

run: rb

.PHONY: bin web 

# OpenGL
bin: main

main:
	$ $(CC) -Wall --std=c++11 -DOPENGL $(INCS) $(FILE_PATH) ./lib/glad/glad.c -lglfw -lrt -lm -ldl -o $(BIN_PATH)/study$(FILE_NUM)

rb:
	$ $(BIN_PATH)/study$(FILE_NUM)

dbg:
	$ $(CC) -Wall --std=c++11 -DOPENGL $(INCS) $(FILE_PATH) ./lib/glad/glad.c -lglfw -lrt -lm -ldl -o $(BIN_PATH)/study$(FILE_NUM)
	$ gdb $(BIN_PATH)/study$(FILE_NUM)

# # WebGL
# web:
# 	$ $(WCC) -Wall --std=c++11 -s USE_WEBGL2=1 -s FULL_ES3=1 -DOPENGL_ES -s USE_GLFW=3 --preload-file shaders -I $(INCLUDE_PATH) $(FILE_PATH) ./include/glad.c -lglfw -lrt -lm -ldl -o ./web/study$(FILE_NUM).html

# rw:
# 	$ firefox ./web/study$(FILE_NUM).html

# Tools
pkg-conf:
	$ pkg-config --static --libs glfw3




