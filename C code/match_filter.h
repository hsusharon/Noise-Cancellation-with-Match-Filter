#include <stdlib.h>
#include <stdio.h>
#include <math.h>

struct complex_val{  // help we keep track of complex value 
    float real;   // real part
    float img;    // imaginary part
}complex_val;

struct complex_val* create_complex_1d_arr(int col){
    struct complex_val *arr = (struct complex_val*)malloc(col * sizeof(struct complex_val));
    if(arr == NULL){
        printf("Out of memory\n");
        exit(0);
    }
    return arr;
}

// complex value multiplication operation
struct complex_val complex_multi(struct complex_val value1, struct complex_val value2){
    struct complex_val temp;
    temp.real = value1.real * value2.real + -1 * value1.img * value2.img;
    temp.img = value1.real * value2.img + value1.img * value2.real;
    return temp;
}

// complex value division operation
struct complex_val complex_division(struct complex_val complex1, struct complex_val complex2){
    // output = complex1 / complex2;
    struct complex_val temp;
    temp.real = complex2.real;
    temp.img = -1.0 * complex2.img;

    float deno = complex2.real * complex2.real + complex2.img * complex2.img;
    struct complex_val nomi = complex_multi(complex1, temp);
    nomi.real = nomi.real / deno;
    nomi.img = nomi.img / deno;
    return nomi;
}

// create a DFT table to decrease the computational complexity
struct complex_val** create_table_DFT(int N){
    // we can create a DFT table to only calculate cos() and sin() value once to optimize the run time
    // store the complex number into a table 
    //create a 2D array
    printf("Creating %d*%d DFT table....", N, N);
    struct complex_val **DFT_table = (struct complex_val**)malloc(N*sizeof(struct complex_val*)); 
    if(DFT_table == NULL){
        printf("Out of memory \n");
        exit(0);
    }
    for(int i=0;i<N;i++){
        DFT_table[i] = (struct complex_val*)malloc(N*sizeof(struct complex_val));
        if(DFT_table[i] == NULL){
            printf("Out of memory\n");
            exit(0);
        }
    }
    float PI = acos(-1);
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            struct complex_val temp;
            temp.real = cos(2*PI*i*j/N);
            temp.img = sin(2*PI*i*j/N);
            DFT_table[i][j] = temp;
        }
    }
    printf(" DFT table created\n");
    return DFT_table;
}

//FFT
struct complex_val* DFT(short *arr, int N, struct complex_val** DFT_table, int breakpoint){
    //DFT
    struct complex_val *dft_value = create_complex_1d_arr(N);
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            dft_value[i].real += (float)(arr[j]) * DFT_table[i][j].real;
            dft_value[i].img += -1.0 * (float)(arr[j]) * DFT_table[i][j].img;
        }
        //printf("%f %f\n", dft_value[i].real, dft_value[i].img);
        
        
    }
    return dft_value;
}

//inverse fft
struct complex_val* iDFT(struct complex_val *arr, int N, struct complex_val** DFT_table){
    float PI = acos(-1);
    struct complex_val *idft_value = create_complex_1d_arr(N);
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            struct complex_val temp = complex_multi(arr[j], DFT_table[j][i]);
            idft_value[i].real += (temp.real)/N;
            idft_value[i].img += (temp.img)/N;
        }
        //printf("%f  %f\n", idft_value[i].real, idft_value[i].img);
    }
}

// compute the magnitude of a complex value
short* magnitude(struct complex_val *arr, int N){
    short *new_arr = (short*)malloc(N*sizeof(short));
    for(int i=0;i<N;i++){
        float temp = (arr[i].real * arr[i].real) + (arr[i].img * arr[i].img);
        new_arr[i] = (short)sqrt(temp);
    }
    return new_arr;
}

struct complex_val* roomFilter(struct complex_val *dft_ori, struct complex_val *dft_echo, int N){
    // H(f) = Y(f) / X(f) 
    struct complex_val *roomTF = create_complex_1d_arr(N);
    for(int i=0;i<N;i++){
        roomTF[i] = complex_division(dft_echo[i], dft_ori[i]); // dft_echo(f) / dft_ori(f)
    }
    return roomTF;
}

//find the match filter function
void matchFilter(struct complex_val *room_arr, int N){
    // H'(f) = 1/H(f)
    struct complex_val temp;
    temp.real = 1; temp.img = 0;
    struct complex_val *matchFilterTF = create_complex_1d_arr(N);
    for(int i=0;i<N;i++){
        matchFilterTF[i] = complex_division(temp, room_arr[i]);
    }
 
}


//find the room transfer function
void roomTransferFunction_mode1(short **ori_audio, short **echo_audio, int *parameters){
    printf("\n------------Calculating Room Transfer Function -----------\n\n");
    int frames = parameters[0];
    int samples_per_frame = parameters[1];
    printf("Processing %d frames and %d samples per frame\n", frames, samples_per_frame);
    struct complex_val **DFT_table = create_table_DFT(samples_per_frame);  // create a DFT table to optimize the run time

    for(int i=0;i<frames;i++){
        printf(" -> Processing frame: %d / %d", i, frames);
        short *temp_ori = (short*)malloc(samples_per_frame * sizeof(short));  // store frame data into temp array
        short *temp_echo = (short*)malloc(samples_per_frame * sizeof(short)); 
        for(int j = 0;j<samples_per_frame;j++){ 
            temp_ori[j] = ori_audio[i][j];
            temp_echo[j] = echo_audio[i][j];
        }
        // find the dft of both audio 
        printf("\n      Computing DFT ...");
        struct complex_val *dft_ori = DFT(temp_ori, samples_per_frame, DFT_table, i);
        struct complex_val *dft_echo = DFT(temp_echo, samples_per_frame, DFT_table, i);

        //find the transfer function of the room
        // printf("\nCalculating the transfer function .... \n");
        // struct complex_val *roomTransferFunction;
        // roomTransferFunction = roomFilter(dft_ori, dft_echo, samples_per_frame);

        printf("   Finish\n");
        free(temp_ori);
        free(temp_echo);
        //return roomTransferFunction;
    }
    free(DFT_table);

}


//new output audio

void new_output_audio(){


}

// new generated audio
void new_observation_audio(){


}


