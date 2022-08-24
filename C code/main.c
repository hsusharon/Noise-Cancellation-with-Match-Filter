#include <stdlib.h>
#include <stdio.h>
#include "data_handler.h"
#include "match_filter.h"
//most wave file are stored in short form


int main(int argc, char** argv){
    int mode;
    // mode 1: process the original audio file, mode 2: capture data from computer microphone
    mode = 1; 
    printf("\n-------------------- Running program ----------------------\n\n");
    
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

        //get the information(header) of the audio files
        struct header *ori_header;
        ori_header = get_header(ori_filename);
        struct header *echo_header;
        echo_header = get_header(echo_filename);

        //get sample rate and sample per frame values
        float sampleRate = (float)ori_header->sr;
        float time = 0.25;  // in seconds
        int sample_per_frame = (int)(time * sampleRate);

        printf("\n----------------- File loaded successfully ------------------------\n\n");

        ori_waveform = zero_padding(ori_waveform, ori_size, sample_per_frame, &ori_size);
        printf("original audio new size:%d\n", ori_size);
        echo_waveform = zero_padding(echo_waveform, echo_size, sample_per_frame, &echo_size);
        printf("echo audio new size: %d\n", echo_size);

        int frames = (int)(ori_size / sample_per_frame);
        printf("Number of frames:%d  Samples per frame:%d\n", frames, sample_per_frame);
        int *parameters = (int*)malloc(2*sizeof(int));
        parameters[0] = frames;
        parameters[1] = sample_per_frame;

        // // stack the audio into smaller frames
        short** stack_ori_waveform = stack_up_arr(ori_waveform, frames, sample_per_frame);
        short** stack_echo_waveform = stack_up_arr(echo_waveform, frames, sample_per_frame);

        //find the match filter of the two audio
        roomTransferFunction_mode1(stack_ori_waveform, stack_echo_waveform, parameters);
        
        // // flatten the processed audio
        // short* flat_ori_waveform = flatten_audio(stack_ori_waveform, sample_per_frame, frames);
        // short *flat_echo_waveform = flatten_audio(stack_echo_waveform, sample_per_frame, frames);

        // get header data create another header for output file
        // printf("\nComputing DFT ...\n");
        // struct complex_val *dft_value = DFT(ori_waveform, ori_size);
        // printf("\nComputing iDFT ...\n");
        // struct complex_val *test = iDFT(dft_value, ori_size);
        // short *new_arr = magnitude(test, ori_size);
        
        // char newfilename[] = "test.wav";
        // write_wavfile(echo_header, new_arr, newfilename);
        printf("\n--------------Free all the dynamic array---------------\n");
        free(ori_header);
        free(echo_header);
        free(stack_ori_waveform);
        free(stack_echo_waveform);
        free(ori_waveform);
        free(echo_waveform);

    }else{
        // get data from computer microphone
    }
    

    return 0;
}