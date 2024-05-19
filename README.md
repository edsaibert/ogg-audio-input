### Handling Dependencies  

obs.:Glad folder generated on website for OpenGL 4.6

1) Installing libVorbis (building from source code)
```
cd libvorbis
./autogen.sh
./configure
make
make install
cd ..
```

2) Make sure you download the driver for your graphics card and the curresponding mesa package.

3) Installing OpenGL assistance packages
```
sudo pacman -S glew sdl2 sdl2_image glm freetype2
```
That's it!