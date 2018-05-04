#include <types.h>
#include <kern/errno.h>
#include <lib.h>
#include <thread.h>
#include <process.h>
#include <curthread.h>
#include <syscall.h>

int sys_getpid(){
	return curthread->p->pid;
}
