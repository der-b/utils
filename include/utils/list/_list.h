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
#ifndef ___list_h__
#define ___list_h__

#ifndef _LIST_OPTIMIZE
#define _LIST_OPTIMIZE 1
#endif

#if _LIST_OPTIMIZE
#define _LIST_ASSUME_NOT_USING_MACROS  1
#define _LIST_INVALID_STRUCT_POSSIBLE  1
#else
#define _LIST_ASSUME_NOT_USING_MACROS  0
#define _LIST_INVALID_STRUCT_POSSIBLE  0
#endif

#include "utils/list.h"

#endif				/* ___list_h__ */
