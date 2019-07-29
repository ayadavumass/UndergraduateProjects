#include "swap.h"

/*Used for hashing*/
unsigned Swap_Table_Hash_Func (const struct hash_elem *p_, void *aux UNUSED)
{
const struct SwapToUserPage *p = hash_entry (p_, struct SwapToUserPage, hash_elem);
return hash_bytes (&p->Sector, sizeof p->Sector);
}


/* Comparison function for hash table */
bool Swap_Table_Less (const struct hash_elem *a_, const struct hash_elem *b_,void *aux UNUSED)
{
const struct SwapToUserPage *a = hash_entry (a_, struct SwapToUserPage, hash_elem);
const struct SwapToUserPage *b = hash_entry (b_, struct SwapToUserPage, hash_elem);
return a->Sector < b->Sector;
}
