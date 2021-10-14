// Buffer cache.
//
// The buffer cache is a linked list of buf structures holding
// cached copies of disk block contents.  Caching disk blocks
// in memory reduces the number of disk reads and also provides
// a synchronization point for disk blocks used by multiple processes.
//
// Interface:
// * To get a buffer for a particular disk block, call bread.
// * After changing buffer data, call bwrite to write it to disk.
// * When done with the buffer, call brelse.
// * Do not use the buffer after calling brelse.
// * Only one process at a time can use a buffer,
//     so do not keep them longer than necessary.


#include "types.h"
#include "param.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "riscv.h"
#include "defs.h"
#include "fs.h"
#include "buf.h"
#define NBUCKET 13
#define BUCKETSZ 8
/*struct {
  struct spinlock lock;
  struct buf buf[NBUF];

  // Linked list of all buffers, through prev/next.
  // Sorted by how recently the buffer was used.
  // head.next is most recent, head.prev is least.
  struct buf head;
} bcache;*/
extern uint ticks;
struct bucket {
  struct spinlock lock;
  struct buf bufs[BUCKETSZ];
};

struct {
  struct spinlock lock;
  struct bucket buckets[NBUCKET];
}bcache;

void
binit(void)
{
  initlock(&bcache.lock, "bcache");
  for (int i = 0; i < NBUCKET; i++) {
    initlock(&bcache.buckets[i].lock, "bcache.bucket");
    for (int j = 0; j < BUCKETSZ; j++)
      initsleeplock(&bcache.buckets[i].bufs[j].lock, "buffer");
  }
}

// Look through buffer cache for block on device dev.
// If not found, allocate a buffer.
// In either case, return locked buffer.
static struct buf*
bget(uint dev, uint blockno)
{
  int k = blockno % NBUCKET;
  acquire(&bcache.buckets[k].lock);
  struct bucket* bkt = &bcache.buckets[k];
  struct buf* b;
  for (b = &bkt->bufs[0]; b < (struct buf*)&bkt->bufs[0] + BUCKETSZ; b++) {
    if (b->dev == dev && b->blockno == blockno) {
      //printf("No. %d is in cache bucket %d, timestamp = %d\n", blockno, k, b->timestamp);
      b->timestamp = ticks; 
      b->refcnt++;
      release(&bcache.buckets[k].lock);
      //release(&bcache.lock);
      acquiresleep(&b->lock);
      return b;
    }
  }
  uint last = -1;
  struct buf* ret = b;
  for (b = &bkt->bufs[0]; b < (struct buf*)&bkt->bufs[0] + BUCKETSZ; b++) {
    if (b->timestamp < last && b->refcnt == 0) {
      last = b->timestamp;
      ret = b;  
    }
  }
  ret->dev = dev;
  ret->blockno = blockno;
  ret->valid = 0;
  ret->timestamp = ticks;
  ret->refcnt = 1;
  release(&bcache.buckets[k].lock);
  acquiresleep(&ret->lock);
  return ret;
  panic("bget: no buffers");
}

// Return a locked buf with the contents of the indicated block.
struct buf*
bread(uint dev, uint blockno)
{
  struct buf *b;

  b = bget(dev, blockno);
  if(!b->valid) {
    virtio_disk_rw(b, 0);
    b->valid = 1;
  }
  return b;
}

// Write b's contents to disk.  Must be locked.
void
bwrite(struct buf *b)
{
  if(!holdingsleep(&b->lock))
    panic("bwrite");
  virtio_disk_rw(b, 1);
}

// Release a locked buffer.
// Move to the head of the most-recently-used list.
void
brelse(struct buf *b)
{
  if(!holdingsleep(&b->lock))
    panic("brelse");
  b->refcnt--;
  releasesleep(&b->lock);
}

void
bpin(struct buf *b) {
  int k = b->blockno % NBUCKET;
  acquire(&bcache.buckets[k].lock);
  b->refcnt++;
  release(&bcache.buckets[k].lock);
}

void
bunpin(struct buf *b) {
  int k = b->blockno % NBUCKET;
  acquire(&bcache.buckets[k].lock);
  b->refcnt--;
  release(&bcache.buckets[k].lock);
}


