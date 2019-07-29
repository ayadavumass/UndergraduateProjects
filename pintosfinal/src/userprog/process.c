#include "userprog/process.h"
#include <debug.h>
#include <inttypes.h>
#include <round.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "userprog/gdt.h"
#include "userprog/pagedir.h"
#include "userprog/tss.h"
#include "filesys/directory.h"
#include "filesys/file.h"
#include "filesys/filesys.h"
#include "threads/flags.h"
#include "threads/init.h"
#include "threads/interrupt.h"
#include "threads/palloc.h"
#include "threads/thread.h"
#include "threads/vaddr.h"
#include "vm/page.h"
#include "vm/swap.h"
#include "devices/disk.h"
#include "vm/memoryMap.h"


struct FrameToUserPage* SecondChanceAlgo();

//#include "threads/thread.c"
//#define testing
static int
get_user (const uint8_t *uaddr)
{
int result;
asm ("movl $1f, %0; movzbl %1, %0; 1:"
: "=&a" (result) : "m" (*uaddr));
return result;
}
static thread_func start_process NO_RETURN;
static bool load (const char *cmdline, void (**eip) (void), void **esp);


/* Starts a new thread running a user program loaded from
   FILENAME.  The new thread may be scheduled (and may even exit)

   before process_execute() returns.  Returns the new process's
   thread id, or TID_ERROR if the thread cannot be created. */

tid_t
process_execute (const char *file_name) 
{
// we need to put in a condition when a process can not be executed.
//printf("%s\n",file_name);
  char *task;
  char *fn_copy, *fn_copy2;
  tid_t tid;

  /* Make a copy of FILE_NAME.
     Otherwise there's a race between the caller and load(). */
  fn_copy = palloc_get_page (0);
  
  if (fn_copy == NULL)
    return TID_ERROR;
  //printf("%p\n",fn_copy);
 strlcpy (fn_copy, file_name, PGSIZE);
  fn_copy2 = palloc_get_page (0);
  
  if (fn_copy2 == NULL)
    return TID_ERROR;

 strlcpy (fn_copy2, file_name, PGSIZE);
/**/
 char *saveptr;

 task = strtok_r(fn_copy2, " ", &saveptr); // SPLICING THE FILE NAME FROM THE ARRAY OF ARUGMENTS PASSED
 struct file *fileP1 = filesys_open(task);
   if(fileP1 == NULL) 

  {
       
      palloc_free_page (fn_copy);
      palloc_free_page (fn_copy2);
      return -1;
   }
	file_close(fileP1); 

		     // task FILE NAME
/**/
  /* Create a new thread to execute FILE_NAME. */
  //printf("parent(pid = %d) starts child process\n", thread_current()->tid);

 tid = thread_create (task, PRI_DEFAULT, start_process, fn_copy);
  
  //printf("tid %d returned to thread with tid %d and child process file name is %s\n",tid,     thread_current()->tid,file_name);
  //printf("sema down called, by thread pid  = %d\n",thread_current()->tid);
 struct thread *t = getThreadFromTid(&(thread_current()->child_list), tid); 
 t->parent=thread_current(); 
 sema_down(&(t->parentChildSemaphore));
//printf("sema down calledfor process_execute by the thread with pid =%d \n",thread_current()->tid); 
//printf("after semadown\n");
  
if(!(t->isLoadedSuccess))
{
  //printf("load success failed for thread with tid %d\n",t->tid);
  tid = -1;
  palloc_free_page (fn_copy); 
  palloc_free_page (fn_copy2);
    return tid;
}

  /*if (tid == TID_ERROR)
   {
    palloc_free_page (fn_copy); 
    return tid;	    
   }*/

   palloc_free_page (fn_copy2);
  return tid;
}

/* A thread function that loads a user process and starts it
   running. */
