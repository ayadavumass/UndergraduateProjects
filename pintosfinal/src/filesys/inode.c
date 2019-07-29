#include "filesys/inode.h"
#include <list.h>
#include <debug.h>
#include <round.h>
#include <string.h>
#include "filesys/filesys.h"
#include "filesys/free-map.h"
#include "threads/malloc.h"
#include "filesys/free-map.h"
#include "filesys/cache.h"
/* Identifies an inode. */

#define INODE_MAGIC 0x494e4f44
#define MAX_DIRECT_BLOCKS 124
#define SECTOR_CAPACITY (128)
#define EXTRA_SINGLE_INDIRECT_BLOCKS (128)
#define EXTRA_DOUBLE_INDIRECT_BLOCKS (128 *128)

/* On-disk inode.
   Must be exactly DISK_SECTOR_SIZE bytes long. */
struct inode_disk			//Now Used As An Index-Block 
  {
    //disk_sector_t start;                /* First data sector. */
    off_t length;                       /* File size in bytes. */
    unsigned magic;                     /* Magic number. */
    disk_sector_t sector_idx[MAX_DIRECT_BLOCKS];        /* Array Used That Maps Ith Sector Of The Inode To The Ith Sector On The Filesystem-Disk  . */
    disk_sector_t single_indirect_block ;
    disk_sector_t double_indirect_block ;
  };

/* Returns the number of sectors to allocate for an inode SIZE
   bytes long. */
static inline size_t
bytes_to_sectors (off_t size)
{
  return DIV_ROUND_UP (size, DISK_SECTOR_SIZE);
}

/* In-memory inode. */
struct inode 
  {
    struct list_elem elem;              /* Element in inode list. */
    disk_sector_t sector;               /* Sector number of disk location. */
    int open_cnt;                       /* Number of openers. */
    bool removed;                       /* True if deleted, false otherwise. */
    int deny_write_cnt;                 /* 0: writes ok, >0: deny writes. */
    bool is_File;
  };

/* Returns the disk sector that contains byte offset POS within
   INODE.
   Returns -1 if INODE does not contain data for a byte at offset
   POS. */
static disk_sector_t
byte_to_sector (const struct inode_disk *disk_inode, off_t pos) 
{
  disk_sector_t required_sector_idx = pos / DISK_SECTOR_SIZE ; 
  disk_sector_t required_sector_number = required_sector_idx + 1 ;
  disk_sector_t sector_required, sector_second_level_index_block ;
  struct cache_block *cache_block_first_level, *cache_block_second_level ;
  int sector_ofs_first_level, sector_ofs_second_level ;

  // Getting The Inode Disk From The File-System Disk 
  //struct cache_block *cache_block_inode_disk = cache_get_block (inode->sector, true, false);
  //struct inode_disk *disk_inode = cache_block_inode_disk->data ;
  //
 // if(inode->sector == 219)  printf("disk inode length %d\n",disk_inode->length);
  if (pos < disk_inode->length  || (pos == disk_inode->length && (disk_inode->length % DISK_SECTOR_SIZE) != 0))
    {
	 if (required_sector_number <= MAX_DIRECT_BLOCKS) 	
             sector_required = disk_inode->sector_idx[required_sector_idx] ;
         else if (required_sector_number > MAX_DIRECT_BLOCKS  && required_sector_number <= (MAX_DIRECT_BLOCKS + EXTRA_SINGLE_INDIRECT_BLOCKS))
		{
			//printf("SINGLE INDIRECT BLOCK READ \n");
			// Getting The First Level Index Block Into The Buffer Cache 
			cache_block_first_level = cache_get_block (disk_inode->single_indirect_block, true, false);
			 // Calculating The sector offset level for the Data Block Entry  In BYTES			
			sector_ofs_first_level = (required_sector_number - MAX_DIRECT_BLOCKS - 1) * (sizeof(disk_sector_t)) ;
			//reading at the appropriate offset to get the sector number of the data block
			cache_mem_read_block (cache_block_first_level, &(sector_required), sizeof(disk_sector_t), sector_ofs_first_level , 0);
			cache_release_access (cache_block_first_level); 
		}
	 else if (required_sector_idx > MAX_DIRECT_BLOCKS  && required_sector_idx <= (MAX_DIRECT_BLOCKS + EXTRA_SINGLE_INDIRECT_BLOCKS + EXTRA_DOUBLE_INDIRECT_BLOCKS)) 
		{
			printf("Double INDIRECT BLOCK READ %d\n",required_sector_idx);
			// Getting The First Level Index Block Into The Buffer Cache 
			cache_block_first_level = cache_get_block (disk_inode->double_indirect_block, true, false);
                        // Calculating The sector offset level for the second index block entry
			sector_ofs_first_level = ((required_sector_number - (MAX_DIRECT_BLOCKS + EXTRA_SINGLE_INDIRECT_BLOCKS)-1) / (SECTOR_CAPACITY) ) * sizeof(disk_sector_t) ;
			printf("sector_ofs_first_level %d\n",sector_ofs_first_level);
			// Reading The First Level Index Block For The Sector Of The Second Level Index-Block
                        cache_mem_read_block (cache_block_first_level, &(sector_second_level_index_block), sizeof(disk_sector_t), sector_ofs_first_level , 0);
                        cache_release_access (cache_block_first_level); 
			// Getting The Second Level Block Into The Cache Buffer 
			cache_block_second_level = cache_get_block (sector_second_level_index_block, true, false);				
			// sector_ofs_second_level is the offset for the data block in the second level block entry for the data block
			sector_ofs_second_level = ((required_sector_number - (MAX_DIRECT_BLOCKS + EXTRA_SINGLE_INDIRECT_BLOCKS)-1) % (SECTOR_CAPACITY)) * sizeof(disk_sector_t);
			printf("sector_ofs_second_level %d\n",sector_ofs_second_level);
                        // Reading at  sector_ofs_second_level (offset) to get the sector number of the data block in the second level index block
			cache_mem_read_block (cache_block_first_level, &(sector_required), sizeof(disk_sector_t), sector_ofs_second_level , 0);
			cache_release_access (cache_block_second_level); 
                         
		} 
        //cache_release_access (cache_block_inode_disk); 
    	return sector_required ;
    }
  else 				// That A File Seek Has Been Done And It Is Trying to Write Beyond The End Of File 
    {
     //printf("pos %d, length %d\n",pos, disk_inode->length);
    //cache_release_access (cache_block_inode_disk);
    return -1;
    } 
}

