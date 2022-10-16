#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <string.h>
#include <fstream>
#include <ctype.h>
#include "memory.h"
#include "cpu.h"

using namespace std;

int main(int argc, char* argv[])
{
    // Taking arguments as a file pointer and into timer.
    char *file = argv[1];
    int interrupt_timer = atoi(argv[2]);
    
    
    // Creating two pipes
    int MemtoC[2];
    int CtoMem[2];
    
    // Readin the results of the pipe initializing.
    int readResult = pipe(MemtoC);
    int writeResult = pipe(CtoMem);
    
    // Validating the pipes.
    if(readResult == -1 || writeResult == -1)
    {
        cout << "Pipes failed to open" << endl;
        exit(0);
    }
    
    // Fork to create two new processes.
    int process = fork();
    
    switch(process)
    {
        case -1: // Error case
        
                cout << "The fork has failed" << endl;
                exit(-1);
          
        case 0: // Child --> Memory
                initializeMemory(file);
                processMemory(MemtoC[1], CtoMem[0]);
                close(MemtoC[1]);
                close(CtoMem[0]);
                exit(0);
        default: // Else its just the parent process.
                sleep(1);
                intializeCPU(interrupt_timer, CtoMem[1], MemtoC[0]);
                processCPU();
                close(MemtoC[0]);
                close(CtoMem[1]);
                exit(0);
        
    }
    
}