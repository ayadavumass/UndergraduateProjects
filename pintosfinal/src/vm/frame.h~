#include<hash.h>
#include<debug.h>
#include "threads/synch.h"

struct list Frame_Queue;
struct semaphore frame_sema;
struct lock frame_lock;
struct FrameToUserPage
{
	//struct hash_elem hash_elem; /* Hash table element. */
	struct list_elem queue_elem;
	void *Frame_Addr;
	void *UserPage_Addr;
	struct thread *thread ;
};


