//Simple definitions for processes. May need to be expanded.
#define MAX_PID 32

#include <thread.h>
#include <addrspace.h>

extern int pid_table[32];

struct process{
	char *name; //name of process
	int pid;    //we're managing pids within the processes
	struct addrspace *space;   //We'll need to point to virtual memory
};

void process_bootstrap(void);  //initialize the process system

int pid_allocate(void);  //allocate an availible pid

struct process * process_create(char *name);  //self-explanetory

void process_destroy(struct process *p);
