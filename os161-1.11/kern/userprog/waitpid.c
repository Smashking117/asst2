#include <types.h>
#include <kern/errno.h>
#include <machine/spl.h>
#include <lib.h>
#include <thread.h>
#include <process.h>
#include <curthread.h>
#include <syscall.h>

int sys_waitpid(int pid, userptr_t status, int options){
	(void)options;
	struct thread *waiton = get_thread_from_pid(pid);
	if(waiton->exiting==1){
		status = (userptr_t)(waiton->exit_code);
		return 0;
	}
	splhigh();
	thread_sleep(&waiton);
	spl0();
	status = (userptr_t)(waiton->exit_code);
	return 0;
}
