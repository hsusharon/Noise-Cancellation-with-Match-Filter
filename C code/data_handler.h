#include <stdlib.h>
#include <stdio.h>
#include <math.h>

struct header{  // header file of a wav file
    char RIFF[4];  
    int filesize;  
    char fileFormat[4]; 
    char formatChunk[4];
    int formatLength;
    short audioFormat;
    short channelNum;
    int sr;
    int byteRate;
    short blockAlign;
    short BitsPerSample;
    char subChunk2ID[4];
    int subChunk2Size;
}header;



//read value to specific memory
void read_header(FILE *fptr, struct header *headerData){
    fread(headerData->RIFF, sizeof(headerData->RIFF), sizeof(char), fptr);
    fread(&headerData->filesize, 1, sizeof(int), fptr);
    fread(headerData->fileFormat, sizeof(headerData->fileFormat), sizeof(char), fptr);
    fread(headerData->formatChunk, sizeof(headerData->formatChunk), sizeof(char), fptr);
    fread(&headerData->formatLength, 1, sizeof(int), fptr);
    fread(&headerData->audioFormat, 1, sizeof(short), fptr);
    fread(&headerData->channelNum, 1, sizeof(short), fptr);
    fread(&headerData->sr, 1, sizeof(int), fptr);
    fread(&headerData->byteRate, 1, sizeof(int), fptr);
    fread(&headerData->blockAlign, 1, sizeof(short), fptr);
    fread(&headerData->BitsPerSample, 1, sizeof(short), fptr);
    fread(headerData->subChunk2ID, sizeof(headerData->subChunk2ID), sizeof(char), fptr);
    fread(&headerData->subChunk2Size, 1, sizeof(int), fptr);
}

// read audio file data
short* read_wavfile(char *filename, int *audio_len){
    FILE *fptr = fopen(filename, "rb");
    if (fptr == NULL){  // make sure the file is opened correctly
        printf("Unable to open file %s \n", filename);
        exit(0);
    }
    // Read the header file
    struct header *headerData;
    headerData = (struct header*)malloc(1*sizeof(struct header));
    read_header(fptr, headerData);
    printf("Audio size:%d  BitsPerSample:%d  sr:%d\n", 
            headerData->filesize, headerData->BitsPerSample, headerData->sr);
    int byte_per_sample = headerData -> BitsPerSample / 8;
    int sample_num = headerData->filesize / byte_per_sample;
    short *audio_data = (short*)malloc(sample_num * sizeof(short));
    int idx = 0;
    short temp;
    while(fread(&temp, 1, sizeof(short), fptr)){
        audio_data[idx ] = temp;
        idx++;
    }
    *audio_len = idx;
    printf("Finish reading file: %s with length %d\n\n", filename, *audio_len);
    return audio_data;
}

// get header value
struct header* get_header(char *filename){
    FILE *fptr = fopen(filename, "rb");
    if (fptr == NULL){  // make sure the file is opened correctly
        printf("Unable to open file %s \n", filename);
        exit(0);
    }
    struct header *headerData;
    headerData = (struct header*)malloc(1*sizeof(struct header));
    read_header(fptr, headerData);
    return headerData;
}

// write header
void write_header(FILE *fptr, struct header *headerData){
    fwrite(headerData->RIFF, 4, sizeof(char), fptr);
    fwrite(&headerData->filesize, 1, sizeof(int), fptr);
    fwrite(headerData->fileFormat, 4, sizeof(char), fptr);
    fwrite(headerData->formatChunk, 4, sizeof(char), fptr);
    fwrite(&headerData->formatLength, 1, sizeof(int), fptr);
    fwrite(&headerData->audioFormat, 1, sizeof(short), fptr);
    fwrite(&headerData->channelNum, 1, sizeof(short), fptr);
    fwrite(&headerData->sr, 1, sizeof(int), fptr);
    fwrite(&headerData->byteRate, 1, sizeof(int), fptr);
    fwrite(&headerData->blockAlign, 1, sizeof(short), fptr);
    fwrite(&headerData->BitsPerSample, 1, sizeof(short), fptr);
    fwrite(headerData->subChunk2ID, 4, sizeof(char), fptr);
    fwrite(&headerData->subChunk2Size, 1, sizeof(int), fptr);
}

// create new audio file
void write_wavfile(struct header *headerData, short *arr, char *filename){
    FILE *fptr = fopen(filename, "wb");
    if(fptr == NULL){
        printf("Could not find file %s\n", filename);
        exit(0);
    }
    write_header(fptr, headerData);

    int byte_per_sample = headerData -> BitsPerSample / 8;
    int sample_num = headerData->filesize / byte_per_sample;
    for(int i=0;i<sample_num;i++){
        fwrite(&arr[i], 1, sizeof(short), fptr);
    }
    fclose(fptr);
}

// allocate a 1D dynamic array
short* create_1D_arr(int col){
    short* arr = (short*)malloc(col  * sizeof(short));
    if(arr == NULL){
        printf("Out of memory\n");
        exit(0);
    }
}

// allocate a 2D dynamic array
short** create_2D_arr(int row, int col){
    short **arr = (short**)malloc(row * sizeof(short*));
    if(arr == NULL){
        printf("Out of memory\n");
        exit(0);
    }
    
    for(int i=0;i<row;i++){
        arr[i] = (short*)malloc(col * sizeof(short));
        if(arr[i] == NULL){
            printf("Out of memory\n");
        exit(0);
        }
    }
    return arr;
}

// zero padding
short* zero_padding(short *arr, int arrsize, int target, int *new_size){
    int remain = arrsize%target;
    remain = target - remain;
    *new_size = remain + arrsize;
    // printf("add additional %d samples\n", remain);
    short *new_arr = create_1D_arr(remain+arrsize);
    for(int i=0;i<arrsize;i++){
        new_arr[i] = arr[i]; 
    }
    for(int i=0;i<remain;i++){
        new_arr[i + arrsize] = 0;  // pad the remain values with zeros
    }
    return new_arr;
}

// stack audiofile up
short** stack_up_arr(short* arr, int frames, int sample_per_frame){
    
    short** stack_arr = create_2D_arr(frames, sample_per_frame);
    int samples = frames * sample_per_frame;
    int sample_counter = 0;
    int frame_num = 0;
    for(int i=0;i<samples;i++){
        stack_arr[frame_num][sample_counter] = arr[i];
        sample_counter++;
        if(sample_counter == sample_per_frame){
            sample_counter = 0;
            // printf("%d  ", frame_num);
            frame_num ++ ;
        } 
        
    }
    //printf("%d %d \n", frame_num, sample_counter);
    return stack_arr;
}


// flatten the audio
short* flatten_audio(short** arr, int sample_per_frame, int frames){
    short* new_arr = create_1D_arr(sample_per_frame * frames);
    for(int i=0;i<frames;i++){
        for(int j=0;j<sample_per_frame;j++){
            new_arr[i*sample_per_frame + j] = arr[i][j]; 
        }
    }
    return new_arr;
}



