void intializeCPU(int secondArgument, int rp, int wp);
void processCPU();
int readMemory(int address);
void writeMemory(int address, int metadata);
int fetch();
void execute();
void push(int state);
int pop();