/* List of open inodes, so that opening a single inode twice
   returns the same `struct inode'. */
static struct list open_inodes;

/* Initializes the inode module. */
void
inode_init (void) 
{
  list_init (&open_inodes);
}
// Writes SIZE BYTES zeros on the disk_inode starting at the ofs ..
bool inode_fill_zeros (struct inode_disk *disk_inode, int size, int ofs , struct inode *inode)
{

	if (size == 0) return true ;
        int zeros_filled = 0;
        // Getting The Current Sector Index 
       while (size > 0)
       {		
	       zeros_filled = size < DISK_SECTOR_SIZE ? size : DISK_SECTOR_SIZE	;
	       int current_sector_idx =  byte_to_sector (disk_inode,ofs) ;
		//printf("sector index %d, ofs = %d\n", current_sector_idx, ofs);
	       struct cache_block *cache_block_data = cache_get_block (current_sector_idx, true, false);
		//printf("sector index %p", cache_block_data);
	       char buffer[zeros_filled] ; int count = 0 ;
		while (count < zeros_filled)
		 {
			buffer[count] = 0;
			 count++ ;
		 }
		cache_mem_write_block(cache_block_data, buffer,  zeros_filled, 0, 0) ; // Sector Offset = 0, Other Case Already dealt with
		cache_release_access (cache_block_data);
		size -= zeros_filled ; 	
                ofs += zeros_filled ;
        }
	return true;
}

