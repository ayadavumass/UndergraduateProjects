#include "threads/synch.h"
#include "filesys/cache.h"
#include "devices/disk.h"
#include "devices/disk.h"
#include <ctype.h>
#include <debug.h>
#include <stdbool.h>
#include <stdio.h>
#include "devices/timer.h"
#include "threads/io.h"
#include "threads/interrupt.h"
#include "threads/synch.h"
#include "filesys/inode.h"
#include <list.h>
#include <debug.h>
#include <round.h>
#include <string.h>
#include "filesys/filesys.h"
#include "filesys/free-map.h"
#include "threads/malloc.h"
#include "filesys/free-map.h"
#include "threads/thread.h"
#define PRI_DEFAULT 10
// reserve a block in buffer cache dedicated to hold this sector
struct cache_block *cache_get_block (disk_sector_t sector, bool exclusive, bool readAheadSector)
{
    if (sector == -1) printf("fail\n");
    struct cache_block *cacheBlock ;	
    lock_acquire(&bufferCache_lock);
    size_t bufferCache_size = list_size (&(bufferCache_list));
    cacheBlock = isCachePresent ( sector );
    if (cacheBlock != NULL)
	{
		//if(sector == 220) printf("sector 220 accessed\n");
		 goto SUCCESS ; // Cache-Block For The Current Sector Is Already Present.. 
	}
    else
    {
	    if (bufferCache_size >= 64)		// Apply Eviction Algorithm Here....
			{
				//if(sector == 220) printf("sector 220 accessed\n");
                                cacheBlock = getCacheBlock();
				init_cache_block(cacheBlock);
			}
		else
			{
				cacheBlock = malloc( sizeof(struct cache_block));
				init_cache_block(cacheBlock);
				initLocks (cacheBlock);
				
			}
				cacheBlock->sector = sector ;
				cacheBlock->shared = !(exclusive) ;
				list_push_back(&(bufferCache_list), &(cacheBlock->elem));
				cache_write_block (cacheBlock);
				goto SUCCESS ;
      }	
SUCCESS:
cacheBlock->access_cnt++;
lock_release(&bufferCache_lock);
if (readAheadSector)
{
	;//cache_readahead (sector+1);
}
return cacheBlock ;
}

// Initialize The Attributes Of A Cache Block
void init_cache_block (struct cache_block * cacheBlock)
{
	cacheBlock->sector = NULL ;
        cacheBlock->dirty = false ;
	cacheBlock->shared = false ;
	cacheBlock->accessed = false ;
	cacheBlock->num_running_readers = 0;
	cacheBlock->num_running_writers = 0 ;
        cacheBlock->num_total_readers = 0;
        cacheBlock->num_total_writers = 0 ;
	cacheBlock->access_cnt = 0 ;
}

void initLocks (struct cache_block *cacheBlock)
{
		sema_init(&(cacheBlock->reader_sema),0);
		sema_init(&(cacheBlock->writer_sema),0);
		lock_init(&(cacheBlock->mutex_lock));
}
// release access to cache block
void cache_put_block(struct cache_block *b)
{
	struct cache_block *cacheBlock;
	struct list_elem *currElem  = list_begin(&(bufferCache_list));
	while (currElem != list_end(&(bufferCache_list)))
		{
			cacheBlock = list_entry(currElem, struct cache_block, elem);
			if (cacheBlock == b)
			    {
				evict_cacheBlock(cacheBlock);
				free(cacheBlock);
				return ;
			    }
			currElem = list_next(currElem);	
		}
}
//Clock Algorithm Implementation
struct cache_block *getCacheBlock()
{
	struct list *list = &(bufferCache_list);
	struct cache_block *cacheBlock;
	struct list_elem *currElem  = list_begin((list));
	if(list_empty(list)) return ;				//Extra Precaution should not actually occur
	while (!list_empty(list))
		{
			if(currElem == list_end(list))	currElem = list_begin(list);	//For Circular Loop ..
			cacheBlock = list_entry(currElem, struct cache_block, elem);
			if(cacheBlock->access_cnt == 0)			//If There Is No Reader-Writer Sitting On The Buffer-Cache ..
			{
				if (cacheBlock->accessed)
				    {
					cacheBlock->accessed = false;
				    }
				else
				    {
					lock_acquire (&(cacheBlock->mutex_lock));					
					evict_cacheBlock(cacheBlock);
					lock_release (&(cacheBlock->mutex_lock));
					return cacheBlock ;
				    }
			}
		  currElem = list_next(currElem);	
		}
}
//Evicting A Cache Block From The Cache Buffer
struct cache_block *evict_cacheBlock(struct cache_block *cacheBlock)
{
        
