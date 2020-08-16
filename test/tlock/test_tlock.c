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
#include <unistd.h>
#include <utils/log.h>
#include "../test_header.h"
#include <utils/tlock.h>
#include <time.h>

pthread_t lock_id;
/*
 * threads()
 */
void *threads(void *arg) 
{
    int i;
    unsigned int wait;
    TLock *lock = arg;
    pthread_t id = pthread_self();
    //INFO("Hello Thread. %lu", id);

    for (i = 0; i < 1000; i++) {
        wait = rand() % 20;
        usleep(wait);
        //ERROR("  %lu is OUT.", id);

        if (tl_lock(lock)) {
            ERROR("tl_lock() faild in %lu.", id);
            goto err;
        }

        if (tl_lock(lock)) {
            ERROR("tl_lock() faild in %lu.", id);
            goto err;
        }
        //ERROR("+ %lu is IN.", id);

        if (lock_id) {
            CRIT("lock_id is still set to %lu.", lock_id);
            goto err;
        }

        lock_id = id;

        wait = rand() % 10;
        usleep(wait);

        //ERROR("- %lu LEAVES", id);
        lock_id = 0;
        if (tl_unlock(lock)) {
            ERROR("tl_lock() faild in %lu.", id);
            goto err;
        }

        if (tl_unlock(lock)) {
            ERROR("tl_lock() faild in %lu.", id);
            goto err;
        }

    }


    return NULL;
err:
    return NULL;
}


/*
 * test_parallel()
 */
int test_parallel(TLock *lock)
{
    pthread_t t1;
    pthread_t t2;
    pthread_t t3;
    pthread_t t4;
    pthread_t t5;
    pthread_t t6;
    pthread_t t7;
    pthread_t t8;
    pthread_t t9;
    pthread_t t10;

    lock_id = 0;

    if (pthread_create(&t1, NULL, threads, lock)) {
        ERROR("pthread_create() faild");
        goto err;
    }

    if (pthread_create(&t2, NULL, threads, lock)) {
        ERROR("pthread_create() faild");
        goto err;
    }

    if (pthread_create(&t3, NULL, threads, lock)) {
        ERROR("pthread_create() faild");
        goto err;
    }

    if (pthread_create(&t4, NULL, threads, lock)) {
        ERROR("pthread_create() faild");
        goto err;
    }

    if (pthread_create(&t5, NULL, threads, lock)) {
        ERROR("pthread_create() faild");
        goto err;
    }

    if (pthread_create(&t6, NULL, threads, lock)) {
        ERROR("pthread_create() faild");
        goto err;
    }

    if (pthread_create(&t7, NULL, threads, lock)) {
        ERROR("pthread_create() faild");
        goto err;
    }

    if (pthread_create(&t8, NULL, threads, lock)) {
        ERROR("pthread_create() faild");
        goto err;
    }

    if (pthread_create(&t9, NULL, threads, lock)) {
        ERROR("pthread_create() faild");
        goto err;
    }

    if (pthread_create(&t10, NULL, threads, lock)) {
        ERROR("pthread_create() faild");
        goto err;
    }

    if (pthread_join(t10, NULL)) {
        ERROR("pthread_join() faild for %lu.", t1);
        goto err;
    }

    if (pthread_join(t9, NULL)) {
        ERROR("pthread_join() faild for %lu.", t1);
        goto err;
    }

    if (pthread_join(t8, NULL)) {
        ERROR("pthread_join() faild for %lu.", t1);
        goto err;
    }

    if (pthread_join(t7, NULL)) {
        ERROR("pthread_join() faild for %lu.", t1);
        goto err;
    }

    if (pthread_join(t6, NULL)) {
        ERROR("pthread_join() faild for %lu.", t1);
        goto err;
    }

    if (pthread_join(t5, NULL)) {
        ERROR("pthread_join() faild for %lu.", t1);
        goto err;
    }

    if (pthread_join(t4, NULL)) {
        ERROR("pthread_join() faild for %lu.", t1);
        goto err;
    }

    if (pthread_join(t3, NULL)) {
        ERROR("pthread_join() faild for %lu.", t1);
        goto err;
    }

    if (pthread_join(t2, NULL)) {
        ERROR("pthread_join() faild for %lu.", t1);
        goto err;
    }

    if (pthread_join(t1, NULL)) {
        ERROR("pthread_join() faild for %lu.", t1);
        goto err;
    }

    if (lock_id) {
        ERROR("lock_id is not 0.");
        goto err;
    }

    return UT_SUCCESS;
err:
    return UT_FAIL;
}