static void
start_process (void *file_name_)
{
// printf("%s",file_name_);
  	if(file_name_== NULL)
	{
		thread_current()->set_status=-1;
	        thread_exit();
	}
  char *file_name = file_name_;
  struct intr_frame if_;
  bool success;void *argadd;
 // printf("hi1\n");
  /* Initialize interrupt frame and `executable. */
  memset (&if_, 0, sizeof if_);
  if_.gs = if_.fs = if_.es = if_.ds = if_.ss = SEL_UDSEG;
  if_.cs = SEL_UCSEG;
  if_.eflags = FLAG_IF | FLAG_MBS;
/**/ // printf("hi2\n");
	char *saveptr, *retptr;
	retptr = strtok_r(file_name, " " , &saveptr); // to ignore the filename here
	//printf("file name %s\n",retptr);	
	//Generating the file name from the string of arguments 
/**/
 
  success = load (retptr, &if_.eip, &if_.esp);
  //parent synchro lock release
/**/  //	  printf("hi3\n");
		char *addArgument[50];
		 // declaring an array of 4 arguments for storing the address of the stack location where the arguments get stored 
		int i = 1, argc = 0;
		retptr = strtok_r(NULL, " " , &saveptr); 
		//if_.esp = if_.esp - 4;
		//printf("intial value of sp %p \n",if_.esp);
		while(retptr!= NULL)
		{
		//if(get_user(if_.esp) == -1)
		//printf("token %s\n",retptr);	
		if_.esp = if_.esp - (int)(strlen(retptr)+1);
//printf("hi2\n");
		addArgument[i] = if_.esp;
		// storing the 	address location of the stack where the arguments are being stored 
		if(get_user(if_.esp) == -1)
                {
		//printf("exit1\n");
		thread_current()->set_status = -1;
		//printf("exit2\n");
		//return ;
		thread_exit() ;	
		}
		strlcpy(if_.esp, retptr , (strlen(retptr))+1);
		//printf("15\n");		
		#ifdef testing	
		printf("address %p value %s\n",if_.esp,(char *)if_.esp, (strlen(retptr)));
		#endif
		i++;
		retptr = strtok_r(NULL, " " , &saveptr); 
		//retptr gets the arguments from splicing of the file name 
                }
//printf("hi3\n");
		if_.esp = if_.esp - (int)(strlen(file_name)+1);
//printf("hi4\n");
		addArgument[0] = if_.esp;
   		strlcpy(if_.esp, file_name , (strlen(file_name))+1);
//		printf("hi5\n");
                #ifdef testing
		printf("address %p value %s \n",if_.esp,(char *)if_.esp);
		#endif
		
  		argc = i;if_.esp = if_.esp - 4 ;
//printf("hi6\n");
		*(char *)(if_.esp) = (char *) 0 ; 			//storing a zero in the stack after the last argument 
		#ifdef testing
		printf("address %p value %p \n",if_.esp,*(char **)if_.esp);
		#endif
		
		while(i > 0)
		{
		i--;
		if_.esp = if_.esp -4;
		//printf("hi7\n");
		*(char **)if_.esp =  (char *) addArgument[i];         //storing the arguments' address in the stack
		#ifdef testing
		printf("address %p value %p \n",if_.esp, *(char **)(if_.esp));
		#endif
					       //assuming each address stores 4 bytes		
		}
		if_.esp = if_.esp - sizeof(char *);
		//printf("hi8\n");
		*(char **)if_.esp = (char *)(if_.esp + 4);//printf("hi9\n");
		#ifdef testing
		printf("address %p value %p \n",if_.esp, *(char **)(if_.esp));
		#endif
		if_.esp = if_.esp - sizeof(int);//printf("hi11\n");
		*(int *)if_.esp =  (argc);
		#ifdef testing
		printf("address %p value %d \n",if_.esp, *(int *)(if_.esp));
		#endif
		if_.esp = if_.esp - sizeof(int);//printf("hi12\n");
		*(int *)if_.esp = 0 ;
		#ifdef testing
		printf("address %p value %d \n",if_.esp, *(int *)(if_.esp));
		//printf("address %p value %s \n",argadd+3, (char *)(argadd+3));
		#endif
		//hex_dump(if_.esp, if_.esp, 200 ,true);
/**/
  //printf("hi13 %s\n", file_name_);
  /* If load failed, quit. */
//  printf("success %d tid %d \n",success,thread_current()->tid);
  
   
   
   palloc_free_page (file_name);
  if (!success) 
{
//         printf("loaded sucess = false for thread with pid = %d",thread_current()->tid);
         thread_exit ();
}
else
 {
   thread_current()->isLoadedSuccess = true ;
   //printf("loaded sucess = true for thread with pid = %d",thread_current()->tid);
 }
sema_up(&(thread_current()->parentChildSemaphore));



//printf("sema up called by thread after load  with pid %d\n",thread_current()->tid);  
  /* Start the user process by simulating a return from an
     interrupt, implemented by intr_exit (in
     threads/intr-stubs.S).  Because intr_exit takes all of its
     arguments on the stack in the form of a `struct intr_frame',
     we just point the stack pointer (%esp) to our stack frame
     and jump to it. */
  asm volatile ("movl %0, %%esp; jmp intr_exit" : : "g" (&if_) : "memory");
  NOT_REACHED ();
}

/* Waits for thread TID to die and returns its exit status.  If
   it was terminated by the kernel (i.e. killed due to an
   exception), returns -1.  If TID is invalid or if it was not a
   child of the calling process, or if process_wait() has already
   been successfully called for the given TID, returns -1
   immediately, without waiting.

   This function will be implemented in problem 2-2.  For now, it
   does nothing. */
int process_wait (tid_t child_tid UNUSED) 
{
//printf("process_wait() called for thread with tid %d \n",child_tid);
/**/
	printLocklist(&swapTable_lock);
  struct thread *t;
  struct list *tid_thread_list = &(thread_current()->child_list);
  if(!list_empty((tid_thread_list)))
  {
  	t = getThreadFromTid(tid_thread_list, child_tid);  
  	//checking for the thread -tid in the tid-thread list
  	if(t == NULL) return -1 ;
  	// return -1 when the thread tid does not exist in the thread tid list 
	 else
  	{
	
	if(t->isLoadedSuccess)       
       {

//if(frame_lock.holder!=NULL)
	//printf("frame lock exit%d\n",frame_lock.holder->tid);
//	if(swapTable_lock.holder!=NULL)
	//printf("swapTable lock exit %d\n",swapTable_lock.holder->tid);
//printf("wait parent before %d\n",thread_current()->tid);

       		sema_down(&(t->childwaitSemaphore)); 		//Parent waits on the child.	
//printf("wait parent after %d\n",thread_current()->tid);
	}
        char *task = t->name ;
        //printf("return status %d for thread with id %d\n",t->set_status,thread_current()->tid);
   	int ret_status = t->set_status;

	printf("%s: exit(%d)\n", task, ret_status);
//if(swapTable_lock!=NULL)
	
	//if(frame_lock.holder!=NULL)
///	printf("frame lock exit%d\n",frame_lock.holder->tid);
//	if(swapTable_lock.holder!=NULL)
//	printf("swapTable lock exit %d\n",swapTable_lock.holder->tid);
        
	list_pop_entry(tid_thread_list, child_tid);	// incorrect as we should pop that thread whose id is child_tid 
	
	//list_remove(&t->elem);
	list_remove(&t->parent_elem);

        //  palloc_free_page (t);			// done to free the child's page 
	return ret_status;
	}
  }
  else
{
//printf("in else\n");
  return -1 ;
}
 /**/
}


