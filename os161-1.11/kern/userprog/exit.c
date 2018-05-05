#include <types.h>
#include <kern/errno.h>
#include <lib.h>
#include <thread.h>
#include <curthread.h>
#include <syscall.h>

void sys__exit(int code){
	curthread->exit_code = code;
	thread_exit();
}