/*
 * test_serial()
 */
int test_serial(TLock *lock)
{

    // 1
    if (tl_lock(lock)) {
        ERROR("tl_lock() faild.");
        goto err;
    }

    // 2 
    if (tl_lock(lock)) {
        ERROR("tl_lock() faild.");
        goto err;
    }

    // 3 
    if (tl_lock(lock)) {
        ERROR("tl_lock() faild.");
        goto err;
    }

    // 4 
    if (tl_lock(lock)) {
        ERROR("tl_lock() faild.");
        goto err;
    }

    // 4 
    if (tl_unlock(lock)) {
        ERROR("tl_lock() faild.");
        goto err;
    }

    // 3 
    if (tl_unlock(lock)) {
        ERROR("tl_lock() faild.");
        goto err;
    }

    // 2 
    if (tl_unlock(lock)) {
        ERROR("tl_lock() faild.");
        goto err;
    }

    // 1 
    if (tl_unlock(lock)) {
        ERROR("tl_lock() faild.");
        goto err;
    }

    return UT_SUCCESS;
err:
    return UT_FAIL;
}


/*
 * test_serial_non_recursive()
 */
int test_serial_non_recursive(TLock *lock)
{
    if (tl_lock(lock)) {
        ERROR("tl_lock() faild.");
        goto err;
    }

    if (!tl_trylock(lock)) {
        ERROR("tl_lock() succeeded, but it shouldn't.");
        goto err;
    }

    if (tl_unlock(lock)) {
        ERROR("tl_unlock() faild.");
        goto err;
    }
    
    ///////

    if (tl_trylock(lock)) {
        ERROR("tl_lock() faild.");
        goto err;
    }

    if (!tl_trylock(lock)) {
        ERROR("tl_lock() succeeded, but it shouldn't.");
        goto err;
    }

    if (tl_unlock(lock)) {
        ERROR("tl_unlock() faild.");
        goto err;
    }

    return UT_SUCCESS;
err:
    return UT_FAIL;
}


/*
 * main()
 */
int main(int argc, char **argv) 
{
    TLock _lock;
    TLock *lock = &_lock;

    srand(time(NULL));

    printf("recursive\n");
    if (tl_init(lock, TLOCK_FLAGS_RECURSIVE)) {
        ERROR("tl_init() faild.");
        goto err;
    }

    if (test_serial(lock)) {
        ERROR("Serial test faild.");
        goto err_mut;
    }

    if (test_parallel(lock)) {
        ERROR("Parallel test faild.");
        goto err_mut;
    }

    if (tl_destroy(lock)) {
        ERROR("tl_destory() faild.");
        goto err;
    }

    ///////////
    printf("non recursive\n");
    if (tl_init(lock, 0)) {
        ERROR("tl_init() faild.");
        goto err;
    }

    if (test_serial_non_recursive(lock)) {
        ERROR("Serial test faild.");
        goto err_mut;
    }

    if (tl_destroy(lock)) {
        ERROR("tl_destory() faild.");
        goto err;
    }

    return UT_SUCCESS;
err_mut:
    if (tl_destroy(lock)) {
        WARN("tl_destory() faild.");
    }
err:
    return UT_FAIL;
}

