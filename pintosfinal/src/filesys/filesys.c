#include "filesys/filesys.h"
#include <debug.h>
#include <stdio.h>
#include <string.h>
#include "filesys/file.h"
#include "filesys/free-map.h"
#include "filesys/inode.h"
#include "filesys/directory.h"
#include "devices/disk.h"
#include "userprog/syscall.h"
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "vm/page.h"
#include "vm/memoryMap.h"
#include "userprog/gdt.h"
#include "threads/vaddr.h"
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


/* The disk that contains the file system. */
struct disk *filesys_disk;
bool checkPath (char *path, struct dir **parent_dir, char *newDirectory_name);
bool checkIfAllocated (char *path, struct dir **parent_dir, char *newDirectory_name);
static void do_format (void);

/* Initializes the file system module.
   If FORMAT is true, reformats the file system. */
void
filesys_init (bool format) 
{
  filesys_disk = disk_get (0, 1);
  if (filesys_disk == NULL)
    PANIC ("hd0:1 (hdb) not present, file system initialization failed");

  inode_init ();
  free_map_init ();

  if (format) 
    do_format ();

  free_map_open ();
  printf("filesys init done\n");
}

/* Shuts down the file system module, writing any unwritten data
   to disk. */
void
filesys_done (void) 
{
  free_map_close ();
}


/* Opens the file with the given NAME.
   Returns the new file if successful or a null pointer
   otherwise.
   Fails if no file named NAME exists,
   or if an internal memory allocation fails. */
/*struct file *
filesys_open (const char *name)
{
  struct dir *dir = dir_open_root ();
  struct inode *inode = NULL;

  if (dir != NULL)
    dir_lookup (dir, name, &inode);
  dir_close (dir);
  return file_open (inode);
}

/* Creates a file named NAME with the given INITIAL_SIZE.
   Returns true if successful, false otherwise.
   Fails if a file named NAME already exists,
   or if internal memory allocation fails. */
bool
filesys_create (const char *path, off_t initial_size) 
{
  disk_sector_t inode_sector = 0;
  struct dir *parent_dir  ;			//dir_open(inode_open(thread_current()->currentDirectory_sector)) ;; 
  char *name = malloc ((NAME_MAX+1) * sizeof(char));
  bool  y = false;

  y = checkIfAllocated (path, &parent_dir, name);

  if (y)						 /// Already Has Been Allocated Or Path Is Incorrect
	{
		free (name); 
		return false;
	}
  bool success = (parent_dir != NULL
                  && free_map_allocate (1, &inode_sector)
                  && inode_create (inode_sector, initial_size)
                  && dir_add (parent_dir, name, inode_sector, true));
  if (!success && inode_sector != 0)
    free_map_release (inode_sector, 1);
  dir_close (parent_dir);
  free (name) ;
  return success;
}

/* Opens the file with the given NAME.
   Returns the new file if successful or a null pointer
   otherwise.
   Fails if no file named NAME exists,
   or if an internal memory allocation fails. */
struct file *
filesys_open (const char *path)
{
  /*Handling The Case For Root Directory..*/
  if (path[0] == '/'  && strlen(path) == 1)
  {
	return (file_open (inode_open(ROOT_DIR_SECTOR)));	
  }
   if (strcmp(path,"") == 0)
  {
	return NULL;	
  }

  struct dir *dir ;
  char *name = malloc ((NAME_MAX+1) * sizeof(char));
  bool y = checkPath (path, &dir, (name));
  if (!y)
	{
		
		free (name);	
		return false;
	}

  struct inode *inode = NULL;
  if (dir != NULL) 	
    dir_lookup (dir, name, &inode);
  dir_close (dir);
  free (name);
  
  return file_open (inode);
}

/* Deletes the file named NAME.
   Returns true if successful, false on failure.
   Fails if no file named NAME exists,
   or if an internal memory allocation fails. */
