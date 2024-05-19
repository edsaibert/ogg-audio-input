### Handling Dependencies  


Installing libVorbis
```
cd ~
wget git@github.com:xiph/vorbis.git
cd vorbis
./autogen.sh
./configure
make
make install
cd ..
rm -r vorbis/
```

Installing OpenGL assistance packages
```
sudo pacman -S glew sdl2 sdl2_image glm freetype2
```
