#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "vm/page.h"
#include "vm/memoryMap.h"
#include "userprog/gdt.h"
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/vaddr.h"
#include "vm/memoryMap.h"
#include "vm/swap.h"
#include "vm/frame.h"
#include <string.h>
#include "filesys/directory.h"
#include <stdio.h>
#include <string.h>
#include <list.h>
#include "filesys/filesys.h"
#include "filesys/inode.h"
#include "threads/malloc.h"
#include "filesys/inode.h"

#define PGSIZE 4096
#define EXIT_SUCCESS 0
#define PHYS_BASE 0xc0000000

//bool getParentDirectory (char *path, struct dir **parent_dir, char **newDirectory_name);
static void syscall_handler (struct intr_frame *);

static int
get_user (const uint8_t *uaddr)
{
kernel_Fault_Type = 0; 
int result;
asm ("movl $1f, %0; movzbl %1, %0; 1:"
: "=&a" (result) : "m" (*uaddr));
return result;
}

static bool
put_user (uint8_t *udst, uint8_t byte)
{
int error_code;
asm ("movl $1f, %0; movb %b2, %1; 1:"
: "=&a" (error_code), "=m" (*udst) : "q" (byte));
return error_code != -1;
}


void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
  struct FileMap *fMap;
  struct file *filePointer;
  struct dir *dirPointer ;
  int retStatus, fileDescriptor, childPid,  retVal ; 
  unsigned sizeOfBuffer ;
  void *buffer ;
  void *userStack = f->esp ;
  struct descriptorMap *dMap;
  char *directoryName = NULL ;
    struct inode *inode ;
    struct dir *root_dir, *sub_dir, *parent_dir, *cur_dir  ;
    struct dir_entry *ep; off_t *ofsp ;
    disk_sector_t sector, sector_allocated;
    int sector_idx = 0;
    bool y = false ;
    char *pathName,  *path, *fileName ;

  if(userStack > PHYS_BASE - 5)
{
  thread_current()->set_status=-1;
		//freeResource(thread_current());
		thread_exit();//we need to free the thread's resources before exiting it.
} 
 if(get_user(userStack)==-1)
	{
		thread_current()->set_status=-1;
		//freeResource(thread_current());
		thread_exit();
	}
  int systemCallNumber = *((int *)userStack);
  #ifdef testing
  printf("systemCallNumber = %d\n",systemCallNumber);
  #endif	
  switch(systemCallNumber)
  {
   case SYS_HALT:
   break;
   case SYS_EXIT:
//printf("asd\n");
  // printf("list %d\n",list_size(&thread_current()->descriptor_table));
   userStack = userStack + sizeof(int) ;
   retStatus = *(int *)userStack ;
   //IF PARENT WAITS ON IT THEN THE CHILD HAS TO RETURN THE retStatus to the parent -- ??
   if(retStatus == EXIT_SUCCESS);	 	
   #ifdef testing
   printf("exiting successfully\n");
   #endif
   
   thread_current()->set_status=retStatus;
/**/
//freeResource(thread_current());
/**/		
   //freeResource(thread_current());
   thread_exit();
   
   
   break;
   case SYS_WRITE:   
   #ifdef testing
   printf("in system write\n");
   #endif	

   userStack = userStack + sizeof(int);
   fileDescriptor = *(int *)userStack ;
   userStack = userStack + sizeof(void *);
   
   buffer = *(void **)userStack ;
   userStack = userStack + sizeof(unsigned);
   sizeOfBuffer = *(int *)userStack ;
  
   if(sizeOfBuffer == 0)
   {    
     f->eax = 0;
     return;
   }
   if(get_user(buffer) == -1 || fileDescriptor == 0)
   {
    thread_current()->set_status= - 1;	
 	 thread_exit();
   }
   if(fileDescriptor == 1)
   putbuf(buffer, sizeOfBuffer);
   else
   {  

   dMap = getDescriptorMapPointerFromDescriptorTable(fileDescriptor); 
   if(dMap == NULL)
   {
         thread_current()->set_status= - 1;
 	 thread_exit();
   }
   if (dMap->is_File == false)
	{
	    f->eax = -1 ;	
	    return ;	
	}
  filePointer = dMap->fileName ;
  if(filePointer == NULL)
   {
         thread_current()->set_status= - 1;
 	 thread_exit();
   }

    retVal = file_write (filePointer, buffer, sizeOfBuffer) ;
    //printf("retval is %d, sizeOfBuffer %d\n",retVal, sizeOfBuffer);
   }
    #ifdef testing
    printf("size %u\n",sizeOfBuffer);			
    printf("%s",(char *)buffer);	
    printf("fileDescriptor %d\n",fileDescriptor);	
   #endif	
    f->eax = retVal ;
   

   break;

   case SYS_WAIT:
   userStack = userStack + sizeof(int);
   childPid = *(int *)userStack ; 
   f->eax =  process_wait(childPid);
  // Check if the process with ID = pid is still alive -- wait if it is alive
  // if the child process terminated but did not call exit return -1 
   break; 

   case SYS_EXEC:

   userStack = userStack + sizeof(char *);
   char *executableFileName = *(char **)userStack ;
     if(get_user(executableFileName) == -1)
    {
     return ;
     }
   int childTid = process_execute (executableFileName);
   if(childTid == -1)  
	{
		f->eax = -1 ;
	}
   else f->eax =  childTid ;
   break;

   case SYS_OPEN:
   userStack = userStack + sizeof(char *);
   char *path = *(char **)userStack ;			// Now It has become the path name... 
   bool is_File;	
   struct dir *dir_, *pdir;

   if(get_user(path) == -1  ||    (path >= PHYS_BASE))
   {
	 thread_current()->set_status = - 1;
         thread_exit();	
 	 return;
    }
          if (strcmp(path,"") == 0)
         {
		f->eax = -1;
		return ;
         }
    struct descriptorMap *desMap = malloc(sizeof(struct descriptorMap)); 
   // Identifying whether a file or a directory is being opened
	char *name_ = malloc ((NAME_MAX+1) * sizeof(char));
        if (strcmp(path,"/") == 0)
	{         
	   
	   dir_ = dir_open_root() ;
	  
	   is_File = false;
	}
	else
	{
        bool y_ = checkPath (path, &pdir, (name_));
        if (!y_)
	  {
		free (name_);	
		free (desMap);
		return false;
	  } 
	struct inode *inode_ = NULL ;	
        if (pdir != NULL) 	
        dir_lookup (pdir, name_, &inode_);
	if(inode_ != NULL)		// Case that the current path corresponds to a directory
	{
		is_File =  getIsFile(inode_);
		dir_ = dir_open (inode_);
		
	}
        }
	if (is_File == false)
	{	
		desMap->dir = dir_ ;
	        desMap->inode = dir_get_inode(dir_);	 
	}
      else
	{ 
   		struct file *fileP = filesys_open(path);
               
   		if(fileP == NULL) 
  		 {
		   f->eax = -1;
		   free (desMap);	
   		   return ;
  		 }
	    desMap->fileName = fileP ;
	    is_File = true ;	
	    desMap->inode = file_get_inode(fileP) ;	
	}
   int descriptorNumber = ++(thread_current()->availableDescriptor) ;
   desMap->fd = descriptorNumber;
   desMap->is_File = is_File;
   
   list_push_front(&(thread_current()->descriptor_table), &(desMap->elem));
   f->eax = descriptorNumber ;
   free (name_);
   break;

   case SYS_CREATE:

   userStack = userStack + sizeof(char *);
   path = *(char **)userStack ;
   if((path) == NULL || get_user (path) == -1)
   {
 	thread_current()->set_status= - 1;	
 	thread_exit();
   }
  
   userStack = userStack + sizeof(unsigned);
   unsigned fileSize = *(unsigned *)userStack ;
   bool retValue = filesys_create (path, fileSize);
   f->eax = retValue ;

   break ;

 case SYS_READ:
 retVal = 0 ;
 userStack = userStack + sizeof(int);
 fileDescriptor = *(int *)userStack ;
 userStack = userStack + sizeof(void *);
 buffer = *(void **)userStack ;
 userStack = userStack + sizeof(unsigned);
 sizeOfBuffer = *(int *)userStack ;
 if(sizeOfBuffer != 0)
 {
 if(buffer >= PHYS_BASE)
 {
	
    thread_current()->set_status = -1;	
    thread_exit();
  }
 else
 {
 filePointer = getFilePointerFromDescriptorTable(fileDescriptor); 
 if(filePointer == NULL)
   {
    thread_current()->set_status = -1;	
    thread_exit();
   }
if(get_user(buffer)==-1)
{
thread_current()->set_status = -1;	
    thread_exit();
	
}

 retVal = file_read (filePointer, buffer, sizeOfBuffer) ;

if(retVal!=sizeOfBuffer)
{
thread_current()->set_status = -1;	
    thread_exit();
	
}

 
  }
 }
 f->eax = retVal ;
 break;

 case SYS_CLOSE:

 userStack = userStack + sizeof(int);
 fileDescriptor = *(int *)userStack ;
 if(fileDescriptor == 0  || fileDescriptor == 1)
 { 
    thread_current()->set_status = -1;	
    thread_exit();	
  }
 struct descriptorMap *dMap_ = removeFilePointerFromDescriptorTable(fileDescriptor); 
  if(dMap_ == NULL)
   {
    thread_current()->set_status = -1;
    thread_exit();
   }
  if(dMap_->is_File)
    file_close(dMap_->fileName);
   else	
    dir_close (dMap_->dir);
  return ;
 
 break;

 case SYS_TELL:
 userStack = userStack + sizeof(int);
 fileDescriptor = *(int *)userStack ;
 if(fileDescriptor == 0  || fileDescriptor == 1)
 { 
    thread_exit();	
  }
 filePointer = getFilePointerFromDescriptorTable(fileDescriptor); 
 if(filePointer == NULL)
   {
    thread_current()->set_status = -1;
    thread_exit();
   }
 f->eax = file_tell (filePointer) ;
 break;

 case SYS_FILESIZE:
 userStack = userStack + sizeof(int);
 fileDescriptor = *(int *)userStack ;
 filePointer = getFilePointerFromDescriptorTable(fileDescriptor); 
 if(filePointer == NULL)
   {
    thread_current()->set_status = -1;	
  //  freeResource(thread_current());
    thread_exit();
   }
 f->eax = file_length(filePointer);
  break;
 
 case SYS_SEEK:
  userStack = userStack + sizeof(int);
 fileDescriptor = *(int *)userStack ;
 userStack = userStack + sizeof(unsigned);
 unsigned position = *(unsigned *)userStack ;
 filePointer = getFilePointerFromDescriptorTable(fileDescriptor); 
 if(filePointer == NULL)
   {
    thread_current()->set_status = -1;
   // freeResource(thread_current());	
    thread_exit();
   } 
 file_seek(filePointer,position );
 break; 

/* Implementation Of Memory Map System Call */
  case SYS_MMAP:
  userStack = userStack + sizeof(int);
  fileDescriptor = *(int *)userStack ;
  userStack = userStack + sizeof(void *);
  buffer = *(void **)userStack ;
  if(buffer!=pg_round_down(buffer))
	{
		f->eax=-1;
		return;
	}
  if((buffer>=0x8048000 && buffer <=0x807ad24)||(buffer>=(PHYS_BASE - PGSIZE)&&buffer<=PHYS_BASE)|| buffer==NULL)
	{
		f->eax=-1;
		return;
	}
  
  struct thread *currentThread = thread_current();
  fMap = malloc(sizeof (struct FileMap));
  fMap->fileVirtualAddress = buffer ;
  
  fMap->fileDescriptor = fileDescriptor ;
  filePointer = getFilePointerFromDescriptorTable(fileDescriptor);
  if(filePointer==NULL)
	{
		f->eax = -1 ;
		return;
	} 
 int length = file_length(filePointer);
  fMap->fileLength=length;
  void *upage = pg_round_down(buffer);
  int Offset =  filePointer->pos ;

  size_t read_bytes = length ;
  size_t zero_bytes = (4096 - (read_bytes % 4096 ))%4096;

  if(!Overlap_Check(read_bytes,zero_bytes,upage))
	{
		f->eax=-1;
		return;
	}
  hash_insert(&(currentThread->memoryMapTable), &(fMap->hash_elem)); 


 
  while (read_bytes > 0 || zero_bytes > 0) 
    {
      /* Calculate how to fill this page.
         We will read PAGE_READ_BYTES bytes from FILE
         and zero the final PAGE_ZERO_BYTES bytes. */
   size_t    page_read_bytes = read_bytes < PGSIZE ? read_bytes : PGSIZE;
   size_t     page_zero_bytes = PGSIZE - page_read_bytes;

      /* Updating the S-Page Table. */
                struct SuppPageTable *HashStruct = malloc(sizeof(struct SuppPageTable));
	       	HashStruct->UserPage_VAddr = upage;
		(HashStruct->File_Info).file = filePointer;
		(HashStruct->File_Info).Actual_Offset = Offset ;
		(HashStruct->File_Info).page_read_bytes = page_read_bytes ;
		(HashStruct->File_Info).page_zero_bytes = page_zero_bytes ;
		(HashStruct->File_Info).writable = true ;		 //writable;
		HashStruct->isSwap=true;
	//printf("swapTable lock acquire %d \n",thread_current()->tid);	
	
		lock_acquire(&frame_lock);
//printf("swapTable lock %d \n",thread_current()->tid);
                  
		hash_insert(&(thread_current()->Supp_Page_Table), &(HashStruct->hash_elem));
		File_Write_Swap(filePointer,Offset,page_read_bytes,page_zero_bytes,HashStruct);
		lock_release(&frame_lock);
	
//printf("swapTable lock release %d \n",thread_current()->tid);
		
	
	
      read_bytes -= page_read_bytes;
      zero_bytes -= page_zero_bytes;
      upage += PGSIZE;
      Offset += PGSIZE;
    }
  f->eax = fileDescriptor ; // Using File Descriptor itself as Map Id.
  
  break;

  case SYS_MUNMAP:
  userStack = userStack + sizeof(int);
  fileDescriptor = *(int *)userStack ;
  fMap = malloc(sizeof (struct FileMap));
  fMap->fileDescriptor = fileDescriptor ;
  struct hash_elem *elem;  
  elem = hash_delete(&(thread_current()->memoryMapTable), &(fMap->hash_elem));
  struct FileMap *fileMap = hash_entry (elem, struct FileMap, hash_elem);
  void *fileVirtualAddress = fileMap->fileVirtualAddress ;
  filePointer = getFilePointerFromDescriptorTable(fileDescriptor);
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
			file_write_at(filePointer,pagedir_get_page(thread_current()->pagedir,upage),page_read_bytes,(HashStruct->File_Info).Actual_Offset);
		}
		hash_delete(&(thread_current()->Supp_Page_Table), &(HashStruct->hash_elem));
		pagedir_clear_page (thread_current()->pagedir, upage);
		read_bytes -= page_read_bytes;
      		zero_bytes -= page_zero_bytes;
      		upage += PGSIZE;
      		Offset += PGSIZE;
    }
  /**/
  case SYS_REMOVE:

   userStack = userStack + sizeof(char *);
   fileName = *(char **)userStack ;
   if((fileName) == NULL || get_user(fileName) == -1)
	   {
	 	thread_current()->set_status= - 1;	
	 	thread_exit();
	   }
   
   
   retValue = filesys_remove (fileName);

   f->eax = retValue ;
   break;

