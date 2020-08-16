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
