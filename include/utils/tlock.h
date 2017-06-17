#ifndef __TLOCK_H__
#define __TLOCK_H__

#include <stdlib.h>
#include <pthread.h>

#define TLOCK_RET_SUCCESS (0)
#define TLOCK_RET_UNKNOWN (-1)

struct _tlock;

typedef ssize_t TLRet;
typedef struct _tlock TLock;

struct _tlock {
        pthread_mutex_t mutex;
};

TLRet tl_init(TLock *lock);
TLRet tl_lock(TLock *lock);
TLRet tl_unlock(TLock *lock);
TLRet tl_destroy(TLock *lock);
#endif