	if (cacheBlock == NULL) return NULL ;
	//if(cacheBlock->dirty)
		{	
			disk_write (filesys_disk, cacheBlock->sector, cacheBlock->data);
	        }
	list_remove(&(cacheBlock->elem));
	return cacheBlock;
}

// write sector from the disk into the cache, returns pointer to data
void *cache_write_block (struct cache_block *b)
{
         disk_sector_t sector= b->sector ;
         uint8_t *buffer = b->data ;
	 disk_read (filesys_disk, sector, buffer);	
/*	Synchonization Not Needed Here As This Method Is Called From When A Cache Block Is First Installed Into The Buffer ... At That No Reader-Writer Can sit on it ... Because Of bufferCache_lock Synchronization...	*/
//End Reader Wrtiter Solution ... For A Writer Solution 
	return buffer ;
}

// read from cache block into disk, returns pointer to data
void *cache_read_block (struct cache_block *b)
{
	if (b == NULL) return NULL ;
	disk_sector_t sector= b->sector ;
	void *buffer = b->data ;
	disk_write (filesys_disk, sector, buffer);
//Reader-Writer Solution Applied Here......
/*	
Synchronization Here Is Not Applied As This Method Is Called Only In Case When Eviction Occurs, and in that case no reader-writer can sit on it ...*/
	return buffer ;
}

// Writes SIZE BYTES into CACHE'S BLOCK (at an offset of SECTOR_OFS) From the BUFFER (at an offset of BUFFER_OFS)
void cache_mem_write_block (struct cache_block *b, void *buffer, int size, int sector_ofs, int buffer_ofs)
{
	if (b == NULL) return NULL ;
	disk_sector_t sector= b->sector ;
	void *cache_data = b->data ;
	//Reader-Writer Solution Applied Here...... For A Writer
	
        lock_acquire(&b->mutex_lock);
		b->num_total_writers += 1 ;
		if(b->num_running_readers ==  0 && b->num_running_writers == 0) 
		    {	
			b->num_running_writers = 1;
			sema_up(&(b->writer_sema));
		     }
	lock_release(&b->mutex_lock);
	sema_down(&(b->writer_sema));
			memcpy (cache_data + sector_ofs, buffer + buffer_ofs, size);      // Critical Section writing into the Buffer Here..
			b->accessed = true ;
			b->dirty = true ;
			
	lock_acquire(&b->mutex_lock);
		b->num_running_writers -= 1;
		b->num_total_writers   -= 1;
	    while(b->num_total_readers > b->num_running_readers)
		{
				b->num_running_readers += 1 ;
				sema_up(&(b->reader_sema));
		}
	lock_release(&b->mutex_lock);

//End Reader Wrtiter Solution ... For A Writer Solution 
	return cache_data ;
}

