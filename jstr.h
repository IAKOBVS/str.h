#ifndef JSTR_H_DEF
#define JSTR_H_DEF

#ifdef __cplusplus
extern "C" {
#endif /* __cpluslus */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
}
#endif /* __cpluslus */

#include "jstr_macros.h"
#include "jstr_ctype.h"
#include "jstr_string.h"

#define JSTR_EXIT_ON_MALLOC_ERROR 1

#define JSTR_EXTERN_C  1
#define JSTR_NAMESPACE 0

/* This is executed every time a malloc error is encountered. */
JSTR_NOINLINE
static void JSTR_ERR(void) JSTR_NOEXCEPT
{
	fprintf(stderr, "%s:%d:%s:Can't malloc:", __FILE__, __LINE__, __func__);
	perror("");
#if JSTR_EXIT_ON_MALLOC_ERROR
	exit(1);
#endif /* JSTR_EXIT_ON_MALLOC_ERROR */
}

#if JSTR_NAMESPACE && defined(__cplusplus)
#	define JSTR_IN_NAMESPACE 1
namespace jstr {
#endif /* JSTR_NAMESPACE */
#if JSTR_EXTERN_C && defined(__cplusplus)
extern "C" {
#endif /* JSTR_EXTERN_C */

#define JSTR_RST JSTR_RESTRICT

#define JSTR_MALLOC_ERR(p, malloc_fail) \
	do {                            \
		if (unlikely(!(p))) {   \
			JSTR_ERR();     \
			malloc_fail;    \
		}                       \
	} while (0)

#define JSTR_GROW(oldcap, newcap) \
	while (((oldcap) *= 2) < (newcap))

#define JSTR_REALLOC(p, oldcap, newcap, malloc_fail)        \
	do {                                                \
		JSTR_GROW(oldcap, newcap);                  \
		(p) = JSTR_CAST(char *) realloc(p, newcap); \
		JSTR_MALLOC_ERR(p, malloc_fail);            \
	} while (0)

/*
  exit(1) if ptr is NULL.
*/
JSTR_INLINE
static void jstr_err(char *JSTR_RST const p) JSTR_NOEXCEPT
{
	if (unlikely(!p))
		JSTR_ERR();
}

/*
  free(p) and set p to NULL.
*/
JSTR_INLINE
JSTR_NONNULL_ALL
static void jstr_del(char *JSTR_RST p) JSTR_NOEXCEPT
{
	free(p);
	p = NULL;
}

JSTR_INLINE
JSTR_NONNULL_ALL
static void jstr_alloc(char **JSTR_RST const s,
		       size_t *JSTR_RST const cap,
		       const size_t top) JSTR_NOEXCEPT
{
	enum { GROWTH_MULTIPLIER = 2 };
	*s = JSTR_CAST(char *) malloc(top * 2);
	JSTR_MALLOC_ERR(*s, return);
	*cap = top * GROWTH_MULTIPLIER;
}

JSTR_INLINE
JSTR_NONNULL_ALL
static void jstr_alloc_append_mem(char **JSTR_RST const dst,
				 size_t *JSTR_RST const dsz,
				 size_t *JSTR_RST const dcap,
				 const char *JSTR_RST const src,
				 const size_t slen) JSTR_NOEXCEPT
{
	enum { GROWTH_MULTIPLIER = 2 };
	jstr_alloc(dst, dcap, slen * GROWTH_MULTIPLIER);
	if (unlikely(!*dst))
		return;
	*dsz = slen;
	memcpy(*dst, src, slen + 1);
}

JSTR_INLINE
JSTR_NONNULL_ALL
static void jstr_alloc_append(char **JSTR_RST const dst,
			      size_t *JSTR_RST const dsz,
			      size_t *JSTR_RST const dcap,
			      const char *JSTR_RST const src) JSTR_NOEXCEPT
{
	jstr_alloc_append_mem(dst, dsz, dcap, src, strlen(src));
}

typedef struct jstr_t {
	size_t size;
	size_t cap;
	char *data;

#ifdef __cplusplus

	JSTR_INLINE
	JSTR_NONNULL_ALL
	jstr_t(const size_t len) JSTR_NOEXCEPT
	{
		jstr_alloc(&this->data, &this->cap, len);
	}

	JSTR_INLINE
	JSTR_NONNULL_ALL
	jstr_t(const char *JSTR_RST const src) JSTR_NOEXCEPT
	{
		jstr_alloc_append(&this->data, &this->size, &this->cap, src);
	}

	JSTR_INLINE
	JSTR_NONNULL_ALL
	jstr_t(const char *JSTR_RST const src,
	       const size_t slen) JSTR_NOEXCEPT
	{
		jstr_alloc_append_mem(&this->data, &this->size, &this->cap, src, slen);
	}

	JSTR_INLINE
	~jstr_t(void) JSTR_NOEXCEPT
	{
		free(this->data);
	}

	/*
	  free(STR) and set STR to NULL.
	*/
	JSTR_INLINE
	void del(void) JSTR_NOEXCEPT
	{
		free(this->data);
		this->data = NULL;
	}

	/*
	  exit(1) if STR is NULL.
	*/
	JSTR_INLINE
	void err(void) JSTR_NOEXCEPT
	{
		if (unlikely(!this->data))
			exit(1);
	}

	JSTR_INLINE
	void print(void) JSTR_NOEXCEPT
	{
		fwrite(this->data, 1, this->size, stdout);
	}

	JSTR_INLINE
	void print_stderr(void) JSTR_NOEXCEPT
	{
		fwrite(this->data, 1, this->size, stderr);
	}

	JSTR_INLINE
	void debug_print(void) JSTR_NOEXCEPT
	{
		fprintf(stderr, "size:%zu\ncap:%zu\n", this->size, this->cap);
		fprintf(stderr, "data:%s\n", this->data);
	}
#endif /* __cpluslus */

} jstr_t;

JSTR_INLINE
JSTR_NONNULL_ALL
static void jstr_debug(const jstr_t *JSTR_RST const j)
{
	fprintf(stderr, "size:%zu\ncap:%zu\n", j->size, j->cap);
	fprintf(stderr, "data:%s\n", j->data);
}

/*
   Append SRC to DST.
   Use non-f version for bounds checking.
*/
JSTR_INLINE
JSTR_NONNULL_ALL
static void jstr_append_mem_f(char **JSTR_RST const dst,
			    size_t *JSTR_RST const dsz,
			    const char *JSTR_RST const src,
			    const size_t slen) JSTR_NOEXCEPT
{
	memcpy(*dst, src, slen + 1);
	*dsz += slen;
}

/*
   Append SRC to DST.
   Use non-f version for bounds checking.
*/
JSTR_INLINE
JSTR_NONNULL_ALL
static void jstr_appendf(char **JSTR_RST const dst,
			 size_t *JSTR_RST const dsz,
			 const char *JSTR_RST const src) JSTR_NOEXCEPT
{
	jstr_append_mem_f(dst, dsz, src, strlen(src));
}

/*
   Append SRC to DST.
*/
JSTR_INLINE
JSTR_NONNULL_ALL
static void jstr_append_mem(char **JSTR_RST const dst,
			   size_t *JSTR_RST const dsz,
			   size_t *JSTR_RST const dcap,
			   const char *JSTR_RST const src,
			   const size_t slen) JSTR_NOEXCEPT
{
	if (*dcap < *dsz + slen)
		JSTR_REALLOC(*dst, *dcap, *dsz + slen, return);
	jstr_append_mem_f(dst, dsz, src, slen);
}

/*
   Append SRC to DST.
*/
JSTR_INLINE
JSTR_NONNULL_ALL
static void jstr_append(char **JSTR_RST const dst,
			size_t *JSTR_RST const dsz,
			size_t *JSTR_RST const dcap,
			const char *JSTR_RST const src) JSTR_NOEXCEPT
{
	jstr_append_mem(dst, dsz, dcap, src, strlen(src));
}

/*
  Count occurences of C in S.
  Return value:
  Occurences of C in S.
*/
JSTR_INLINE
JSTR_CONST
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
static int jstr_countc(const char *JSTR_RST s,
		       const int c) JSTR_NOEXCEPT
{
	int count = 0;
	for (;; ++s)
		if (*s == c)
			++count;
		else if (unlikely(!*s))
			break;
	return count;
}

/*
  Count occurences of NE in HS.
  Return value:
  occurences of NE in HS.
*/
JSTR_INLINE
JSTR_CONST
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
#ifdef JSTR_HAS_MEMMEM

static int jstr_count_mem(const char *JSTR_RST hs,
			 const char *JSTR_RST const ne,
			 const size_t hlen,
			 const size_t nlen) JSTR_NOEXCEPT
{
	int count = 0;
	for (const char *const end = hs + hlen;
	     (hs = JSTR_CAST(char *) memmem(hs, end - hs, ne, nlen));
	     ++count, hs += nlen)
		;
	return count;
}

/*
  Count occurences of NE in HS.
  Return value:
  occurences of NE in HS.
*/
JSTR_INLINE
JSTR_CONST
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
static int jstr_count(const char *JSTR_RST const hs,
		      const char *JSTR_RST const ne) JSTR_NOEXCEPT
{
	return jstr_count_mem(hs, ne, strlen(hs), strlen(ne));
}

#else

static int jstr_count(const char *JSTR_RST hs,
		      const char *JSTR_RST const ne) JSTR_NOEXCEPT
{
	int count = 0;
	while ((hs = strstr(hs, ne)))
		++count;
	return count;
}

#endif /* __JSTR_HAS_MEMMEM */

/*
  Reverse S.
*/
JSTR_INLINE
JSTR_NONNULL_ALL
static void jstr_rev_mem(char *JSTR_RST s,
			const size_t n) JSTR_NOEXCEPT
{
	if (unlikely(!*s))
		return;
	char *end = s + n - 1;
	char tmp;
	do {
		tmp = *s;
		*s++ = *end;
		*end-- = tmp;
	} while (s < end);
}

/*
  Reverse S.
*/
JSTR_INLINE
JSTR_NONNULL_ALL
static void jstr_rev(char *JSTR_RST const s) JSTR_NOEXCEPT
{
	jstr_rev_mem(s, strlen(s));
}

/*
  Trim spaces in [ \t] from end of S.
  Return value:
  Pointer to '\0' in S;
  NULL if SLEN is 0.
*/
JSTR_INLINE
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
static char *jstr_trim_mem_p(char *JSTR_RST const s,
			    const size_t n) JSTR_NOEXCEPT
{
	if (unlikely(!*s))
		return s;
	char *end = s + n - 1;
	do {
		switch (*end--) {
		case '\t':
		case ' ':
			continue;
		default:
			end += 2;
			*end = '\0';
		}
		break;
	} while (end >= s);
	return end;
}

/*
  Trim spaces in [ \t] from end of S.
  Return value:
  Pointer to '\0' in S;
  NULL if SLEN is 0.
*/
JSTR_INLINE
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
static char *jstr_trim_p(char *JSTR_RST const s) JSTR_NOEXCEPT
{
	return jstr_trim_mem_p(s, strlen(s));
}

/*
  Remove first C in S.
  Return value:
  1 if a C is found;
  0 otherwise.
*/
JSTR_INLINE
JSTR_NONNULL_ALL
static int jstr_removec(char *JSTR_RST s,
			const int c) JSTR_NOEXCEPT
{
	if (unlikely(!*s))
		return 0;
	do {
		if (*s == c) {
			for (char *d = s; (*d++ = *s++);)
				;
			return 1;
		}
	} while (*++s);
	return 0;
}

/*
  Remove all C in S.
  Return value:
  Pointer to '\0' in S;
*/
JSTR_INLINE
JSTR_NONNULL_ALL
static char *jstr_removeallc(char *JSTR_RST s,
			     const int c) JSTR_NOEXCEPT
{
	if (unlikely(!*s))
		return s;
	char *dst = s;
	do {
		if (*s != c)
			*dst++ = *s;
	} while (*++s);
	*dst = '\0';
	return dst;
}

/*
  Remove characters in REJECT in S.
  Return value:
  Pointer to '\0' in S;
  NULL if REJECT is empty.
*/
JSTR_INLINE
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
static char *jstr_stripspn_p(char *JSTR_RST s,
			     const char *JSTR_RST reject) JSTR_NOEXCEPT
{
	enum {
		ACCEPT = 0,
		REJECT,
		NUL,
		ASCII_SIZE = 256,
	};
	if (unlikely(!reject[0]))
		return s;
	if (unlikely(!reject[1]))
		return jstr_removeallc(s, *reject);
	unsigned char tbl[ASCII_SIZE];
	memset(tbl, ACCEPT, 64);
	memset(&tbl[64], ACCEPT, 64);
	memset(&tbl[128], ACCEPT, 64);
	memset(&tbl[192], ACCEPT, 64);
	tbl[0] = NUL;
	do
		tbl[(unsigned char)*reject++] = REJECT;
	while (*reject);
	for (const unsigned char *src = (unsigned char *)s;; ++src) {
		switch (tbl[*src]) {
		case ACCEPT:
			*s++ = *src;
		case REJECT:
			continue;
		case NUL:;
		}
		break;
	}
	*s = '\0';
	return s;
}

/*
  Replace first SEARCH in REPLACE.
*/
JSTR_INLINE
JSTR_NONNULL_ALL
static void jstr_replacec(char *JSTR_RST s,
			  const int srch,
			  const int rplc) JSTR_NOEXCEPT
{
	if (unlikely(!*s))
		return;
	for (;;) {
		if (*s == srch) {
			*s = rplc;
			break;
		}
		if (unlikely(!*++s))
			break;
	}
}

/*
  Remove first HS in S.
  Return value:
  Pointer to '\0' in S.
*/
JSTR_INLINE
JSTR_WARN_UNUSED
JSTR_NONNULL_ALL
static char *jstr_remove_mem_p(char *JSTR_RST const s,
			      const char *JSTR_RST const ne,
			      const size_t slen,
			      const size_t nlen) JSTR_NOEXCEPT
{
	char *const p = JSTR_CAST(char *) jstr_memmem(s, slen, ne, nlen);
	if (p) {
		memmove(p, p + nlen, (s + slen) - p);
		return s + slen - nlen;
	}
	return s + slen;
}

/*
  Remove first HS in S.
  Return value:
  Pointer to '\0' in S.
*/
JSTR_INLINE
JSTR_WARN_UNUSED
JSTR_NONNULL_ALL
static char *jstr_remove_p(char *JSTR_RST const s,
			   const char *JSTR_RST const ne,
			   const size_t slen) JSTR_NOEXCEPT
{
	return jstr_remove_mem_p(s, ne, slen, strlen(ne));
}

/*
  Remove all HS in S.
  Return value:
  Pointer to '\0' in S.
*/
JSTR_WARN_UNUSED
JSTR_NONNULL_ALL
static char *jstr_removeall_mem_p(char *JSTR_RST s,
				 const char *JSTR_RST const ne,
				 size_t slen,
				 size_t nlen) JSTR_NOEXCEPT
{
	char *dst = s;
	switch (nlen) {
	case 0:
		return s;
	case 1: {
		jstr_removeallc(s, *ne);
		return s + slen - 1;
		break;
	}
	case 2: {
		const uint16_t nw = ne[0] << 8 | ne[1];
		uint16_t sw = s[0] << 8 | s[1];
		for (++s, slen -= 2; slen--; sw = sw << 8 | *s)
			if (sw != nw)
				*dst++ = *(s++ - 1);
			else
				s += 2;
		*dst = '\0';
		break;
	}
	case 3: {
		const uint32_t nw = ne[0] << 24 | ne[1] << 16 | ne[2] << 8;
		uint32_t sw = s[0] << 24 | s[1] << 16 | s[2] << 8;
		for (s += 3, slen -= 3; slen--; sw = (sw | *s++) << 8)
			if (sw != nw)
				*dst++ = *(s - 3);
			else
				s += 2;
		*dst = '\0';
		break;
	}
	case 4: {
		const uint32_t nw = ne[0] << 24 | ne[1] << 16 | ne[2] << 8 | ne[3];
		uint32_t sw = s[0] << 24 | s[1] << 16 | s[2] << 8 | s[3];
		for (s += 4, slen -= 4; slen--; sw = sw << 8 | *s++)
			if (sw != nw)
				*dst++ = *(s - 4);
			else
				s += 3;
		*dst = '\0';
		break;
	}
	default: {
		const uint16_t nw = ne[0] << 8 | ne[nlen - 1];
		const char *const end = s + slen - nlen;
		if (nlen < 15)
			while (s <= end)
				if (nw == (s[0] << 8 | s[nlen - 1])
				    && !memcmp(s, ne, nlen))
					s += nlen;
				else
					*dst++ = *s++;
		else
			for (const size_t off = nlen - 9; s <= end;)
				if (nw == (s[0] << 8 | s[nlen - 1])
				    && !memcmp(s + off, ne + off, 8)
				    && !memcmp(s, ne, nlen))
					s += nlen;
				else
					*dst++ = *s++;
		memcpy(dst, s, end + nlen - s + 1);
		return dst + (end + nlen - s);
		break;
	}
	}
	return dst;
}

/*
  Remove all HS in S.
  Return value:
  Pointer to '\0' in S.
*/
JSTR_INLINE
JSTR_WARN_UNUSED
JSTR_NONNULL_ALL
static char *jstr_removeall_p(char *JSTR_RST const s,
			      const char *JSTR_RST const ne,
			      const size_t slen) JSTR_NOEXCEPT
{
	return jstr_removeall_mem_p(s, ne, slen, strlen(ne));
}

/*
  Replace all SEARCH in REPLACE.
*/
JSTR_INLINE
JSTR_NONNULL_ALL
static void jstr_replaceallc(char *JSTR_RST s,
			     const int srch,
			     const int rplc) JSTR_NOEXCEPT
{
	if (unlikely(*s))
		return;
	for (;;) {
		if (*s == srch)
			*s = rplc;
		if (unlikely(!*++s))
			break;
	}
}

/*
  Replace first SEARCH in S with REPLACE.
*/
JSTR_NONNULL_ALL
static void jstr_replace_mem(char **JSTR_RST const s,
			    size_t *JSTR_RST const ssz,
			    size_t *JSTR_RST const scap,
			    const char *JSTR_RST const srch,
			    const char *JSTR_RST const rplc,
			    const size_t slen,
			    const size_t rlen) JSTR_NOEXCEPT
{
	switch (rlen) {
	case 0:
		*ssz = jstr_remove_mem_p(*s, srch, *ssz, slen) - *s;
		return;
		break;
	case 1:
		if (slen == 1) {
			jstr_replacec(*s, *srch, *rplc);
			--*ssz;
			return;
		}
		break;
	}
	char *mtc = JSTR_CAST(char *) jstr_memmem(*s, *ssz, srch, slen);
	if (!mtc)
		return;
	if (rlen <= slen || *scap > *ssz + rlen - slen + 1) {
		memmove(mtc + rlen,
			mtc + slen,
			(*s + *ssz + 1) - mtc + slen);
		memcpy(mtc, rplc, rlen);
	} else {
		JSTR_GROW(*scap, *ssz + rlen + 1);
		char *const tmp = JSTR_CAST(char *) malloc(*scap);
		JSTR_MALLOC_ERR(tmp, return);
		memcpy(tmp, *s, mtc - *s);
		memcpy(tmp + (mtc - *s), rplc, rlen);
		memcpy(tmp + (mtc - *s) + rlen,
		       mtc + rlen,
		       (*s + *ssz + 1) - (mtc + rlen));
		free(*s);
		*s = tmp;
	}
	*ssz += (long long)(rlen - slen);
}

/*
  Replace first SEARCH in S with REPLACE.
*/
JSTR_INLINE
JSTR_NONNULL_ALL
static void jstr_replace(char **JSTR_RST const s,
			 size_t *JSTR_RST const ssz,
			 size_t *JSTR_RST const scap,
			 const char *JSTR_RST const srch,
			 const char *JSTR_RST const rplc) JSTR_NOEXCEPT
{
	jstr_replace_mem(s, ssz, scap, srch, rplc, strlen(srch), strlen(rplc));
}

/*
  Replace all SEARCH in S with REPLACE.
*/
JSTR_NONNULL_ALL
static void jstr_replaceall_mem(char **JSTR_RST const s,
			       size_t *JSTR_RST const ssz,
			       size_t *JSTR_RST const scap,
			       const char *JSTR_RST const srch,
			       const char *JSTR_RST const rplc,
			       const size_t slen,
			       const size_t rlen) JSTR_NOEXCEPT
{
	switch (rlen) {
	case 0: {
		*ssz = jstr_removeall_mem_p(*s, srch, *ssz, slen) - *s;
		return;
		break;
	}
	case 1:
		if (slen == 1) {
			jstr_replaceallc(*s, *srch, *rplc);
			--*ssz;
			return;
		}
		break;
	}
	char *mtc = *s;
	if (rlen <= slen)
		while ((mtc = JSTR_CAST(char *) jstr_memmem(mtc, (*s + *ssz) - mtc, srch, slen))) {
			memmove(mtc + rlen,
				mtc + slen,
				(*s + *ssz + 1) - mtc + slen);
			memcpy(mtc, rplc, rlen);
			mtc += rlen;
			*ssz += (long long)(rlen - slen);
		}
	else
		for (char *tmp;
		     (mtc = JSTR_CAST(char *) jstr_memmem(mtc, (*s + *ssz) - mtc, srch, slen));
		     *ssz += (long long)(rlen - slen))
			if (*scap > *ssz + rlen - slen + 1) {
				memmove(mtc + rlen,
					mtc + slen,
					(*s + *ssz + 1) - mtc + slen);
				memcpy(mtc, rplc, rlen);
				mtc += rlen;
			} else {
				JSTR_GROW(*scap, *ssz + rlen + 1);
				tmp = JSTR_CAST(char *) malloc(*scap);
				JSTR_MALLOC_ERR(tmp, return);
				memcpy(tmp, *s, mtc - *s);
				memcpy(tmp + (mtc - *s), rplc, rlen);
				memcpy(tmp + (mtc - *s) + rlen,
				       mtc + rlen,
				       (*s + *ssz + 1) - (mtc + rlen));
				mtc = tmp + (mtc - *s) + rlen;
				free(*s);
				*s = tmp;
			}
}

/*
  Replace all SEARCH in S with REPLACE.
*/
JSTR_INLINE
JSTR_NONNULL_ALL
static void jstr_replaceall(char **JSTR_RST const s,
			    size_t *JSTR_RST const ssz,
			    size_t *JSTR_RST const scap,
			    const char *JSTR_RST const srch,
			    const char *JSTR_RST const rplc) JSTR_NOEXCEPT
{
	jstr_replaceall_mem(s, ssz, scap, srch, rplc, strlen(srch), strlen(rplc));
}

/*
  Insert SRC into DST[AT].
*/
JSTR_INLINE
JSTR_NONNULL_ALL
static void jstr_insert_mem(char **JSTR_RST const dst,
			   size_t *JSTR_RST const dsz,
			   size_t *JSTR_RST const dcap,
			   const size_t at,
			   const char *JSTR_RST const src,
			   const size_t slen) JSTR_NOEXCEPT
{
	if (at + slen > *dsz) {
		JSTR_REALLOC(*dst, *dcap, *dsz + slen + 1, return);
		*dsz += slen;
		*(*dst + *dsz) = '\0';
	}
	memcpy(*dst + at, src, slen);
}

/*
  Insert SRC into DST[AT].
*/
JSTR_INLINE
JSTR_NONNULL_ALL
static void jstr_insert(char **JSTR_RST const dst,
			size_t *JSTR_RST const dsz,
			size_t *JSTR_RST const dcap,
			const size_t at,
			const char *JSTR_RST const src) JSTR_NOEXCEPT
{
	jstr_insert_mem(dst, dsz, dcap, at, src, strlen(src));
}

/*
  Insert SRC after C in DST.
*/
JSTR_INLINE
JSTR_NONNULL_ALL
static void jstr_insertaftc_mem(char **JSTR_RST const dst,
				 size_t *JSTR_RST const dsz,
				 size_t *JSTR_RST const dcap,
				 const int c,
				 const char *JSTR_RST const src,
				 const size_t slen) JSTR_NOEXCEPT
{
	const char *const p = JSTR_CAST(char *) memchr(*dst, c, *dsz);
	if (!p)
		return;
	jstr_insert_mem(dst, dsz, dcap, p - *dst + 1, src, slen);
}

/*
  Insert SRC after C in DST.
*/
JSTR_INLINE
JSTR_NONNULL_ALL
static void jstr_insertaftc(char **JSTR_RST const dst,
			      size_t *JSTR_RST const dsz,
			      size_t *JSTR_RST const dcap,
			      const int c,
			      const char *JSTR_RST const src) JSTR_NOEXCEPT
{
	const char *const p = JSTR_CAST(char *) memchr(*dst, c, *dsz);
	if (!p)
		return;
	jstr_insert_mem(dst, dsz, dcap, p - *dst + 1, src, strlen(src));
}

/*
  Insert SRC after all C in DST.
*/
JSTR_INLINE
JSTR_NONNULL_ALL
static void jstr_insertaftallc_mem(char **JSTR_RST const dst,
				    size_t *JSTR_RST const dsz,
				    size_t *JSTR_RST const dcap,
				    const int c,
				    const char *JSTR_RST const src,
				    const size_t slen) JSTR_NOEXCEPT
{
	size_t off = 0;
	for (char *p;
	     (p = JSTR_CAST(char *) memchr(*dst + off, c, *dsz - off));
	     off += *dst - p + 1)
		jstr_insert_mem(dst, dsz, dcap, p - *dst + 1, src, slen);
}

/*
  Insert SRC after all C in DST.
*/
JSTR_INLINE
JSTR_NONNULL_ALL
static void jstr_insertaftallc(char **JSTR_RST const dst,
				 size_t *JSTR_RST const dsz,
				 size_t *JSTR_RST const dcap,
				 const int c,
				 const char *JSTR_RST const src) JSTR_NOEXCEPT
{
	jstr_insertaftallc_mem(dst, dsz, dcap, c, src, strlen(src));
}

/*
  Insert SRC after end of NE in DST.
*/
JSTR_INLINE
JSTR_NONNULL_ALL
static void jstr_insertaft_mem(char **JSTR_RST const dst,
				size_t *JSTR_RST const dsz,
				size_t *JSTR_RST const dcap,
				const char *JSTR_RST const ne,
				const char *JSTR_RST const src,
				const size_t nlen,
				const size_t slen) JSTR_NOEXCEPT
{
	if (unlikely(!*(ne + 1))) {
		jstr_insertaftc_mem(dst, dsz, dcap, *ne, src, slen);
		return;
	}
	const char *const p = JSTR_CAST(char *) jstr_memmem(*dst, *dsz, ne, nlen);
	if (!p)
		return;
	jstr_insert_mem(dst, dsz, dcap, p - *dst + nlen, src, slen);
}

/*
  Insert SRC after end of NE in DST.
*/
JSTR_INLINE
JSTR_NONNULL_ALL
static void jstr_insertaft(char **JSTR_RST const dst,
			     size_t *JSTR_RST const dsz,
			     size_t *JSTR_RST const dcap,
			     const char *JSTR_RST const ne,
			     const char *JSTR_RST const src) JSTR_NOEXCEPT
{
	jstr_insertaft_mem(dst, dsz, dcap, ne, src, strlen(ne), strlen(src));
}

/*
  Insert SRC after all end of NE in DST.
*/
JSTR_INLINE
JSTR_NONNULL_ALL
static void jstr_insertaftall_mem(char **JSTR_RST const dst,
				   size_t *JSTR_RST const dsz,
				   size_t *JSTR_RST const dcap,
				   const char *JSTR_RST const ne,
				   const char *JSTR_RST const src,
				   const size_t nlen,
				   const size_t slen) JSTR_NOEXCEPT
{
	if (unlikely(!*(ne + 1))) {
		jstr_insertaftallc_mem(dst, dsz, dcap, *ne, src, slen);
		return;
	}
	size_t off = 0;
	for (char *p; (p = JSTR_CAST(char *) jstr_memmem(*dst + off, *dsz - off, ne, nlen)); off += *dst - p + nlen)
		jstr_insert_mem(dst, dsz, dcap, p - *dst + nlen, src, slen);
}

/*
  Insert SRC after all end of NE in DST.
*/
JSTR_INLINE
JSTR_NONNULL_ALL
static void jstr_insertaftall(char **JSTR_RST const dst,
				size_t *JSTR_RST const dsz,
				size_t *JSTR_RST const dcap,
				const char *JSTR_RST const ne,
				const char *JSTR_RST const src) JSTR_NOEXCEPT
{
	jstr_insertaftall_mem(dst, dsz, dcap, ne, src, strlen(ne), strlen(src));
}

/*
  Slip SRC into DST[AT].
*/
JSTR_INLINE
JSTR_NONNULL_ALL
static void jstr_slip_mem(char **JSTR_RST const dst,
			 size_t *JSTR_RST const dsz,
			 size_t *JSTR_RST const dcap,
			 const size_t at,
			 const char *JSTR_RST const src,
			 const size_t slen) JSTR_NOEXCEPT
{
	if (*dcap > *dsz + slen + 1) {
		memmove(*dst + at + slen,
			*dst + at,
			*dsz - at + 1);
		memcpy(*dst + at, src, slen);
	} else {
		JSTR_GROW(*dcap, *dsz + slen + 1);
		char *const tmp = JSTR_CAST(char *) malloc(*dcap);
		JSTR_MALLOC_ERR(tmp, return);
		memcpy(tmp, *dst, at);
		memcpy(tmp + at, src, slen);
		memcpy(tmp + at + slen,
		       *dst + at,
		       *dsz - at + 1);
		free(*dst);
		*dst = tmp;
	}
	*dsz += slen;
}

/*
  Slip SRC into DST[AT].
*/
JSTR_INLINE
JSTR_NONNULL_ALL
static void jstr_slip(char **JSTR_RST const dst,
		      size_t *JSTR_RST const dsz,
		      size_t *JSTR_RST const dcap,
		      const size_t at,
		      const char *JSTR_RST const src) JSTR_NOEXCEPT
{
	jstr_slip_mem(dst, dsz, dcap, at, src, strlen(src));
}

/*
  Slip SRC after C in DST.
*/
JSTR_INLINE
JSTR_NONNULL_ALL
static void jstr_slipaftc_mem(char **JSTR_RST const dst,
			       size_t *JSTR_RST const dsz,
			       size_t *JSTR_RST const dcap,
			       const int c,
			       const char *JSTR_RST const src,
			       const size_t slen) JSTR_NOEXCEPT
{
	const char *const p = JSTR_CAST(char *) memchr(*dst, c, *dsz);
	if (!p)
		return;
	jstr_slip_mem(dst, dsz, dcap, p - *dst + 1, src, slen);
}

/*
  Slip SRC after C in DST.
*/
JSTR_INLINE
JSTR_NONNULL_ALL
static void jstr_slipaftc(char **JSTR_RST const dst,
			    size_t *JSTR_RST const dsz,
			    size_t *JSTR_RST const dcap,
			    const int c,
			    const char *JSTR_RST const src) JSTR_NOEXCEPT
{
	jstr_slipaftc_mem(dst, dsz, dcap, c, src, strlen(src));
}

/*
  Slip SRC after all C in DST.
*/
JSTR_INLINE
JSTR_NONNULL_ALL
static void jstr_slipaftallc_mem(char **JSTR_RST const dst,
				  size_t *JSTR_RST const dsz,
				  size_t *JSTR_RST const dcap,
				  const int c,
				  const char *JSTR_RST const src,
				  const size_t slen) JSTR_NOEXCEPT
{
	size_t off = 0;
	for (char *p;
	     (p = JSTR_CAST(char *) memchr(*dst + off, c, *dsz - off));
	     off += p - *dst + 1)
		jstr_slip_mem(dst, dsz, dcap, p - *dst + 1, src, slen);
}

/*
  Slip SRC after all C in DST.
*/
JSTR_INLINE
JSTR_NONNULL_ALL
static void jstr_slipaftallc(char **JSTR_RST const dst,
			       size_t *JSTR_RST const dsz,
			       size_t *JSTR_RST const dcap,
			       const int c,
			       const char *JSTR_RST const src) JSTR_NOEXCEPT
{
	jstr_slipaftallc_mem(dst, dsz, dcap, c, src, strlen(src));
};
/*
  Slip SRC after end of NE in DST.
*/
JSTR_INLINE
JSTR_NONNULL_ALL
static void jstr_slipaft_mem(char **JSTR_RST const dst,
			      size_t *JSTR_RST const dsz,
			      size_t *JSTR_RST const dcap,
			      const char *JSTR_RST const ne,
			      const char *JSTR_RST const src,
			      const size_t nlen,
			      const size_t slen) JSTR_NOEXCEPT
{
	if (unlikely(!*(ne + 1))) {
		jstr_slipaftc_mem(dst, dsz, dcap, *ne, src, slen);
		return;
	}
	const char *const p = JSTR_CAST(char *) jstr_memmem(*dst, *dsz, ne, nlen);
	if (!p)
		return;
	jstr_slip_mem(dst, dsz, dcap, p - *dst + nlen, src, slen);
}

/*
  Slip SRC after end of NE in DST.
*/
JSTR_INLINE
JSTR_NONNULL_ALL
static void jstr_slipaft(char **JSTR_RST const dst,
			   size_t *JSTR_RST const dsz,
			   size_t *JSTR_RST const dcap,
			   const char *JSTR_RST const ne,
			   const char *JSTR_RST const src) JSTR_NOEXCEPT
{
	jstr_slipaft_mem(dst, dsz, dcap, ne, src, strlen(ne), strlen(src));
}

/*
  Slip SRC after all end of NE in DST.
*/
JSTR_INLINE
JSTR_NONNULL_ALL
static void jstr_slipaftall_mem(char **JSTR_RST const dst,
				 size_t *JSTR_RST const dsz,
				 size_t *JSTR_RST const dcap,
				 const char *JSTR_RST const ne,
				 const char *JSTR_RST const src,
				 const size_t nlen,
				 const size_t slen) JSTR_NOEXCEPT
{
	if (unlikely(!*(ne + 1))) {
		jstr_slipaftallc_mem(dst, dsz, dcap, *ne, src, slen);
		return;
	}
	size_t off = 0;
	for (char *p;
	     (p = JSTR_CAST(char *) jstr_memmem(*dst + off, *dsz - off, ne, nlen));
	     off += p - *dst + nlen)
		jstr_slip_mem(dst, dsz, dcap, p - *dst + nlen, src, slen);
}

/*
  Slip SRC after all end of NE in DST.
*/
JSTR_INLINE
JSTR_NONNULL_ALL
static void jstr_slipaftall(char **JSTR_RST const dst,
			      size_t *JSTR_RST const dsz,
			      size_t *JSTR_RST const dcap,
			      const char *JSTR_RST const ne,
			      const char *JSTR_RST const src) JSTR_NOEXCEPT
{
	jstr_slipaftall_mem(dst, dsz, dcap, ne, src, strlen(ne), strlen(src));
}

/*
  Checks if S2 is in end of S1.
  Return value:
  0 if true;
  1 if false.
*/
JSTR_INLINE
JSTR_CONST
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
static int jstr_endswith_mem(const char *JSTR_RST const s1,
			    const char *JSTR_RST const s2,
			    const size_t l1,
			    const size_t l2) JSTR_NOEXCEPT
{
	return (l1 < l2) ? 1 : memcmp(s1 + l1 - l2, s2, l2);
}

/*
  Checks if S2 is in end of S1.
  Return value:
  0 if true;
  1 if false.
*/
JSTR_INLINE
JSTR_CONST
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
static int jstr_endswith(const char *JSTR_RST const s1,
			 const char *JSTR_RST const s2,
			 const size_t l1) JSTR_NOEXCEPT
{
	return jstr_endswith_mem(s1, s2, l1, strlen(s2));
}

#if defined(__cplusplus) && JSTR_EXTERN_C
}
#endif /* JSTR_EXTERN_C */
#if defined(__cplusplus) && JSTR_NAMESPACE
}
#endif /* JSTR_NAMESPACE */

#undef JSTR_RST
#undef JSTR_REPLACE
#undef JSTR_MALLOC_ERR
#undef JSTR_GROW
#undef JSTR_REALLOC

#endif /* JSTR_H_DEF */