/*System Calls For File System .....*/
   case SYS_CHDIR:                  /* Change the current directory. */
    userStack = userStack + sizeof(char *);
    // Directory Name To Which The Current Working Directory Is Changed
    pathName = *(char **)userStack ;
    // Storing The Parent Of The Current Directory     // Parsing the path name
     if (strcmp(pathName,"/") == 0)
	{
		 thread_current()->currentDirectory_sector = ROOT_DIR_SECTOR ;
	         f->eax = true ;
		return;
	}
    char *dir_name = malloc ((NAME_MAX+1) * sizeof(char));
   
    y = checkPath (pathName, &(parent_dir), (dir_name));

     if (!y)
	{
		f->eax = false;
       		return ;	 
        }
    y = dir_lookup (parent_dir, dir_name, &(inode));
    if (!y) 
		{
			printf("No such Directory %s\n",dir_name);	
			f->eax = false;
			dir_close(parent_dir);
			free (dir_name);
			return ;
		}
    thread_current()->currentDirectory_sector = inode_get_inumber (inode) ;
    inode_close (inode);
    f->eax = true ;
    dir_close(parent_dir);
    free (dir_name);
    return ;
    break;

  case  SYS_MKDIR:                  /* Create a directory. */
    userStack = userStack + sizeof(char *);
    // Directory Name Which Has To Be Made 
    pathName = *(char **)userStack ;
    char *newDirectory_name = malloc ((NAME_MAX+1) * sizeof(char));
    // Search From The Root Directory --> Absolute Path Provided
    // Call A Method To Get The Name Of The Directory To Create The Current Directory In
     y = checkIfAllocated (pathName, &(parent_dir), (newDirectory_name));
     if (y)
	{
		f->eax = false;
       		return ;	 
        }
