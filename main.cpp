
#include <iostream>
#include <ogg/ogg.h>
#include <vorbis/vorbisfile.h>
#include <stdio.h>
#include <vector>
// #include <fstream>

using namespace std;

int main(int agrc, char* argc[]) {
    OggVorbis_File *stream;      // Arquivo que inicia a bitstream de dados
    FILE *ogg;                  // Arquivo onde será 'escrito' o áudio
    char* buffer;               // Buffer FILE

    ogg = fopen("audio.ogg", "r+");

    if (ogg != NULL){
        // ov_open(ogg, stream, NULL, 0);
        printf("Open file");
        // ov_clear(stream);
    }
    else
        return 0;

    return 1;

}


