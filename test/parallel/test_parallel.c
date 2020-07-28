#include "../test_header.h"
#include <utils/parallel.h>
#include <utils/log.h>
#include <utils/tlock.h>

void *test(void *data)
{
    TLock *lock = data;

    if (tl_lock(lock)) {
        WARN("tl_lock() faild.");
    }

    INFO("Hier bin ich!");

    if (tl_unlock(lock)) {
        WARN("tl_unlock() faild.");
    }
}

int main(int argc, char **argv)
{
    TLock lock;
    if (parallel_init()) {
        ERROR("parallel_init() faild.");
        goto err;
    }

    if (tl_init(&lock, 0)) {
        ERROR("tl_init() faild.");
        goto err_parallel;
    }

    if (parallel(test, &lock)) {
        ERROR("parallel() faild.");
        goto err_tlock;
    }

    if (tl_destroy(&lock)) {
        ERROR("tl_destroy() faild.");
        goto err_parallel;
    }

    if (parallel_clean()) {
        ERROR("parallel_clean() faild.");
        goto err;
    }

    return UT_SUCCESS;

err_tlock:
    if (tl_destroy(&lock)) {
        WARN("tl_destroy() faild.");
    }
err_parallel:
    if (parallel_clean()) {
        WARN("parallel_clean() faild.");
    }
err:
    return UT_FAIL;
}
