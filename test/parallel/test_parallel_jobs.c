#include "../test_header.h"
#include <utils/parallel.h>
#include <utils/log.h>
#include <utils/tlock.h>
#include <utils/list.h>

#define NUMBER_OF_JOBS 1000
#define JOB_SCALE 1000

struct _job {
    size_t name;
    size_t work;
};

struct _data {
    TLock stdout_lock;
    List jobs;
    size_t pos;
};

/*
 * get()
 */
void *get(void *previous_job, void *_data)
{
    struct _data *data = _data;
    void *job = L_GET(&data->jobs, data->pos);
    (data->pos)++;
    return job;
}


/*
 * process()
 */
void process(void *_job, void *_data)
{
    struct _data *data = _data;
    struct _job *job = _job;

    if (tl_lock(&data->stdout_lock)) {
        WARN("tl_lock() faild.");
    }

    INFO("Got Job number %zu: Count down from %zu.", job->name, job->work);

    if (tl_unlock(&data->stdout_lock)) {
        WARN("tl_unlock() faild.");
    }

    while(job->work) {
        job->work--;
    }

    if (tl_lock(&data->stdout_lock)) {
        WARN("tl_lock() faild.");
    }

    INFO("Job number %zu is done.", job->name);

    if (tl_unlock(&data->stdout_lock)) {
        WARN("tl_unlock() faild.");
    }
}


/*
 * main()
 */
int main(int argc, char **argv)
{
    struct _data data;
    LOpts opts;


    if (parallel_init()) {
        ERROR("parallel_init() faild.");
        goto err;
    }

    if (tl_init(&data.stdout_lock, 0)) {
        ERROR("tl_init() faild.");
        goto err_parallel;
    }

    memset(&opts, 0, sizeof(opts));
    opts.datasize = sizeof(struct _job);

    if (L_INIT(&data.jobs, LIST_TYPE_ARRAY, &opts)) {
        ERROR("L_INIT() faild.");
        goto err_tlock;
    }

    for (size_t i = 0; NUMBER_OF_JOBS > i; ++i) {
        struct _job j;
        j.name = i;
        j.work = (rand() % 0xff) * JOB_SCALE;

        if (NULL == L_PUSHBACK(&data.jobs, &j)) {
            ERROR("L_PUSHBACK() faild.");
            goto err_list;
        }
    }

    data.pos = 0;

    if (parallel_jobs(get , process, NULL, &data)) {
        ERROR("parallel() faild.");
        goto err_list;
    }

    LIter iter;
    struct _job *j;

    L_FOREACH(&data.jobs, &iter, j) {
        if (j->work) {
            ERROR("Result after parallel_jobs() is wrong.");
            goto err_list;
        }
    }

    if (L_DESTROY(&data.jobs)) {
        ERROR("L_DESTROY() faild.");
        goto err_tlock;
    }

    if (tl_destroy(&data.stdout_lock)) {
        ERROR("tl_destroy() faild.");
        goto err_parallel;
    }

    if (parallel_clean()) {
        ERROR("parallel_clean() faild.");
        goto err;
    }

    return UT_SUCCESS;

err_list:
    if (L_DESTROY(&data.jobs)) {
        WARN("L_DESTROY() faild.");
    }
err_tlock:
    if (tl_destroy(&data.stdout_lock)) {
        WARN("tl_destroy() faild.");
    }
err_parallel:
    if (parallel_clean()) {
        WARN("parallel_clean() faild.");
    }
err:
    return UT_FAIL;
}