bool
filesys_remove (const char *path) 
{
  struct dir *dir ;
  bool is_Empty ;
  disk_sector_t sector ;
  if (strcmp(path,"/") == 0) return false;
  char *file_name = malloc ((NAME_MAX+1) * sizeof(char));
  bool z = checkPath (path, &dir, file_name);
  if (!z)
	{
		free (file_name);	
		return false;
	} 
  struct inode *inode;
  bool a = dir_lookup (dir ,file_name, &(inode));
  if (getIsFile (inode) == false)		// This Is A Directory
	{
		sector = inode_get_inumber (inode);
		struct dir *d = dir_open (inode);
		if (inode_length (inode) > getDirEntrySize() * 2)
			is_Empty = false;
		else
			is_Empty = true ;
		dir_close (d);
		if (sector == ROOT_DIR_SECTOR || sector == thread_current()->currentDirectory_sector || inode_open_cnt(inode) > 1 || !(is_Empty) )
			return false ;
		
	}
  
  inode_close (inode);
  bool success = dir != NULL && dir_remove (dir, file_name);
  dir_close (dir); 
  free (file_name);	
  return success;
}

/* Formats the file system. */
static void
do_format (void)
{
  printf ("Formatting file system...");
  free_map_create ();
  if (!dir_create (ROOT_DIR_SECTOR, 16))
    PANIC ("root directory creation failed");
  free_map_close ();
  printf ("done.\n");
}
/* Checks if the complete path exists or not ..Returns true if it does -- Called in case Of Open.*/ 
bool checkPath (char *path, struct dir **parent_dir, char *newDirectory_name)
{
if (strcmp(path,"") == 0) 
{
	return true;
}

struct inode *inode ;
struct dir *cur_dir ;
char *save_ptr, *token ;
disk_sector_t parent_dir_sector, cur_dir_sector;
char *path_ = malloc ((strlen(path) + 1) * sizeof (char));

if (path_ == NULL)
{
     printf("Page Allocation Error In CheckPath() \n");
     return false;
}

strlcpy (path_, path, strlen(path)+1);

bool y = false;
    if (path_[0] == '/') 		// Case 1 , Start From The Root Directory , First Verify That All But The Last exists
	{	
		cur_dir_sector = ROOT_DIR_SECTOR ;	
        }        
	else
	 {
	       cur_dir_sector = thread_current()->currentDirectory_sector ;	 
	}	
	
	 /*   if(path_[0] == '.' && path_[1] == '.')		// Case That mkdir ../has been called
		{	
			if (thread_current()->currentDirectory_sector != ROOT_DIR_SECTOR)
			{
				cur_dir = dir_open(inode_open(thread_current()->currentDirectory_sector));
				cur_dir_sector = getParentDirectorySector (cur_dir) ;
				dir_close (cur_dir);
			}
			else 
			{		
				cur_dir_sector = ROOT_DIR_SECTOR ;	
			}
			//path_ = & (path_[3]);
			token = strtok_r (path_, "/", &save_ptr) ; 
        		token = strtok_r (NULL, "/", &save_ptr);
		} 
  
           else if(path_[0] == '.' && path_ == '/')			// Case That mkdir ./ has been callled
	      {
		//path_ = & (path_[2]);
		token = strtok_r (path_, "/", &save_ptr) ; 
		token = strtok_r (NULL, "/", &save_ptr);
		cur_dir_sector = thread_current()->currentDirectory_sector ;		
	      } 
	    else  // Search From The Current Directory --> Relative To The Current Directory  // Case That mkdir pathname has been called 
		{
			cur_dir_sector = thread_current()->currentDirectory_sector ;	
			token = strtok_r (path_, "/", &save_ptr) ; 
		}		
	   }*/
		for (token = strtok_r (path_, "/", &save_ptr) ;  token != NULL; token = strtok_r (NULL, "/", &save_ptr))
		{
		      cur_dir = dir_open(inode_open(cur_dir_sector)); 
		      y = dir_lookup (cur_dir, token, &(inode));
		//printf("bool %d",y);	
		      if (!y) break;	
		      parent_dir_sector = cur_dir_sector ;	
		      cur_dir_sector = inode_get_inumber (inode);		// It may be a file or a directory
		      inode_close (inode);
                      dir_close (cur_dir);		
		     	// If it is a file then there is a problem here 
		      strlcpy(newDirectory_name, token, strlen(token)+1);		      	
		}
	
                if (token == NULL || strlen (token) == 0)
		{
                        *parent_dir = dir_open(inode_open(parent_dir_sector)) ;	
			free(path_);
			return true ;
		}
// Directory name indicated by token not found, identify whether this is the last directory name or no
			// Case that a directory name within the directory name string is not found -- Checking For The Last Token
		   //printf("Incorrect Path !!!!  Error No such Directory/File %sFound\n", token);
		   dir_close (cur_dir);
		   free(path_);	
                   return false ;
}


