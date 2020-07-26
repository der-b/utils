/* Copyright 2015 Bernd Lehmann (der-b@der-b.com)
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
#include <stdio.h>
#include <stdlib.h>
#include <utils/list.h>
#include "unittest.h"


/*
 * ut_generic()
 */
int ut_generic(enum list_type type)
{
#ifndef _LIST_PREALLOC
    List *list = NULL;
#else 
    List _list;
    List *list = &_list;
#endif
    LOpts opts;
    int i;
    struct complex_data cnt;
    struct complex_data data;

    memset(&opts, 0, sizeof(opts));
    opts.datasize = sizeof(int);

    cnt.number = 0;
    cnt.number2 = 42.23;

    data.number = 13117;
    data.number2 = 13117.42;

#ifndef _LIST_PREALLOC
    /*** new list ***/
    printf("L_NEW\n");
    list = L_NEW(type, &opts);
    if (!list) {
        fprintf(stderr, "ERR: L_NEW returned NULL pointer\n");
        return UT_FAIL;
    }
#else 
    /*** init list ***/
    printf("L_INIT\n");
    if (L_INIT(&_list, type, &opts)) {
        fprintf(stderr, "ERR: L_INIT did not return LIST_OK pointer\n");
        return UT_FAIL;
    }
#endif

    ut_state(list);

    /*** try popfront ***/
    printf("L_POPFRONT\n");
    if (LIST_EMPTY != L_POPFRONT(list, &cnt)) {
        fprintf(stderr,
                "ERR: L_POPFRONT returned not LIST_EMPTY\n");
        return UT_FAIL;
    }

    /*** try popback ***/
    printf("L_POPBACK\n");
    if (LIST_EMPTY != L_POPBACK(list, &cnt)) {
        fprintf(stderr,
                "ERR: L_POPBACK returned not LIST_EMPTY\n");
        return UT_FAIL;
    }

    /*** pushback 10 elements ***/
    printf("L_PUSHBACK 10 elements\n");
    for (i = 0; i < 10; ++i, ++cnt.number) {
        if (!L_PUSHBACK(list, &cnt)) {
            fprintf(stderr,
                    "ERR: L_PUSHBACK returned LIST_ERR\n");
            return UT_FAIL;
        }
    }

    ut_state(list);

    /*** pushfront 10 elements ***/
    printf("L_PUSHFRONT 10 elements\n");
    for (i = 0; i < 10; ++i, ++cnt.number) {
        if (!L_PUSHFRONT(list, &cnt)) {
            fprintf(stderr,
                    "ERR: L_PUSHFRONT returned LIST_ERR\n");
            return UT_FAIL;
        }
    }

    ut_state(list);

    /*** insert 10 elements ***/
    printf("L_INSERT 10 elements\n");
    for (i = 0; i < 10; ++i, ++cnt.number) {
        if (!L_INSERT(list, 5 + i, &cnt)) {
            fprintf(stderr,
                    "ERR: L_INSERT returned LIST_ERR\n");
            return UT_FAIL;
        }
    }

    ut_state(list);

    /*** remove 5 elements ***/
    printf("L_REMOVE 5 elements\n");
    for (i = 0; i < 5; ++i, ++cnt.number) {
        if (L_REMOVE(list, 12, &data)) {
            fprintf(stderr,
                    "ERR: L_REMOVE returned LIST_ERR\n");
            return UT_FAIL;
        }
        printf("got back %d\n", data.number);
    }

    ut_state(list);

    /*** popfront 3 elements ***/
    printf("L_POPFRONT  elements\n");
    for (i = 0; i < 3; ++i, ++cnt.number) {
        if (L_POPFRONT(list, &data)) {
            fprintf(stderr,
                    "ERR: L_POPFRONT returned LIST_ERR\n");
            return UT_FAIL;
        }
        printf("got back %d\n", data.number);
    }

    ut_state(list);

    /*** popback 3 elements ***/
    printf("L_POPBACK  elements\n");
    for (i = 0; i < 3; ++i, ++cnt.number) {
        if (L_POPBACK(list, &data)) {
            fprintf(stderr,
                    "ERR: L_POPBACK returned LIST_ERR\n");
            return UT_FAIL;
        }
        printf("got back %d\n", data.number);
    }

    ut_state(list);

    /*** remove last elements ***/
    printf("L_REMOVE last elements\n");
    if (L_REMOVE(list, L_SIZE(list) - 1, &data)) {
        fprintf(stderr,
                "ERR: L_REMOVE returned LIST_ERR\n");
        return UT_FAIL;
    }
    printf("got back %d\n", data.number);

    ut_state(list);

    /*** clear list ***/
#ifndef _LIST_PREALLOC
    if (L_FREE(list)) {
        return UT_FAIL;
    }
#else
    if (L_DESTROY(&_list)) {
        return UT_FAIL;
    }
#endif


    return UT_SUCCESS;
}