/* Grows The File's Inode By The LENGTH Bytes.
   Returns True If That Enough Sectors Are Free Else False Is Returned ...
*/
bool inode_grow (struct inode_disk *disk_inode, off_t length)
{
       ASSERT (length >= 0);		
       bool success = false ;  
       bool done = false ;      
       int count_sector = 1 ;
       int sectors_required = bytes_to_sectors (length); 
       int initial_sector_idx =  bytes_to_sectors (disk_inode->length) -1 ; //length(disk_inode->length +  DISK_SECTOR_SIZE -1) / DISK_SECTOR_SIZE  -1 ;
       int current_sector_count = initial_sector_idx + 1 ; 
       disk_sector_t sector_allocated, sector_allocated_1, sector_allocated_2, data_block_sector ;
       int sector_ofs ;
       int length_remain = length ;
       int length_increment = 0 ; 	
       //printf("current sector count %d, disk_inode_length %d,length %d\n",current_sector_count, disk_inode->length, length);
     while (count_sector <= sectors_required)
      {  
        if (current_sector_count >= MAX_DIRECT_BLOCKS && current_sector_count < ((MAX_DIRECT_BLOCKS + EXTRA_SINGLE_INDIRECT_BLOCKS)))// Case That The Single Indirect Block Is Needed
		{
			//printf("SINGLE INDIRECT BLOCK REQUIRED , current sector count %d\n",current_sector_count);
                       /*Bring The Single Indirect Block Into The Buffer Cache --- Memory  For Writing On To It ... */
			struct cache_block *cache_block = cache_get_block (disk_inode->single_indirect_block, true, false);
			success = free_map_allocate (1, &(sector_allocated));
                        sector_ofs =  (current_sector_count - MAX_DIRECT_BLOCKS) * sizeof(disk_sector_t) ;
			//printf ("sector written on %d\n", sector_allocated);
			cache_mem_write_block (cache_block, &(sector_allocated), sizeof(disk_sector_t), sector_ofs , 0);
			cache_release_access (cache_block); 
			data_block_sector = sector_allocated ;
		}
	else if (current_sector_count >= (MAX_DIRECT_BLOCKS + EXTRA_SINGLE_INDIRECT_BLOCKS)) // Case That Even 2 * 2 ^ 16 BYTES USED UP. Now We need To Use The Second Indirect Block ..
		{
			//printf("DOUBLE INDIRECT BLOCK REQUIRED\n");
			struct cache_block *cache_block = cache_get_block (disk_inode->double_indirect_block, true, false); // First Level Indirect Block
			if ((current_sector_count - (MAX_DIRECT_BLOCKS + SECTOR_CAPACITY)) % (SECTOR_CAPACITY) == 0)
				{                        
				   free_map_allocate (1, &(sector_allocated_1));   // Allocating A Sector To The Second Level Block
				   //Calculating Offset In Terms Of Bytes Where A New Entry Has To Be Made Into The First LevelBlock Entry
				   sector_ofs = ((current_sector_count - (MAX_DIRECT_BLOCKS + SECTOR_CAPACITY)) / (SECTOR_CAPACITY)) * sizeof(disk_sector_t) ; 
                         	   //Making An Entry Corresponding To That Allocated Block In The First Level Indirect Block
				   cache_mem_write_block (cache_block, &(sector_allocated), sizeof(disk_sector_t), sector_ofs , 0);		    					 
				}
			//Calculating Sector Offset In Terms Of Bytes
			int sector_ofs_1 = ((current_sector_count - (MAX_DIRECT_BLOCKS + SECTOR_CAPACITY))/(SECTOR_CAPACITY)) * sizeof(disk_sector_t) ;		
                        cache_mem_read_block (cache_block, &(sector_allocated),sizeof(disk_sector_t),sector_ofs , 0) ;
                        cache_release_access (cache_block); 
                        printf("sector_ofs_1 %d\n",sector_ofs_1);
                        //sector_allocated is the sector entry for the second level block 
                       struct cache_block *cache_block_second_level = cache_get_block (sector_allocated, true, false); // First Level Indirect Block
                       // sector_ofs_second_level is the offset for the data block on the second level Index Block cache_block_second_level
                       int sector_ofs_second_level = ((current_sector_count - (MAX_DIRECT_BLOCKS + SECTOR_CAPACITY)) % (SECTOR_CAPACITY)) * sizeof (disk_sector_t) ;
                           printf("sector_ofs_1 %d\n",sector_ofs_second_level);
			// sector_allocated_2 is the sector number for the "final" data block on the third level here
			success = free_map_allocate (1, &(sector_allocated_2));
 			// writing it down in the second level Cache Block Itself
		       cache_mem_write_block (cache_block_second_level, &(sector_allocated_2),sizeof(disk_sector_t),0 , 0) ; 
                       cache_release_access (cache_block_second_level); 
			/**/			
			data_block_sector = sector_allocated_2 ;
                         printf("sec all %d\n sec sount%d\n",sector_allocated_2,current_sector_count);
		}
	else if (current_sector_count >= (MAX_DIRECT_BLOCKS + EXTRA_SINGLE_INDIRECT_BLOCKS + EXTRA_DOUBLE_INDIRECT_BLOCKS)) 
		{
				printf ("MEMORY SIZE MORE THAN 8 GB REQUIRED\n");
				return false;
		}			
	else  {				
		success = free_map_allocate (1, &(disk_inode->sector_idx[initial_sector_idx + count_sector])) ;
                data_block_sector =  disk_inode->sector_idx[initial_sector_idx + count_sector];
                
		//printf("data block allocated %d\n",data_block_sector);
	      }	  
	if (success) 	
		      {	
			count_sector++ ;
                        current_sector_count++ ;
			if (length_remain > DISK_SECTOR_SIZE) 
			     {
				length_remain -= DISK_SECTOR_SIZE ;  
				length_increment = DISK_SECTOR_SIZE ;                        	        
			      }
			else  
			     {			
			    	length_increment = length_remain ;	// To Account For Partial Length Files --> Files That Do Not Occupy Complete Sector Size For The Last Sector
				length_remain = 0 ;
			     }
			// Increasing the length of the inode
			disk_inode->length += length_increment ;
			// Writing zeros equal to length increment to the sector DATA_BLOCK_SECTOR on the disk  
			// Cache Block For The Data 
			/*printf ("sector allocated = %d\n",data_block_sector);
			struct cache_block *data_block_sector = cache_get_block (data_block_sector, true, false);
			char buffer[length_increment] ; int count = 0 ;
			while (count < length_increment) buffer[count++] = 0;
			cache_mem_write_block(data_block_sector, buffer, length_increment,0,0) ;
                    	cache_release_access (data_block_sector); */
		       }	
	/*   else 
                {
                  int count_sector_release = 1; 
                  //Code To Release The Free Map Goes Here...
		        while (count_sector_release <= sectors_required)
			{	        
			  free_map_release (disk_inode->sector_idx[initial_sector_idx + count_sector_release],1);
		          count_sector_release++ ;  
			  disk_inode->length -= DISK_SECTOR_SIZE ;
			}
                 return success ;
                 }*/

	// called in loops for non-contiguous allocation of sectors to the file ..
	  
        /*Else Case Not Handled When Enough Space For The File Is Not Available*/
         } 
       return success ;// will be true if code reaches here 
}
/* Initializes an inode with LENGTH bytes of data and
   writes the new inode to sector SECTOR on the file system
   disk.
   Returns true if successful.
   Returns false if memory or disk allocation fails. */
