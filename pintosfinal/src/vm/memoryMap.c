#include "memoryMap.h"

/*Used for hashing*/
unsigned MemoryMapTable_Hash_Func (const struct hash_elem *p_, void *aux UNUSED)
{
const struct FileMap *p = hash_entry (p_, struct FileMap, hash_elem);
return hash_bytes (&p->fileDescriptor, sizeof p->fileDescriptor);
}


/* Comparison function for hash table */
bool MemoryMapTable_Less (const struct hash_elem *a_, const struct hash_elem *b_,void *aux UNUSED)
{
const struct FileMap *a = hash_entry (a_, struct FileMap, hash_elem);
const struct FileMap *b = hash_entry (b_, struct FileMap, hash_elem);
return a->fileDescriptor < b->fileDescriptor;
}

