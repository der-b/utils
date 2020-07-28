#ifndef __PARALLEL_H__
#define __PARALLEL_H__

#define PARALLEL_OK            ( 0)
#define PARALLEL_ERR           (-1)
#define PARALLEL_NOT_INIT      (-2)
#define PARALLEL_ALREADY_INIT  (-3)
#define PARALLEL_THREAD_CREATE (-4)

/**
 * Initialized the prallel environment.
 * May allocate some memory. Threfor parallel_clean() needs to be called
 * before program termination.
 */
int parallel_init();

/**
 * Cleans up the prarallel environemnt.
 */
int parallel_clean();

/**
 * This function spawns as many threads as cpu cores are available and
 * executes func() on each thread. If this function is recursively called
 * in fun, than the second call will not create new threads. In this
 * case, fun will be executed single threaded within the already existing
 * thread.
 *
 * The parameter to func, is the same pointer as the parameter data
 * of parallel.
 *
 * parallel_init() needs to be called once per program and before this
 * call. If parallel_init() was not called, this function returns 
 * PARALLEL_NOT_INIT.
 *
 * If NO_THREADING is defined at compile time, then this function executes
 * func() single threaded.
 */
int parallel(void *(*func)(void *data), void *data);

#endif
