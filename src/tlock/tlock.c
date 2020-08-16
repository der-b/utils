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
#include <utils/tlock.h>
#include <utils/log.h>


/*
 * tl_init();
 */
TLRet tl_init(TLock *lock, uint32_t flags)
{
    TLRet ret = TLOCK_RET_UNKNOWN;
    pthread_mutexattr_t att;

    if (pthread_mutexattr_init(&att)) {
        ERROR("pthread_mutexattr_init() faild.");
        goto err;
    }

    if (flags && TLOCK_FLAGS_RECURSIVE) {
        if (pthread_mutexattr_settype(&att, PTHREAD_MUTEX_RECURSIVE)) {
            ERROR("pthread_mutexattr_settype() faild.");
            goto err_att;
        }
    }

    if (pthread_mutex_init(&lock->mutex, &att)) {
        ERROR("pthread_mutex_init() faild.");
        goto err_att;
    }

    if (pthread_mutexattr_destroy(&att)) {
        ERROR("pthread_mutexattr_init() faild.");
        goto err;
    }

    return TLOCK_RET_SUCCESS;
err_att:
    if (pthread_mutexattr_destroy(&att)) {
        WARN("pthread_mutexattr_init() faild.");
    }
err:
    return ret;
}


/*
 * tl_lock();
 */
TLRet tl_lock(TLock *lock)
{
    if (pthread_mutex_lock(&lock->mutex)) {
        ERROR("pthread_mutex_lock() faild.");
        return TLOCK_RET_UNKNOWN;
    }
    return TLOCK_RET_SUCCESS;
}


/*
 * tl_trylock();
 */
TLRet tl_trylock(TLock *lock)
{
    if (pthread_mutex_trylock(&lock->mutex)) {
        return TLOCK_RET_UNKNOWN;
    }
    return TLOCK_RET_SUCCESS;
}


/*
 * tl_unlock();
 */
TLRet tl_unlock(TLock *lock)
{
    if (pthread_mutex_unlock(&lock->mutex)) {
        ERROR("pthread_mutex_unlock() faild.");
        return TLOCK_RET_UNKNOWN;
    }
    return TLOCK_RET_SUCCESS;
}


/*
 * tl_destroy();
 */
TLRet tl_destroy(TLock *lock)
{
    TLRet ret = TLOCK_RET_UNKNOWN;

    if (pthread_mutex_destroy(&lock->mutex)) {
        ERROR("pthread_mutex_init() faild.");
        goto err;
    }

    return TLOCK_RET_SUCCESS;
err:
    return ret;
}
