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

/**
 * @file
 * The parallel\* functions are an abstraction layer for the parallel 
 * execution of program code. They automatically determine number of CPUs and
 * spawn threads accordingly as parallel().
 *
 * The function parallel_jobs() also handles some memory protection to avoid
 * race conditions.
 */
#ifndef __PARALLEL_H__
#define __PARALLEL_H__

#define PARALLEL_OK            ( 0)
#define PARALLEL_ERR           (-1)
#define PARALLEL_NOT_INIT      (-2)
#define PARALLEL_ALREADY_INIT  (-3)
#define PARALLEL_THREAD_CREATE (-4)

/**
 * Initialized the parallel environment.
 * May allocate some memory. Therefor parallel_clean() needs to be called
 * before program termination.
 */
int parallel_init();

/**
 * Cleans up the parallel environment.
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
 * The return value of func is ignored.
 *
 * parallel_init() needs to be called once per program and before this
 * call. If parallel_init() was not called, this function returns 
 * PARALLEL_NOT_INIT.
 *
 * If NO_THREADING is defined at compile time, then this function executes
 * func() single threaded.
 */
int parallel(void *(*func)(void *data), void *data);

/**
 * @brief This function creates as many threads as cpu cores are available and
 * each thread executes the three provided functions.
 *
 * The concept is, that the 'get' function provides pointer to a job,
 * which the current thread shall work on within the 'execute' function.
 * The 'collect' function is responsible to gather the results of the
 * 'execute' function and merge them together to form the result of the
 * parallel_jobs() function.
 *
 * The 'get' function is responsible to provide a pointer to
 * a job as return value. If it returns a null pointer, than the thread
 * will end its execution (the 'execute' is not executed). Two arguments
 * are provided to the 'get' function. The argument 'previous_job' is a 
 * pointer to a job, which the 'get' function * was returning in the 
 * previous call. For the first call, 'previous_job' is NULL. The second
 * parameter is the 'data' pointer, which was provided to the function
 * parallel_jobs(). The 'get' function is protected by a mutex, so that
 * it is guaranteed, that two threads does not execute the 'get' function
 * at the same time.
 *
 * The 'execute' function is called after the 'get' function and the returned
 * pointer from 'get' is provided as first argument to 'execute'. The second
 * argument is the 'data' pointer, which was provided to the function
 * parallel_jobs(). This function is not protected by a mutex and is therefore
 * executed in parallel by the threads.
 *
 * The 'collect' function is executed after 'execute' function has finished.
 * The first argument to the 'collect' function is the same pointe, which the
 * 'execute' function was getting. Furthermore, the 'collect' function is
 * protected by its own mutex, so that it is guaranteed, that only one thread
 * is executing the 'collect' function at the same time. Please note, the 'get'
 * function and the 'collect' function have their own mutexes, so that it is
 * possible, that one thread executes the 'get' function and another thread is
 * executing the 'collect' function. 
 *
 * If the argument 'get' or 'execute' is a null pointer, than this function 
 * returns PARALLEL_ERR. If the argument 'collect' is a null pointer,
 * than this function is not executed.
 */
int parallel_jobs(void *(*get)(void *previous_job, void *data),
                  void (*execute)(void *job, void *data),
                  void (*collect)(void *job, void *data),
                  void *data);

#endif
