#ifndef PTHREAD_H
#include <pthread.h>
#endif

typedef struct pthread_seqlock
{                           /* TODO: implement the structure */
   pthread_mutex_t rw_lock; // khoa cho bien count_reader
   pthread_mutex_t lock;    // khoa cho viec read & write

   int sequence; //
   int count_reader;
} pthread_seqlock_t;

static inline unsigned pthread_seqlock_rdunlock(pthread_seqlock_t *rw);

static inline void pthread_seqlock_init(pthread_seqlock_t *rw)
{
   /* TODO: ... */
   pthread_mutex_init(&rw->rw_lock, NULL);
   pthread_mutex_init(&rw->lock, NULL);
   rw->sequence = 0;
   rw->count_reader = 0;
}

static inline void pthread_seqlock_wrlock(pthread_seqlock_t *rw)
{
   /* TODO: ... */
   // Neu chi con 1 reader thi unlock
   // va lock bien rw_lock lai de thuc hien write
   if (rw->count_reader == 1)
   {
      // goi ham nay de count_reader = 0
      // Nhung reader khac se phai doi write finish
      pthread_seqlock_rdunlock(rw);
   }
   pthread_mutex_lock(&rw->rw_lock);
   rw->sequence++;
}

static inline void pthread_seqlock_wrunlock(pthread_seqlock_t *rw)
{
   /* TODO: ... */
   pthread_mutex_unlock(&rw->rw_lock);
   rw->sequence--;
}

static inline unsigned pthread_seqlock_rdlock(pthread_seqlock_t *rw)
{
   /* TODO: ... */
   // Lock bien 'lock' lai de xu ly count_reader
   pthread_mutex_lock(&rw->lock);
   rw->count_reader++;
   // Neu count_reader = 1 thi la reader dau tien, lock bien rw_lock lai
   // Neu count_reader > 1 thi khong can lock bien rw_lock
   // Nhieu reader co the read cung luc
   if (rw->count_reader == 1)
   {
      if (pthread_mutex_lock(&rw->rw_lock) == 0)
      {
         pthread_mutex_unlock(&rw->lock);
         return 1;
      }
   }
   // unlock bien 'lock' de cac reader khac xu ly
   pthread_mutex_unlock(&rw->lock);
   return 0;
}

static inline unsigned pthread_seqlock_rdunlock(pthread_seqlock_t *rw)
{
   /* TODO: ... */
   // Lock bien 'lock' lai de xu ly count_reader
   pthread_mutex_lock(&rw->rw_lock);
   rw->count_reader--;
   // Neu count_reader = 0 thi la reader cuoi cung, unlock bien rw_lock
   // Neu count_reader > 0 thi chua duoc unlock bien rw_lock
   // Tat ca reader thuc hien read xong moi unlock bien rw_lock
   if (rw->count_reader == 0)
   {
      if (pthread_mutex_unlock(&rw->rw_lock) == 0)
      {
         pthread_mutex_unlock(&rw->lock);
         return 1;
      }
   }
   // Neu reader bi chiem quyen boi writer
   // se du ra 1 rdunlock ---> count < 0 chinh lai thanh 0
   if (rw->count_reader < 0)
   {
      rw->count_reader = 0;
   }
   // unlock bien 'lock' de cac reader khac xu ly
   pthread_mutex_unlock(&rw->lock);
   return 0;
}
