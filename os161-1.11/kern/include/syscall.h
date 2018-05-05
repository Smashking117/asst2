#ifndef _SYSCALL_H_
#define _SYSCALL_H_

/*
 * Prototypes for IN-KERNEL entry points for system call implementations.
 */

int sys_reboot(int code);

int sys_getpid();

int sys_fork(int32_t retval, void *data1);

int sys_execv(char *program, char**args);

int sys_waitpid(int pid, userptr_t status, int options);

void sys__exit(int code);

#endif /* _SYSCALL_H_ */
