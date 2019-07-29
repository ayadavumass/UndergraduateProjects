#include<hash.h>
#include<debug.h>
struct File_Info_Struct
{
	struct file *file;
	int page_read_bytes;
	int page_zero_bytes;
	bool writable;
	int Actual_Offset;
	int Sector;
	
};
struct SuppPageTable
{
	struct hash_elem hash_elem; /* Hash table element. */
	void *UserPage_VAddr;
	struct File_Info_Struct File_Info;
	bool isSwap;
	
};





bool SuppPage_Table_Less (const struct hash_elem *a_, const struct hash_elem *b_,void *aux UNUSED);
unsigned SuppPage_Table_Hash_Func (const struct hash_elem *p_, void *aux UNUSED);
struct SuppPageTable *page_lookup (const void *address,struct hash *Hash_Table);
