This project aims to find the transfer function of the room and for room cancelation.

Intro:
We demonstrate in the ReadMe.md file on how this project would theoratically work.
So then we generated three different types of noisy sound that could pick up by the microphone by the speaker.
original.wav is the original output audio for the speaker
1. echo_audio.wav: simulates a room that generates echo with approximately 3 seconds of delay
2. rain_audio.wav: simulates the rain outside would add noise to the observed audio indoor
3. humanspeech.wav: simulates when people talk in the room that also add noise to the observer

Compiler:
This program is using gcc compiler(https://gcc.gnu.org/install/download.html)

Structure:
There are three different files taht run this code(clone the repository to look into it)
main.c: the C code that holds all the function together
data_handler.h: handles all the function in reading wav files and writing the wave files
match_filter.h: calculates the transfer function of the room and filter, return the desired output data

Run Code:
Compile main.c only with command: gcc -o main main.c
Run code with command: main
It shows the progress of the process and the frame number it is running. 

Change the mode(line 15 in main.c) to process different audio files. 
mode 1: process the echo audio file
mode 2: process the rain audio file
mode 3: process the human speech audio file

Output files:
You can access all output files in Process_output directory


Warning:
For the code to run, do not move any files to other locations  
