#include <types.h>
#include <kern/errno.h>
#include <lib.h>
#include <process.h>
#include <thread.h>
#include <curthread.h>

int pid_table[32] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; //initialize pid table

void process_bootstrap(const char *name){
	curthread -> p = process_create(name);
}

struct process * process_create(const char *name){
	struct process *ret = (struct process *)kmalloc(sizeof(struct process));
	ret->name = name;
	ret->pid = pid_allocate();
	if(ret->pid == -1){
		return NULL;
	}
	return ret;
}

void process_destroy(struct process *p){
	pid_table[(p->pid)-1] = 0;
	kfree(p);
}

int pid_allocate(){
	int i;
	for(i=0;i<32;i++){
		if(pid_table[i] == 0){
			pid_table[i] = 1;
			return i+1;
		}
	}
	return -1;
}
