#ifndef FILESYS_INODE_H
#define FILESYS_INODE_H

#include <stdbool.h>
#include "filesys/off_t.h"
#include "devices/disk.h"
#define INITIALIZED_INODE_LIST_SIZE 400 

struct bitmap;
struct bitmap *inodeListBitmap ;
// Array that stores the sector number corresponding to a particular inode number.
disk_sector_t sector_Inode_number[INITIALIZED_INODE_LIST_SIZE];

void inode_init (void);
bool inode_create (disk_sector_t, off_t);
struct inode *inode_open (disk_sector_t);
struct inode *inode_reopen (struct inode *);
disk_sector_t inode_get_inumber (const struct inode *);
void inode_close (struct inode *);
void inode_remove (struct inode *);
off_t inode_read_at (struct inode *, void *, off_t size, off_t offset);
off_t inode_write_at (struct inode *, const void *, off_t size, off_t offset);
void inode_deny_write (struct inode *);
void inode_allow_write (struct inode *);
off_t inode_length (const struct inode *);
bool inode_create (disk_sector_t sector, off_t length);
// Grows The Current Inode By Length Bytes
// Gets The Inode Disk For A Particular Sector
struct inode_disk *getInodeDisk (disk_sector_t sector) ;
bool init_Inode_Bitmap ();
void setIsFile (struct inode *inode, bool is_File);
bool getIsFile (struct inode *inode);
int inode_open_cnt (struct inode *inode);
#endif /* filesys/inode.h */
