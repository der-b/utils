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
#include <assert.h>

/*
 * __unsigned_mul32()
 */
int32_t __unsigned_mul32(uint32_t * c, uint32_t a, uint32_t b)
{
	uint64_t tmp;

	tmp = (uint64_t) a *b;

	if (__MAX(uint32_t) < tmp) {
		goto err;
	}

	*c = __MAX(uint32_t) & tmp;

	return OVERFLOW_OK;
      err:
	return OVERFLOW_ERR;
}


/*
 * __signed_mul32()
 */
int32_t __signed_mul32(int32_t * c, int32_t a, int32_t b)
{
	int64_t tmp;

	tmp = (int64_t) a *b;

	if (__MAX(int32_t) < tmp || __MIN(int32_t) > tmp) {
		goto err;
	}

	*c = __MAX(uint32_t) & tmp;

	return OVERFLOW_OK;
      err:
	return OVERFLOW_ERR;
}


/*
 * __signed_mul64()
 */
int32_t __signed_mul64(int64_t * c, int64_t a, int64_t b)
{
	uint64_t tmp;

	if (0 > a && 0 > b) {

		if (__unsigned_mul64(&tmp, -a, -b)) {
			goto err;
		}

		if (__MAX(int64_t) < tmp) {
			goto err;
		}

		*c = tmp;

	} else if (0 > a && 0 <= b) {

		if (__unsigned_mul64(&tmp, -a, b)) {
			goto err;
		}

		if ((uint64_t) __MAX(int64_t) + 1 < tmp) {
			goto err;
		}

		*c = -tmp;

	} else if (0 <= a && 0 > b) {

		if (__unsigned_mul64(&tmp, a, -b)) {
			goto err;
		}

		if ((uint64_t) __MAX(int64_t) + 1 < tmp) {
			goto err;
		}

		*c = -tmp;
	} else {

		if (__unsigned_mul64(&tmp, a, b)) {
			goto err;
		}

		if (__MAX(int64_t) < tmp) {
			goto err;
		}

		*c = tmp;
	}

	return OVERFLOW_OK;

      err:
	return OVERFLOW_ERR;
}


/*
 * __unsigned_mul64()
 */
int32_t __unsigned_mul64(uint64_t * c, uint64_t a, uint64_t b)
{
	uint64_t tmp;

	// lower 32 bit part of a
	uint64_t la = __MAX(uint32_t) & a;

	// lower 32 bit part of b
	uint64_t lb = __MAX(uint32_t) & b;

	// higher 32 bit part of a
	uint64_t ha = a >> 32;

	// higher 32 bit part of b
	uint64_t hb = b >> 32;

	/* If this asserts fails, then partitioning in two 32 bit integer above
	 * has an error
	 */
	assert(a == ((ha << 32) | la));
	assert(b == ((hb << 32) | lb));

	/* if both, ha and hb, are not 0, then the multiplication will overflow */
	if (ha && hb) {
		goto err;
	}

	/* Either ha or hb have to be zero, so one side of the addition is always
	 * zero. Since we know that all variables are 32 bit, the 64 bit 
	 * multiplication will not overflow.
	 */

	*c = ha * lb + hb * la;

	/* did the multiplication overflow in int32_t? */
	if (*c > __MAX(uint32_t)) {
		goto err;
	}

	tmp = *c << 32;
	if (__ADD(*c, tmp, la * lb)) {
		goto err;
	}

	return OVERFLOW_OK;

      err:
	return OVERFLOW_ERR;
}
