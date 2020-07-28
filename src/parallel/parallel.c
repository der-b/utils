#include <utils/parallel.h>
#include <utils/log.h>
#include <utils/tlock.h>
#include <utils/list.h>
#include <sys/sysinfo.h>
#include <stdint.h>
#include <pthread.h>

struct {
    TLock recursive;
    uint8_t initialized;
    int cpus;
} __parallel_data;


/*
 * parallel_init()
 */
int parallel_init()
{
    if (__parallel_data.initialized) {
        return PARALLEL_ALREADY_INIT;
    }
    __parallel_data.initialized = 1;

    __parallel_data.cpus = get_nprocs();

    INFO("Found %d hardware threads.", __parallel_data.cpus);

    if (tl_init(&__parallel_data.recursive, 0)) {
        goto err;
    }

    return PARALLEL_OK;
err:
    __parallel_data.initialized = 0;
    return PARALLEL_ERR;
}


/*
 * parallel_clean()
 */
int parallel_clean()
{
    if (!__parallel_data.initialized) {
        return PARALLEL_NOT_INIT;
    }
    __parallel_data.initialized = 0;

    if (tl_destroy(&__parallel_data.recursive)) {
        goto err;
    }
    __parallel_data.cpus = 0;

    return PARALLEL_OK;
err:
    __parallel_data.initialized = 1;
    return PARALLEL_ERR;
}


/*
 * parallel()
 */
int parallel(void *(*func)(void *data), void *data)
{
    int ret = PARALLEL_OK;
    int i;
    List threads;
    LOpts opts;

    if (!__parallel_data.initialized) {
        return PARALLEL_NOT_INIT;
    }

    memset(&opts, 0, sizeof(opts));
    opts.datasize = sizeof(pthread_t);

    if (L_INIT(&threads, LIST_TYPE_DLINKED, &opts)) {
        goto err;
    }

    // If we are not already parallelized, then start parallelisation
    if (TLOCK_RET_SUCCESS == tl_trylock(&__parallel_data.recursive)) {
        pthread_t *t;

        for (i = 0; i < __parallel_data.cpus; ++i) {
            t = L_NEWELEMENT(&threads);

            if (!t) {
                // We leave the loop, to clean up the rest, but set an error state
                // to inform the caller
                ret = PARALLEL_ERR;
                break;
            }

            if (pthread_create(t, NULL, func, data)) {
                ret = PARALLEL_THREAD_CREATE;
                break;
            }
        }

        int tmp_ret = PARALLEL_OK;
        while (L_SIZE(&threads)) {
            pthread_t *t_for_join = L_LAST(&threads);

            // if we had an error before and t_for_join has the
            // same address as thread t, than this thread can't be
            // joined.
            if (PARALLEL_OK != ret && t == t_for_join) {
                L_POPBACK(&threads, NULL);
                continue;
            }

            if (pthread_join(*t_for_join, NULL)) {
               tmp_ret = PARALLEL_ERR;
            }

            L_POPBACK(&threads, NULL);
        }

        if (PARALLEL_OK == ret && PARALLEL_OK != tmp_ret) {
            ret = tmp_ret;
        }

        tl_unlock(&__parallel_data.recursive);

    // If we already are parallelized, than do func serial
    } else {
        func(data);
    }

    if (L_DESTROY(&threads)) {
        WARN("Destroying of thread list failed. Is the memory corrupt?");
    }

    return ret;
err:
    return PARALLEL_ERR;
}

