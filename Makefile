CC = gcc
INCLUDES = -I$(glfw_inc) 
LIBRARIES = -L$(glfw_lib) 
SHADERS = ./shaders

glfw = d:/external/glfw-3.1
glfw_inc = $(glfw)/include
glfw_lib = $(glfw)/lib64

#glad = d:/external/glad-c

# glad = d:/external/glad-c
# glad_inc = $(glad)/include


CFLAGS = -Wall -g $(INCLUDES)
LDFLAGS = $(LIBRARIES) -lglfw -lglm -lglut -lstdc++  -lvorbisfile -L /usr/lib -lGL 

TARGET = main
cpp_files = main.cpp $(SHADERS)/*.cpp
objects = $(cpp_files:.cpp=.o) glad.o

install:
	mkdir -p lib

	@echo "Downloading PortAudio lib"
	curl https://files.portaudio.com/archives/pa_stable_v190700_20210406.tgz | tar -xz -C lib

	@echo "Building PortAudio lib"
	cd lib/portaudio && ./configure && $(MAKE) -j

.PHONY: install


all: $(TARGET)

$(TARGET): $(objects) 
        $(CC) -o $@ $^ $(LDFLAGS)

.PHONY : clean
clean:
	-rm $(TARGET) $(objects)



