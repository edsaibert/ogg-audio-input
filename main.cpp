#include <iostream>
#include <vorbis/vorbisfile.h>
#include <stdio.h>
#include <vector>
#include <fstream>

using namespace std;

int main(int agrc, char* argc[]) {
    OggVorbis_File stream;
    ofstream ogg;
    ogg.open("audio.ogg", ios::binary);

    if (ogg.is_open()){
        // trocar a funcao para ov_open
        ov_fopen( &stream);
    }

}


