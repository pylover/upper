#include "common.h"


struct Settings settings = {
    DEFAULT_OUTPUT,
    DEFAULT_BAUDRATE,
    0,
    DEFAULT_TCPPORT,
    false,
    false,
};


/*
 * Find the first occurrence of find in s, where the search is limited to the
 * first slen characters of s.
 */
char * strnstr(const char *s, const char *find, size_t slen) {
	char c, sc;
	size_t len;

	if ((c = *find++) != '\0') {
		len = strlen(find);
		do {
			do {
				if (slen-- < 1 || (sc = *s++) == '\0')
					return (NULL);
			} while (sc != c);
			if (len > slen)
				return (NULL);
		} while (strncmp(s, find, len) != 0);
		s--;
	}
	return ((char *)s);
}
