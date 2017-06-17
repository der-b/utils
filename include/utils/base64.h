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
#ifndef __BASE64_H__
#define __BASE64_H__

#include <stdint.h>
#include <sys/types.h>

#define BASE64_SUCCESS       (0)
#define BASE64_BUFFERTOSMALL (1)
#define BASE64_WRONGLITERAL  (2)
#define BASE64_WRONGPADDING  (3)

int16_t base64_toChar(uint8_t x);
int16_t base64_toBin(uint8_t x);
int base64_encode(const uint8_t * bytes, const size_t len_bytes,
		  char *base64, size_t len_base64);
int base64_decode(const char *base64, uint8_t * bytes, size_t * len_bytes);

#endif