bool inode_create (disk_sector_t sector, off_t length)
{
 //printf("inode cretae called\n");
  struct inode_disk *disk_inode = NULL;
  bool success = false;
  disk_sector_t sector_required = 0;
  int offset = 0;
  ASSERT (length >= 0);
/* Both The Single And Double Indirect Blocks are Initialized to NULL. */
  /* If this assertion fails, the inode structure is not exactly
     one sector in size, and you should fix that. */
  ASSERT (sizeof *disk_inode == DISK_SECTOR_SIZE);

  disk_inode = calloc (1, sizeof *disk_inode);
  if (disk_inode != NULL)
    {  
       /* This can be optimized further by allocating these indirect blocks only when needed .. */
      free_map_allocate (1, &(disk_inode->single_indirect_block));
      free_map_allocate (1, &(disk_inode->double_indirect_block)); 
      size_t sectors = bytes_to_sectors (length);
      disk_inode->length = 0 ;				// Initially length set to zero
      disk_inode->magic = INODE_MAGIC;
      int count_sector = 0;
 
      /*Initially we create a  file Of Size Zero and then grow it ..*/	
        if (inode_grow (disk_inode, length)  || length == 0)
	 { 
          disk_write (filesys_disk, sector, disk_inode);	// Writing The Disk Inode Onto The File System ..
          
          if (sectors > 0) 
            {
              static char zeros[DISK_SECTOR_SIZE];
              size_t i;
              
              for (i = 0; i < sectors; i++)  
                                       {
						     if (length > i*DISK_SECTOR_SIZE)
								offset = i*DISK_SECTOR_SIZE ;
							else
								offset = length ;
			 			     sector_required = byte_to_sector (disk_inode, offset);	                                       
						     disk_write (filesys_disk, sector_required, zeros); 
                                       }  
 		// writing zeros for initialization on the file-system --- we will have to write it on the file system from an array of sectors instead of writing  directly .....
             } 
            success = true;
           }
      free (disk_inode);
    }
//printf("inode cretae done\n");
  return success;
}

