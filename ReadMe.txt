Hari Kiran Jana
4348.005

ReadMe
My project consists of files:
1. main.cpp - This is the file that contains the initializing of pipes and forks. It also runs my memory and CPU.
2. CPU.cpp - This file contains all the instructions that need to be executed and also has all registers and interrupt handling.
3. memory.cpp - This file maakes the memory array and stores data from the text file, and also has the read and write functions.
4. cpu.h - This is a header file that contians my methods in cpu.cpp that were used in main to run the code.
5. memory.h - This is a header file that contians my methods in memory.cpp that were used in main to run the code.

TO complie: 
g++ main.cpp cpu.cpp memory.cpp -o os 
NOTE: The header files should be in the same folder as the cpp files so that they can compile properly.

To RUN: For all samples. // os is my executable file but it can be anything.
./os sample1.txt 30
./os sample2.txt 30
./os sample3.txt 30
./os sample4.txt 30
./os sample5.txt 30