//reads SIZE bytes from CACHE BLOCK  (at an offset of SECTOR_OFS) into the BUFFER (at an offset of BUFFER_OFS)
void cache_mem_read_block (struct cache_block *b, void *buffer, int size, int sector_ofs, int buffer_ofs)
{
	if (b == NULL) return NULL ;
	disk_sector_t sector= b->sector ;
	void *cache_data = b->data ;
//Reader-Writer Solution Applied Here...... For A Reader
       lock_acquire(&b->mutex_lock);
		b->num_total_readers += 1 ;
		if(b->num_running_writers ==  0) 
		    {	
			b->num_running_readers += 1;
			sema_up(&(b->reader_sema));
		     }
	lock_release(&b->mutex_lock);

	sema_down(&(b->reader_sema));
		 memcpy ( buffer + buffer_ofs, cache_data + sector_ofs, size);
		 b->accessed = true ;
	lock_acquire(&b->mutex_lock);
		b->num_running_readers -= 1;
		b->num_total_readers   -= 1;
	    if(b->num_running_readers == 0 && b->num_total_writers > b->num_running_writers)
		{
				b->num_running_writers = 1 ;
				sema_up(&(b->writer_sema));
		}
	lock_release(&b->mutex_lock);

//End Reader Reader Solution
	return cache_data ;
}

// fill cache block with zeros, returns pointer to data
void *cache_zero_block(struct cache_block *b)
{
	void *buffer = b->data ;
	/* Writing Zeros To A Cache Block */
	memset (buffer, 0, DISK_SECTOR_SIZE);
	/* End Of Write ..*/
	return buffer ;
}
// mark cache block dirty (must be written back)
void cache_mark_block_dirty(struct cache_block *b)
{
	b->dirty = true ;
}

//initialize other variables related to the Buffer Cache
void init_BufferCache()
{
   list_init(&(bufferCache_list)); 	
   lock_init(&(bufferCache_lock));
   initSectorQueue();	
   thread_create ("ReadAhead Daemon", PRI_DEFAULT, readAhead_daemon, NULL) ;
   sema_init (&(readAhead_sema) , 0) ;
}

void cache_readahead(disk_sector_t s)
 {
   sectorQueuelock();
   sectorQueuepush (s);
    sectorQueueunlock();
   sema_up (&(readAhead_sema));
  
}

//Daemon Thread For Read-Ahead
void readAhead_daemon ()
{
  disk_sector_t s ;
  while (true) 
  {
    sema_down (&(readAhead_sema));
    sectorQueuelock();
    s = sectorQueuepop();
    sectorQueueunlock();
    cache_get_block (s, true, false);
  }
	
}
//Find A Cache Block For The Given Sector
struct cache_block *isCachePresent (disk_sector_t sector)
{
	struct list_elem *currElem ;
	struct cache_block *cacheBlock ;
	struct list *list = &(bufferCache_list);
	if(list_empty(&(bufferCache_list))) return NULL ;
        currElem = list_begin(&(bufferCache_list));
	while (currElem != list_end(list))
		{
			cacheBlock = list_entry(currElem, struct cache_block, elem);
			if (cacheBlock->sector == sector)
			    {
				return cacheBlock ;
			    }
						
		currElem = list_next(currElem);
		}
	return NULL ;
}
//Releases Access On A Block By The Current Process
void  cache_release_access (struct cache_block *b)
{
	lock_acquire(&(b->mutex_lock));
	if(b->access_cnt <= 0)
		printf("Access Count Error\n");
	else
		b->access_cnt--;
	lock_release(&(b->mutex_lock));
}


	void initSectorQueue ()
	{
		lock_init (&(sectorQueue_lock)) ;
		list_init (&(sector_list));
		
	}
	
	bool sectorQueueempty ()
	{
		return (list_empty(&(sector_list)));
	}
		
	void sectorQueuelock ()
	{
		lock_acquire (&(sectorQueue_lock)) ;	
	}
		
	void sectorQueueunlock ()
	{
		lock_release (&(sectorQueue_lock)) ;	
	}
	void sectorQueuepush (disk_sector_t sector)
	{
		struct sectorQueueEntry *s = malloc (sizeof(struct sectorQueueEntry));
		s->sector = sector ;		
		list_push_back (&(sector_list), &s->elem);
	}
	disk_sector_t sectorQueuepop ()
	{
		if (list_empty(&(sector_list))) return NULL ;
		struct list_elem *currElem = list_pop_front (&(sector_list)) ;
		struct sectorQueueEntry *q = list_entry (currElem, struct sectorQueueEntry, elem);
		disk_sector_t s = q->sector ;
		free (q); 
		return s ;
	}
