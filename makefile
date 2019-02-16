
# Variables
FILE_NUM = 1
FILE_PATH = ./study$(FILE_NUM).cpp
INCLUDE_PATH = ./include
BIN_PATH = ../bin
CC = g++
WCC = em++

all: bin web

run: rb

.PHONY: bin web 

# OpenGL
bin:
	$ $(CC) -Wall --std=c++11 -DOPENGL -I $(INCLUDE_PATH) $(FILE_PATH) ./include/glad.c -lglfw -lrt -lm -ldl -o $(BIN_PATH)/study$(FILE_NUM)

rb:
	$ $(BIN_PATH)/study$(FILE_NUM)

dbg:
	$ $(CC) -Wall -g --std=c++11 -DOPENGL -I $(INCLUDE_PATH) $(FILE_PATH) ./include/glad.c -lglfw -lrt -lm -ldl -o $(BIN_PATH)/study$(FILE_NUM)
	$ gdb $(BIN_PATH)/study$(FILE_NUM)

# # WebGL
# web:
# 	$ $(WCC) -Wall --std=c++11 -s USE_WEBGL2=1 -s FULL_ES3=1 -DOPENGL_ES -s USE_GLFW=3 --preload-file shaders -I $(INCLUDE_PATH) $(FILE_PATH) ./include/glad.c -lglfw -lrt -lm -ldl -o ./web/study$(FILE_NUM).html

# rw:
# 	$ firefox ./web/study$(FILE_NUM).html

# Tools
pkg-conf:
	$ pkg-config --static --libs glfw3




