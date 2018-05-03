#include <types.h>
#include <kern/errno.h>
#include <lib.h>
#include <process.h>
#include <addrspace.h>
#include <thread.h>
#include <curthread.h>

void process_bootstrap(void){
	int pid_table[32] = {0}; //initialize pid table
	char *name = "initial";
	curthread -> process = process_create(name);
}

struct process * process_create(char *name){
	struct process *ret = (struct process *)kmalloc(sizeof(struct process));
	ret->name = name;
	ret->space = as_create();
	ret->pid = pid_allocate();
	if(ret->pid == -1){
		return NULL;
	}
	return ret;
}

void process_destroy(struct process *p){
	pid_table[pid-1] = 0;
	as_destroy(p->space);
	kfree(p);
}

int pid_allocate(){
	int i = 0;
	for(i;i<32;i++){
		if(pid_table[i] == 0){
			return i+1;
		}
	}
	return -1;
}