void printLocklist(struct lock *lock)
{
	if(list_size(&(lock->semaphore.waiters))<=0)
	return;
	struct list *locklist=&(lock->semaphore.waiters);
	struct thread * t;

	struct list_elem *currElem;
	
	if(!list_empty(locklist))
	{
		for (currElem = ((list_begin (locklist))); currElem != list_end (locklist);currElem = list_next (currElem))
			
			{
				t=list_entry(list_remove(currElem ),struct thread,elem);
				if(t!=NULL)
				printf("thread swapTable Lock %d\n",t->tid);
				
			}
	}
	
}
/* Free the current process's resources. */
void
process_exit (void)
{
//printf("Process_exit tid %d status %d\n",thread_current()->tid,thread_current()->set_status);
lock_acquire(&frame_lock);
  struct thread *cur = thread_current ();
  uint32_t *pd;
  
	
checkMemoryMap();
 freeSuppTable();
  freeFrameTable();




  freeResource(thread_current());

  file_close(thread_current()->fileName);
 
	
  	
  



  /* Destroy the current process's page directory and switch back
     to the kernel-only page directory. */
  pd = cur->pagedir;
  if (pd != NULL) 
    {
      /* Correct ordering here is crucial.  We must set
         cur->pagedir to NULL before switching page directories,
         so that a timer interrupt can't switch back to the
         process page directory.  We must activate the base page
         directory before destroying the process's page
         directory, or our active page directory will be one
         that's been freed (and cleared). */
      cur->pagedir = NULL;
      pagedir_activate (NULL);
      pagedir_destroy (pd);
    }

lock_release(&frame_lock);
}
void freeSuppTable()
{
	void *upage;
	
	
	struct hash_iterator i,j;
	hash_first (&i, &(thread_current()->Supp_Page_Table));
	bool Last = 0 ;
	
	struct SuppPageTable  *Supp=NULL;
struct SuppPageTable  *Supp1=NULL;
	
	while (hash_next (&i))
	{
		Supp1 = hash_entry (hash_cur (&i), struct SuppPageTable  , hash_elem);
		
		if(Last == 1)
		{		
			if(Supp!=NULL)
			{
				//hash_delete(&(thread_current()->Supp_Page_Table), &(Supp->hash_elem));	
				free(Supp);
				Supp=NULL;
			}
			Last = 0;
		}
	Supp=Supp1;
		
		Last = 1;	
	   	
	}
}
/*Method that frees the Frame Table For Reclamation.*/
void freeFrameTable()
{
	struct list *frameTable = &(Frame_Queue);
	struct list_elem *currElem;
	struct FrameToUserPage * f=NULL;
	struct list_elem *remove_elem;
	bool var=false;
	if(!list_empty(frameTable))
	{
		for (currElem = ((list_begin (frameTable))); currElem != list_end (frameTable);currElem = list_next (currElem))
			
			{
				if(f!=NULL&&var==true)
				{
					list_remove(remove_elem);
					free(f);
					
					var=false;
					f=NULL;
				}
				f=list_entry(currElem ,struct FrameToUserPage,queue_elem);
				if(f->thread->tid==thread_current()->tid)
				{
					var=true;
					//f->thread=NULL;
					//f->UserPage_Addr=NULL;
					remove_elem=currElem;
					
					
				}
				
			}
		
	}
}
/* Method to destroy Supplementary Page Table.*/
void destroySuppPageTable()
{
	hash_destroy(&(thread_current()->Supp_Page_Table), NULL);
}
/* Sets up the CPU for running user code in the current
   thread.
   This function is called on every context switch. */
void
process_activate (void)
{
  struct thread *t = thread_current ();
  
  
  /* Activate thread's page tables. */
  pagedir_activate (t->pagedir);

  /* Set thread's kernel stack for use in processing
     interrupts. */
  tss_update ();
}

/* We load ELF binaries.  The following definitions are taken
   from the ELF specification, [ELF1], more-or-less verbatim.  */

/* ELF types.  See [ELF1] 1-2. */
typedef uint32_t Elf32_Word, Elf32_Addr, Elf32_Off;
typedef uint16_t Elf32_Half;

/* For use with ELF types in printf(). */
#define PE32Wx PRIx32   /* Print Elf32_Word in hexadecimal. */
#define PE32Ax PRIx32   /* Print Elf32_Addr in hexadecimal. */
#define PE32Ox PRIx32   /* Print Elf32_Off in hexadecimal. */
#define PE32Hx PRIx16   /* Print Elf32_Half in hexadecimal. */

/* Executable header.  See [ELF1] 1-4 to 1-8.
   This appears at the very beginning of an ELF binary. */
struct Elf32_Ehdr
  {
    unsigned char e_ident[16];
    Elf32_Half    e_type;
    Elf32_Half    e_machine;
    Elf32_Word    e_version;
    Elf32_Addr    e_entry;
    Elf32_Off     e_phoff;
    Elf32_Off     e_shoff;
    Elf32_Word    e_flags;
    Elf32_Half    e_ehsize;
    Elf32_Half    e_phentsize;
    Elf32_Half    e_phnum;
    Elf32_Half    e_shentsize;
    Elf32_Half    e_shnum;
    Elf32_Half    e_shstrndx;
  };

