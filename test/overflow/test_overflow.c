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
#include <utils/overflow.h>
#include <utils/log.h>
#include <utils/limits.h>
#include <stdint.h>

#include "../test_header.h"

#define getType(a) ((sizeof(int8_t)  == sizeof(a)) ? ("int8_t ") : ( \
                    (sizeof(int16_t) == sizeof(a)) ? ("int16_t") : ( \
                    (sizeof(int32_t)  == sizeof(a)) ? ("int32_t") : ( \
                    (sizeof(int64_t)  == sizeof(a)) ? ("int64_t") : ("unknown")))))

#define getSign(a) (((typeof(a))-1 < 0) ? (" ") : \
                                          ("u"))


/*
 * check_MUL()
 */
int check_MUL()
{
#define _CHECK_MUL(type) \
do { \
        type a; \
        type b; \
        type c; \
        \
        a = __MAX(type); \
        b = 0; \
        if (__MUL(c, a, b) || 0 != c) { \
                ERROR("Multiplication of %lu and %lu faild (" #type ")", (uint64_t)a, (uint64_t)b); \
                goto err; \
        } \
        \
        a = 0; \
        b = __MAX(type); \
        if (__MUL(c, a, b) || 0 != c) { \
                ERROR("Multiplication of %lu and %lu faild (" #type ")", (uint64_t)a, (uint64_t)b); \
                goto err; \
        } \
        \
        a = __MAX(type); \
        b = 1; \
        if (__MUL(c, a, b) || __MAX(type) != c) { \
                ERROR("Multiplication of %lu and %lu faild (" #type ")", (uint64_t)a, (uint64_t)b); \
                goto err; \
        } \
        \
        a = 1; \
        b = __MAX(type); \
        if (__MUL(c, a, b) || __MAX(type) != c) { \
                ERROR("Multiplication of %lu and %lu faild (" #type ")", (uint64_t)a, (uint64_t)b); \
                goto err; \
        } \
        \
        a = __MAX(type) / 2; \
        b = 2; \
        if (__MUL(c, a, b) || __MAX(type) - 1 != c) { \
                ERROR("Multiplication of %lu and %lu faild (" #type ")", (uint64_t)a, (uint64_t)b); \
                goto err; \
        } \
        \
        a = 2; \
        b = __MAX(type) / 2; \
        if (__MUL(c, a, b) || __MAX(type) - 1 != c) { \
                ERROR("Multiplication of %lu and %lu faild (" #type ")", (uint64_t)a, (uint64_t)b); \
                goto err; \
        } \
        \
        if (__IS_SIGNED(type)) { \
                a = -1; \
                b = __MAX(type); \
                if (__MUL(c, a, b) || __MIN(type) + 1 != c) { \
                        ERROR("Multiplication of %ld and %ld faild (" #type ")", (int64_t)a, (int64_t)b); \
                        goto err; \
                } \
                \
                a = __MAX(type); \
                b = -1; \
                if (__MUL(c, a, b) || __MIN(type) + 1 != c) { \
                        ERROR("Multiplication of %ld and %ld faild (" #type ")", (int64_t)a, (int64_t)b); \
                        goto err; \
                } \
        }\
        \
        a = __MAX(type); \
        b = 2; \
        if (!__MUL(c, a, b)) { \
                ERROR("Multiplication of %lu and %lu should faild but didn't. (" #type ")", (uint64_t)a, (uint64_t)b); \
                goto err; \
        } \
        \
        a = 2; \
        b = __MAX(type); \
        if (!__MUL(c, a, b)) { \
                ERROR("Multiplication of %lu and %lu should faild but didn't. (" #type ")", (uint64_t)a, (uint64_t)b); \
                goto err; \
        } \
        \
        a = __MAX(type); \
        b = __MAX(type); \
        if (!__MUL(c, a, b)) { \
                ERROR("Multiplication of %lu and %lu should faild but didn't. (" #type ")", (uint64_t)a, (uint64_t)b); \
                goto err; \
        } \
        \
        if (__IS_SIGNED(type)) { \
                a = __MIN(type); \
                b = 2; \
                if (!__MUL(c, a, b)) { \
                        ERROR("Multiplication of %ld and %ld should faild but didn't. (" #type ")", (int64_t)a, (int64_t)b); \
                        goto err; \
                } \
                \
                a = 2; \
                b = __MIN(type); \
                if (!__MUL(c, a, b)) { \
                        ERROR("Multiplication of %ld and %ld should faild but didn't. (" #type ")", (int64_t)a, (int64_t)b); \
                        goto err; \
                } \
                \
                a = __MIN(type); \
                b = __MIN(type); \
                if (!__MUL(c, a, b)) { \
                        ERROR("Multiplication of %ld and %ld should faild but didn't. (" #type ")", (int64_t)a, (int64_t)b); \
                        goto err; \
                } \
        }\
} while (0);

	_CHECK_MUL(int8_t);
	_CHECK_MUL(uint8_t);
	_CHECK_MUL(int16_t);
	_CHECK_MUL(uint16_t);
	_CHECK_MUL(int32_t);
	_CHECK_MUL(uint32_t);
	_CHECK_MUL(int64_t);
	_CHECK_MUL(uint64_t);

#undef _CHECK_MUL

#define _CHECK_MUL2(type1, type2) \
{ \
        type1 a; \
        type1 b; \
        type1 c; \
        \
        if (__IS_SIGNED(type1)) { \
                a = __MAX(type1) / __MAX(type2); \
                b = (type1)__MAX(type2); \
                if (__MUL(c, a, b) || __MAX(type1) - 1 != c) { \
                        ERROR("Multiplication of %ld and %ld faild (" #type1 ", " #type2 ")", (int64_t)a, (int64_t)b); \
                        goto err; \
                } \
                \
                a = (type1)__MAX(type2); \
                b = __MAX(type1) / __MAX(type2); \
                if (__MUL(c, a, b) || __MAX(type1) - 1 != c) { \
                        ERROR("Multiplication of %ld and %ld faild (" #type1 ", " #type2 ")", (int64_t)a, (int64_t)b); \
                        goto err; \
                } \
                \
                a = __MIN(type1) / __MAX(type2); \
                b = (type1)__MAX(type2); \
                if (__MUL(c, a, b) || __MIN(type1) + 2 != c) { \
                        ERROR("Multiplication of %ld and %ld faild (" #type1 ", " #type2 ")", (int64_t)a, (int64_t)b); \
                        goto err; \
                } \
                \
                a = (type1)__MAX(type2); \
                b = __MIN(type1) / __MAX(type2); \
                if (__MUL(c, a, b) || __MIN(type1) + 2 != c) { \
                        ERROR("Multiplication of %ld and %ld faild (" #type1 ", " #type2 ")", (int64_t)a, (int64_t)b); \
                        goto err; \
                } \
        } else { \
                a = __MAX(type1) / __MAX(type2); \
                b = (type1)__MAX(type2); \
                if (__MUL(c, a, b) || __MAX(type1) != c) { \
                        ERROR("Multiplication of %lu and %lu faild (" #type1 ", " #type2 ")", (uint64_t)a, (uint64_t)b); \
                        goto err; \
                } \
                \
                a = (type1)__MAX(type2); \
                b = __MAX(type1) / __MAX(type2); \
                if (__MUL(c, a, b) || __MAX(type1) != c) { \
                        ERROR("Multiplication of %lu and %lu faild (" #type1 ", " #type2 ")", (uint64_t)a, (uint64_t)b); \
                        goto err; \
                } \
        } \
        \
        a = __MAX(type1) / __MAX(type2); \
        b = 1 + (type1)__MAX(type2); \
        if (!__MUL(c, a, b)) { \
                ERROR("Multiplication of %lu and %lu shoud faild, but didn't. (" #type1 ", " #type2 ")", (uint64_t)a, (uint64_t)b); \
                goto err; \
        } \
        \
        a = 1 + (type1)__MAX(type2); \
        b = __MAX(type1) / __MAX(type2); \
        if (!__MUL(c, a, b)) { \
                ERROR("Multiplication of %lu and %lu shoud faild, but didn't. (" #type1 ", " #type2 ")", (uint64_t)a, (uint64_t)b); \
                goto err; \
        } \
        \
        if (__IS_SIGNED(type1)) { \
                a = __MIN(type1) / __MAX(type2); \
                b = 1 + (type1)__MAX(type2); \
                if (!__MUL(c, a, b)) { \
                        ERROR("Multiplication of %ld and %ld shoud faild, but didn't. (" #type1 ", " #type2 ")", (int64_t)a, (int64_t)b); \
                        goto err; \
                } \
                \
                a = 1 + (type1)__MAX(type2); \
                b = __MIN(type1) / __MAX(type2); \
                if (!__MUL(c, a, b)) { \
                        ERROR("Multiplication of %ld and %ld shoud faild, but didn't. (" #type1 ", " #type2 ")", (int64_t)a, (int64_t)b); \
                        goto err; \
                } \
        } \
} while (0);

	_CHECK_MUL2(uint16_t, uint8_t);
	_CHECK_MUL2(int16_t, int8_t);
	_CHECK_MUL2(uint32_t, uint16_t);
	_CHECK_MUL2(int32_t, int16_t);
	_CHECK_MUL2(uint64_t, uint32_t);
	_CHECK_MUL2(int64_t, int32_t);

#undef _CHECK_MUL2

	return UT_SUCCESS;

      err:
	return UT_FAIL;
}


/*
 * check_imlicit_conversion()
 */
int check_implicit_conversion()
{
	uint8_t u8_a = 0;
	uint8_t u8_b = 0;
	uint16_t u16_a = 0;
	uint16_t u16_b = 0;
	uint32_t u32_a = 0;
	uint32_t u32_b = 0;
	uint64_t u64_a = 0;
	uint64_t u64_b = 0;
	int8_t i8_a = 0;
	int8_t i8_b = 0;
	int16_t i16_a = 0;
	int16_t i16_b = 0;
	int32_t i32_a = 0;
	int32_t i32_b = 0;
	int64_t i64_a = 0;
	int64_t i64_b = 0;

#define _CAST_TO_I32(a, b) \
do { \
        if (!__IS_SIGNED(a + b) || (32/8) != sizeof(a + b)) { \
                ERROR("Unexpected type after (" #a " + " #b ")"); \
                goto err; \
        } \
} while(0);

	// each arithmetic operation with a int32_t or smaler signed/unsigned
	// integer should result in an int32_t
	_CAST_TO_I32(u8_a, u8_b);
	_CAST_TO_I32(u8_a, u16_b);
	_CAST_TO_I32(u8_a, i8_b);
	_CAST_TO_I32(u8_a, i16_b);
	_CAST_TO_I32(u8_a, i32_b);
	_CAST_TO_I32(i8_a, u8_b);
	_CAST_TO_I32(i8_a, u16_b);
	_CAST_TO_I32(i8_a, i8_b);
	_CAST_TO_I32(i8_a, i16_b);
	_CAST_TO_I32(i8_a, i32_b);
	_CAST_TO_I32(u16_a, u8_b);
	_CAST_TO_I32(u16_a, u16_b);
	_CAST_TO_I32(u16_a, i8_b);
	_CAST_TO_I32(u16_a, i16_b);
	_CAST_TO_I32(u16_a, i32_b);
	_CAST_TO_I32(i16_a, u8_b);
	_CAST_TO_I32(i16_a, u16_b);
	_CAST_TO_I32(i16_a, i8_b);
	_CAST_TO_I32(i16_a, i16_b);
	_CAST_TO_I32(i16_a, i32_b);
	_CAST_TO_I32(i32_a, u8_b);
	_CAST_TO_I32(i32_a, u16_b);
	_CAST_TO_I32(i32_a, i8_b);
	_CAST_TO_I32(i32_a, i16_b);
	_CAST_TO_I32(i32_a, i32_b);
#undef _CAST_TO_I32

#define _CAST_TO_U32(a, b) \
do { \
        if (__IS_SIGNED(a + b) || (32/8) != sizeof(a + b)) { \
                ERROR("Unexpected type after (" #a " + " #b ")"); \
                goto err; \
        } \
} while(0);

	// each arithmetic operation with a uint32_t should result in uint32_t
	_CAST_TO_U32(u32_a, u8_b);
	_CAST_TO_U32(u32_a, u16_b);
	_CAST_TO_U32(u32_a, u32_b);
	_CAST_TO_U32(u32_a, i8_b);
	_CAST_TO_U32(u32_a, i16_b);
	_CAST_TO_U32(u32_a, i32_b);
#undef _CAST_TO_U32

#define _CAST_TO_I64(a, b) \
do { \
        if (!__IS_SIGNED(a + b) || (64/8) != sizeof(a + b)) { \
                ERROR("Unexpected type after (" #a " + " #b ")"); \
                goto err; \
        } \
} while(0);

	// each arithmetic operation with a int64_t should result in int64_t
	// except with uint64_t
	_CAST_TO_I64(i64_a, u8_b);
	_CAST_TO_I64(i64_a, u16_b);
	_CAST_TO_I64(i64_a, u32_b);
	_CAST_TO_I64(i64_a, i8_b);
	_CAST_TO_I64(i64_a, i16_b);
	_CAST_TO_I64(i64_a, i32_b);
	_CAST_TO_I64(i64_a, i64_b);
#undef _CAST_TO_I64

#define _CAST_TO_U64(a, b) \
do { \
        if (__IS_SIGNED(a + b) || (64/8) != sizeof(a + b)) { \
                ERROR("Unexpected type after (" #a " + " #b ")"); \
                goto err; \
        } \
} while(0);

	// each arithmetic operation with a uint64_t should result in uint64_t
	_CAST_TO_U64(u64_a, u8_b);
	_CAST_TO_U64(u64_a, u16_b);
	_CAST_TO_U64(u64_a, u32_b);
	_CAST_TO_U64(u64_a, u64_b);
	_CAST_TO_U64(u64_a, i8_b);
	_CAST_TO_U64(u64_a, i16_b);
	_CAST_TO_U64(u64_a, i32_b);
	_CAST_TO_U64(u64_a, i64_b);
#undef _CAST_TO_U64


	return UT_SUCCESS;

      err:
	return UT_FAIL;

}


/*
 * check_MIN_MAX()
 */
int check_MIN_MAX(void)
{
	INFO("\r--- int8_t test:");
	INFO("INT8_MIN:      %d", INT8_MIN);
	INFO("__MIN(int8_t): %d", __MIN(int8_t));
	if (INT8_MIN != __MIN(int8_t)) {
		ERROR("INT8_MIN is different to __MIN(int8_t)");
		goto err;
	}
	INFO("INT8_MAX:      %d", INT8_MAX);
	INFO("__MAX(int8_t): %d", __MAX(int8_t));
	if (INT8_MAX != __MAX(int8_t)) {
		ERROR("INT8_MAX is different to __MAX(int8_t)");
		goto err;
	}

	INFO("\r--- uint8_t test:");
	INFO("UCHAR_MIN:      %u", (uint8_t)0);
	INFO("__MIN(uint8_t): %u", __MIN(uint8_t));
	if (0 != __MIN(uint8_t)) {
		ERROR("0 is different to __MIN(uint8_t)");
		goto err;
	}
	INFO("UINT8_MAX:      %u", UINT8_MAX);
	INFO("__MAX(uint8_t): %u", __MAX(uint8_t));
	if (UINT8_MAX != __MAX(uint8_t)) {
		ERROR("UINT8_MAX is different to __MAX(uint8_t)");
		goto err;
	}


	INFO("\r--- int16_t test:");
	INFO("INT16_MIN:      %d", INT16_MIN);
	INFO("__MIN(int16_t): %d", __MIN(int16_t));
	if (INT16_MIN != __MIN(int16_t)) {
		ERROR("INT16_MIN is different to __MIN(int16_t)");
		goto err;
	}
	INFO("INT16_MAX:      %d", INT16_MAX);
	INFO("__MAX(int16_t): %d", __MAX(int16_t));
	if (INT16_MAX != __MAX(int16_t)) {
		ERROR("INT16_MAX is different to __MAX(int16_t)");
		goto err;
	}


	INFO("\r--- uint16_t test:");
	INFO("UINT16_MIN:      %u", (uint16_t)0);
	INFO("__MIN(uint16_t): %u", __MIN(uint16_t));
	if (0 != __MIN(uint16_t)) {
		ERROR("0 is different to __MIN(uint16_t)");
		goto err;
	}
	INFO("UINT16_MAX:      %u", UINT16_MAX);
	INFO("__MAX(uint16_t): %u", __MAX(uint16_t));
	if (UINT16_MAX != __MAX(uint16_t)) {
		ERROR("UINT16_MAX is different to __MAX(uint16_t)");
		goto err;
	}

	INFO("\r--- int32_teger test:");
	INFO("INT32_MIN:      %d", INT32_MIN);
	INFO("__MIN(int32_t): %d", __MIN(int32_t));
	if (INT32_MIN != __MIN(int32_t)) {
		ERROR("INT32_MIN is different to __MIN(int32_t)");
		goto err;
	}
	INFO("INT32_MAX:      %d", INT32_MAX);
	INFO("__MAX(int32_t): %d", __MAX(int32_t));
	if (INT32_MAX != __MAX(int32_t)) {
		ERROR("INT32_MAX is different to __MAX(int32_t)");
		goto err;
	}


	INFO("\r--- uint32_teger test:");
	INFO("UINT32_MIN:      %u", (uint32_t)0);
	INFO("__MIN(uint32_t): %u", __MIN(uint32_t));
	if (0 != __MIN(uint32_t)) {
		ERROR("0 is different to __MIN(uint32_t)");
		goto err;
	}
	INFO("UINT32_MAX:      %u", UINT32_MAX);
	INFO("__MAX(uint32_t): %u", __MAX(uint32_t));
	if (UINT32_MAX != __MAX(uint32_t)) {
		ERROR("UINT32_MAX is different to __MAX(uint32_t)");
		goto err;
	}


	INFO("\r--- int64_t test:");
	INFO("INT64_MIN:      %ld", INT64_MIN);
	INFO("__MIN(int64_t): %ld", __MIN(int64_t));
	if (INT64_MIN != __MIN(int64_t)) {
		ERROR("INT64_MIN is different to __MIN(int64_t)");
		goto err;
	}
	INFO("INT64_MAX:      %ld", INT64_MAX);
	INFO("__MAX(int64_t): %ld", __MAX(int64_t));
	if (INT64_MAX != __MAX(int64_t)) {
		ERROR("INT64_MAX is different to __MAX(int64_t)");
		goto err;
	}


	INFO("\r--- uint64_t test:");
	INFO("UINT64_MIN:      %lu", (uint64_t)0ul);
	INFO("__MIN(uint64_t): %lu", __MIN(uint64_t));
	if (0 != __MIN(uint64_t)) {
		ERROR("0 is different to __MIN(uint64_t)");
		goto err;
	}
	INFO("UINT64_MAX:      %lu", UINT64_MAX);
	INFO("__MAX(uint64_t): %lu", __MAX(uint64_t));
	if (UINT64_MAX != __MAX(uint64_t)) {
		ERROR("UINT64_MAX is different to __MAX(uint64_t)");
		goto err;
	}

	return UT_SUCCESS;

      err:
	return UT_FAIL;
}

/*
 * check_ASSIGN()
 */
int check_ASSIGN(void)
{

#define _TEST_ASSIGN(type1, type2) \
do{ \
        type1 a; \
        type2 b; \
        \
        a = 10; \
        b = 0; \
        if (__ASSIGN(a, b) || 0 != a) { \
                ERROR("assining zero faild."); \
                goto err; \
        } \
        \
        a = 0; \
        b = 1; \
        if (__ASSIGN(a, b) || 1 != a) { \
                ERROR("assining positive value faild."); \
                goto err; \
        } \
        \
        a = 0; \
        if (__ASSIGN(a, __MAX(type1)) || __MAX(type1) != a) { \
                ERROR("assining maximum positive value faild."); \
                goto err; \
        } \
        \
        if (__MIN(type1) == 0) { \
                if (!__ASSIGN(a, -1)) { \
                        ERROR("Successful assigned negetive value to an unsigned datatype."); \
                        goto err; \
                } \
        } else { \
                if (__ASSIGN(a, -1) || -1 != a) { \
                        ERROR("unable to assigned a negetive value to an signed datatype."); \
                        goto err; \
                } \
        } \
        \
        if (0 == __MIN(type1) && 0 == __MIN(type2)) { \
                if (__MAX(type1) < __MAX(type2)) { \
                        if (!__ASSIGN(a, __MAX(type2))) { \
                                ERROR("Successful assigned the maximum of %s%s to an %s%s variable.", getSign(type2), getType(type2), getSign(type1), getType(type1)); \
                                goto err; \
                        } \
                } else { \
                        if (__ASSIGN(a, __MAX(type2))) { \
                                ERROR("Could not assigned the maximum of %s%s to an %s%s variable.", getSign(type2), getType(type2), getSign(type1), getType(type1)); \
                                goto err; \
                        } \
                } \
        } else if (0 == __MIN(type1) && 0 != __MIN(type2)) { \
                if (!__ASSIGN(a, __MIN(type2))) { \
                        ERROR("Successfull assigned negative value of to an unsigned value."); \
                        goto err; \
                } \
                \
                if (__MAX(type1) < __MAX(type2)) { \
                        if (!__ASSIGN(a, __MAX(type2))) { \
                                ERROR("Successful assigned the maximum of %s%s to an %s%s variable.", getSign(type2), getType(type2), getSign(type1), getType(type1)); \
                                goto err; \
                        } \
                } else { \
                        if (__ASSIGN(a, __MAX(type2))) { \
                                ERROR("Could not assigned the maximum of %s%s to an %s%s variable.", getSign(type2), getType(type2), getSign(type1), getType(type1)); \
                                goto err; \
                        } \
                } \
        } else if (0 != __MIN(type1) && 0 == __MIN(type2)) { \
                if (__MAX(type1) < __MAX(type2)) { \
                        if (!__ASSIGN(a, __MAX(type2))) { \
                                ERROR("Successful assigned the maximum of %s%s to an %s%s variable.", getSign(type2), getType(type2), getSign(type1), getType(type1)); \
                                goto err; \
                        } \
                } else { \
                        if (__ASSIGN(a, __MAX(type2))) { \
                                ERROR("Could not assign the maximum of %s%s to an %s%s variable.", getSign(type2), getType(type2), getSign(type1), getType(type1)); \
                                goto err; \
                        } \
                } \
        } else { \
                if (__MIN(type1) > __MIN(type2)) { \
                        if (!__ASSIGN(a, __MIN(type2))) { \
                                ERROR("Successful assigned the minimum of %s%s to an %s%s variable.", getSign(type2), getType(type2), getSign(type1), getType(type1)); \
                                goto err; \
                        } \
                } else { \
                        if (__ASSIGN(a, __MIN(type2))) { \
                                ERROR("Could not assign the minimum of %s%s to an %s%s variable.", getSign(type2), getType(type2), getSign(type1), getType(type1)); \
                                goto err; \
                        } \
                } \
                \
                if (__MAX(type1) < __MAX(type2)) { \
                        if (!__ASSIGN(a, __MAX(type2))) { \
                                ERROR("Successful assigned the maximum of %s%s to an %s%s variable.", getSign(type2), getType(type2), getSign(type1), getType(type1)); \
                                goto err; \
                        } \
                } else { \
                        if (__ASSIGN(a, __MAX(type2))) { \
                                ERROR("Could not assign the maximum of %s%s to an %s%s variable.", getSign(type2), getType(type2), getSign(type1), getType(type1)); \
                                goto err; \
                        } \
                } \
        } \
} while(0);

	_TEST_ASSIGN(int8_t, int8_t);
	_TEST_ASSIGN(int8_t, uint8_t);
	_TEST_ASSIGN(int8_t, int16_t);
	_TEST_ASSIGN(int8_t, uint16_t);
	_TEST_ASSIGN(int8_t, int32_t);
	_TEST_ASSIGN(int8_t, uint32_t);
	_TEST_ASSIGN(int8_t, int64_t);
	_TEST_ASSIGN(int8_t, uint64_t);

	_TEST_ASSIGN(uint8_t, int8_t);
	_TEST_ASSIGN(uint8_t, uint8_t);
	_TEST_ASSIGN(uint8_t, int16_t);
	_TEST_ASSIGN(uint8_t, uint16_t);
	_TEST_ASSIGN(uint8_t, int32_t);
	_TEST_ASSIGN(uint8_t, uint32_t);
	_TEST_ASSIGN(uint8_t, int64_t);
	_TEST_ASSIGN(uint8_t, uint64_t);

	_TEST_ASSIGN(int16_t, int8_t);
	_TEST_ASSIGN(int16_t, uint8_t);
	_TEST_ASSIGN(int16_t, int16_t);
	_TEST_ASSIGN(int16_t, uint16_t);
	_TEST_ASSIGN(int16_t, int32_t);
	_TEST_ASSIGN(int16_t, uint32_t);
	_TEST_ASSIGN(int16_t, int64_t);
	_TEST_ASSIGN(int16_t, uint64_t);

	_TEST_ASSIGN(uint16_t, int8_t);
	_TEST_ASSIGN(uint16_t, uint8_t);
	_TEST_ASSIGN(uint16_t, int16_t);
	_TEST_ASSIGN(uint16_t, uint16_t);
	_TEST_ASSIGN(uint16_t, int32_t);
	_TEST_ASSIGN(uint16_t, uint32_t);
	_TEST_ASSIGN(uint16_t, int64_t);
	_TEST_ASSIGN(uint16_t, uint64_t);

	_TEST_ASSIGN(int32_t, int8_t);
	_TEST_ASSIGN(int32_t, uint8_t);
	_TEST_ASSIGN(int32_t, int16_t);
	_TEST_ASSIGN(int32_t, uint16_t);
	_TEST_ASSIGN(int32_t, int32_t);
	_TEST_ASSIGN(int32_t, uint32_t);
	_TEST_ASSIGN(int32_t, int64_t);
	_TEST_ASSIGN(int32_t, uint64_t);

	_TEST_ASSIGN(uint32_t, int8_t);
	_TEST_ASSIGN(uint32_t, uint8_t);
	_TEST_ASSIGN(uint32_t, int16_t);
	_TEST_ASSIGN(uint32_t, uint16_t);
	_TEST_ASSIGN(uint32_t, int32_t);
	_TEST_ASSIGN(uint32_t, uint32_t);
	_TEST_ASSIGN(uint32_t, int64_t);
	_TEST_ASSIGN(uint32_t, uint64_t);

	_TEST_ASSIGN(int64_t, int8_t);
	_TEST_ASSIGN(int64_t, uint8_t);
	_TEST_ASSIGN(int64_t, int16_t);
	_TEST_ASSIGN(int64_t, uint16_t);
	_TEST_ASSIGN(int64_t, int32_t);
	_TEST_ASSIGN(int64_t, uint32_t);
	_TEST_ASSIGN(int64_t, int64_t);
	_TEST_ASSIGN(int64_t, uint64_t);

	_TEST_ASSIGN(uint64_t, int8_t);
	_TEST_ASSIGN(uint64_t, uint8_t);
	_TEST_ASSIGN(uint64_t, int16_t);
	_TEST_ASSIGN(uint64_t, uint16_t);
	_TEST_ASSIGN(uint64_t, int32_t);
	_TEST_ASSIGN(uint64_t, uint32_t);
	_TEST_ASSIGN(uint64_t, int64_t);
	_TEST_ASSIGN(uint64_t, uint64_t);

	return UT_SUCCESS;

      err:
	return UT_FAIL;

#undef _TEST_ASSIGN
}


/*
 * check_ADD_SUB()
 */
int check_ADD_SUB(void)
{
#define _CHECK_ADD_SUB(stype, utype) \
do {\
        stype a;\
        stype b;\
        stype c;\
        utype ua;\
        utype ub;\
        utype uc;\
\
        a = 10;\
        b = 10;\
        if (__ADD(c, a, b) && 20 != c ) {\
                ERROR("10 + 10 != 20.");\
                goto err;\
        }\
\
        a = 20;\
        b = 10;\
        if (__SUB(c, a, b) && 10 != c ) {\
                ERROR("20 + 10 != 10.");\
                goto err;\
        }\
\
        a = __MAX(stype);\
        b = 1;\
        if (!__ADD(c, a, b)) {\
                ERROR("Adding __MAX(%s%s) and 1 should cause overflow, but macro does not detect this.", getSign(stype), getType(stype));\
                goto err;\
        }\
\
        a = __MIN(stype);\
        b = 1;\
        if (!__SUB(c, a, b)) {\
                ERROR("Subtracting 1 from __MIN(%s%s) should cause underflow, but macro does not detect this.", getSign(stype), getType(stype));\
                goto err;\
        }\
\
        ua = 10;\
        ub = 10;\
        if (__ADD(uc, ua, ub) && 20 != uc ) {\
                ERROR("10 + 10 != 20.");\
                goto err;\
        }\
\
        ua = 20;\
        ub = 10;\
        if (__SUB(uc, ua, ub) && 10 != uc ) {\
                ERROR("20 + 10 != 10.");\
                goto err;\
        }\
\
        ua = __MAX(utype);\
        ub = 1;\
        if (!__ADD(uc, ua, ub)) {\
                ERROR("Adding __MAX(%s%s) and 1 should cause overflow, but macro does not detect this.", getSign(utype), getType(utype));\
                goto err;\
        }\
\
        ua = __MIN(utype);\
        ub = 1;\
        if (!__SUB(uc, ua, ub)) {\
                ERROR("Subtracting 1 from __MIN(%s%s) should cause underflow, but macro does not detect this.", getSign(utype), getType(utype));\
                goto err;\
        }\
}while(0);

	_CHECK_ADD_SUB(int8_t, uint8_t);
	_CHECK_ADD_SUB(int8_t, uint16_t);
	_CHECK_ADD_SUB(int8_t, uint32_t);
	_CHECK_ADD_SUB(int8_t, uint64_t);

	_CHECK_ADD_SUB(int16_t, uint8_t);
	_CHECK_ADD_SUB(int16_t, uint16_t);
	_CHECK_ADD_SUB(int16_t, uint32_t);
	_CHECK_ADD_SUB(int16_t, uint64_t);

	_CHECK_ADD_SUB(int32_t, uint8_t);
	_CHECK_ADD_SUB(int32_t, uint16_t);
	_CHECK_ADD_SUB(int32_t, uint32_t);
	_CHECK_ADD_SUB(int32_t, uint64_t);

	_CHECK_ADD_SUB(int64_t, uint8_t);
	_CHECK_ADD_SUB(int64_t, uint16_t);
	_CHECK_ADD_SUB(int64_t, uint32_t);
	_CHECK_ADD_SUB(int64_t, uint64_t);

	return UT_SUCCESS;

      err:
	return UT_FAIL;

#undef _ADD_SUB
}


/*
 * main()
 */
int main(int argc, char **argv)
{
	int ret = 0;

	ret = check_MIN_MAX();
	if (UT_SUCCESS != ret) {
		goto err;
	}

	ret = check_implicit_conversion();
	if (UT_SUCCESS != ret) {
		goto err;
	}

	ret = check_ASSIGN();
	if (UT_SUCCESS != ret) {
		goto err;
	}

	ret = check_ADD_SUB();
	if (UT_SUCCESS != ret) {
		goto err;
	}

	ret = check_MUL();
	if (UT_SUCCESS != ret) {
		goto err;
	}

	return UT_SUCCESS;

      err:
	return ret;
}
