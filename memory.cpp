#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <ctype.h>
#include <fstream>
#include "memory.h"

using namespace std;

    const int SIZE = 2000;
    int mem[SIZE];
  
// This function basically takes in a filename and loads the contents into the memory array.  
void initializeMemory(char *fileName)
{
    int counter = 0; 
  
  
    // creating a file pointer and reading in a file name.
    FILE *name = fopen(fileName, "r");
    char buffer[100];
    char *line; 


    while(fgets(buffer, sizeof(buffer), name)) 
    {
        // We are reading the line into memory.
        line = strtok(buffer, "\n");
        
        // Validating the line that was read into the buffer.
        if(line != NULL){
            // if the number is a digit we store into the buffer.
            if(isdigit(buffer[0]))
                mem[counter++] = atoi(buffer); // loading the digits into memory.    
            else if(buffer[0] == '.') // If the line starts with a . making the counter at that addresss in memory.
                counter = atoi(&buffer[1]);
        }
    } // end of while.
    fclose(name);
    
}
    
  


// This function enacts the memory, and comunicates with the CPU through a write and read pipe.
void processMemory(int wp, int rp)
{
   char operation;
   int  address, metadata;
   bool state = true; 
   
   
   // We are reading a value from the pipe.
   read(rp, &operation, sizeof(operation));
   
   
   // Switch cases used to differentiate and write each command.
   while(state)
   {
 
       
       switch(operation)
         {
           case 'R': // this is used to read from memory.
                     read(rp, &address, sizeof(address));
                     write(wp, &mem[address], sizeof(mem[address]));
                     break;
           case 'W': // This is to write to memory.
                     read(rp, &address, sizeof(address));  
                     read(rp, &metadata, sizeof(address));		 
                     mem[address] = metadata;						
                     break; 
           case 'E': // Exit case.
                   state = false;
                   exit(0);
           default: // if the command is not valid we return error.
                   cout << "Command is not valid " << operation;
                   exit(-1);
                   break;
         }
         
         read(rp, &operation, sizeof(operation)); // Reading the next command.
     }
 
   }
   