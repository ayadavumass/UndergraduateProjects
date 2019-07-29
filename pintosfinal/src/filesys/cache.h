#ifndef CACHE_H
#define  CACHE_H

#include  <list.h>
#include "threads/synch.h"
#include "devices/disk.h"
#include <ctype.h>
#include <debug.h>
#include <stdbool.h>
#include <stdio.h>
#include "devices/timer.h"
#include "threads/io.h"
#include "threads/interrupt.h"
#include "threads/synch.h"
#define BLOCK_SIZE 512
/* Interface For Cache Block */

struct  list bufferCache_list ;
struct lock bufferCache_lock ;
struct list sector_list ;
struct lock sectorQueue_lock ;
	void initSectorQueue ();
	bool sectorQueueempty ();
	void sectorQueuelock ();
	void sectorQueueunlock ();
	void sectorQueuepush (disk_sector_t sector);
	disk_sector_t sectorQueuepop ();

/*Structure To Be Pushed Into The Sector Waiting Queue ..*/
struct sectorQueueEntry 
{
	struct list_elem elem;
	disk_sector_t sector ;
};

 struct semaphore readAhead_sema ;

/*Structure For Cache Block */
struct cache_block 
{
     struct list_elem elem ;
     disk_sector_t sector ;
     bool dirty ;
     bool accessed ;
     bool shared ;
     int num_running_readers ;
     int num_running_writers ;
     int num_total_readers ;
     int num_total_writers ;
     struct semaphore reader_sema ;
     struct semaphore writer_sema ;
     struct lock mutex_lock ;
     uint8_t data[BLOCK_SIZE] ;
     int access_cnt ; 
     bool isInit ;	
			 	
};
// reserve a block in buffer cache dedicated to hold this sector
struct cache_block *cache_get_block (disk_sector_t sector, bool exclusive, bool readAhead);
// release access to cache block
void cache_put_block(struct cache_block *b);
// read from cache block into disk, returns pointer to data
void *cache_read_block(struct cache_block *b);
// write to  cache block from disk, returns pointer to data
void *cache_write_block(struct cache_block *b);
// reads SIZE bytes from CACHE BLOCK'S DATA into the BUFFER
void cache_mem_read_block (struct cache_block *b, void *buffer, int size, int sector_ofs, int buffer_ofs);
// writes SIZE bytes from BUFFER into the CACHE BLOCK'S DATA
void cache_mem_write_block (struct cache_block *b, void *buffer, int size, int sector_ofs, int buffer_ofs);
// fill cache block with zeros, returns pointer to data
void *cache_zero_block(struct cache_block *b);
// mark cache block dirty (must be written back)
void cache_mark_block_dirty(struct cache_block *b);
// Initialize The Attributes Of A Cache Block
void init_cache_block(struct cache_block * cacheBlock);
//initialize other variables related to the Buffer Cache
void init_BufferCache();
// Releases Access On A Block By The Current Process
void  cache_release_access (struct cache_block *b);
//Evicting a block from the Buffer Cache
struct cache_block *evict_cacheBlock(struct cache_block *cacheBlock);
//Gets A Cache Block From The Buffer Cache, 
struct cache_block *getCacheBlock();
struct cache_block *isCachePresent (disk_sector_t sector);
void readAhead_daemon ();
void cache_readahead(disk_sector_t s);
void init_BufferCache();
#endif