/* Program header.  See [ELF1] 2-2 to 2-4.
   There are e_phnum of these, starting at file offset e_phoff
   (see [ELF1] 1-6). */
struct Elf32_Phdr
  {
    Elf32_Word p_type;
    Elf32_Off  p_offset;
    Elf32_Addr p_vaddr;
    Elf32_Addr p_paddr;
    Elf32_Word p_filesz;
    Elf32_Word p_memsz;
    Elf32_Word p_flags;
    Elf32_Word p_align;
  };

/* Values for p_type.  See [ELF1] 2-3. */
#define PT_NULL    0            /* Ignore. */
#define PT_LOAD    1            /* Loadable segment. */
#define PT_DYNAMIC 2            /* Dynamic linking info. */
#define PT_INTERP  3            /* Name of dynamic loader. */
#define PT_NOTE    4            /* Auxiliary info. */
#define PT_SHLIB   5            /* Reserved. */
#define PT_PHDR    6            /* Program header table. */
#define PT_STACK   0x6474e551   /* Stack segment. */

/* Flags for p_flags.  See [ELF3] 2-3 and 2-4. */
#define PF_X 1          /* Executable. */
#define PF_W 2          /* Writable. */
#define PF_R 4          /* Readable. */

static bool setup_stack (void **esp);
static bool validate_segment (const struct Elf32_Phdr *, struct file *);
static bool load_segment (struct file *file, off_t ofs, uint8_t *upage,
                          uint32_t read_bytes, uint32_t zero_bytes,
                          bool writable);

/* Loads an ELF executable from FILE_NAME into the current thread.
   Stores the executable's entry point into *EIP
   and its initial stack pointer into *ESP.
   Returns true if successful, false otherwise. */
bool
load (const char *file_name, void (**eip) (void), void **esp) 
{
  struct thread *t = thread_current ();
  struct Elf32_Ehdr ehdr;
  struct file *file = NULL;
  off_t file_ofs;
  bool success = false;
  int i;

  /* Allocate and activate page directory. */
  t->pagedir = pagedir_create ();
  if (t->pagedir == NULL) 
    goto done;
  process_activate ();
  /* Open executable file. */
  file = filesys_open (file_name);

  if (file == NULL) 
    {
      printf ("load: %s: open failed\n", file_name);
      goto done; 
    }

  /* Read and verify executable header. */
  if (file_read (file, &ehdr, sizeof ehdr) != sizeof ehdr
      || memcmp (ehdr.e_ident, "\177ELF\1\1\1", 7)
      || ehdr.e_type != 2
      || ehdr.e_machine != 3
      || ehdr.e_version != 1
      || ehdr.e_phentsize != sizeof (struct Elf32_Phdr)
      || ehdr.e_phnum > 1024) 
    {
      printf ("load: %s: error loading executable\n", file_name);
      goto done; 
    }

  /* Read program headers. */
  file_ofs = ehdr.e_phoff;
  for (i = 0; i < ehdr.e_phnum; i++) 
    {
      struct Elf32_Phdr phdr;

      if (file_ofs < 0 || file_ofs > file_length (file))
        goto done;
      file_seek (file, file_ofs);

      if (file_read (file, &phdr, sizeof phdr) != sizeof phdr)
        goto done;
      file_ofs += sizeof phdr;
      switch (phdr.p_type) 
        {
        case PT_NULL:
        case PT_NOTE:
        case PT_PHDR:
        case PT_STACK:
        default:
          /* Ignore this segment. */
          break;
        case PT_DYNAMIC:
        case PT_INTERP:
        case PT_SHLIB:
          goto done;
        case PT_LOAD:
          if (validate_segment (&phdr, file)) 
            {
              bool writable = (phdr.p_flags & PF_W) != 0;
              uint32_t file_page = phdr.p_offset & ~PGMASK;
              uint32_t mem_page = phdr.p_vaddr & ~PGMASK;
              uint32_t page_offset = phdr.p_vaddr & PGMASK;
              uint32_t read_bytes, zero_bytes;
              if (phdr.p_filesz > 0)
                {
                  /* Normal segment.
                     Read initial part from disk and zero the rest. */
                  read_bytes = page_offset + phdr.p_filesz;
                  zero_bytes = (ROUND_UP (page_offset + phdr.p_memsz, PGSIZE)
                                - read_bytes);
                }
              else 
                {
                  /* Entirely zero.
                     Don't read anything from disk. */
                  read_bytes = 0;
                  zero_bytes = ROUND_UP (page_offset + phdr.p_memsz, PGSIZE);
                }
              if (!load_segment (file, file_page, (void *) mem_page,
                                 read_bytes, zero_bytes, writable))
                goto done;
            }
          else
            goto done;
          break;
        }
    }

  /* Set up stack. */
  if (!setup_stack (esp))
    goto done;

  /* Start address. */
  *eip = (void (*) (void)) ehdr.e_entry;
//  printf("eip %p\n",*eip);

  success = true;
  thread_current()->fileName = file ;
  file_deny_write(file);
 done:
  /* We arrive here whether the load is successful or not. */
  //file_close (file);
  if(!success) file_close (file);
  return success;
}

/* load() helpers. */

static bool install_page (void *upage, void *kpage, bool writable);

/* Checks whether PHDR describes a valid, loadable segment in
   FILE and returns true if so, false otherwise. */