// Case That this is the last directory name i.e it has to be entered into the directory structure
			// Creating a new directory here ..
				// Allocating a new bitmap to the directory from the list of bitmaps for the inodes 
				sector_idx = bitmap_scan_and_flip (inodeListBitmap, 0, 1, false);
				if (sector_idx == -1) 		// Should get a new sector from the disk ///// Not Implemented
					{
						//printf ("No Sector Left For Directory Inode Allocation\n");
 						f->eax = false ;
						return ; 
					}
			// Getting the actual sector  number from the sector index Sector_idx
                        sector_allocated = sector_Inode_number[sector_idx];
		// Creating The Directory At The Alloted Sector 
                        y = dir_create (sector_allocated, DIRECTORY_SIZE);
			if (!y) 
				{
					//printf("Directory Creation Failed\n");
					f->eax = false ;
					return ;
				}
		// Entering Parent Directory Information In The Subdirectory					
                        sub_dir = dir_open(inode_open(sector_allocated)) ;
		// Adding .. and . into the directory structure
			y = dir_add (sub_dir, "..", getInodeSector (parent_dir),false) ;
			y = dir_add (sub_dir, ".", sector_allocated, false) ;
		//	 
                        dir_close (sub_dir);
		// Inserting Directory Entry Into The Parent Directory
			dir_add (parent_dir, newDirectory_name, sector_allocated, false);
			//printf("adding directory name %s to the directry with sector %d",newDirectory_name,getInodeSector (parent_dir ));
			f->eax = true ;	
			dir_close (parent_dir);	
        		return ;
    
    break;
    case SYS_READDIR:                /* Reads a directory entry. */
    
    userStack = userStack + sizeof(int);
    fileDescriptor = *(int *)userStack ; 
    userStack = userStack + sizeof(void *);
    char * name = *(void **)userStack ;
    dMap = getDescriptorMapPointerFromDescriptorTable(fileDescriptor); 
    dirPointer = dMap->dir;
    f->eax = dir_readdir (dirPointer, name); 
    return ;

    break; 
    case SYS_ISDIR:                  /* Tests if a fd represents a directory. */

    userStack = userStack + sizeof(int);
    fileDescriptor = *(int *)userStack ;
    dMap = getDescriptorMapPointerFromDescriptorTable(fileDescriptor); 
    if (dMap->is_File == false)
	{	
		f->eax = true ;
	} 
    else
		f->eax = false ;
    return ;		
    break;
    case SYS_INUMBER:   		/* Returns the Inode Number For The Current File / Directory ..*/
	
	
    userStack = userStack + sizeof(int);
    fileDescriptor = *(int *)userStack ;
    dMap = getDescriptorMapPointerFromDescriptorTable(fileDescriptor); 
    f->eax =  inode_get_inumber (dMap->inode);	
    return ;

    break ;
