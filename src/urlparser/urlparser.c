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
#include <utils/urlparser.h>
#include <utils/log.h>

// possible flags: REG_EXTENDED, REG_ICASE, REG_NOSUB, REG_NEWLINE
#define COMPTYPE (REG_EXTENDED)

// possible flags: REG_NOTBOL, REG_NOTEOL
#define EFLAGS 0

#define ERRBUFSIZE 1024
#define PMAXMATCH 255

#define _URLPARSER_WHOLE (0)
#define _URLPARSER_ISABSOLUTE (2)
#define _URLPARSER_ABS_SCHEME (3)
#define _URLPARSER_ABS_HOST (19)
#define _URLPARSER_IS_IP (29)
#define _URLPARSER_ABS_PORT (31)
#define _URLPARSER_ABS_PATH (37)

#define _URLPARSER_REL_PATH (115)

/*
 * urlparser_init()
 */
int32_t urlparser_init(URLParser * urlp)
{
	int32_t ret;
	char errbuf[ERRBUFSIZE];

	ret = regcomp(&urlp->preg, URLPARSER_URI_REFERENCE, COMPTYPE);
	if (ret) {
		regerror(ret, &urlp->preg, errbuf, ERRBUFSIZE);
		ERROR("regcomp() faild (%s)\n", errbuf);
		return URLPARSER_ERR;
	}

	return 0;
}

/*
 * urlparser_url()
 */
URLParserRes urlparser_url(URLParser * urlp, const char *url)
{
	int32_t ret;
	char errbuf[ERRBUFSIZE];
	regmatch_t match[PMAXMATCH];
	URLParserRes res;

	memset(&res, 0, sizeof(URLParserRes));

	ret = regexec(&urlp->preg, url, PMAXMATCH, match, EFLAGS);
	if (ret) {
		regerror(ret, &urlp->preg, errbuf, ERRBUFSIZE);
#ifndef NDEBUG
		fprintf(stderr, "regexec() faild (%s)\n", errbuf);
#endif
		DEBUG("Regex dosn't matched.");
		res.res = URLPARSER_ERR;
	} else {
		res.res = URLPARSER_SUCCESS;
                res.length = match[_URLPARSER_WHOLE].rm_eo - match[_URLPARSER_WHOLE].rm_so;

		if (0 <= match[_URLPARSER_ISABSOLUTE].rm_so) {
			res.isAbsoluteURI = 1;

			res.scheme.start = match[_URLPARSER_ABS_SCHEME].rm_so;
			res.scheme.end = match[_URLPARSER_ABS_SCHEME].rm_eo;

			res.host.start = match[_URLPARSER_ABS_HOST].rm_so;
			res.host.end = match[_URLPARSER_ABS_HOST].rm_eo;
                        res.host.isIP = 0 <= match[_URLPARSER_IS_IP].rm_eo;

			res.port.start = match[_URLPARSER_ABS_PORT].rm_so;
			res.port.end = match[_URLPARSER_ABS_PORT].rm_eo;

			res.path.start = match[_URLPARSER_ABS_PATH].rm_so;
			res.path.end = match[_URLPARSER_ABS_PATH].rm_eo;

		} else {
			res.isAbsoluteURI = 0;

			res.scheme.start = -1;
			res.host.start = -1;
			res.port.start = -1;
			res.path.start = match[_URLPARSER_REL_PATH].rm_so;
			res.path.end = match[_URLPARSER_REL_PATH].rm_eo;
		}
#ifndef NDEBUG
#warning "Code for debugging active!"
		size_t j;
		char *helpbuf;
		printf("matches:\n");
		//for( j = 0; j < MAXMATCH && -1 != match[j].rm_so; j++ ) {
		for (j = 0; j < PMAXMATCH; j++) {
			if (0 <= match[j].rm_so) {
				helpbuf =
				    malloc((match[j].rm_eo -
					    match[j].rm_so +
					    1) * sizeof(char));
				if (!helpbuf) {
					fprintf(stderr,
						"Not enough memory.");
					res.res = 1;
					return res;
				}
				helpbuf[match[j].rm_eo - match[j].rm_so] =
				    0;
				strncpy(helpbuf, &url[match[j].rm_so],
					match[j].rm_eo - match[j].rm_so);
				printf(" (%zu) %s \n", j, helpbuf);
				free(helpbuf);
			}
		}
#endif
	}


	return res;
}


/*
 * urlparser_free()
 */
int32_t urlparser_free(URLParser * urlp)
{
	regfree(&urlp->preg);

	return 0;
}
