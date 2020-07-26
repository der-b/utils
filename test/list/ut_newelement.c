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
#include <utils/log.h>
#include "unittest.h"


/*
 * ut_newelement()
 */
int ut_newelement(enum list_type type)
{
    List _list;
    List *list = &_list;
    LOpts opts;
    int i;

    memset(&opts, 0, sizeof(opts));
    opts.datasize = sizeof(int);

    /*** init list ***/
    printf("L_INIT\n");
    if (L_INIT(&_list, type, &opts)) {
        ERROR("L_INIT did not return LIST_OK pointer\n");
        return UT_FAIL;
    }

    for (int i = 0; 10 > i; ++i) {
        int *tmp = L_NEWELEMENT(list);
        if (!tmp) {
            ERROR("L_NEWELEMENT() faild.");
            return UT_FAIL;
        }
    }

    if (10 != L_SIZE(list)) {
        ERROR("Wrong L_SIZE of list.");
        return UT_FAIL;
    }

    for (int i = 0; 10 > i; ++i) {
        int *tmp = L_GET(list, i);
        *tmp = i;
    }

    ut_state(list);

    printf("L_DESTROY\n");
    if (L_DESTROY(&_list)) {
        return UT_FAIL;
    }

    return UT_SUCCESS;
}
