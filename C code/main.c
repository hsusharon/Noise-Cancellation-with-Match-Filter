#include <stdlib.h>
#include <stdio.h>
#include "data_handler.h"
#include "match_filter.h"
//most wave file are stored in short form


int main(int argc, char** argv){
    int mode;
    /*
    mode 1: process the echo audio file
    mode 2: process the rain audio file
    mode 3: process the human speech audiofile
    */
    mode = 2; 
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

        //find the transfer function of the room
        struct complex_val **roomTransferFunction; // a complex number matrix with size frames*sample_per_frame
        roomTransferFunction = roomTransferFunction_mode1(stack_ori_waveform, stack_echo_waveform, parameters);

        //find the match filter
        struct complex_val **matchTransferFunction; // a complex numnber matrix with size frames*sample_per_frame
        matchTransferFunction = find_match_filter(roomTransferFunction, parameters);


        //get the DFT table
        struct complex_val **DFT_table = create_table_DFT(sample_per_frame);
        // generate the new output waveform
        short **newoutput_stacked_audio;
        newoutput_stacked_audio = filter_convolve(stack_ori_waveform, matchTransferFunction, DFT_table, parameters);
        // generate the new observed waveform
        short **newobserved_stacked_audio;
        newobserved_stacked_audio = filter_convolve(newoutput_stacked_audio, roomTransferFunction, DFT_table, parameters);
        
        printf("------------------------- Write new audio ------------------------------\n\n");
        // flatten the processed audio
        short* newoutput_waveform = flatten_audio(newoutput_stacked_audio, sample_per_frame, frames);
        short *newobserve_waveform = flatten_audio(newobserved_stacked_audio, sample_per_frame, frames);

        // create data
        char newfilename_output[] = "Process_output/new_output_echo.wav";
        write_wavfile(echo_header, newoutput_waveform, newfilename_output);
        printf(" -> Finish writing %s\n", newfilename_output);

        char newfilename_observe[] = "Process_output/new_observe_echo.wav";
        write_wavfile(echo_header, newobserve_waveform, newfilename_observe);
        printf(" -> Finish writing %s \n", newfilename_observe);


        printf("\n -------------- Free all the dynamic array---------------\n\n");
        free(ori_header);
        free(echo_header);
        free(stack_ori_waveform);
        free(stack_echo_waveform);
        free(ori_waveform);
        free(echo_waveform);
        free(DFT_table);

        free(roomTransferFunction);
        free(matchTransferFunction);
        free(newoutput_stacked_audio);
        free(newobserved_stacked_audio);
        free(newoutput_waveform);
        free(newobserve_waveform);
        printf(" -------------------- Process Done ------------------------\n\n");

    }
    else if(mode == 2){  // process rain audio file
        // read the processing file 
        char ori_filename[] = "../audio/ori_audio.wav";
        short *ori_waveform;
        int ori_size = 0;
        ori_waveform = read_wavfile(ori_filename, &ori_size); // retrun only the wave form of the audio

        char echo_filename[] = "../audio/rain_audio.wav";
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

        //find the transfer function of the room
        struct complex_val **roomTransferFunction; // a complex number matrix with size frames*sample_per_frame
        roomTransferFunction = roomTransferFunction_mode1(stack_ori_waveform, stack_echo_waveform, parameters);

        //find the match filter
        struct complex_val **matchTransferFunction; // a complex numnber matrix with size frames*sample_per_frame
        matchTransferFunction = find_match_filter(roomTransferFunction, parameters);


        //get the DFT table
        struct complex_val **DFT_table = create_table_DFT(sample_per_frame);
        // generate the new output waveform
        short **newoutput_stacked_audio;
        newoutput_stacked_audio = filter_convolve(stack_ori_waveform, matchTransferFunction, DFT_table, parameters);
        // generate the new observed waveform
        short **newobserved_stacked_audio;
        newobserved_stacked_audio = filter_convolve(newoutput_stacked_audio, roomTransferFunction, DFT_table, parameters);
        
        printf("------------------------- Write new audio ------------------------------\n\n");
        // flatten the processed audio
        short* newoutput_waveform = flatten_audio(newoutput_stacked_audio, sample_per_frame, frames);
        short *newobserve_waveform = flatten_audio(newobserved_stacked_audio, sample_per_frame, frames);

        // create data
        char newfilename_output[] = "Process_output/new_output_rain.wav";
        write_wavfile(echo_header, newoutput_waveform, newfilename_output);
        printf(" -> Finish writing %s\n", newfilename_output);

        char newfilename_observe[] = "Process_output/new_observe_rain.wav";
        write_wavfile(echo_header, newobserve_waveform, newfilename_observe);
        printf(" -> Finish writing %s \n", newfilename_observe);


        printf("\n -------------- Free all the dynamic array---------------\n\n");
        free(ori_header);
        free(echo_header);
        free(stack_ori_waveform);
        free(stack_echo_waveform);
        free(ori_waveform);
        free(echo_waveform);
        free(DFT_table);

        free(roomTransferFunction);
        free(matchTransferFunction);
        free(newoutput_stacked_audio);
        free(newobserved_stacked_audio);
        free(newoutput_waveform);
        free(newobserve_waveform);
        printf(" -------------------- Process Done ------------------------\n\n");

    }
    else if(mode == 3){
        // read the processing file 
        char ori_filename[] = "../audio/ori_audio.wav";
        short *ori_waveform;
        int ori_size = 0;
        ori_waveform = read_wavfile(ori_filename, &ori_size); // retrun only the wave form of the audio

        char echo_filename[] = "../audio/humanspeech.wav";
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

        //find the transfer function of the room
        struct complex_val **roomTransferFunction; // a complex number matrix with size frames*sample_per_frame
        roomTransferFunction = roomTransferFunction_mode1(stack_ori_waveform, stack_echo_waveform, parameters);

        //find the match filter
        struct complex_val **matchTransferFunction; // a complex numnber matrix with size frames*sample_per_frame
        matchTransferFunction = find_match_filter(roomTransferFunction, parameters);


        //get the DFT table
        struct complex_val **DFT_table = create_table_DFT(sample_per_frame);
        // generate the new output waveform
        short **newoutput_stacked_audio;
        newoutput_stacked_audio = filter_convolve(stack_ori_waveform, matchTransferFunction, DFT_table, parameters);
        // generate the new observed waveform
        short **newobserved_stacked_audio;
        newobserved_stacked_audio = filter_convolve(newoutput_stacked_audio, roomTransferFunction, DFT_table, parameters);
        
        printf("------------------------- Write new audio ------------------------------\n\n");
        // flatten the processed audio
        short* newoutput_waveform = flatten_audio(newoutput_stacked_audio, sample_per_frame, frames);
        short *newobserve_waveform = flatten_audio(newobserved_stacked_audio, sample_per_frame, frames);

        // create data
        char newfilename_output[] = "Process_output/new_output_speech.wav";
        write_wavfile(echo_header, newoutput_waveform, newfilename_output);
        printf(" -> Finish writing %s\n", newfilename_output);

        char newfilename_observe[] = "Process_output/new_observe_speech.wav";
        write_wavfile(echo_header, newobserve_waveform, newfilename_observe);
        printf(" -> Finish writing %s \n", newfilename_observe);


        printf("\n -------------- Free all the dynamic array---------------\n\n");
        free(ori_header);
        free(echo_header);
        free(stack_ori_waveform);
        free(stack_echo_waveform);
        free(ori_waveform);
        free(echo_waveform);
        free(DFT_table);

        free(roomTransferFunction);
        free(matchTransferFunction);
        free(newoutput_stacked_audio);
        free(newobserved_stacked_audio);
        free(newoutput_waveform);
        free(newobserve_waveform);
        printf(" -------------------- Process Done ------------------------\n\n");



    }
    else{
        printf("The goal of this mode is to pick up audio from the your microphone\n");
        printf("This function is not ready yet! Please run this program again and select mode 1-3\n\n");
    }
    

    return 0;
}