static bool
validate_segment (const struct Elf32_Phdr *phdr, struct file *file) 
{
  /* p_offset and p_vaddr must have the same page offset. */
  if ((phdr->p_offset & PGMASK) != (phdr->p_vaddr & PGMASK)) 
    return false; 

  /* p_offset must point within FILE. */
  if (phdr->p_offset > (Elf32_Off) file_length (file)) 
    return false;

  /* p_memsz must be at least as big as p_filesz. */
  if (phdr->p_memsz < phdr->p_filesz) 
    return false; 

  /* The segment must not be empty. */
  if (phdr->p_memsz == 0)
    return false;
  
  /* The virtual memory region must both start and end within the
     user address space range. */
  if (!is_user_vaddr ((void *) phdr->p_vaddr))
    return false;
  if (!is_user_vaddr ((void *) (phdr->p_vaddr + phdr->p_memsz)))
    return false;

  /* The region cannot "wrap around" across the kernel virtual
     address space. */
  if (phdr->p_vaddr + phdr->p_memsz < phdr->p_vaddr)
    return false;

  /* Disallow mapping page 0.
     Not only is it a bad idea to map page 0, but if we allowed
     it then user code that passed a null pointer to system calls
     could quite likely panic the kernel by way of null pointer
     assertions in memcpy(), etc. */
  if (phdr->p_vaddr < PGSIZE)
    return false;

  /* It's okay. */
  return true;
}

/* Loads a segment starting at offset OFS in FILE at address
   UPAGE.  In total, READ_BYTES + ZERO_BYTES bytes of virtual
   memory are initialized, as follows:

        - READ_BYTES bytes at UPAGE must be read from FILE
          starting at offset OFS.

        - ZERO_BYTES bytes at UPAGE + READ_BYTES must be zeroed.

   The pages initialized by this function must be writable by the
   user process if WRITABLE is true, read-only otherwise.

   Return true if successful, false if a memory allocation error
   or disk read error occurs. */
static bool
load_segment (struct file *file, off_t ofs, uint8_t *upage,
              uint32_t read_bytes, uint32_t zero_bytes, bool writable) 
{
  ASSERT ((read_bytes + zero_bytes) % PGSIZE == 0);
  ASSERT (pg_ofs (upage) == 0);
  ASSERT (ofs % PGSIZE == 0);

  file_seek (file, ofs);
  int Offset=ofs;
  while (read_bytes > 0 || zero_bytes > 0) 
    {
      /* Calculate how to fill this page.
         We will read PAGE_READ_BYTES bytes from FILE
         and zero the final PAGE_ZERO_BYTES bytes. */
      size_t page_read_bytes = read_bytes < PGSIZE ? read_bytes : PGSIZE;
      size_t page_zero_bytes = PGSIZE - page_read_bytes;

      /* Get a page of memory. */
     //  uint8_t *kpage = palloc_get_page (PAL_USER);
      struct SuppPageTable *HashStruct = malloc(sizeof(struct SuppPageTable));
		HashStruct->UserPage_VAddr=upage;
		(HashStruct->File_Info).file=file;
		(HashStruct->File_Info).Actual_Offset=Offset;
		(HashStruct->File_Info).page_read_bytes=page_read_bytes;
		(HashStruct->File_Info).page_zero_bytes=page_zero_bytes;
		(HashStruct->File_Info).writable=writable;
		HashStruct->isSwap=false;
		hash_insert(&(thread_current()->Supp_Page_Table), &(HashStruct->hash_elem));
		
	//	printf("Hash Insert upage %p \n",upage);
	
      /*if (kpage == NULL)
        {
		read_bytes -= page_read_bytes;
      		zero_bytes -= page_zero_bytes;
      		upage += PGSIZE;
		continue;
		
		//return false;
	}*/
      /* Load this page. */
      /*if (file_read (file, kpage, page_read_bytes) != (int) page_read_bytes)
        {
          palloc_free_page (kpage);
          return false; 
        }
      memset (kpage + page_read_bytes, 0, page_zero_bytes);

      /* Add the page to the process's address space. */
      /*if (!install_page (upage, kpage, writable)) 
        {
          palloc_free_page (kpage);
          return false; 
        }*/

      /* Advance. */
      read_bytes -= page_read_bytes;
      zero_bytes -= page_zero_bytes;
      upage += PGSIZE;
      Offset += PGSIZE;
    }
  return true;
}

/* Create a minimal stack by mapping a zeroed page at the top of
   user virtual memory. */
static bool
setup_stack (void **esp) 
{
	lock_acquire(&frame_lock);
  uint8_t *kpage;
  bool success = false;

  kpage = palloc_get_page (PAL_USER | PAL_ZERO);
  if (kpage != NULL) 
    {
      success = install_page (((uint8_t *) PHYS_BASE) - PGSIZE, kpage, true);
      if (success)
	{
	//printf("EROOR\n");
        *esp = PHYS_BASE ;
	thread_current()->Stack_Bottom=PHYS_BASE-PGSIZE;
	}
      else
        palloc_free_page (kpage);
    }
lock_release(&frame_lock);
  return success;
}


void PageFault_Stack()
{
	lock_acquire(&frame_lock);
  	uint8_t *kpage;
 	bool success = false;
	kpage = palloc_get_page (PAL_USER | PAL_ZERO);
  	if (kpage != NULL) 
    	{
      		success = install_page (thread_current()->Stack_Bottom - PGSIZE, kpage, true);
      		if (success)
		{
			//printf("EROOR\n");
        		//*esp = PHYS_BASE ;
			thread_current()->Stack_Bottom=thread_current()->Stack_Bottom-PGSIZE;
		}
      		else
        		palloc_free_page (kpage);
    	}
lock_release(&frame_lock);
 // return success;

}

