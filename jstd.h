#ifndef JSTD_DEF_H__
#define JSTD_DEF_H__

#ifdef __cplusplus
#	include <cstddef>
#	include <cstring>
extern "C" {
#else
#	 include <stddef.h>
#	 include <string.h>
#endif // __cplusplus

#ifdef __cplusplus
}
#endif // __cpluslus

#include "macros.h"

#ifdef __cplusplus
extern "C" {
#endif // __cpluslus
	
JSTR_INLINE__
JSTR_WARN_UNUSED__
int jstd_arealnum(const char *JSTR_RESTRICT__ s) JSTR_NOEXCEPT__
{
	for (;; ++s) {
		switch (*s) {
		default:
			return 0;
		case '\0':
			return 1;
		JSTR_CASE_ALPHANUM;
		}
	}
}

JSTR_INLINE__
JSTR_WARN_UNUSED__
int jstd_arealpha(const char *JSTR_RESTRICT__ s) JSTR_NOEXCEPT__
{
	for (;; ++s) {
		switch (*s) {
		default:
			return 0;
		case '\0':
			return 1;
		JSTR_CASE_ALPHA;
		}
	}
}

JSTR_INLINE__
JSTR_WARN_UNUSED__
int jstd_aredigits(const char *JSTR_RESTRICT__ s) JSTR_NOEXCEPT__
{
	for (;; ++s) {
		switch (*s) {
		default:
			return 0;
		case '\0':
			return 1;
		JSTR_CASE_DIGIT;
		}
	}
}

JSTR_INLINE__
JSTR_WARN_UNUSED__
int jstd_toupper(const int c) JSTR_NOEXCEPT__
{
	switch (c) {
	JSTR_CASE_LOWER
		return c - 'a' + 'A';
	}
	return c;
}

JSTR_INLINE__
JSTR_WARN_UNUSED__
JSTR_CONST__
int jstd_tolower(const int c) JSTR_NOEXCEPT__
{
	switch (c) {
	JSTR_CASE_UPPER
		return c - 'A' + 'a';
	}
	return c;
}

JSTR_INLINE__
void jstd_toupper_s(char *JSTR_RESTRICT__ s) JSTR_NOEXCEPT__
{
	for (;; ++s) {
		switch (*s) {
		JSTR_CASE_LOWER
			*s = *s - 'a' + 'A';
		default:
			continue;
		case '\0':;
		}
		break;
	}
}

JSTR_INLINE__
void jstd_tolower_s(char *JSTR_RESTRICT__ s) JSTR_NOEXCEPT__
{
	for (;; ++s) {
		switch (*s) {
		JSTR_CASE_UPPER
			*s = *s - 'A' + 'a';
		default:
			continue;
		case '\0':;
		}
		break;
	}
}

JSTR_INLINE__
void jstd_capitalize(char *JSTR_RESTRICT__ const s) JSTR_NOEXCEPT__
{
	*s = jstd_toupper(*s);
}

JSTR_INLINE__
void jstd_uncapitalize(char *JSTR_RESTRICT__ const s) JSTR_NOEXCEPT__
{
	*s = jstd_tolower(*s);
}

JSTR_INLINE__
JSTR_CONST__
JSTR_WARN_UNUSED__
int jstd_count_c(const char *JSTR_RESTRICT__ s, const int c) JSTR_NOEXCEPT__
{
	int count = 0;
	while (*s)
		if (*s == c)
			++count;
	return count;
}

#ifndef JSTR_HAS_MEMRCHR__

/* memrchr */
JSTR_INLINE__
JSTR_CONST__
JSTR_WARN_UNUSED__
char *jstd_memrchr(char *s, const int c, size_t n)
{
	char *JSTR_RESTRICT__ end = s + n - 1;
	for ( ; end <= s; --end)
		if (*end == c)
			return end;
	return NULL;
}

#else

#define jstd_memrchr(s, c, n) \
	memrchr(s, c, n)

#endif // ! JSTR_HAS_MEMRCHR__

JSTR_INLINE__
JSTR_CONST__
JSTR_WARN_UNUSED__
#ifdef JSTR_HAS_MEMMEM__
int jstd_count_s(const char *JSTR_RESTRICT__ haystack,
		size_t haystacklen,
		const char *JSTR_RESTRICT__ needle,
		size_t needlelen) JSTR_NOEXCEPT__
{
	int count = 0;
	for (const char *JSTR_RESTRICT__ old = haystack;
		(haystack = JSTR_CAST__(char *)memmem(haystack, haystacklen, needle, needlelen));
		haystacklen -= (haystack - old), ++count);
	return count;
}
#else
int jstd_count_s(const char *JSTR_RESTRICT__ haystack,
		const char *JSTR_RESTRICT__ needle) JSTR_NOEXCEPT__
{
	int count = 0;
	while ((haystack = JSTR_CAST__(char *)strstr(haystack, needle)))
		++count;
	return count;
}
#endif // __JSTR_HAS_MEMMEM__

JSTR_INLINE__
JSTR_CONST__
JSTR_WARN_UNUSED__
int jstd_casecmp(const char *JSTR_RESTRICT__ s1,
		const char *JSTR_RESTRICT__ s2) JSTR_NOEXCEPT__
{
	for (char c;; ++s1, ++s2) {
		switch (*s1) {
		JSTR_CASE_LOWER
			c = *s1;
			break;
		JSTR_CASE_UPPER
			c = *s1 - 'A' + 'a';
			break;
		case '\0':
			return *s2;
		}
		switch (*s2) {
		JSTR_CASE_LOWER
			if (*s2 != c)
				return 1;
			break;
		JSTR_CASE_UPPER
			if ((*s2 - 'A' + 'a') != c)
				return 1;
			break;
		case '\0':
			return 1;
		}
	}
}

JSTR_INLINE__
void jstd_memrev(char *JSTR_RESTRICT__ s, size_t slen) JSTR_NOEXCEPT__
{
	char *JSTR_RESTRICT__ end = s + slen - 1;
	for (char tmp; s < end; ++s, --end) {
		tmp = *s;
		*s = *end;
		*end = tmp;
	}
}

#define jstd_strrev(s)            \
	jstd_memrev(s, strlen(s))

JSTR_INLINE__
void jstd_strswap(char **JSTR_RESTRICT__ s1, char **JSTR_RESTRICT__ s2) JSTR_NOEXCEPT__
{
	char *JSTR_RESTRICT__ const tmp = *s1;
	*s1 = *s2;
	*s2 = tmp;
}

JSTR_INLINE__
void jstd_memstrip_c(char *JSTR_RESTRICT__ s, const int c, size_t n) JSTR_NOEXCEPT__
{
	for (const char *JSTR_RESTRICT__ end = s + n ;
			(s = JSTR_CAST__(char *)memchr(s, c, n)); ) {
		n = end-- - s;
		memmove(s, s + 1, n);
	}
}

#define jstd_strstrip_c(s, c)            \
	jstd_memstrip_c(s, c, strlen(s))

JSTR_INLINE__
void jstd_memstrip(char *JSTR_RESTRICT__ s, int c, size_t n) JSTR_NOEXCEPT__
{
	const char *JSTR_RESTRICT__ tmp;
	for (const char *JSTR_RESTRICT__ end = s + n;
			(s = JSTR_CAST__(char *)memchr(s, c, n)); ) {
		tmp = s;
		for (;;) {
			if (unlikely(!*tmp))
				goto END;
			if (*++tmp == c)
				--end;
			else
				break;
		}
		n = end-- - s;
		memmove(s, tmp, n);
	}
	return;
END:
	memmove(s, tmp, n);
}

#define jstd_strstrip(s, c)            \
	jstd_memstrip(s, c, strlen(s))

JSTR_INLINE__
void jstd_memtrim(char *JSTR_RESTRICT__ s, size_t slen) JSTR_NOEXCEPT__
{
	for (char *JSTR_RESTRICT__ end = s + slen - 1 ;
			end >= s; ) {
		switch (*end) {
		case '\t':
		case ' ':
			*end-- = '\0';
			continue;
		}
		break;
	}
}

#define jstd_strtrim(s)            \
	jstd_memtrim(s, strlen(s))

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // JSTD_DEF_H_