/* Reads an inode from SECTOR
   and returns a `struct inode' that contains it.
   Returns a null pointer if memory allocation fails. */
struct inode *
inode_open (disk_sector_t sector) 
{
  struct list_elem *e;
  struct inode *inode;

  /* Check whether this inode is already open. */
  for (e = list_begin (&open_inodes); e != list_end (&open_inodes);
       e = list_next (e)) 
    {
      inode = list_entry (e, struct inode, elem);
      if (inode->sector == sector) 
        {
          inode_reopen (inode);
          return inode; 
        }
    }

  /* Allocate memory. */
  inode = malloc (sizeof *inode);
  if (inode == NULL)
    return NULL;

  /* Initialize. */
  list_push_front (&open_inodes, &inode->elem);
  inode->sector = sector;
  inode->open_cnt = 1;
  inode->deny_write_cnt = 0;
  inode->removed = false;
  return inode;
}

/* Reopens and returns INODE. */
struct inode *
inode_reopen (struct inode *inode)
{
  if (inode != NULL)
    inode->open_cnt++;
  return inode;
}

/* Returns INODE's inode number. */
disk_sector_t
inode_get_inumber (const struct inode *inode)
{
  return inode->sector;
}

/* Closes INODE and writes it to disk.
   If this was the last reference to INODE, frees its memory.
   If INODE was also a removed inode, frees its blocks. */
void
inode_close (struct inode *inode) 
{
/* Ignore null pointer. */
  if (inode == NULL)
    return;
  struct cache_block *cache_block_inode_disk = cache_get_block (inode->sector, true, false);
  /* Release resources if this was the last opener. */
  if (--inode->open_cnt == 0)
    {
      /* Remove from inode list and release lock. */
      list_remove (&inode->elem);

      /* Deallocate blocks if removed. */
      if (inode->removed) 
        {
        //Release To Be Done In Terms Of Per Sector Release -- Releasing the inode from the hard disk itself..
	int sector_count  = 0 ;
        struct inode_disk *disk_inode = cache_block_inode_disk->data ;
        while (sector_count < bytes_to_sectors (disk_inode->length))	// release implemented 
		{	        
		  free_map_release (disk_inode->sector_idx[sector_count],1);
                  sector_count++ ;  
		}
           free_map_release (inode->sector,1);
        }
    cache_release_access (cache_block_inode_disk);		
    free (inode); 
    //evict_cacheBlock (cache_block_inode_disk);
    }
  
}

/* Marks INODE to be deleted when it is closed by the last caller who
   has it open. */
void
inode_remove (struct inode *inode) 
{
  ASSERT (inode != NULL);
  inode->removed = true;
}

/* Reads SIZE bytes from INODE into BUFFER, starting at position OFFSET.
   Returns the number of bytes actually read, which may be less
   than SIZE if an error occurs or end of file is reached. */
off_t
inode_read_at (struct inode *inode, void *buffer_, off_t size, off_t offset) 
{
  uint8_t *buffer = buffer_;
  off_t bytes_read = 0;
  uint8_t *bounce = NULL;
  struct cache_block *cacheBlock ; 

  struct cache_block *cache_block_inode_disk = cache_get_block (inode->sector, true, false);
  struct inode_disk *disk_inode = cache_block_inode_disk->data ;
  
  while (size > 0) 
    {
      /* Disk sector to read, starting byte offset within sector. */
      disk_sector_t sector_idx = byte_to_sector (disk_inode, offset);
      int sector_ofs = offset % DISK_SECTOR_SIZE;

      /* Bytes left in inode, bytes left in sector, lesser of the two. */
      off_t inode_left = inode_length (inode) - offset;
      int sector_left = DISK_SECTOR_SIZE - sector_ofs;
      int min_left = inode_left < sector_left ? inode_left : sector_left;

      /* Number of bytes to actually copy out of this sector. */
      int chunk_size = size < min_left ? size : min_left;
      if (chunk_size <= 0)
        break;
/*    Pre-Fetch Next When 
	1) inode_left > sector_left
	2) size > chunk_size
	Can Be Passed As An Argument To The function cache_get_block ();
*/
	bool readAhead = inode_left > sector_left && size > sector_left ;
//Apply Synchronization Here.. -- Implied Here By Cache Access And Release 
     cacheBlock = cache_get_block (sector_idx, true, false);		// Allocating a Cache Block To The Specified Sector Index
     cache_mem_read_block (cacheBlock, buffer, chunk_size, sector_ofs, bytes_read);	// Reading From Cache Block Into The Buffer 
     cache_release_access (cacheBlock);
/*      if (sector_ofs == 0 && chunk_size == DISK_SECTOR_SIZE) 
        {
          /* Read full sector directly into caller's buffer. */
/*          disk_read (filesys_disk, sector_idx, buffer + bytes_read); 
        }
      else 
        {

          /* Read sector into bounce buffer, then partially copy
             into caller's buffer. */
/*          if (bounce == NULL) 
            {
              bounce = malloc (DISK_SECTOR_SIZE);
              if (bounce == NULL)
                break;
            }
          disk_read (filesys_disk, sector_idx, bounce);
          memcpy (buffer + bytes_read, bounce + sector_ofs, chunk_size);
        }
      printf("name %d",e->name);
      /* Advance. */
      size -= chunk_size;
      offset += chunk_size;
      bytes_read += chunk_size;
    }
cache_release_access (cache_block_inode_disk); 
  return bytes_read;
}

