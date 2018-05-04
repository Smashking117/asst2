//Simple definitions for processes. May need to be expanded.

extern int pid_table[32];

struct process{
	const char *name; //name of process
	int pid;    //we're managing pids within the processes
};

void process_bootstrap(const char *name);  //initialize the process system

int pid_allocate(void);  //allocate an availible pid

struct process * process_create(const char *name);  //self-explanetory

void process_destroy(struct process *p);