/**/


 default:;
		
  }		

}

/* Method to free pages of a given file-desriptor from the main-memory */
void freeFilePages(int fileDescriptor)
{
struct hash_iterator i;
struct FileMap *f ;
hash_first (&i, &(thread_current()->memoryMapTable));
bool Last = 0 ; 
while (hash_next (&i))
{
	if(Last == 1)
		{
			free(f->fileVirtualAddress);	//Removing the file from the virtual memory.
			free(f);
			Last = 0;
		}
	f = hash_entry (hash_cur (&i), struct FileMap, hash_elem);
	if(f->fileDescriptor == fileDescriptor)
	{
	   //printf("hello\n");
	   Last = 1;	
	   hash_delete(&(thread_current()->memoryMapTable), &(f->hash_elem));
	 // removes the hash_elem for the file from the Memory Map Table
	   	
	}
}

if(Last == 1)
		{
			free(f->fileVirtualAddress);	//Removing the file from the virtual memory.
			free(f);
			Last = 0;
		}

}

/* Code To Get The Correct Directory Files Parent Directory */
/* Takes In A String PATH and returns a PARENT_DIRECTORY, also return the new directory's name  */
/*bool getParentDirectory (char *path, struct dir **parent_dir, char **newDirectory_name)
{
struct dir_entry *ep; off_t *ofsp ;

if (strlen(path) == 0 || strcmp(path,"") == 0) 
{
	return false;
}

struct dir *cur_dir ;
char *save_ptr, *token ;
struct inode *inode ;
disk_sector_t parentDirectory_sector, cur_dir_sector;
bool y = false;
    if (path[0] == '/') 		// Case 1 , Start From The Root Directory , First Verify That All But The Last exists
	{	
		cur_dir_sector = ROOT_DIR_SECTOR ;
		//parentDirectory_sector = ROOT_DIR_SECTOR ;			
		//path = & (path[1]);		// Easy For Tokenizing
        }        
	else
	 {	
			
	 }
	    if(path[0] == '.' && path[1] == '.')		// Case That mkdir ../has been called
		{	
			if (thread_current()->currentDirectory_sector != ROOT_DIR_SECTOR)
			{
				cur_dir = dir_open(inode_open(thread_current()->currentDirectory_sector));
				cur_dir_sector = getParentDirectorySector(cur_dir) ;
                                dir_close (cur_dir) ;
			}
			else 
			{		
				cur_dir_sector = ROOT_DIR_SECTOR ;	
			}
			path = & (path[3]);
		} 
  
           else if(path[0] == '.' && path == '/')			// Case That mkdir ./ has been callled
	      {
		path = & (path[2]);
		cur_dir_sector = thread_current()->currentDirectory_sector ;	
	      } 
	    else  // Search From The Current Directory --> Relative To The Current Directory  // Case That mkdir pathname has been called 
		{
			cur_dir_sector = thread_current()->currentDirectory_sector  ;	
		}		
	   }
		for (token = strtok_r (path, "/", &save_ptr); token != NULL; token = strtok_r (NULL, "/", &save_ptr))
			{
			      cur_dir  = dir_open (inode_open (cur_dir_sector)) ;
			      y = dir_lookup (cur_dir, token, &(inode));
			      if (!y) break;	
			      cur_dir_sector = inode_get_inumber (inode);
			      inode_close (inode);			
		              dir_close (cur_dir);	
		              cur_dir = dir_open(inode_open(cur_dir_sector)) ;
			}
                if (token == NULL)
		{
			printf("File Already Exists\n");
			dir_close (cur_dir);			
			return false ;
		}
// Directory name indicated by token not found, identify whether this is the last directory name or not
		if (strlen(save_ptr) != 0)			// Case that a directory name within the directory name string is not found -- Checking For The Last Token
		{
		   printf("Incorrect Path !!!!  Error No such Directory/File %sFound\n", token);
		   dir_close (cur_dir);
                   return false ;
		} 	
		else
		{ 
		   *newDirectory_name = token ;
		   *parent_dir = cur_dir ;
                   return true;	
		}
}*/