/* Writes SIZE bytes from BUFFER into INODE, starting at OFFSET.
   Returns the number of bytes actually written, which may be
   less than SIZE if end of file is reached or an error occurs.
   (Normally a write at end of file would extend the inode, but
   growth is not yet implemented.) */
off_t
inode_write_at (struct inode *inode, const void *buffer_, off_t size, off_t offset)  // With File Growth The Size BYTES  File Have To Be Written To The File-System Disk ..
{
  const uint8_t *buffer = buffer_;
  off_t bytes_written = 0;
  uint8_t *bounce = NULL;
  struct cache_block *cacheBlock ; 
  struct cache_block *cache_block_inode_disk = cache_get_block (inode->sector, true, false);
  struct inode_disk *disk_inode = cache_block_inode_disk->data ;
  disk_sector_t index_block_idx = inode->sector;
  int sector_left ;
  int last_sector_occupied = 0;
  int last_sector_left = 0 ;

  if (disk_inode->length != 0)
	{	
		if (disk_inode->length % DISK_SECTOR_SIZE != 0)
			{
				last_sector_occupied = disk_inode->length % DISK_SECTOR_SIZE ;
				last_sector_left = DISK_SECTOR_SIZE - last_sector_occupied ;
			}
		else
			last_sector_occupied = DISK_SECTOR_SIZE ;
	}
  
  if (inode->deny_write_cnt)
{
cache_release_access (cache_block_inode_disk); 
    return 0;
}
  
 // Case Of Growth By SEEK
  if(offset > disk_inode->length)			// Growth due to file seek
    {	
        int growth_desired = offset - disk_inode->length ;					// 2 cases 1) when extra block required 2) when growth on the same block is to be done
	if (growth_desired <= last_sector_left)		// No new Page Is Required Now
	{
		// Code For Partial Growth Goes Here -- Grows The Last Index Sector By Size Equal To GROWTH_DESIRED Past The EOF -- fills with zeros ....
		inode_fill_zeros (disk_inode, growth_desired, disk_inode->length, inode);
		disk_inode->length += growth_desired ;
	}
	else 
	{
           if (last_sector_left != 0)	
		{
			inode_fill_zeros (disk_inode, last_sector_left, disk_inode->length, inode) ;
			disk_inode->length += growth_desired ;
			growth_desired -= last_sector_left ;
		}
			int ini_ofs = disk_inode->length ;
			(inode_grow (disk_inode, growth_desired)); ////printf("growth done\n");
			cache_mark_block_dirty(cache_block_inode_disk);
                        inode_fill_zeros (disk_inode, growth_desired, ini_ofs, inode) ;
	}
    }
//cache_release_access (cache_block_inode_disk);
  while (size > 0) 
    {
      /* Sector to write, starting byte offset within sector. */
      disk_sector_t sector_idx = byte_to_sector (disk_inode, offset);
      int sector_ofs = offset % DISK_SECTOR_SIZE;

      /* Bytes left in inode, bytes left in sector, lesser of the two. */
      off_t inode_left = inode_length (inode) - offset;
      if (inode_length(inode) == offset  && (offset % DISK_SECTOR_SIZE) == 0) 
      sector_left = 0;	
      else 
      sector_left = DISK_SECTOR_SIZE - sector_ofs;		// sector_left > 0
       
      int min_left = inode_left < sector_left ? inode_left : sector_left;

      /* Number of bytes to actually write into this sector. */
      int chunk_size = size < min_left ? size : min_left;
     
      if (chunk_size <= 0)// Occurs only when inode left = 0 Here File Extension Has To Be Implemented --- Allocate Extra Sectors to The Inode If They Can Be Done ...And Grow The File
	{
			if (size <= sector_left)  // No Need To Allocate Any New Sector
			{
			   chunk_size = size ;
                          if(sector_idx == -1) printf ("1) sector_idx -1\n");
			   // Cache Data Block Got From The FileSystem Disk
			   cacheBlock = cache_get_block (sector_idx, true, false);
			   // Writing On The Cache Block The Remaining Bytes From The Buffer Into The Same Sector
			   cache_mem_write_block (cacheBlock, buffer, chunk_size, sector_ofs, bytes_written);
			   // Also The File Length Has To Increase 
			  //struct cache_block *cache_block_inode_disk = cache_get_block (inode->sector, true, false);
      			 // struct inode_disk *disk_inode = cache_block_inode_disk->data ;
        		  disk_inode->length += chunk_size ; 
                          //cache_release_access (cache_block_inode_disk);; 
			   cache_release_access (cacheBlock);
			   bytes_written += chunk_size ; 
                           size -= chunk_size;
			   offset += chunk_size;	
			   break;		
			}
         		else // size > sector_left
			    {
				if (sector_left != 0)
				{
				   // first write sector_left bytes
				   chunk_size = sector_left ;
                                   sector_idx = byte_to_sector (disk_inode, offset);
				   //if(offset == 64168) printf ("sector for offset >= 64168  %d\n",sector_idx);
                                   if(sector_idx == -1) printf ("2) sector_idx -1\n");
				   // Cache Block Got From The FileSystem Disk
				   cacheBlock = cache_get_block (sector_idx, true, false);
				   // Writing On The Cache Block The Remaining Bytes From The Buffer Into The Same Sector
				  // cache_mem_write_block (cacheBlock, buffer, chunk_size, sector_ofs, bytes_written);
				    cache_release_access (cacheBlock);
				   // Also The File Length Has To Increase 
				   //struct cache_block *cache_block_inode_disk = cache_get_block (inode->sector, true, false);
      				   struct inode_disk *disk_inode = cache_block_inode_disk->data ;
        			   disk_inode->length += chunk_size ; 
                                   cache_release_access (cache_block_inode_disk);	
				   bytes_written += chunk_size ;
				   size -= chunk_size;
			           offset += chunk_size;
				}
				  // struct cache_block *cache_block_inode_disk = cache_get_block (inode->sector, true, false);
      				   //struct inode_disk *disk_inode = cache_block_inode_disk->data ;
				   inode_grow (disk_inode,size);
                                   cache_mark_block_dirty (cache_block_inode_disk);
                                   //cache_release_access (cache_block_inode_disk);
				   while (size >= DISK_SECTOR_SIZE)
					{
						chunk_size = DISK_SECTOR_SIZE ;						
                                                sector_idx = byte_to_sector (disk_inode, offset);
						if(sector_idx == -1) printf ("3) sector_idx -1, offset %d sector %d\n",offset, inode->sector);
						// Cache Block Got From The FileSystem Disk
				   		cacheBlock = cache_get_block (sector_idx, true, false);
				   		// Writing On The Cache Block The Remaining Bytes From The Buffer Into The Same Sector
				   		cache_mem_write_block (cacheBlock, buffer, chunk_size, 0, bytes_written);
						cache_release_access (cacheBlock);
						bytes_written += chunk_size ;
				                size -= chunk_size;
					        offset += chunk_size;
					}
				  if (size < DISK_SECTOR_SIZE && size > 0)	// Last Sector Written Here
					{
						chunk_size = size ;						
                                                sector_idx = byte_to_sector (disk_inode, offset);
						if(sector_idx == -1) printf ("4) sector_idx -1, offset %d sector %d\n",offset, inode->sector);
						// Cache Block Got From The FileSystem Disk
				   		cacheBlock = cache_get_block (sector_idx, true, false);
				   		// Writing On The Cache Block The Remaining Bytes From The Buffer Into The Same Sector
				   		cache_mem_write_block (cacheBlock, buffer, chunk_size, 0, bytes_written);
						cache_release_access (cacheBlock);
						bytes_written += chunk_size ;
				                size -= chunk_size;
					        offset += chunk_size;
					}
			         break;
			    }					
		 			 //  Inode Needs To Grow By One Sector At Least 		inode_grow () ;
	}
       // break;	 
	if(sector_idx == -1) printf ("5) sector_idx -1\n");
      /* Getting The Cache Block For The Current Sector .. */
       cacheBlock = cache_get_block (sector_idx, true, false);	
       
      if (sector_ofs == 0 && chunk_size == DISK_SECTOR_SIZE) 
        {
           cache_mem_write_block (cacheBlock, buffer, chunk_size, sector_ofs, bytes_written);
          /* Write full sector to buffer. */
        }
      else 
        {
          /* If the sector does not contain data before or after the chunk
             we're writing,  Otherwise we start with a sector of all zeros. */
          if (!(sector_ofs > 0 || chunk_size < sector_left))
	   {
		 cache_zero_block( cacheBlock );
           } 	
          /* Writing Into The Cache From Memory .. Writers Operation ..*/
	  cache_mem_write_block (cacheBlock, buffer, chunk_size, sector_ofs, bytes_written);
        }
      cache_release_access (cacheBlock);
      /* Advance. */
      size -= chunk_size;
      offset += chunk_size;
      bytes_written += chunk_size;
    }
  //free (bounce);
  //struct cache_block *cache_block_inode_disk = cache_get_block (inode->sector, true, false);
  //disk_inode = cache_block_inode_disk->data ;
 // cache_release_access (cacheBlock);
cache_release_access (cache_block_inode_disk); 
  return bytes_written;
}

