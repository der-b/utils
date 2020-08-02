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
#ifndef __URLPARSER_H__
#define __URLPARSER_H__

#include <sys/types.h>
#include <regex.h>

#define URLPARSER_LOWALPHA      "[[:lower:]]"
#define URLPARSER_UPALPHA       "[[:upper:]]"
#define URLPARSER_ALPHA         "[[:alpha:]]"
#define URLPARSER_DIGIT         "[[:digit:]]"
#define URLPARSER_ALPHANUM      "[[:alnum:]]"
#define URLPARSER_MARK          "[-_\\.!~\\*'\\(\\)]"
#define URLPARSER_RESERVED      "[;/\\?:@&=\\+\\$,]"
#define URLPARSER_UNRESERVED    "(" URLPARSER_ALPHANUM "|" URLPARSER_MARK ")"
#define URLPARSER_HEX           "(" URLPARSER_DIGIT "|[A-Fa-f])"
#define URLPARSER_ESCAPED       "(%%" URLPARSER_HEX URLPARSER_HEX ")"
#define URLPARSER_URIC          "(" URLPARSER_RESERVED "|" URLPARSER_UNRESERVED "|" URLPARSER_ESCAPED ")"
#define URLPARSER_SCHEME        "(" URLPARSER_ALPHA "(" URLPARSER_ALPHANUM "|[\\+\\.-])*)"
#define URLPARSER_PCHAR         "(" URLPARSER_UNRESERVED "|" URLPARSER_ESCAPED "|[:@&=\\+\\$,])"
#define URLPARSER_PARAM         "(" URLPARSER_PCHAR"*)"
#define URLPARSER_SEGMENT       "(" URLPARSER_PCHAR "*(;|" URLPARSER_PARAM ")*)"
#define URLPARSER_PATH_SEGMENTS "(" URLPARSER_SEGMENT "(/" URLPARSER_SEGMENT ")*)"
#define URLPARSER_URIC_NO_SLASH "(" URLPARSER_UNRESERVED "|" URLPARSER_ESCAPED "|[;\\?:@&=\\+\\$,])"
#define URLPARSER_OPAQUE_PART   "(" URLPARSER_URIC_NO_SLASH URLPARSER_URIC "*)"
#define URLPARSER_PATH          "(" URLPARSER_ABS_PATH "|" URLPARSER_OPAQUE_PART ")"
#define URLPARSER_ABS_PATH      "(/" URLPARSER_PATH_SEGMENTS ")"
#define URLPARSER_DOMAINLABEL   "(" URLPARSER_ALPHANUM "|(" URLPARSER_ALPHANUM "(" URLPARSER_ALPHANUM "|-)*" URLPARSER_ALPHANUM "))"
#define URLPARSER_TOPLABEL      "(" URLPARSER_ALPHA "|(" URLPARSER_ALPHA "(" URLPARSER_ALPHANUM "|-)*" URLPARSER_ALPHANUM  "))"
#define URLPARSER_HOSTNAME      "((" URLPARSER_DOMAINLABEL "\\.)*" URLPARSER_TOPLABEL "(\\.)?)"
#define URLPARSER_IPV4ADDRESS   "(" URLPARSER_DIGIT "*\\." URLPARSER_DIGIT "*\\." URLPARSER_DIGIT "*\\." URLPARSER_DIGIT "*)"
#define URLPARSER_HOST          "(" URLPARSER_HOSTNAME "|" URLPARSER_IPV4ADDRESS ")"
#define URLPARSER_PORT          "("URLPARSER_DIGIT "*)"
#define URLPARSER_HOSTPORT      "(" URLPARSER_HOST "(:" URLPARSER_PORT ")?)"
#define URLPARSER_USERINFO      "((" URLPARSER_UNRESERVED "|" URLPARSER_ESCAPED "|[;:&=\\+\\$,])*)"	// TODO: check whether "*" makes sense
#define URLPARSER_SERVER        "((" URLPARSER_USERINFO "@)?" URLPARSER_HOSTPORT ")"
#define URLPARSER_REG_NAME      "((" URLPARSER_RESERVED "|" URLPARSER_ESCAPED "|[;:@&=\\+\\$,])+)"	// TODO: check whether "1*" in rfc2396 can be translated to "+"
#define URLPARSER_AUTHORITY     "(" URLPARSER_SERVER "|" URLPARSER_REG_NAME ")"
#define URLPARSER_NET_PATH      "(//" URLPARSER_AUTHORITY "(" URLPARSER_ABS_PATH "))"
#define URLPARSER_QUERY         "(" URLPARSER_URIC "*)"
#define URLPARSER_HIER_PART     "((" URLPARSER_NET_PATH "|" URLPARSER_ABS_PATH ")(\\?" URLPARSER_QUERY ")?)"
#define URLPARSER_ABSOLUTEURI   "(" URLPARSER_SCHEME ":(" URLPARSER_HIER_PART "|" URLPARSER_OPAQUE_PART "))"
#define URLPARSER_FRAGMENT      "(" URLPARSER_URIC "*)"
#define URLPARSER_REL_SEGMENT   "((" URLPARSER_UNRESERVED "|" URLPARSER_ESCAPED "|[;@&=\\+\\$,])+)"
#define URLPARSER_REL_PATH      "(" URLPARSER_REL_SEGMENT URLPARSER_ABS_PATH"+)"
#define URLPARSER_RELATIVEURI   "((" URLPARSER_NET_PATH "|" URLPARSER_ABS_PATH "|" URLPARSER_REL_PATH ")" URLPARSER_QUERY "?)"
#define URLPARSER_URI_REFERENCE "^(" URLPARSER_ABSOLUTEURI "|" URLPARSER_RELATIVEURI ")(#" URLPARSER_FRAGMENT ")?"

#define URLPARSER_SUCCESS  ( 0)
#define URLPARSER_ERR      (-1)

struct urlparser;
struct urlparser_result;
typedef struct urlparser URLParser;
typedef struct urlparser_result URLParserRes;

struct urlparser {
    regex_t preg;
};

struct urlparser_result {
    int32_t res;
    /* if the parsed URL is an absolute path, then 1 otherwise unequal 1 */
    int8_t isAbsoluteURI;
    /* length of the recognized URL */
    size_t length;

    struct {
        ssize_t start;
        ssize_t end;
    } scheme;

    struct {
        ssize_t start;
        ssize_t end;
        /* if the parsed host is a IP than 1.
         * if the parsed host was a hostname than unequal 0. */
        int8_t isIP;
    } host;

    struct {
        ssize_t start;
        ssize_t end;
    } port;

    struct {
        ssize_t start;
        ssize_t end;
    } path;
};

int32_t urlparser_init(URLParser * urlp);
URLParserRes urlparser_url(URLParser * urlp, const char *url);
int32_t urlparser_free(URLParser * urlp);

#endif
