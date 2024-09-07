portAudio_inc = ./lib/portaudio/include
portAudio_dep = -lrt -lasound -ljack -pthread 
portAudio_lib = ./lib/portaudio/lib/.libs/libportaudio.a

CC = gcc
INCLUDES = -I$(portAudio_inc)
LIBRARIES = -L$(glfw_lib) $(portAudio_lib)
SHADERS = ./shaders
AUDIO = ./audio
OPENGL = ./opengl
YIN = ./yin

glfw = d:/external/glfw-3.1
glfw_inc = $(glfw)/include
glfw_lib = $(glfw)/lib64

#glad = d:/external/glad-c

# glad = d:/external/glad-c
# glad_inc = $(glad)/include


CFLAGS = -Wall -g $(INCLUDES)
LDFLAGS = $(LIBRARIES) -lglfw -lglm -lm -lglut -lstdc++  -lvorbisfile -L /usr/lib -lGL $(portAudio_dep) -lportaudio

TARGET = main
cpp_files = main.cpp $(OPENGL)/opengl.cpp $(AUDIO)/audio.cpp $(YIN)/yin.cpp
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
        $(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY : clean
clean:
	-rm $(TARGET) $(objects)



