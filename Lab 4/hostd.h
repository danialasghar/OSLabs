int findFreeMemoryIndex(int memSize,int realTime);
void allocateMemory(int fromIndex, int size);
void freeMemory(int fromIndex, int size);
int checkEnoughMemory(int memSize, int realTime);
void printProc(struct proc item);
void printFreeMemory();
void executeRealTimeProcess(struct proc realTimeItem,int runTime);
void executeProcess(struct proc *realTimeItem,int runTime);
void runExistingProcess(struct proc *realTimeItem);
int checkResources (struct proc *realTimeItem);
void allocateResources(struct proc *realTimeItem);
void deallocateResources(struct proc *realTimeItem);
void sigstp();
void sigCont();
void sigint();

