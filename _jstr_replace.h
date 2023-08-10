#ifndef JSTR_H_REPLACE_DEF
#define JSTR_H_REPLACE_DEF

#ifdef __cplusplus
extern "C" {
#endif /* __cpluslus */
#include <stdlib.h>
#include <string.h>
#ifdef __cplusplus
}
#endif /* __cpluslus */

#include "_jstr_macros.h"
#include "_jstr_private_replace_memmem.h"
#include "_jstr_string.h"

#define JSTR_INLINE_IF_CONSTEXPR

#ifdef __cplusplus
namespace jstr {
#endif /* JSTR_NAMESPACE */

#define JSTR_ASCII_SIZE 256

/*
  Remove first C in S.
  Return value:
  Pointer to '\0' in S;
*/
JSTR_INLINE
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
JSTR_RETURNS_NONNULL
static char *jstr_rmc_mem_p(char *JSTR_RST s,
			    const int c,
			    const size_t sz) JSTR_NOEXCEPT
{
	const char *const sstart = s;
	s = (char *)memchr(s, c, sz);
	if (unlikely(!s))
		return s + sz;
	memmove(s, s + 1, sz - (s - sstart) + 1);
	return s + sz - (s - sstart);
}

/*
  Remove first C in S.
  Return value:
  Pointer to '\0' in S;
*/
JSTR_INLINE
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
JSTR_RETURNS_NONNULL
static char *jstr_rmc_p(char *JSTR_RST s,
			const int c) JSTR_NOEXCEPT
{
#ifdef JSTR_HAS_STRCHRNUL
	s = strchrnul(s, c);
	if (unlikely(!*s))
		return s;
	size_t len = strlen(s);
	memmove(s, s + 1, len);
	return s + len - 1;
#else
	return jstr_rmc_mem_p(s, c, strlen(s));
#endif /* HAS_STRCHRNUL */
}

/*
  Remove all C in S.
  Return value:
  Pointer to '\0' in S.
*/
JSTR_INLINE
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
JSTR_RETURNS_NONNULL
static char *jstr_rmallc_mem_p(char *JSTR_RST s,
			       const int c,
			       const size_t sz) JSTR_NOEXCEPT
{
	unsigned char *dst = (unsigned char *)s;
	unsigned char *old = dst;
	unsigned char *p = dst;
	const unsigned char *const end = dst + sz;
	while ((p = (unsigned char *)memchr(p, c, end - p))) {
		if (likely(dst != old))
			memmove(dst, old, p - old);
		dst += (p - old);
		old += (p - old);
		++old;
		++p;
	}
	memmove(dst, old, end - old + 1);
	return (char *)dst + (end - old);
}

/*
  Remove all C in S.
  Return value:
  Pointer to '\0' in S;
*/
JSTR_INLINE
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
JSTR_RETURNS_NONNULL
static char *jstr_rmallc_p(char *JSTR_RST s,
			   const int c) JSTR_NOEXCEPT
{
#ifdef JSTR_HAS_STRCHRNUL
	unsigned char *dst = (unsigned char *)s;
	unsigned char *old = dst;
	unsigned char *p = dst;
	while (*(p = (unsigned char *)strchrnul((char *)p, c))) {
		if (likely(dst != old))
			memmove(dst, old, p - old);
		dst += (p - old);
		old += (p - old);
		++old;
		++p;
	}
	memmove(dst, old, p - old + 1);
	return (char *)dst + (p - old);
#else
	return jstr_rmallc_mem_p(s, c, strlen(s));
#endif /* HAS_STRCHRNUL */
}

/*
  Remove N C in S.
  Return value:
  Pointer to '\0' in S.
*/
JSTR_INLINE
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
JSTR_RETURNS_NONNULL
static char *jstr_rmnc_mem_p(char *JSTR_RST s,
			     const int c,
			     size_t n,
			     const size_t sz) JSTR_NOEXCEPT
{
	unsigned char *dst = (unsigned char *)s;
	unsigned char *old = dst;
	unsigned char *p = dst;
	const unsigned char *const end = dst + sz;
	while (n-- && (p = (unsigned char *)memchr(p, c, end - p))) {
		if (likely(dst != old))
			memmove(dst, old, p - old);
		dst += (p - old);
		old += (p - old);
		++old;
		++p;
	}
	memmove(dst, old, end - old + 1);
	return (char *)dst + (end - old);
}

/*
  Remove N C in S.
  Return value:
  Pointer to '\0' in S;
*/
JSTR_INLINE
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
JSTR_RETURNS_NONNULL
static char *jstr_rmnc_p(char *JSTR_RST s,
			 const int c,
			 size_t n) JSTR_NOEXCEPT
{
#ifdef JSTR_HAS_STRCHRNUL
	unsigned char *dst = (unsigned char *)s;
	unsigned char *old = dst;
	unsigned char *p = dst;
	while (n-- && *(p = (unsigned char *)strchrnul((char *)p, c))) {
		if (likely(dst != old))
			memmove(dst, old, p - old);
		dst += (p - old);
		old += (p - old);
		++old;
		++p;
	}
	memmove(dst, old, p - old + 1);
	return (char *)dst + (p - old);
#else
	return jstr_rmnc_mem_p(s, c, n, strlen(s));
#endif /* HAS_STRCHRNUL */
}

/*
  Remove characters in REJECT in S.
  Return value:
  Pointer to '\0' in S.
*/
JSTR_INLINE
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
JSTR_RETURNS_NONNULL
static char *jstr_stripspn_p(char *JSTR_RST const s,
			     const char *JSTR_RST const _rjct) JSTR_NOEXCEPT
{
	unsigned char *dst = (unsigned char *)s;
	unsigned char *old = dst;
	unsigned char *p = dst;
	while (*(p = p + strcspn((char *)p, _rjct))) {
		if (likely(dst != old))
			memmove(dst, old, p - old);
		dst += (p - old);
		old += (p - old);
		++old;
		++p;
	}
	memmove(dst, old, p - old + 1);
	return (char *)dst + (p - old);
}

/*
  Remove first HS in S.
  Return value:
  Pointer to '\0' in S.
*/
JSTR_INLINE
JSTR_WARN_UNUSED
JSTR_NONNULL_ALL
JSTR_RETURNS_NONNULL
static char *jstr_rm_mem_p(char *JSTR_RST const s,
			   const char *JSTR_RST const _searc,
			   const size_t sz,
			   const size_t _searclen) JSTR_NOEXCEPT
{
	char *const p = (char *)PRIVATE_JSTR_MEMMEM(s, sz, _searc, _searclen);
	if (unlikely(!p))
		return s + sz;
	memmove(p, p + _searclen, (s + sz) - p);
	return s + sz - _searclen;
}

/*
  Remove N HS in S.
  Return value:
  Pointer to '\0' in S.
*/
JSTR_WARN_UNUSED
JSTR_NONNULL_ALL
JSTR_INLINE
JSTR_RETURNS_NONNULL
static char *jstr_rmn_mem_p_constexpr(char *JSTR_RST s,
				      const char *JSTR_RST const _searc,
				      size_t n,
				      size_t sz,
				      const size_t _searclen) JSTR_NOEXCEPT
{
	switch (_searclen) {
	case 0: return s + sz;
	case 1: return jstr_rmnc_mem_p(s, *_searc, n, sz);
	case 2: return private_jstr_rmn_memmem2(s, _searc, n, sz);
#if PRIVATE_JSTR_USE_MEMMEM34
	case 3: return private_jstr_rmn_memmem3(s, _searc, n, sz);
	case 4: return private_jstr_rmn_memmem4(s, _searc, n, sz);
#endif
	default: return private_jstr_rmn_memmem5(s, _searc, n, sz, _searclen);
	}
}

/*
  Remove N HS in S.
  Return value:
  Pointer to '\0' in S.
*/
JSTR_WARN_UNUSED
JSTR_NONNULL_ALL
JSTR_INLINE_IF_CONSTEXPR
JSTR_RETURNS_NONNULL
static char *jstr_rmn_mem_p(char *JSTR_RST s,
			    const char *JSTR_RST const _searc,
			    size_t n,
			    size_t sz,
			    const size_t _searclen) JSTR_NOEXCEPT
{
	return jstr_rmn_mem_p_constexpr(s, _searc, n, sz, _searclen);
}

/*
  Remove all HS in S.
  Return value:
  Pointer to '\0' in S.
*/
JSTR_WARN_UNUSED
JSTR_NONNULL_ALL
JSTR_INLINE
JSTR_RETURNS_NONNULL
static char *jstr_rmall_mem_p_constexpr(char *JSTR_RST s,
					const char *JSTR_RST const _searc,
					size_t sz,
					const size_t _searclen) JSTR_NOEXCEPT
{
	switch (_searclen) {
	case 0: return s + sz;
	case 1: return jstr_rmallc_mem_p(s, *_searc, sz);
	case 2: return private_jstr_rmall_memmem2(s, _searc, sz);
#if PRIVATE_JSTR_USE_MEMMEM34
	case 3: return private_jstr_rmall_memmem3(s, _searc, sz);
	case 4: return private_jstr_rmall_memmem4(s, _searc, sz);
#endif
	default: return private_jstr_rmall_memmem5(s, _searc, sz, _searclen);
	}
}

/*
  Remove all HS in S.
  Return value:
  Pointer to '\0' in S.
*/
JSTR_WARN_UNUSED
JSTR_NONNULL_ALL
JSTR_INLINE_IF_CONSTEXPR
JSTR_RETURNS_NONNULL
static char *jstr_rmall_mem_p(char *JSTR_RST s,
			      const char *JSTR_RST const _searc,
			      size_t sz,
			      const size_t _searclen) JSTR_NOEXCEPT
{
	return jstr_rmall_mem_p_constexpr(s, _searc, sz, _searclen);
}

/*
  Remove all HS in S.
  Return value:
  Pointer to '\0' in S.
*/
JSTR_WARN_UNUSED
JSTR_NONNULL_ALL
JSTR_INLINE_IF_CONSTEXPR
JSTR_RETURNS_NONNULL
JSTR_INLINE
static char *jstr_rmall_p(char *JSTR_RST s,
			  const char *JSTR_RST const _searc,
			  const size_t _searclen) JSTR_NOEXCEPT
{
	return jstr_rmall_mem_p_constexpr(s, _searc, strlen(s), _searclen);
}

/*
  Remove all HS in S.
  Return value:
  Pointer to '\0' in S.
*/
JSTR_NONNULL_ALL
JSTR_INLINE_IF_CONSTEXPR
JSTR_INLINE
static void jstr_rmall_mem_j(Jstring *JSTR_RST const j,
			     const char *JSTR_RST const _searc,
			     const size_t _searclen) JSTR_NOEXCEPT
{
	j->size = jstr_rmall_mem_p_constexpr(j->data, _searc, j->size, _searclen) - j->data;
}

/*
  Remove all HS in S.
  Return value:
  Pointer to '\0' in S.
*/
JSTR_NONNULL_ALL
JSTR_INLINE_IF_CONSTEXPR
JSTR_INLINE
static void jstr_rmall_j(Jstring *JSTR_RST const j,
			 const char *JSTR_RST const _searc) JSTR_NOEXCEPT
{
	return jstr_rmall_mem_j(j, _searc, strlen(_searc));
}

/*
  Replace first SEARCH in REPLACE.
*/
JSTR_INLINE
JSTR_NONNULL_ALL
static void jstr_rplcc_mem(char *JSTR_RST s,
			   const int _searc,
			   const int _rplc,
			   const size_t sz) JSTR_NOEXCEPT
{
	s = (char *)memchr(s, _searc, sz);
	if (unlikely(!s))
		return;
	*s = _rplc;
}

/*
  Replace first SEARCH in REPLACE.
*/
JSTR_INLINE
JSTR_NONNULL_ALL
static void jstr_rplcc(char *JSTR_RST s,
		       const int _searc,
		       const int _rplc) JSTR_NOEXCEPT
{
	s = strchr(s, _searc);
	if (unlikely(!s))
		return;
	*s = _rplc;
}

/*
  Replace all SEARCH in REPLACE.
*/
JSTR_INLINE
JSTR_NONNULL_ALL
static void jstr_rplcallc_mem(char *JSTR_RST s,
			      const int _searc,
			      const int _rplc,
			      const size_t sz) JSTR_NOEXCEPT
{
	const char *JSTR_RST const end = s + sz;
	while ((s = (char *)memchr(s, _searc, end - s)))
		*s++ = _rplc;
}

/*
  Replace all SEARCH in REPLACE.
*/
JSTR_INLINE
JSTR_NONNULL_ALL
static void jstr_rplcallc(char *JSTR_RST s,
			  const int _searc,
			  const int _rplc) JSTR_NOEXCEPT
{
	while ((s = (strchr(s, _searc))))
		*s++ = _rplc;
}

/*
  Replace N SEARCH in REPLACE.
*/
JSTR_INLINE
JSTR_NONNULL_ALL
static void jstr_rplcnc_mem(char *JSTR_RST s,
			    const int _searc,
			    const int _rplc,
			    size_t n,
			    const size_t sz) JSTR_NOEXCEPT
{
	const char *JSTR_RST const end = s + sz;
	while (--n && (s = (char *)memchr(s, _searc, end - s)))
		*s++ = _rplc;
}

/*
  Replace N SEARCH in REPLACE.
*/
JSTR_INLINE
JSTR_NONNULL_ALL
static void jstr_rplcnc(char *JSTR_RST s,
			const int _searc,
			const int _rplc,
			size_t n) JSTR_NOEXCEPT
{
	while ((s = strchr(s, _searc)))
		*s++ = _rplc;
}

/*
  Replace first SEARCH in S with REPLACE.
  Assumes that SEARCHLEN > REPLACELEN.
*/
JSTR_NONNULL_ALL
JSTR_INLINE
JSTR_WARN_UNUSED
JSTR_RETURNS_NONNULL
static char *jstr_rplc_mem_p_f(char *JSTR_RST const s,
			       const char *JSTR_RST const _searc,
			       const char *JSTR_RST const _rplc,
			       const size_t sz,
			       const size_t _searclen,
			       const size_t _rplclen) JSTR_NOEXCEPT
{
	switch (_rplclen) {
	case 0:
		return jstr_rm_mem_p(s, _searc, sz, _searclen);
	case 1:
		if (_searclen == 1) {
			jstr_rplcc_mem(s, *_searc, *_rplc, sz);
			return s + sz;
		}
		/* FALLTHROUGH */
	default: {
		char *mtc = (char *)PRIVATE_JSTR_MEMMEM(s, sz, _searc, _searclen);
		if (unlikely(!mtc))
			return s + sz;
		memmove(mtc + _rplclen,
			mtc + _searclen,
			(s + sz + 1) - mtc + _searclen);
		memcpy(mtc, _rplc, _rplclen);
		return s + sz - _rplclen - _searclen;
	}
	}
}

/*
  Replace first SEARCH in S with REPLACE.
*/
JSTR_NONNULL_ALL
JSTR_INLINE
static void jstr_rplc_mem_constexpr(char **JSTR_RST const s,
				    size_t *JSTR_RST const sz,
				    size_t *JSTR_RST const cap,
				    const char *JSTR_RST const _searc,
				    const char *JSTR_RST const _rplc,
				    const size_t _searclen,
				    const size_t _rplclen) JSTR_NOEXCEPT
{
	switch (_rplclen) {
	case 0:
		*sz = jstr_rm_mem_p(*s, _searc, *sz, _searclen) - *s;
		return;
	case 1:
		if (_searclen == 1) {
			jstr_rplcc_mem(*s, *_searc, *_rplc, *sz);
			return;
		}
		/* FALLTHROUGH */
	default: {
		char *mtc = (char *)PRIVATE_JSTR_MEMMEM(*s, *sz, _searc, _searclen);
		if (unlikely(!mtc))
			return;
		if (_rplclen <= _searclen || *cap > *sz + _rplclen - _searclen + 1) {
			memmove(mtc + _rplclen,
				mtc + _searclen,
				(*s + *sz + 1) - mtc + _searclen);
			memcpy(mtc, _rplc, _rplclen);
		} else {
			JSTR_GROW(*cap, *sz + _rplclen + 1);
			char *const tmp = (char *)malloc(*cap);
			JSTR_MALLOC_ERR(tmp, return);
			memcpy(tmp, *s, mtc - *s);
			memcpy(tmp + (mtc - *s), _rplc, _rplclen);
			memcpy(tmp + (mtc - *s) + _rplclen,
			       mtc + _rplclen,
			       (*s + *sz + 1) - (mtc + _rplclen));
			free(*s);
			*s = tmp;
		}
		*sz += _rplclen - _searclen;
	}
	}
}

/*
  Replace first SEARCH in S with REPLACE.
*/
JSTR_NONNULL_ALL
JSTR_INLINE_IF_CONSTEXPR
JSTR_MAYBE_UNUSED
static void jstr_rplc_mem(char **JSTR_RST const s,
			  size_t *JSTR_RST const sz,
			  size_t *JSTR_RST const cap,
			  const char *JSTR_RST const _searc,
			  const char *JSTR_RST const _rplc,
			  const size_t _searclen,
			  const size_t _rplclen) JSTR_NOEXCEPT
{
	return jstr_rplc_mem_constexpr(s, sz, cap, _searc, _rplc, _searclen, _rplclen);
}

/*
  Replace last SEARCH in S with REPLACE.
*/
JSTR_NONNULL_ALL
JSTR_INLINE
JSTR_MAYBE_UNUSED
JSTR_NONNULL_ALL
static void jstr_rplclast_mem(char **JSTR_RST const s,
			      size_t *JSTR_RST const sz,
			      size_t *JSTR_RST const cap,
			      const char *JSTR_RST const _searc,
			      const char *JSTR_RST const _rplc,
			      const size_t _searclen,
			      const size_t _rplclen) JSTR_NOEXCEPT
{
	char *mtc = (char *)jstr_memrmem(*s, *sz, _searc, _searclen);
	if (unlikely(!mtc))
		return;
	if (_rplclen <= _searclen || *cap > *sz + _rplclen - _searclen + 1) {
		memmove(mtc + _rplclen,
			mtc + _searclen,
			(*s + *sz + 1) - mtc + _searclen);
		memcpy(mtc, _rplc, _rplclen);
	} else {
		JSTR_GROW(*cap, *sz + _rplclen + 1);
		char *const tmp = (char *)malloc(*cap);
		JSTR_MALLOC_ERR(tmp, return);
		memcpy(tmp, *s, mtc - *s);
		memcpy(tmp + (mtc - *s), _rplc, _rplclen);
		memcpy(tmp + (mtc - *s) + _rplclen,
		       mtc + _rplclen,
		       (*s + *sz + 1) - (mtc + _rplclen));
		free(*s);
		*s = tmp;
	}
	*sz += _rplclen - _searclen;
}

/*
  Replace N SEARCH in S with REPLACE.
  Assumes that S have enough space for REPLACE.
*/
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
JSTR_INLINE
JSTR_RETURNS_NONNULL
static char *jstr_rplcn_mem_p_f_constexpr(char *JSTR_RST s,
					  const char *JSTR_RST const _searc,
					  const char *JSTR_RST const _rplc,
					  size_t n,
					  size_t sz,
					  const size_t _searclen,
					  const size_t _rplclen) JSTR_NOEXCEPT
{
	if (unlikely(_rplclen == 0))
		return jstr_rmn_mem_p(s, _searc, n, sz, _searclen);
	if (_rplclen <= _searclen) {
		switch (_searclen) {
		case 0: return s + sz;
		case 1:
			jstr_rplcnc_mem(s, *_searc, *_rplc, n, sz);
			return s + sz;
		case 2:
			return private_jstr_rplcn_memmem2(s, _searc, _rplc, n, sz, _rplclen);
#if PRIVATE_JSTR_USE_MEMMEM34
		case 3: return private_jstr_rplcn_memmem3(s, _searc, _rplc, n, sz, _rplclen);
		case 4: return private_jstr_rplcn_memmem4(s, _searc, _rplc, n, sz, _rplclen);
#endif
		default: return private_jstr_rplcn_memmem5(s, _searc, _rplc, n, sz, _searclen, _rplclen);
		}
	}
	return private_jstr_rplcn_f(s, _searc, _rplc, n, sz, _searclen, _rplclen);
}

/*
  Replace N SEARCH in S with REPLACE.
  Assumes that S have enough space for REPLACE.
*/
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
JSTR_INLINE_IF_CONSTEXPR
JSTR_MAYBE_UNUSED
JSTR_RETURNS_NONNULL
static char *jstr_rplcn_mem_p_f(char *JSTR_RST s,
				const char *JSTR_RST const _searc,
				const char *JSTR_RST const _rplc,
				size_t n,
				size_t sz,
				const size_t _searclen,
				const size_t _rplclen) JSTR_NOEXCEPT
{
	return jstr_rplcn_mem_p_f_constexpr(s, _searc, _rplc, n, sz, _searclen, _rplclen);
}

/*
  Replace all SEARCH in S with REPLACE.
  Assumes that S have enough space for REPLACE.
*/
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
JSTR_INLINE
JSTR_RETURNS_NONNULL
static char *jstr_rplcall_mem_p_f_constexpr(char *JSTR_RST s,
					    const char *JSTR_RST const _searc,
					    const char *JSTR_RST const _rplc,
					    size_t sz,
					    const size_t _searclen,
					    const size_t _rplclen) JSTR_NOEXCEPT
{
	if (unlikely(_rplclen == 0))
		return jstr_rmall_mem_p(s, _searc, sz, _searclen);
	if (_rplclen <= _searclen) {
		switch (_searclen) {
		case 0: return s + sz;
		case 1:
			jstr_rplcallc_mem(s, *_searc, *_rplc, sz);
			return s + sz;
		default: return private_jstr_rplcall_memmem5(s, _searc, _rplc, sz, _searclen, _rplclen);
		case 2:
			return private_jstr_rplcall_memmem2(s, _searc, _rplc, sz, _rplclen);
#if PRIVATE_JSTR_USE_MEMMEM34
		case 3: return private_jstr_rplcall_memmem3(s, _searc, _rplc, sz, _rplclen);
		case 4: return private_jstr_rplcall_memmem4(s, _searc, _rplc, sz, _rplclen);
#endif
		}
	}
	return private_jstr_rplcall_f(s, _searc, _rplc, sz, _searclen, _rplclen);
}

/*
  Replace all SEARCH in S with REPLACE.
  Assumes that S have enough space for REPLACE.
*/
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
JSTR_INLINE_IF_CONSTEXPR
JSTR_MAYBE_UNUSED
JSTR_RETURNS_NONNULL
static char *jstr_rplcall_mem_p_f(char *JSTR_RST s,
				  const char *JSTR_RST const _searc,
				  const char *JSTR_RST const _rplc,
				  size_t sz,
				  const size_t _searclen,
				  const size_t _rplclen) JSTR_NOEXCEPT
{
	return jstr_rplcall_mem_p_f_constexpr(s, _searc, _rplc, sz, _searclen, _rplclen);
}

/*
  Replace N SEARCH in S with REPLACE.
*/
JSTR_NONNULL_ALL
JSTR_INLINE
static void jstr_rplcn_mem_constexpr(char **JSTR_RST const s,
				     size_t *JSTR_RST const sz,
				     size_t *JSTR_RST const cap,
				     const char *JSTR_RST const _searc,
				     const char *JSTR_RST const _rplc,
				     size_t n,
				     const size_t _searclen,
				     const size_t _rplclen) JSTR_NOEXCEPT
{
	if (unlikely(_rplclen == 0)) {
		*sz = jstr_rmn_mem_p(*s, _searc, n, *sz, _searclen) - *s;
		return;
	}
	if (_rplclen <= _searclen) {
		switch (_searclen) {
		case 0: return;
		case 1:
			jstr_rplcnc_mem(*s, *_searc, *_rplc, n, *sz);
			return;
		case 2:
			*sz = private_jstr_rplcn_memmem2(*s, _searc, _rplc, n, *sz, _rplclen) - *s;
			return;
#if PRIVATE_JSTR_USE_MEMMEM34
		case 3:
			*sz = private_jstr_rplcn_memmem3(*s, _searc, _rplc, n, *sz, _rplclen) - *s;
			return;
		case 4:
			*sz = private_jstr_rplcn_memmem4(*s, _searc, _rplc, n, *sz, _rplclen) - *s;
			return;
#endif
		default:
			*sz = private_jstr_rplcn_memmem5(*s, _searc, _rplc, n, *sz, _searclen, _rplclen) - *s;
			return;
		}
	}
	private_jstr_rplcn_grow(s, sz, cap, _searc, _rplc, n, _searclen, _rplclen);
}

/*
  Replace N SEARCH in S with REPLACE.
*/
JSTR_NONNULL_ALL
JSTR_INLINE_IF_CONSTEXPR
JSTR_MAYBE_UNUSED
static void jstr_rplcn_mem(char **JSTR_RST const s,
			   size_t *JSTR_RST const sz,
			   size_t *JSTR_RST const cap,
			   const char *JSTR_RST const _searc,
			   const char *JSTR_RST const _rplc,
			   size_t n,
			   const size_t _searclen,
			   const size_t _rplclen) JSTR_NOEXCEPT
{
	jstr_rplcn_mem_constexpr(s, sz, cap, _searc, _rplc, n, _searclen, _rplclen);
}

/*
  Replace all SEARCH in S with REPLACE.
*/
JSTR_NONNULL_ALL
JSTR_INLINE
static void jstr_rplcall_mem_constexpr(char **JSTR_RST const s,
				       size_t *JSTR_RST const sz,
				       size_t *JSTR_RST const cap,
				       const char *JSTR_RST const _searc,
				       const char *JSTR_RST const _rplc,
				       const size_t _searclen,
				       const size_t _rplclen) JSTR_NOEXCEPT
{
	if (unlikely(_rplclen == 0)) {
		*sz = jstr_rmall_mem_p(*s, _searc, *sz, _searclen) - *s;
		return;
	}
	if (_rplclen <= _searclen) {
		switch (_searclen) {
		case 0: return;
		case 1:
			jstr_rplcallc_mem(*s, *_searc, *_rplc, *sz);
			return;
		case 2:
			*sz = private_jstr_rplcall_memmem2(*s, _searc, _rplc, *sz, _rplclen) - *s;
			return;
#if PRIVATE_JSTR_USE_MEMMEM34
		case 3:
			*sz = private_jstr_rplcall_memmem3(*s, _searc, _rplc, *sz, _rplclen) - *s;
			return;
		case 4:
			*sz = private_jstr_rplcall_memmem4(*s, _searc, _rplc, *sz, _rplclen) - *s;
			return;
#endif
		default:
			*sz = private_jstr_rplcall_memmem5(*s, _searc, _rplc, *sz, _searclen, _rplclen) - *s;
			return;
		}
	}
	private_jstr_rplcall_grow(s, sz, cap, _searc, _rplc, _searclen, _rplclen);
}

/*
  Replace all SEARCH in S with REPLACE.
*/
JSTR_NONNULL_ALL
JSTR_INLINE_IF_CONSTEXPR
JSTR_MAYBE_UNUSED
static void jstr_rplcall_mem(char **JSTR_RST const s,
			     size_t *JSTR_RST const sz,
			     size_t *JSTR_RST const cap,
			     const char *JSTR_RST const _searc,
			     const char *JSTR_RST const _rplc,
			     const size_t _searclen,
			     const size_t _rplclen) JSTR_NOEXCEPT
{
	jstr_rplcall_mem_constexpr(s, sz, cap, _searc, _rplc, _searclen, _rplclen);
}

#ifdef __cplusplus
} /* namespace jstr */
#endif /* __cpluslus */

