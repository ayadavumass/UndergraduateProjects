#ifndef MemoryMap_H
#define MemoryMap_H

#include<hash.h>
#include<debug.h>


/*		Structure that stores the mapping between the file descriptor and virtual address for that file.*/
struct FileMap 
{
	struct hash_elem hash_elem ;
	void *fileVirtualAddress ;
	int fileDescriptor ;
	int fileLength;
};
/**/

bool MemoryMapTable_Less (const struct hash_elem *a_, const struct hash_elem *b_,void *aux UNUSED);
unsigned MemoryMapTable_Hash_Func (const struct hash_elem *p_, void *aux UNUSED);

#endif Memory_Map_H
