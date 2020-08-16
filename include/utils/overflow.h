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
#ifndef __overflow_h__
#define __overflow_h__

#include <stdint.h>

/* Note: this macros works only with the gcc, since "typeof" is a non standard extension */

/* see https://www.fefe.de/intof.html for more details on integer overflows */

/* ==== Important ====
 * This implementation has no protection for unintentional side effcts 
 * like the implementation from the link above. Since one can never be sure 
 * that any macro is free of side effect, i consider it as very bad code 
 * style when a macro is called in a way which can lead to side effects. 
 * In this case the mistake is done by caller of the macro.
 *
 * The behavior of all macros is undefined, if a 'dest' parameter is the
 * same variable as 'src' or 'a' or 'b'.
 * For example the behavior for following calls are undefined:
 * __ADD(a, a, b);
 * __ASSIGN(a, a);
 * __SUB(a, b, a);
 * __MUL(a, a, a);
 *
 * This code is not standard C and uses a gcc extension. If anybody knows a C
 * compliant implementation please write an email at der-b [at] der-b [dot] com
 */

#define OVERFLOW_ERR (0 == 0)
#define OVERFLOW_OK (0 != 0)

#define __HALF_MAX_SIGNED(type) ((type)1 << (sizeof(type) * 8 - 2))
#define __MAX_SIGNED(type)      (__HALF_MAX_SIGNED(type) - 1 + __HALF_MAX_SIGNED(type))
#define __MIN_SIGNED(type)	(-1 - __MAX_SIGNED(type))

/* returns the minimal value of a given type */
#define __MIN(type) (((type)-1 < 1)?(__MIN_SIGNED(type)):((type)0))

/* returns the maximum value of a given type */
#define __MAX(type) ((type)~__MIN(type))

/* if the assignment fails, the value of dest is undefined */
#define __ASSIGN(dest,src) ((dest) = (src), (src) != (dest) || (((src) < 1) != ((dest) < 1)))

/* addition */
#define __ADD(dest, a, b) (((b) < 1) ? \
                ((__MIN(typeof(dest)) - (b) <= (a)) ? __ASSIGN(dest, (a) + (b)) : OVERFLOW_ERR): \
                ((__MAX(typeof(dest)) - (b) >= (a)) ? __ASSIGN(dest, (a) + (b)) : OVERFLOW_ERR))

/* subtraction */
#define __SUB(dest, a, b) (((b) < 1) ? \
                ((__MAX(typeof(dest)) + (b) >= (a)) ? __ASSIGN(dest, (a) - (b)) : 1): \
                ((__MIN(typeof(dest)) + (b) <= (a)) ? __ASSIGN(dest, (a) - (b)) : 1))

#define __INC(dest) ({ \
                typeof(dest) tmp; \
                (__ADD(tmp, dest, 1))?(1):(dest = tmp, 0); \
                })

#define __DEC(dest) ({ \
                typeof(dest) tmp; \
                (__SUB(tmp, dest, 1))?(1):(dest = tmp, 0); \
                })


#define __IS_SIGNED(a) ((typeof(a))-1 < 0)
#define __IS_64BIT(a) (sizeof(uint64_t) == sizeof(a))

#define __MUL(dest, a, b) (__IS_64BIT(a * b) ? __MUL64(dest, a, b) : __MUL32(dest, a, b))

#define __MUL32(dest, a, b) (__IS_SIGNED(a * b)?__MUL_I32(dest, a, b):__MUL_U32(dest, a, b))

/* as long as the check_implicit_conversion() check works, this should also work
 * correctly */
#define __MUL_I32(dest, a, b) ({ \
                int32_t __tmp__; \
                __signed_mul32(&__tmp__, a, b) || __ASSIGN(dest, __tmp__); \
                })

/* unfortunately, signed operands will be cast to unsigned. Have to be checkted
 * TODO: Check whether the compile is able to optimize the assign checks away if
 * a or b are unsigned. If not include extra checks using __IS_SIGNED ... this
 * should work.
 */
#define __MUL_U32(dest, a, b) ({ \
                uint32_t __tmp__; \
                (__ASSIGN(__tmp__, a) || __ASSIGN(__tmp__, b))?(OVERFLOW_ERR):(__unsigned_mul32(&__tmp__, a, b) || __ASSIGN(dest, __tmp__)); \
                })

#define __MUL64(dest, a, b) (__IS_SIGNED(a * b)?__MUL_I64(dest, a, b):__MUL_U64(dest, a, b))

#define __MUL_I64(dest, a, b) ({ \
                int64_t __tmp__; \
                __signed_mul64(&__tmp__, a, b) || __ASSIGN(dest, __tmp__); \
                })


#define __MUL_U64(dest, a, b) ({ \
                uint64_t __tmp__; \
                (__ASSIGN(__tmp__, a) || __ASSIGN(__tmp__, b))?(OVERFLOW_ERR):(__unsigned_mul64(&__tmp__, a, b) || __ASSIGN(dest, __tmp__)); \
                })


int32_t __signed_mul32(int32_t * c, int32_t a, int32_t b);
int32_t __unsigned_mul32(uint32_t * c, uint32_t a, uint32_t b);

int32_t __signed_mul64(int64_t * c, int64_t a, int64_t b);
int32_t __unsigned_mul64(uint64_t * c, uint64_t a, uint64_t b);

#endif
