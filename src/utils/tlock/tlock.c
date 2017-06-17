#include <utils/tlock.h>
#include <utils/log.h>


/*
 * tl_init();
 */
TLRet tl_init(TLock *lock)
{
        TLRet ret = TLOCK_RET_UNKNOWN;
        pthread_mutexattr_t att;

        if (pthread_mutexattr_init(&att)) {
                ERROR("pthread_mutexattr_init() faild.");
                goto err;
        }

        if (pthread_mutexattr_settype(&att, PTHREAD_MUTEX_RECURSIVE)) {
                ERROR("pthread_mutexattr_settype() faild.");
                goto err_att;
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
