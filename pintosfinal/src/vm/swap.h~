#ifndef SWAP_H
#define SWAP_H

#include<hash.h>
#include<debug.h>
#include "threads/synch.h"

struct hash Swap_Table_Hash;
int CurrSector;
struct disk* swap_disk;
struct lock disk_lock;


struct lock swapTable_lock;
struct lock bitmap_lock;


struct bitmap *Swap_Bit_Map;
struct SwapToUserPage
{
	struct hash_elem hash_elem; /* Hash table element. */
	int Sector;
        
};

bool Swap_Table_Less (const struct hash_elem *a_, const struct hash_elem *b_,void *aux UNUSED);
unsigned Swap_Table_Hash_Func (const struct hash_elem *p_, void *aux UNUSED);

#endif /* end for swap.h	*/
