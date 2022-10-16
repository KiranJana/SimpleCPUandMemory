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
#include "cpu.h"

// Function Prototypes for the methods in the class.
void intializeCPU(int secondArgument);
void processCPU();
int readMemory(int address);
void writeMemory(int address, int metadata);
void execute();
int fetch();
void push(int state);


using namespace std;

// declaring global variables.
    int USER = 0;
    int KERNEL = 1;
    bool complete = true;
    
    int wp;
    int rp;
  
    int userAdd = 0;
    int instr = 1500;
    int timeaddr = 1000;
    int systemStack = 2000;
  
    int PC, SP, IR, AC, X, Y;
    int interrupt_timer;
    int timeout;
  
    bool currentMode;
  
  // Initialize the pipes and the second argument to local variables.
void intializeCPU(int secondArgument, int writePipe, int readPipe)
  {
    // Initializing Variables
    
    PC = AC = X = Y = interrupt_timer = IR = 0;
    SP = 1000;
    
    //Initialize Pipes to global varibales.
    wp = writePipe;
    rp = readPipe;
    
    //Setting mode and the interrupt timeout.
    currentMode = USER;
    timeout = secondArgument;
  }

// Running the CPU and making it communicate with memory.
  void processCPU()
  {
       // While there are executions left we keep running the loop.
      while(complete)
       {
  
              
        // Fetching the first instruction and putting in the IR.
        IR = fetch();    
        // Running all the cases.
        execute();
        interrupt_timer++; // interrupt_timer is going up regardless of the mode.
      
        // If in user mode and time occurs then switch to kernel and strore everything to stack.
        if(currentMode == USER && interrupt_timer >= timeout)
        {
            interrupt_timer -= timeout; // Reset the interrupt_timer.
            // Change to kernel mode
            currentMode = KERNEL;
            // Store to stack.
            int tempSP = SP;
            SP = systemStack;
            push(tempSP);
            push(PC);
            PC = 1000;
        }
        
       }
        // Exit once its done.
        write(wp, "E", sizeof(char));
       
  }


// reading from memory through the given address.
int readMemory(int address)
  {
      // Checking if in the wrong mode.
      if(address >= timeaddr && currentMode == USER)
      {
        cerr << "Memory violation: accessing system adresss " << address << " in user mode" << endl;
        write(wp, "E", sizeof(char));
        exit(-1);
      }
      
      // storing the read value in temp.
      int value;
      
      write(wp, "R", sizeof(char));
      write(wp, &address, sizeof(address)); 
      read(rp, &value, sizeof(value)); // reading the value at the specified address and storing into memory.
      
      return value; // Returing the value that we read.
  }
  
void writeMemory(int address, int metadata)
  {
      // Checking if the mode is correct.
      if(address >= timeaddr && currentMode == USER)
      {
        cerr << "Memory violation: accessing system adresss " << address << " in user mode" << endl;
        write(wp, "E", sizeof(char));
        exit(-1);
      }
      // writing to memory at the given address.
      write(wp, "W", sizeof(char));
      write(wp, &address, sizeof(address));
      write(wp, &metadata, sizeof(metadata));
  }
 
  // Fetching from memory and incrementing the PC.
  int fetch()
  {
      return readMemory(PC++);
  }
  
  // Function to push to the Stack given a state.
  void push(int state)
  {
    SP--;
    writeMemory(SP, state);
  }
  
  // Popping from the stack.
  int pop()
  {
    int temp = readMemory(SP);
    writeMemory(SP, 0);
    SP++;
    return temp;   
  }
  
  // Switch case that runs all the cases.
  void execute()
  {
      switch(IR)
      {
          case 1: // Load the value into the AC.
                AC = fetch();
                break;
          case 2: // Load the value at the address into the AC.
            {
                AC = readMemory(fetch());
                break;
            }
          case 3: //Load the value from the address found in the given address into the AC
            {
                AC = readMemory(readMemory(fetch()));
                break;
            }
          case 4: //Load the value at (address+X) into the AC
            {
                AC = readMemory(fetch() + X);
                break;
            }
          case 5: //Load the value at (address+Y) into the AC
            {
                AC = readMemory(fetch() + Y);
                break;
            }
          case 6: //Load from (Sp+X) into the AC 
            {
                AC = readMemory(SP + X);
                break;
            }
          case 7: //Store the value in the AC into the address
          {
                int address = fetch();
                writeMemory(address, AC);
                break;
          }
          case 8: // Gets a random int from 1 to 100 into the AC
                AC = rand() % 100 + 1;
                break;
          case 9: // If port=1, writes AC as an int to the screen. If port=2, writes AC as a char to the screen
            {
                int operand = fetch();
                
                if(operand == 1)
                {
                    cout << AC;
                }
                else if(operand == 2)
                {
                    cout << (char)AC;
                }
                break;
            }
          case 10: //Add the value in X to the AC
                AC += X;
                break;
          case 11: //Add the value in Y to the AC
                AC += Y;
                break;
          case 12: //Subtract the value in X from the AC
                AC -= X;
                break;
          case 13: //Subtract the value in Y from the AC
                AC -=Y;
                break;
          case 14: //Copy the value in the AC to X
                X = AC; 
                break; 
		  case 15:  // Copy the value in X to the AC
		        AC = X; 
		        break; 
		  case 16:  // Copy the value in the AC to Y
		        Y = AC; 
		        break;
		  case 17:  // Copy the value in Y to the AC
		        AC = Y; 
		        break; 
		  case 18:  // Copy the value in AC to the SP
		        SP = AC; 
		        break; 
		  case 19:  // Copy the value in SP to the AC 
		        AC = SP; 
		        break; 
		  case 20: //Jump to the address
		        PC = fetch();
		        break;
		  case 21: //Jump to the address only if the value in the AC is zero
		  {
		      int value = fetch();
		        if(AC == 0)
		        {
		            PC = value;
		        }
		        break;
		  }
		  case 22: //Jump to the address only if the value in the AC is not zero
		    {
		      int value = fetch();
		        if(AC != 0)
		        {
		            PC = value;
		        }
		        break;
		    }
		  case 23: //Push return address onto stack, jump to the address
		  {
		       int address = fetch();
		       push(PC);
		       PC = address;
		       break;
		  }
		  case 24: //Pop return address from the stack, jump to the address
		  {
		    PC = pop();
		    break;
		  }
		  case 25: //Increment the value in X
		  {
		      X++;
		      break;
		          
		  }
		  case 26: //Decrement the value in X
		          X--;
		          break;
		  case 27: //Push AC onto stack
		        push(AC);
		        break;
		  case 28: //Pop from stack into AC
		        AC = readMemory(SP);
		        SP++;
		        break;
		  case 29: //Perform system call
		  {
		      if(currentMode == USER)
		      {
              currentMode = KERNEL;
              int tempSP = SP;
              SP = systemStack;
              push(tempSP);
              push(PC);
		          PC = 1500;
		          break;
		      }
		   case 30: //Return from system call
		    {
		        PC = readMemory(SP);
		        SP++;
		        SP = readMemory(SP);
		        currentMode = USER;
		        break;		        
		    }
		    
		    case 50: // End Execution.
		    {
            complete = false;
		        write(wp, "E", sizeof(char));
            break;
		    }
		    
		    default: // Final case.
		    {
		        cerr << "ERROR: Invalid Instrcution Entered." << endl;
		        write(wp, "E", sizeof(char));
		        exit(-1);
		    }
		  }
		             
       
      }
      
  }
  
  
  
