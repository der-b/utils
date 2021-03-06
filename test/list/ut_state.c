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
 * state()
 */
void ut_state(List * list)
{
	LIter iter;
	struct complex_data *i;
	printf("State of the list object:\n");
	printf("element size: %lu\n", L_DATASIZE(list));
	printf("length: %lu\n", L_SIZE(list));

	i = L_FIRST(list);
	if (i) {
		printf("first:  %d\n", i->number);
	} else {
		printf("first:  (null)\n");
	}

	i = L_LAST(list);
	if (i) {
		printf("last:   %d\n", i->number);
	} else {
		printf("last:   (null)\n");
	}

	printf("content:");
	L_FOREACH(list, &iter, i) {
		printf(" %d", i->number);
	}
	printf("\n\n");
}