/* Adds a mapping from user virtual address UPAGE to kernel
   virtual address KPAGE to the page table.
   If WRITABLE is true, the user process may modify the page;
   otherwise, it is read-only.
   UPAGE must not already be mapped.
   KPAGE should probably be a page obtained from the user pool
   with palloc_get_page().
   Returns true on success, false if UPAGE is already mapped or
   if memory allocation fails. */
static bool
install_page (void *upage, void *kpage, bool writable)
{


  struct thread *t = thread_current ();

  /* Verify that there's not already a page at that virtual
     address, then map our page there. */
 // return (pagedir_get_page (t->pagedir, upage) == NULL
   //       && pagedir_set_page (t->pagedir, upage, kpage, writable));
//printf("frame lock acquire %d \n",thread_current()->tid);

//printf("frame lock %d \n",thread_current()->tid);
  if(pagedir_get_page (t->pagedir, upage) == NULL&& pagedir_set_page (t->pagedir, upage, kpage, writable))
	{
		
		/*struct FrameToUserPage *HashStruct=malloc(sizeof(struct FrameToUserPage));
		
		HashStruct->Frame_Addr=kpage;
		HashStruct->UserPage_Addr=upage;
		HashStruct->thread = thread_current(); //Storing current thread in along with the frame
		//hash_insert(&Frame_Table_Hash, &(HashStruct->hash_elem));
		list_push_back(&Frame_Queue,&(HashStruct->queue_elem));*/


if(upage<PHYS_BASE&&upage>=PHYS_BASE-PGSIZE);
else
EntryCheck(kpage,upage);



		
//printf("frame lock release%d \n",thread_current()->tid);
//printf("no error\n");
		return 1;
	}else{

//printf("error\n");
//printf("frame lock release%d \n",thread_current()->tid);
		return 0;
	}

//return pagedir_get_page (t->pagedir, upage) == NULL&& pagedir_set_page (t->pagedir, upage, kpage, writable);
}

void freeResource(struct thread *t)
{
struct list *descriptorTable = &(t->descriptor_table);
struct list_elem *currElem, prevElem ;
struct descriptorMap *dm = malloc(sizeof(struct descriptorMap));
struct file *f;
for (currElem = ((list_begin (descriptorTable))); currElem != list_end (descriptorTable);currElem = list_next (currElem))
	{ 
          	free(dm) ;     
		dm = list_entry (currElem, struct descriptorMap, elem);
		if(dm == NULL) return;
		else 
		f = dm->fileName;
		
		if(f != NULL)
		{	
			file_close(f);
			//if(list_begin(descriptorTable)  != currElem)
		//	list_remove(list_prev(currElem));
				
		}	
		else break;
	}
free(dm);
}

void Page_FileSys(struct SuppPageTable *Supp_Table_Entry)
{

	if (Supp_Table_Entry->File_Info.page_read_bytes > 0 || Supp_Table_Entry->File_Info.page_zero_bytes > 0)
	{
		uint8_t *kpage = palloc_get_page (PAL_USER);

		if(kpage==NULL)
		{
				printf("NULL\n");
			
			
		}
		else
		{

			if(!Supp_Table_Entry->isSwap)
			{

				//file_seek(Supp_Table_Entry->File_Info.file,Supp_Table_Entry->File_Info.Actual_Offset);
				if (file_read_at (Supp_Table_Entry->File_Info.file, kpage, Supp_Table_Entry->File_Info.page_read_bytes,Supp_Table_Entry->File_Info.Actual_Offset) != (int) Supp_Table_Entry->File_Info.page_read_bytes)
        			{

          				palloc_free_page (kpage);
          				return ; 
        			}

      				memset (kpage + Supp_Table_Entry->File_Info.page_read_bytes, 0, Supp_Table_Entry->File_Info.page_zero_bytes);
//printf("Not In swap %p %p\n",kpage,Supp_Table_Entry->UserPage_VAddr);
			}else{

				//printf("disk read\n");
				

				Disk_Read(kpage,(Supp_Table_Entry->File_Info).Sector);
				//bitmap_set_multiple (Swap_Bit_Map, 0+(Supp_Table_Entry->File_Info).Sector/8, 1, false);
			
				Supp_Table_Entry->isSwap=false;
				
			
				//uint32_t *pd, void *upage, void *kpage, bool rw);
				
			}


      			/* Add the page to the process's address space. */
      			if (!install_page (Supp_Table_Entry->UserPage_VAddr, kpage, Supp_Table_Entry->File_Info.writable)) 
        		{
				  thread_current()->set_status= - 1;
lock_release(&frame_lock);     
	
 	 thread_exit();  
   			
				palloc_free_page (kpage);
          			return; 
        		}

			//pagedir_set_accessed(thread_current()->pagedir,Supp_Table_Entry->UserPage_VAddr,true);
			
		}

	}

	 
}

