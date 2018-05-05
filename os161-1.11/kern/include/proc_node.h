struct thread;

struct proc_node{
	struct thread *thread;
	struct proc_node *next;
};

extern struct proc_node head;