/* Disables writes to INODE.
   May be called at most once per inode opener. */
void
inode_deny_write (struct inode *inode) 
{
  inode->deny_write_cnt++;
  ASSERT (inode->deny_write_cnt <= inode->open_cnt);
}

/* Re-enables writes to INODE.
   Must be called once by each inode opener who has called
   inode_deny_write() on the inode, before closing the inode. */
void
inode_allow_write (struct inode *inode) 
{

//printf("inodeopenCUNT%d\n",inode->open_cnt);
ASSERT (inode->deny_write_cnt > 0);
//printf()
 ASSERT (inode->deny_write_cnt <= inode->open_cnt);
 inode->deny_write_cnt--;


}

/* Returns the length, in bytes, of INODE's data. */
off_t
inode_length (const struct inode *inode)
{
  struct cache_block *cache_block_inode_disk = cache_get_block (inode->sector, true, false);
  struct inode_disk *disk_inode = cache_block_inode_disk->data ;
  int length = disk_inode->length;
  cache_release_access (cache_block_inode_disk); 
  return length ;
}

// Gets The Inode Disk For A Particular Sector
struct inode_disk *getInodeDisk (disk_sector_t sector) 
{
	struct cache_block *cache_block_inode_disk = cache_get_block (sector, true, false);
        struct inode_disk *disk_inode = cache_block_inode_disk->data ;
        cache_release_access (cache_block_inode_disk);
        return disk_inode ;
}

// Pre-Allocating INITIALIZED_INODE_LIST_SIZE Inodes For New Directories To Utilize Them --> Can Be Done For Files Too ...
bool init_Inode_Bitmap ()
{
	// Initializing the Inode Bitmap  	
		inodeListBitmap = bitmap_create (INITIALIZED_INODE_LIST_SIZE);
	// Allocating Disk Space For INITIALIZED_INODE_LIST_SIZE inodes....
		int count = 0;
                
		for (count = 0; count < INITIALIZED_INODE_LIST_SIZE; count++)	
			{
				free_map_allocate (1, &(sector_Inode_number[count]));	// Allocating sectors on the disk to these inodes that will be used for directory creation ..
			}
       return true ;         
}
void setIsFile (struct inode *inode, bool is_File)
{
	if (inode != NULL)
		inode->is_File = is_File ;
}


bool getIsFile (struct inode *inode)
{
	if (inode != NULL)
		return (inode->is_File) ;
}

int inode_open_cnt (struct inode *inode)
{
	if (inode != NULL)
		return inode->open_cnt;
}