/*Checks Whether The Path Is Correct Or Not And Also Returns true if allocated or if the path is incorrect else false is returned */
bool checkIfAllocated (char *path, struct dir **parent_dir, char *newDirectory_name)
{

if (strcmp(path,"") == 0) 
{
	return true;
}
struct inode *inode ;
struct dir *cur_dir ;
char *save_ptr, *token ;
disk_sector_t parent_dir_sector, cur_dir_sector;
char *path_ = malloc ((strlen(path) + 1) * sizeof (char));
int initial_pos = 0 ;

if (path_ == NULL)
{
     printf("Memory Allocation Error In CheckPath() \n");
     return true;
}
strlcpy (path_, path, strlen(path)+ 1);
bool y = false;
    if (path_[0] == '/') 		// Case 1 , Start From The Root Directory , First Verify That All But The Last exists
	{	
		cur_dir_sector = ROOT_DIR_SECTOR;
        }        
	else
	 {	
		cur_dir_sector = thread_current()->currentDirectory_sector ;	
	 }

	/*    if(path_[0] == '.' && path_[1] == '.')		// Case That mkdir ../has been called
		{	
			if (thread_current()->currentDirectory_sector != ROOT_DIR_SECTOR)
			{
				cur_dir = dir_open(inode_open(thread_current()->currentDirectory_sector));
				cur_dir_sector = getParentDirectorySector (cur_dir) ;
				dir_close (cur_dir);
			}
			else 
			{		
				cur_dir_sector = ROOT_DIR_SECTOR ;	
			}
			//path_ = & (path_[3]);
			initial_pos = 3 ;
			token = strtok_r (path_, "/", &save_ptr );  // Neglecting the .. token
			token = strtok_r (NULL, "/", &save_ptr);
		} 
  
           else if(path_[0] == '.' && path_ == '/')			// Case That mkdir ./ has been callled
	      {
		//initial_pos = 2 ;
		token = strtok_r (path_, "/", &save_ptr) ;		// Neglecting the . token 
		cur_dir_sector = thread_current()->currentDirectory_sector ;	
		token = strtok_r (NULL, "/", &save_ptr);
	      } 
	    else  // Search From The Current Directory --> Relative To The Current Directory  // Case That mkdir pathname has been called 
		{
			cur_dir_sector = thread_current()->currentDirectory_sector ;
			token = strtok_r (path_, "/", &save_ptr ); 
		}		
	   }*/
		for (token = strtok_r (path_, "/", &save_ptr ); token != NULL; token = strtok_r (NULL, "/", &save_ptr))
		{
			
		      cur_dir = dir_open(inode_open(cur_dir_sector)) ;
		      y = dir_lookup (cur_dir, token, &(inode));
		      if (!y) break;	
		      parent_dir_sector = cur_dir_sector ;
		      cur_dir_sector = inode_get_inumber (inode);
		      inode_close (inode);
                      dir_close (cur_dir);	
		} 
                if (strlen(token) == 0 || token == NULL)
		{
			free(path_);		
			return true ;
		}
// Directory name indicated by token not found, identify whether this is the last directory name or no
			// Case that a directory name within the directory name string is not found -- Checking For The Last Token
		if (strlen(save_ptr) != 0 || (strlen(token) > NAME_MAX)) // Case that a directory name within the directory name string is not found -- Checking For The Last Token
		{
		//  printf("Incorrect Path !!!!  Error No such Directory/File %sFound\n", token);
		    free(path_);	
		   dir_close (cur_dir);
                   return true ;
		} 	
		else
		{ 
		   *parent_dir = cur_dir ;
		   strlcpy(newDirectory_name, token, strlen(token)+1);
		   free(path_);	
                   return false;	
		}	  
/**/
}
