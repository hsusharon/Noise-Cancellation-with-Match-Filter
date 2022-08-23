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
        int ori_size = 0;
        ori_waveform = read_wavfile(ori_filename, &ori_size); // retrun only the wave form of the audio

        char echo_filename[] = "../audio/echo_audio.wav";
        short *echo_waveform;
        int echo_size = 0;
        echo_waveform = read_wavfile(echo_filename, &echo_size);

        //test zeropadding and stackup and flatten
        struct header *ori_header;
        ori_header = get_header(ori_filename);
        float sampleRate = (float)ori_header->sr;
        float time = 0.5;
        int sample_per_frame = (int)(time * sampleRate);

        struct header *echo_header;
        echo_header = get_header(echo_filename);


        ori_waveform = zero_padding(ori_waveform, ori_size, sample_per_frame, &ori_size);
        printf("original audio new size:%d\n", ori_size);
        echo_waveform = zero_padding(echo_waveform, echo_size, sample_per_frame, &echo_size);
        printf("echo audio new size: %d\n", echo_size);

        int frames = (int)(ori_size / sample_per_frame);
        printf("Number of frames:%d  Samples per frame:%d\n", frames, sample_per_frame);
        short** stack_ori_waveform;
        stack_ori_waveform = stack_up_arr(ori_waveform, frames, sample_per_frame);
        short* flat_ori_waveform;
        flat_ori_waveform = flatten_audio(stack_ori_waveform, sample_per_frame, frames);

        printf("Finish processing\n");
        //get header data create another header for output file
        
        // char newfilename[] = "test.wav";
        // write_wavfile(echo_header, echo_waveform, newfilename);

        free(ori_header);
        free(echo_header);
        free(ori_waveform);
        free(echo_waveform);

    }else{
        // get data from computer microphone
    }
    

    return 0;
}