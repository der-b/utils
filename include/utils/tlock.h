/* Copyright 2020 Bernd Lehmann (der-b@der-b.com)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef __TLOCK_H__
#define __TLOCK_H__

#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>

#define TLOCK_RET_SUCCESS (0)
#define TLOCK_RET_UNKNOWN (-1)

#define TLOCK_FLAGS_RECURSIVE (1)

struct _tlock;

typedef ssize_t TLRet;
typedef struct _tlock TLock;

struct _tlock {
        pthread_mutex_t mutex;
};

TLRet tl_init(TLock *lock, uint32_t flags);
TLRet tl_lock(TLock *lock);
TLRet tl_trylock(TLock *lock);
TLRet tl_unlock(TLock *lock);
TLRet tl_destroy(TLock *lock);

#endif
