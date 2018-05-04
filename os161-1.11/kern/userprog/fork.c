#include <types.h>
#include <kern/errno.h>
#include <lib.h>
#include <thread.h>
#include <process.h>
#include <syscall.h>
#include <curthread.h>
#include <machine/trapframe.h>

int sys_fork(int32_t retval, void *data1){
	struct process *p = process_create("child");
	retval = p->pid;
	struct trapframe *newtf = kmalloc(sizeof(*data1));
	memcpy(newtf,data1,sizeof(*data1));
	int result = as_copy(curthread->p->space, &(p->space));
	if(result){
		process_destroy(p);
		return result;
	}
	result = thread_fork("child",&newtf,0,md_forkentry,p,NULL);
	return 0;
}