void File_Write_Swap(struct file* filePointer,int Offset,int page_read_bytes,int page_zero_bytes,struct SuppPageTable *Ptr)
{

	uint8_t *kpage = palloc_get_page (0);
	if (file_read_at (filePointer, kpage, page_read_bytes,Offset) != (int) page_read_bytes)
        {
          palloc_free_page (kpage);
          //return false; 
        }
	
      	memset (kpage + page_read_bytes, 0, page_zero_bytes);
	struct SwapToUserPage HashStruct;

	
	size_t page_idx=bitmap_scan_and_flip (Swap_Bit_Map, 0, 1, false);

	CurrSector=0+page_idx*8;

	HashStruct.Sector=CurrSector; 
//printf("swap entrer tid %d\n",thread_current()->tid);
	//hash_insert(&Swap_Table_Hash,&(HashStruct.hash_elem));
//printf("swap exit\n");

	(Ptr->File_Info).Sector=CurrSector;


	Disk_Write(kpage);

	palloc_free_page (kpage);

	
}

void Disk_Read(uint8_t *kpage,int Start_Sector)
{
	if(thread_current()->tid>=3)
	{
		//printf("swapTable lock acquire %d \n",thread_current()->tid);
	
		
		//printf("swapTable lock %d \n",thread_current()->tid);
		bitmap_set_multiple (Swap_Bit_Map, Start_Sector/8, 1, false);
		//printf("Disk Read\n");
		int i=0;
		for(i=0;i<8;i++)
		{
			disk_read(swap_disk,Start_Sector++,kpage+512*i);
		}
		
		//printf("swapTable lock release %d \n",thread_current()->tid);
	}
}

void Disk_Write(uint8_t *kpage)
{
	int i=0;
	//uint8_t *ppage = palloc_get_page (0);
	//printf("Disk_Write\n");
	if(kpage!=NULL)
	for(i=0;i<8;i++)
	{
		//printf("content %s \n",kpage);
		disk_write(swap_disk,CurrSector,kpage+512*i);
		
		
		CurrSector++;	
	}
}

void checkMemoryMap()
{
	void *upage;
	void *filePointer;
	int read_bytes,zero_bytes;
	struct hash_iterator i;
	hash_first (&i, &(thread_current()->memoryMapTable));
	bool Last = 0 ;
	struct FileMap  *fileMap=NULL;
	uint8_t *kpage = palloc_get_page (0);
	
	while (hash_next (&i))
	{
		if(Last == 1)
		{
			if(fileMap!=NULL)
			{
			hash_delete(&(thread_current()->memoryMapTable), &(fileMap->hash_elem));
			free(fileMap);
			}
			Last = 0;
		}
		fileMap = hash_entry (hash_cur (&i), struct FileMap  , hash_elem);
		
		void *fileVirtualAddress = fileMap->fileVirtualAddress ;
  		filePointer = getFilePointerFromDescriptorTable(fileMap->fileDescriptor);
  		/* we need to remove the  entries from the page table. */
 
	
 	 	int fileLength = fileMap->fileLength;
  		fileVirtualAddress = pg_round_down(fileVirtualAddress);
  		upage=fileVirtualAddress;
  		read_bytes = fileLength ;
  		zero_bytes = (4096 - (read_bytes % 4096 ))%4096;
  		while (read_bytes > 0 || zero_bytes > 0) 
    		{
      		/* Calculate how to fill this page.
         	We will read PAGE_READ_BYTES bytes from FILE
         	and zero the final PAGE_ZERO_BYTES bytes. */
   		size_t    page_read_bytes = read_bytes < PGSIZE ? read_bytes : PGSIZE;
   		size_t     page_zero_bytes = PGSIZE - page_read_bytes;

      		/* Updating the S-Page Table. */
                struct SuppPageTable *HashStruct =page_lookup (upage,&(thread_current()->Supp_Page_Table));
		
		if(pagedir_is_dirty(thread_current()->pagedir,upage)&&filePointer!=NULL)
		{
			if(!HashStruct->isSwap)
			{
				//printf("FILE WRITE AT%d\n",file_write_at(filePointer,pagedir_get_page(thread_current()->pagedir,upage),page_read_bytes,(HashStruct->File_Info).Actual_Offset));
				file_write_at(filePointer,upage,page_read_bytes,(HashStruct->File_Info).Actual_Offset);
			}
			else
			{
				Disk_Read(kpage,(HashStruct->File_Info).Sector);
				file_write_at(filePointer,kpage,page_read_bytes,(HashStruct->File_Info).Actual_Offset);
				
				
			}
		}
		//struct hash_elem *Helm=hash_delete(&(thread_current()->Supp_Page_Table), &(HashStruct->hash_elem));
		//struct SuppPageTable *Ptr=hash_entry (Helm, struct SuppPageTable, hash_elem);
		//free(Ptr);
		//palloc_free_page(Ptr->);
		//pagedir_clear_page (thread_current()->pagedir, upage);
		
		read_bytes -= page_read_bytes;
      		zero_bytes -= page_zero_bytes;
      		upage += PGSIZE;
      	
    		}

		Last = 1;	
	   		
	}
	palloc_free_page(kpage);
}


bool Overlap_Check(int read_bytes,int zero_bytes,void *upage)
{

 
  while (read_bytes > 0 || zero_bytes > 0) 
    {
      
   		size_t    page_read_bytes = read_bytes < PGSIZE ? read_bytes : PGSIZE;
   		size_t     page_zero_bytes = PGSIZE - page_read_bytes;
		 
		if(page_lookup(upage,&(thread_current()->Supp_Page_Table))!=NULL)
		{
			
			return false;
		}

     
                
		
		read_bytes -= page_read_bytes;
      		zero_bytes -= page_zero_bytes;
      		upage += PGSIZE;
      		
    }
return true;
}

