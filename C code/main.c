#include <stdlib.h>
#include <stdio.h>
#include "data_handler.h"
#include "match_filter.h"
//most wave file are stored in short form


int main(int argc, char** argv){
    int mode;
    // mode 1: process the original audio file, mode 2: capture data from computer microphone
    mode = 1; 
    
    if(mode == 1){
        // read the processing file 
        char ori_filename[] = "../audio/ori_audio.wav";
        short *ori_waveform;
        ori_waveform = read_wavfile(ori_filename); // retrun only the wave form of the audio

        char echo_filename[] = "../audirao/echo_audio.wav";
        short *echo_waveform;
        echo_waveform = read_wavfile(echo_filename);




        //get header data create another header for output file
        // struct header *new_header;
        // new_header = get_header(ori_filename);
        // char newfilename[] = "test.wav";
        // write_wavfile(new_header, echo_waveform, newfilename);


        free(ori_waveform);
        free(echo_waveform);

    }else{
        // get data from computer microphone
    }
    

    return 0;
}