#undef JSTR_ASCII_SIZE

#if defined(__GNUC__) || defined(__clang__)
#	pragma GCC poison private_jstr_memmem2
#	pragma GCC poison private_jstr_memmem3
#	pragma GCC poison private_jstr_memmem4
#	pragma GCC poison private_jstr_memmem5
#	pragma GCC poison private_jstr_next_pow2_32
#	pragma GCC poison private_jstr_next_pow2_32_constexpr
#	pragma GCC poison private_jstr_next_pow2_64
#	pragma GCC poison private_jstr_next_pow2_64_constexpr
#	pragma GCC poison private_jstr_rplcall_f
#	pragma GCC poison private_jstr_rplcall_grow
#	pragma GCC poison private_jstr_rplcall_memmem2
#	pragma GCC poison private_jstr_rplcall_memmem3
#	pragma GCC poison private_jstr_rplcall_memmem4
#	pragma GCC poison private_jstr_rplcall_memmem5
#	pragma GCC poison private_jstr_rplcn_f
#	pragma GCC poison private_jstr_rplcn_grow
#	pragma GCC poison private_jstr_rplcn_memmem2
#	pragma GCC poison private_jstr_rplcn_memmem3
#	pragma GCC poison private_jstr_rplcn_memmem4
#	pragma GCC poison private_jstr_rplcn_memmem5
#	pragma GCC poison private_jstr_rmall_memmem2
#	pragma GCC poison private_jstr_rmall_memmem3
#	pragma GCC poison private_jstr_rmall_memmem4
#	pragma GCC poison private_jstr_rmall_memmem5
#	pragma GCC poison private_jstr_rmn_memmem2
#	pragma GCC poison private_jstr_rmn_memmem3
#	pragma GCC poison private_jstr_rmn_memmem4
#	pragma GCC poison private_jstr_rmn_memmem5
#endif /* defined(__GNUC__) || defined(__clang__) */

#endif /* JSTR_H_REPLACE_DEF */
