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
#include <utils/base64.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <utils/log.h>
#include "test_header.h"

#define BUF_SIZE   ((size_t)1024 * 1024)
#define BUF_SIZE64 ((size_t)(BUF_SIZE * (3.0/2.0)))

int main(int argc, char **argv)
{
	uint8_t *orig;
	char *base64;
	uint8_t *recovered;
	size_t len;
	size_t i, padding;
	int16_t help;

	srand(time(NULL));

	orig = calloc(BUF_SIZE, sizeof(*orig));
	if (!orig) {
		ERROR("Not enougth Memory! (orig)");
		goto err;
	}

	base64 = calloc(BUF_SIZE64, sizeof(*base64));
	if (!base64) {
		ERROR("Not enougth Memory! (orig)");
		goto err_orig;
	}

	recovered = calloc(BUF_SIZE, sizeof(*recovered));
	if (!recovered) {
		ERROR("Not enougth Memory! (orig)");
		goto err_base64;
	}

	for (help = 0; help < 64; ++help) {
		if (help != base64_toBin(base64_toChar(help))) {
			ERROR
			    ("function test faild: base64_toBin(bas64_toChar(%u))",
			     help);
			goto err_recovered;
		}
	}

	for (help = 0; help < 256; ++help) {
		if (help != '+'
		    && help != '/' && !(help >= '0' && help <= '9')
		    && !(help >= 'a' && help <= 'z')
		    && !(help >= 'A' && help <= 'Z')) {

			if (0 <= base64_toBin(help)) {
				ERROR
				    ("Macro test faild: base64_toBin(%c) = %u",
				     help, base64_toBin(help));
				goto err_recovered;
			}
		}
	}

	for (i = 0; i < BUF_SIZE; ++i) {
		orig[i] = rand() % 256;
	}

	for (i = BUF_SIZE - 20; BUF_SIZE > i; ++i) {
		if (base64_encode(orig, BUF_SIZE - i, base64, BUF_SIZE64)) {
			ERROR("base64_encode() faild.");
			goto err_recovered;
		}

		len = BUF_SIZE;
		if (base64_decode(base64, recovered, &len)) {
			ERROR("base64_encode() faild.");
			goto err_recovered;
		}

		if (BUF_SIZE - i != len) {
			ERROR("returnd len is wrong");
			goto err_recovered;
		}

		padding = (3 - (BUF_SIZE - i) % 3) % 3;

		if (0 < padding) {
			if ('=' != base64[strlen(base64) - 1]) {
				ERROR("Wrong padding. %c",
				      base64[strlen(base64) - 1]);
				goto err_recovered;
			}
			if (1 < padding) {
				if ('=' != base64[strlen(base64) - 2]) {
					ERROR("Wrong padding. %c",
					      base64[strlen(base64) - 2]);
					goto err_recovered;
				}
			}
		}

		if (0 != memcmp(orig, recovered, BUF_SIZE - i)) {
			ERROR("orig != recovered");
			goto err_recovered;
		}
	}

	free(recovered);
	recovered = NULL;
	free(base64);
	base64 = NULL;
	free(orig);
	orig = NULL;

	return UT_SUCCESS;

      err_recovered:
	free(recovered);
	recovered = NULL;
      err_base64:
	free(base64);
	base64 = NULL;
      err_orig:
	free(orig);
	orig = NULL;
      err:
	return UT_FAIL;

}
