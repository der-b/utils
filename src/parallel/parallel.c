#include <utils/parallel.h>
#include <utils/log.h>
#include <utils/tlock.h>
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
#ifdef NO_THREADING
    func(data);
    return PARALLEL_OK;
#else 
    int ret = PARALLEL_OK;
    int i;
    int max;
    pthread_t *threads;

    if (!__parallel_data.initialized) {
        return PARALLEL_NOT_INIT;
    }

    threads = malloc(__parallel_data.cpus * sizeof(pthread_t));
    if (!threads) {
        goto err;
    }

    // If we are not already parallelized, then start parallelisation
    if (TLOCK_RET_SUCCESS == tl_trylock(&__parallel_data.recursive)) {
        pthread_t *t;

        for (i = 0; i < __parallel_data.cpus; ++i) {
            if (pthread_create(&(threads[i]), NULL, func, data)) {
                ret = PARALLEL_THREAD_CREATE;
                break;
            }
        }

        max = i;
        int tmp_ret = PARALLEL_OK;
        for (i = 0; i < max; i++) {
            if (pthread_join(threads[i], NULL)) {
               tmp_ret = PARALLEL_ERR;
            }
        }

        if (PARALLEL_OK == ret && PARALLEL_OK != tmp_ret) {
            ret = tmp_ret;
        }

        tl_unlock(&__parallel_data.recursive);

    // If we already are parallelized, than do func serial
    } else {
        func(data);
    }

    free(threads);

    return ret;
err:
    return PARALLEL_ERR;
#endif
}

