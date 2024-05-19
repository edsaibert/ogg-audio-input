CC = gcc
INCLUDES = -I$(glfw_inc) -I$(glad_inc)
LIBRARIES = -L$(glfw_lib) 

glfw = d:/external/glfw-3.1
glfw_inc = $(glfw)/include
glfw_lib = $(glfw)/lib64

glad = d:/external/glad-c
glad_inc = $(glad)/include

CFLAGS = -Wall -g $(INCLUDES)
LDFLAGS = $(LIBRARIES) -lglfw -lglm -lglut -lglut -lvorbisfile -L /usr/lib -lGL

TARGET = main
cpp_files = main.cpp
objects = $(cpp_files:.cpp=.o)

all: $(TARGET)

$(TARGET): $(objects) 
        $(CC) -o $@ glad.c $^ $(LDFLAGS)

.PHONY : clean
clean:
	-rm $(TARGET) $(objects)