uint8_t * Evict_Page()
{
	//printf("Evicting \n");
	//lock_acquire(&frame_lock);
	struct FrameToUserPage* FrameTableStruct=SecondChanceAlgo();
	


	uint8_t * Evict_Frame=FrameTableStruct->Frame_Addr;
//printf("eviction %p %p \n",Evict_Frame,FrameTableStruct->UserPage_Addr);
/*if(FrameTableStruct->UserPage_Addr==0x804b000)
{char *buf;
buf=0x804be00;
printf("value %p\n", *buf);
}*/
//Evict_Write_Swap(pagedir_get_page(FrameTableStruct->thread->pagedir,FrameTableStruct->UserPage_Addr),&FrameTableStruct->thread->Supp_Page_Table);
	if(FrameTableStruct->thread!=NULL)
	{
	struct SuppPageTable *Ptr=page_lookup (FrameTableStruct->UserPage_Addr,&(FrameTableStruct->thread->Supp_Page_Table));
//printf("swapTable lock acquire %d \n",thread_current()->tid);	

//lock_acquire(&swapTable_lock);
		//printf("swapTable lock %d \n",thread_current()->tid);
	Evict_Write_Swap(Evict_Frame,Ptr);
pagedir_clear_page(FrameTableStruct->thread->pagedir,FrameTableStruct->UserPage_Addr);



	}
list_remove(&(FrameTableStruct->queue_elem));
free(FrameTableStruct);

	
	
//printf("swapTable lock release %d \n",thread_current()->tid);

	//lock_release(&swapTable_lock);
	

	//palloc_free_page(Evict_Frame);
	
	//Evict_Frame=palloc_get_page (PAL_USER);

//lock_release(&frame_lock);
	return Evict_Frame;
		
}
struct FrameToUserPage* SecondChanceAlgo()
{
	
	struct list_elem *frontElem=list_front(&Frame_Queue);
	
	struct FrameToUserPage* frontFrameTableStruct =list_entry(frontElem,struct FrameToUserPage,queue_elem);

	struct FrameToUserPage* FrameTableStruct=frontFrameTableStruct;
	struct FrameToUserPage* FrameTableEvicted;
	
	do
	{
		if(FrameTableStruct->thread!=NULL)
		{
		if(pagedir_is_accessed (FrameTableStruct->thread->pagedir,FrameTableStruct->UserPage_Addr))
		{
			pagedir_set_accessed(FrameTableStruct->thread->pagedir,FrameTableStruct->UserPage_Addr,false);
			list_push_back(&Frame_Queue,list_pop_front(&Frame_Queue));
			FrameTableStruct=list_entry(list_front(&Frame_Queue),struct FrameToUserPage,queue_elem);
			
		}

		else
		{
			
		//	hash_delete(&Frame_Table_Hash,FrameTableEvicted->hash_elem);
			break;
			
		}
		}
		else
		{
			break;
		}
	}while(FrameTableStruct!=frontFrameTableStruct);
	//FrameTableEvicted=list_entry(list_front(&Frame_Queue),struct FrameToUserPage,queue_elem);
	//FrameTableStruct=FrameTableEvicted;


return (struct FrameToUserPage*)FrameTableStruct;
}


void Evict_Write_Swap(uint8_t *kpage,struct SuppPageTable *Ptr)
{

	struct SwapToUserPage HashStruct;
//	sema_down(&disk_sema);
	size_t page_idx=bitmap_scan_and_flip (Swap_Bit_Map, 0, 1, false);
	CurrSector=0+page_idx*8;
	HashStruct.Sector=CurrSector; 
	//hash_insert(&Swap_Table_Hash,&(HashStruct.hash_elem));
	(Ptr->File_Info).Sector=CurrSector;
	Ptr->isSwap=true;
	Disk_Write(kpage);
//	sema_up(&disk_sema);
}


void EntryCheck(void *kpage,void *upage)
{
struct list *frameTable = &(Frame_Queue);
struct list_elem *currElem;
if(!list_empty(frameTable))
{
	for (currElem = list_begin (frameTable); currElem != list_end (frameTable);currElem = list_next (currElem))
	{
		if(list_entry(currElem,struct FrameToUserPage,queue_elem)->Frame_Addr==kpage)
			{
				list_entry(currElem,struct FrameToUserPage,queue_elem)->UserPage_Addr=upage;
				list_entry(currElem,struct FrameToUserPage,queue_elem)->thread = thread_current(); 
				list_entry(currElem,struct FrameToUserPage,queue_elem)->Frame_Addr=kpage;
				return;
			}
	}
}
struct FrameToUserPage *HashStruct=malloc(sizeof(struct FrameToUserPage));
HashStruct->Frame_Addr=kpage;
HashStruct->UserPage_Addr=upage;
HashStruct->thread = thread_current(); //Storing current thread in along with the frame
//hash_insert(&Frame_Table_Hash, &(HashStruct->hash_elem));
list_push_back(&Frame_Queue,&(HashStruct->queue_elem));
return; 

}
 void printfadd(void *add,void *eip,void *esp)
{
	printf("faultadd %p eip %p Stack Pointer %p %d\n",add,eip,esp,thread_current()->tid);
}

void PageFaultBad(struct list * rdqueue)
{


struct list_elem *currElem;
if(!list_empty(rdqueue)&&rdqueue!=NULL)
{
	for (currElem = list_begin (rdqueue); currElem != list_end (rdqueue);currElem = list_next (currElem))
	{
		printf("thread tid %d \n",list_entry(currElem,struct thread,elem)->tid);
			
	}

}
	

	//printf("Bad\n");
}
