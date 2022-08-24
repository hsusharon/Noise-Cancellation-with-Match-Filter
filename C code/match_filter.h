#include <stdlib.h>
#include <stdio.h>
#include <math.h>
float PI = acos(-1);  // define pi with math.h library

struct complex_val{  // help we keep track of complex value 
    float real;   // real part
    float img;    // imaginary part
}complex_val;



//FFT
struct complex_val* FFT(short *arr){

}

//inverse fft
struct complex_val* ifft(struct complex_val *arr){
    
}

short magnitude(struct complex_val *arr){

}

struct complex_val* create_complex_1d_arr(int col){
    struct complex_val *arr = (struct complex_val*)malloc(col * sizeof(struct complex_val));
    if(arr == NULL){
        printf("Out of memory\n");
        exit(0);
    }
}

//find the match filter function
void matchFilter(){
    // H'(f) = 1/H(f)
}

//find the room transfer function
void roomTransferFunction_mode1(short **ori_audio, short **echo_audio, int *parameters){
    printf("\n------------Calculating Room Transfer Function -----------\n\n");
    int frames = parameters[0];
    int samples_per_frame = parameters[1];
    printf("Processing %d frames and %d samples per frame", frames, samples_per_frame);
    for(int i=0;i<frames;i++){
        // fft of the ori audio frame 

        // fft of the echo audio frame

        // divide the every samples in the 
    }

}



//new output audio

void new_output_audio(){


}

// new generated audio
void new_observation_audio(){


}


