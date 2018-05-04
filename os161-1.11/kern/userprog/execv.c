/*
 * Sample/test code for running a user program.  You can use this for
 * reference when implementing the execv() system call. Remember though
 * that execv() needs to do more than this function does.
 */

#include <types.h>
#include <kern/unistd.h>
#include <kern/errno.h>
#include <lib.h>
#include <addrspace.h>
#include <thread.h>
#include <curthread.h>
#include <vm.h>
#include <vfs.h>
#include <test.h>

/*
 * Load program "progname" and start running it in usermode.
 * Does not return except on error.
 *
 * Calls vfs_open on progname and thus may destroy it.
 */
int sys_execv(char *program,char**args)
{
	int size = 0;
	while(args[size]!=NULL){
		size++;
	}
	size += 2;
	char **kbuff = (char**)kmalloc(size*sizeof(char*));
	int i;
	int j;
	for(i=0;i<size;i++){
		j = 0;
		while(args[i][j]!='\0'){
			j++;
		}
		int len = j+2;
		while((len%4)!=0){
			len++;
		}
		if((len-(j+2))!=0){
			int k;
			char temp[len];
			for(k=0;k<(j+2);k++){
				temp[k] = args[i][k];
			}
			while(k!=len){
				temp[k] = '\0';
			}
			char *toadd = temp;
			kbuff[i] = toadd;
		}
		else{
			kbuff[i] = args[i];
		}
	}
	struct vnode *v;
	vaddr_t entrypoint, stackptr;
	int result;

	/* Open the file. */
	result = vfs_open(program, O_RDONLY, &v);
	if (result) {
		return result;
	}

	/* We should be a new thread. */
	assert(curthread->t_vmspace == NULL);

	/* Create a new address space. */
	curthread->t_vmspace = as_create();
	if (curthread->t_vmspace==NULL) {
		vfs_close(v);
		return ENOMEM;
	}

	/* Activate it. */
	as_activate(curthread->t_vmspace);

	/* Load the executable. */
	result = load_elf(v, &entrypoint);
	if (result) {
		/* thread_exit destroys curthread->t_vmspace */
		vfs_close(v);
		return result;
	}

	/* Done with the file now. */
	vfs_close(v);

	/* Define the user stack in the address space */
	result = as_define_stack(curthread->t_vmspace, &stackptr);
	if (result) {
		/* thread_exit destroys curthread->t_vmspace */
		return result;
	}

	result = copyout(kbuff,(userptr_t)stackptr,size*sizeof(char *));
	stackptr -= size*sizeof(char *);

	/* Warp to user mode. */
	md_usermode(0 /*argc*/, NULL /*userspace addr of argv*/,
		    stackptr, entrypoint);
	
	/* md_usermode does not return */
	panic("md_usermode returned\n");
	return EINVAL;
}

