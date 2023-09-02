#ifndef JSTR_STRING_DEF_H
#define JSTR_STRING_DEF_H 1

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

#include "jstr_builder.h"
#include "jstr_config.h"
#include "jstr_ctype.h"
#include "jstr_macros.h"

#include "string-fza.h"
#include "string-fzb.h"
#include "string-fzc.h"
#include "string-fzi.h"
#include "string-misc.h"
#include "string-opthr.h"
#include "string-optype.h"
#include "string-shift.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cpluslus */

#if JSTR_HAVE_MEMMEM
#	define PJSTR_MEMMEM(_hs, hlen, _ne, nlen) memmem(_hs, hlen, _ne, nlen)
#else
#	define PJSTR_MEMMEM(_hs, hlen, _ne, nlen) strstr(_hs, _ne)
#endif /* HAVE_MEMMEM */

#define PJSTR_OPSIZ sizeof(pjstr_op_ty)

/*
  Return value:
  pointer to '\0' in DST.
*/
JSTR_INLINE
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
JSTR_RETURNS_NONNULL
JSTR_NOTHROW
static void *
jstr_mempcpy(char *JSTR_RST const _dst,
	     const char *JSTR_RST const _src,
	     const size_t _n) JSTR_NOEXCEPT
{
#if JSTR_HAVE_MEMPCPY
	return mempcpy(_dst, _src, _n);
#else
	return (char *)memcpy(_dst, _src, _n) + _n;
#endif /* !JSTR_HAVE_STPCPY */
}

/*
  Return value:
  pointer to '\0' in DST.
*/
JSTR_INLINE
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
JSTR_RETURNS_NONNULL
JSTR_NOTHROW
static char *
jstr_stpcpy(char *JSTR_RST const _dst,
	    const char *JSTR_RST const _src) JSTR_NOEXCEPT
{
#if JSTR_HAVE_STPCPY
	return stpcpy(_dst, _src);
#else
	const size_t _len = strlen(_src);
	return (char *)memcpy(_dst, _src, _len + 1) + _len;
#endif /* !JSTR_HAVE_STPCPY */
}

JSTR_NONNULL_ALL
JSTR_MAYBE_UNUSED
JSTR_INLINE
JSTR_RETURNS_NONNULL
JSTR_NOTHROW
static char *
jstr_strchrnul(const char *JSTR_RST const _s,
	       const int _c)
{
#if JSTR_HAVE_STRCHRNUL
	return (char *)strchrnul(_s, _c);
#else
#	if 0 && JSTR_HAVE_ATTR_MAY_ALIAS
	pjstr_op_ty *_sw = (pjstr_op_ty *)PJSTR_PTR_ALIGN_DOWN(_s, PJSTR_OPSIZ);
	pjstr_op_ty _cc = pjstr_repeat_bytes(_c);
	pjstr_op_ty _mask = pjstr_shift_find(pjstr_find_zero_eq_all(*_sw, _cc), (uintptr_t)_s);
	if (_mask)
		return (char *)_sw + pjstr_index_first(_mask);
	while ((!pjstr_has_zero_eq(*_sw++, _cc)))
		;
	return (char *)(_sw - 1) + pjstr_index_first_zero_eq(*(_sw - 1), _cc);
#	else
	/* It seems that strchr() + strlen() is still faster. */
	const char *const _p = strchr(_s, _c);
	return _p ? (char *)_p : (char *)_s + strlen(_s);
#	endif
#endif
}

JSTR_NONNULL_ALL
JSTR_MAYBE_UNUSED
JSTR_INLINE
JSTR_NOTHROW
JSTR_PURE
static size_t
jstr_strnlen(const char *JSTR_RST const _s,
	     const size_t _maxlen)
{
#if JSTR_HAVE_STRNLEN
	return strnlen(_s, _maxlen);
#else
	const char *p = memchr(_s, '\0', _maxlen);
	return p ? p - _s : _maxlen;
#endif
}

JSTR_NONNULL_ALL
JSTR_MAYBE_UNUSED
JSTR_INLINE
JSTR_NOTHROW
static char *
jstr_strdup(const char *JSTR_RST const _s)
{
#if JSTR_HAVE_STRCHRNUL
	return (char *)strdup(_s);
#else
	const size_t len = strlen(_s) + 1;
	void *const p = malloc(len);
	if (jstr_unlikely(p == NULL))
		return NULL;
	return (char *)memcpy(p, _s, len);
#endif /* HAVE_STRCHRNUL */
}

JSTR_NONNULL_ALL
JSTR_MAYBE_UNUSED
JSTR_INLINE
JSTR_NOTHROW
static char *
jstr_stpdup(const char *JSTR_RST const _s,
	    size_t *JSTR_RST const _sz)
{
	*_sz = strlen(_s);
	void *_p = malloc(*_sz + 1);
	if (jstr_unlikely(_p == NULL))
		return NULL;
	return (char *)memcpy(_p, _s, *_sz + 1);
}

/* Copy no more than N bytes of SRC to DEST, stopping when C is found.
   Return the position in DEST one byte past where C was copied, or
   NULL if C was not found in the first N bytes of SRC.  */
JSTR_NONNULL_ALL
JSTR_MAYBE_UNUSED
JSTR_INLINE
JSTR_NOTHROW
static void *
jstr_memccpy(void *JSTR_RST _dst,
	     const void *JSTR_RST _src,
	     int _c,
	     const size_t _n) JSTR_NOEXCEPT
{
#if JSTR_HAVE_MEMCCPY
	return memccpy(_dst, _src, _c, _n);
#else
	void *p = memchr(_src, _c, _n);
	if (p != NULL)
		return jstr_mempcpy(_dst, _src, p - _src + 1);
	memcpy(_dst, _src, _n);
	return NULL;
#endif /* HAVE_MEMCPY */
}

/*
  Return value:
  Pointer to '\0' in DST.
*/
JSTR_INLINE
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
JSTR_RETURNS_NONNULL
JSTR_NOTHROW
static char *
jstr_stpcat(char *JSTR_RST _dst,
	    const char *JSTR_RST _src) JSTR_NOEXCEPT
{
	_dst += strlen(_dst);
	return jstr_stpcpy(_dst, _src);
}

/*
  Compare S1 with S2 case-insensitively.
  Return value:
  0 if strings match;
  non-zero otherwise.
*/
#if JSTR_HAVE_STRNCASECMP
JSTR_INLINE
#endif
JSTR_PURE
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
JSTR_NOTHROW
JSTR_MAYBE_UNUSED
static int
jstr_strncasecmp(const char *JSTR_RST _s1,
		 const char *JSTR_RST _s2,
		 size_t _n) JSTR_NOEXCEPT
{
#if JSTR_HAVE_STRNCASECMP
	return strncasecmp(_s1, _s2, _n);
#else
	if (jstr_unlikely(_n == 0))
		return 0;
	const unsigned char *_p1 = (unsigned char *)_s1;
	const unsigned char *_p2 = (unsigned char *)_s2;
	int ret;
	while ((ret = jstr_tolower_ascii(*_p1) - jstr_tolower_ascii(*_p2++))
	       + !*_p1
	       + !_n--
	       == 0)
		++_p1;
	return ret;
#endif /* HAVE_STRNCASECMP */
}

/*
  Compare S1 with S2 case-insensitively.
  Return value:
  0 if strings match;
  non-zero otherwise.
*/
#if JSTR_HAVE_STRNCASECMP
JSTR_INLINE
#endif
JSTR_PURE
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
JSTR_NOTHROW
JSTR_MAYBE_UNUSED
static int
jstr_strcasecmp_mem(const char *JSTR_RST const _s1,
		    const char *JSTR_RST const _s2,
		    size_t _n) JSTR_NOEXCEPT
{
#if JSTR_HAVE_STRNCASECMP
	return strncasecmp(_s1, _s2, _n);
#else
	const unsigned char *_p1 = (unsigned char *)_s1;
	const unsigned char *_p2 = (unsigned char *)_s2;
	int ret;
	puts("here");
	while ((ret = jstr_tolower_ascii(*_p1++) - jstr_tolower_ascii(*_p2++))
	       ^ !_n--)
		;
	return ret;
#endif /* HAVE_STRNCASECMP */
}

/*
  Compare S1 with S2 case-insensitively.
  Return value:
  0 if strings match;
  non-zero otherwise.
*/
JSTR_PURE
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
JSTR_MAYBE_UNUSED
#if JSTR_HAVE_STRCASECMP
JSTR_INLINE
#endif
JSTR_NOTHROW
static int
jstr_strcasecmp(const char *JSTR_RST _s1,
		const char *JSTR_RST _s2) JSTR_NOEXCEPT
{
#if JSTR_HAVE_STRCASECMP
	return strcasecmp(_s1, _s2);
#else
	const unsigned char *_p1 = (unsigned char *)_s1;
	const unsigned char *_p2 = (unsigned char *)_s2;
	int ret;
	while ((ret = jstr_tolower_ascii(*_p1) - jstr_tolower_ascii(*_p2++))
	       ^ !*_p1)
		++_p1;
	;
	return ret;
#endif
}

JSTR_PURE
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
JSTR_MAYBE_UNUSED
JSTR_NOTHROW
static void *
pjstr_strrstr_mem_bmh(const unsigned char *JSTR_RST _hs,
		      const size_t _hslen,
		      const unsigned char *JSTR_RST const _ne,
		      const size_t _nelen) JSTR_NOEXCEPT
{
#define PJSTR_HASH2(p) (((size_t)(p)[0] - ((size_t)(p)[-1] << 3)) % 256)
#define PJSTR_STRRSTR_BMH(_shift_table_type, _ne_iterator_type)                     \
	do {                                                                        \
		const unsigned char *const _start = _hs - 1;                        \
		_hs += _hslen;                                                      \
		size_t _tmp;                                                        \
		const size_t _mtc1 = _nelen - 1;                                    \
		size_t _off = 0;                                                    \
		_shift_table_type _shift[256];                                      \
		(sizeof(_shift_table_type) == sizeof(size_t))                       \
		? memset(_shift, 0, sizeof(_shift))                                 \
		: (memset(_shift, 0, 64),                                           \
		   memset(_shift + 64, 0, 64),                                      \
		   memset(_shift + 64 + 64, 0, 64),                                 \
		   memset(_shift + 64 + 64 + 64, 0, 64));                           \
		for (_ne_iterator_type _i = 1; _i < (_ne_iterator_type)_mtc1; ++_i) \
			_shift[PJSTR_HASH2(_ne + _i)] = _i;                         \
		const size_t shft1 = _mtc1 - _shift[PJSTR_HASH2(_ne + _mtc1)];      \
		_shift[PJSTR_HASH2(_ne + _mtc1)] = _mtc1;                           \
		do {                                                                \
			do {                                                        \
				_hs -= _mtc1;                                       \
				_tmp = _shift[PJSTR_HASH2(_hs)];                    \
			} while (!_tmp ^ (_hs <= _start));                          \
			_hs -= _tmp;                                                \
			if (_mtc1 < 15 || !memcmp(_hs + _off, _ne + _off, 8)) {     \
				if (!memcmp(_hs, _ne, _mtc1))                       \
					return (void *)_hs;                         \
				_off = (_off >= 8 ? _off : _mtc1) - 8;              \
			}                                                           \
			_hs -= shft1;                                               \
		} while (jstr_likely(_hs > _start));                                \
		return NULL;                                                        \
	} while (0)
	if (jstr_unlikely(_nelen > 256))
		PJSTR_STRRSTR_BMH(size_t, size_t);
	PJSTR_STRRSTR_BMH(uint8_t, int);
#undef PJSTR_HASH2
#undef PJSTR_STRRSTR_BMH
}

#if JSTR_HAVE_MEMRCHR
JSTR_INLINE
#endif
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
JSTR_MAYBE_UNUSED
JSTR_NOTHROW
JSTR_PURE
static void *
jstr_memrchr(const void *JSTR_RST _s,
	     const int _c,
	     const size_t _n) JSTR_NOEXCEPT
{
#if JSTR_HAVE_MEMRCHR
	return (void *)memrchr(_s, _c, _n);
#elif JSTR_HAVE_ATTR_MAY_ALIAS
	const unsigned char *_end = (unsigned char *)_s + _n;
	switch (_n % PJSTR_OPSIZ) {
	case 7:
		if (*_end-- == _c)
			return (void *)(_end + 1);
		/* fallthrough */
	case 6:
		if (*_end-- == _c)
			return (void *)(_end + 1);
		/* fallthrough */
	case 5:
		if (*_end-- == _c)
			return (void *)(_end + 1);
		/* fallthrough */
	case 4:
		if (*_end-- == _c)
			return (void *)(_end + 1);
		/* fallthrough */
	case 3:
		if (*_end-- == _c)
			return (void *)(_end + 1);
		/* fallthrough */
	case 2:
		if (*_end-- == _c)
			return (void *)(_end + 1);
		/* fallthrough */
	case 1:
		if (*_end-- == _c)
			return (void *)(_end + 1);
		/* fallthrough */
	case 0: break;
	}
	const pjstr_op_ty *_sw = (pjstr_op_ty *)_end;
	const pjstr_op_ty _cc = pjstr_repeat_bytes(_c);
	const pjstr_op_ty *const _start = (pjstr_op_ty *)_s - 1;
	for (; _sw > _start; --_sw)
		if (pjstr_has_eq(*_sw, _cc))
			return (void *)((unsigned char *)_sw + pjstr_index_last_eq(*_sw, _cc));
	return NULL;
#else
	const unsigned char *_end = (unsigned char *)_s + _n;
	const unsigned char *_start = (unsigned char *)_s - 1;
	while (_end > _start
	       && *_end-- != (unsigned char)_c)
		;
	return (_end == _start + 1) ? NULL : (void *)_end + 1;
#endif
}

/*
   Find last NE in HS.
   HS MUST be nul terminated.
   Return value:
   Pointer to NE;
   NULL if not found.
*/
JSTR_PURE
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
JSTR_NOTHROW
JSTR_MAYBE_UNUSED
static void *
jstr_strrstr_mem(const void *JSTR_RST const _hs,
		 const size_t _hslen,
		 const void *JSTR_RST const _ne,
		 const size_t _nelen) JSTR_NOEXCEPT
{
	if (jstr_unlikely(_hslen < _nelen))
		return NULL;
	switch (_nelen) {
	case 0:
		return (void *)((unsigned char *)_hs + _hslen);
	case 1:
		return (void *)jstr_memrchr(_hs, *(char *)_ne, _hslen);
	case 2: {
		const unsigned char *const _start = (unsigned char *)_hs - 1;
		const unsigned char *_h = (unsigned char *)_hs + _hslen - 1;
		const unsigned char *_n = (unsigned char *)_ne;
		const uint16_t _nw = _n[1] << 8 | _n[0];
		uint16_t _hw = _h[0] << 8 | _h[-1];
		for (_h -= 2; (_h == _start) ^ (_hw != _nw); _hw = _hw << 8 | *_h--)
			;
		return _hw == _nw ? (void *)(_h + 1) : NULL;
	}
	case 3: {
		const unsigned char *const _start = (unsigned char *)_hs - 1;
		const unsigned char *_h = (unsigned char *)_hs + _hslen - 1;
		const unsigned char *_n = (unsigned char *)_ne;
		const uint32_t _nw = _n[2] << 24 | _n[1] << 16 | _n[0] << 8;
		uint32_t _hw = _h[0] << 24 | _h[-1] << 16 | _h[-2] << 8;
		for (_h -= 3; (_h == _start) ^ (_hw != _nw); _hw = (_hw | *_h--) << 8)
			;
		return _hw == _nw ? (void *)(_h + 1) : NULL;
	}
	case 4: {
		const unsigned char *const _start = (unsigned char *)_hs - 1;
		const unsigned char *_h = (unsigned char *)_hs + _hslen - 1;
		const unsigned char *_n = (unsigned char *)_ne;
		const uint32_t _nw = _n[3] << 24 | _n[2] << 16 | _n[1] << 8 | _n[0];
		uint32_t _hw = _h[0] << 24 | _h[-1] << 16 | _h[-2] << 8 | _h[-3];
		for (_h -= 4; (_h == _start) ^ (_hw != _nw); _hw = _hw << 8 | *_h--)
			;
		return _hw == _nw ? (void *)(_h + 1) : NULL;
	}
	default: return pjstr_strrstr_mem_bmh((unsigned char *)_hs, _hslen, (unsigned char *)_ne, _nelen);
	}
}

/*
   Find last NE in HS.
   Return value:
   Pointer to NE;
   NULL if not found.
*/
JSTR_PURE
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
JSTR_MAYBE_UNUSED
JSTR_NOTHROW
JSTR_INLINE
static char *
jstr_strrstr(const char *JSTR_RST const _hs,
	     const char *JSTR_RST const _ne) JSTR_NOEXCEPT
{
	return (char *)jstr_strrstr_mem(_hs, strlen(_hs), _ne, strlen(_ne));
}

JSTR_PURE
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
JSTR_MAYBE_UNUSED
JSTR_NOTHROW
static char *
pjstr_strcasestr_mem_bmh(const char *JSTR_RST const _hs,
			 const size_t _hslen,
			 const char *JSTR_RST const _ne,
			 const size_t _nelen) JSTR_NOEXCEPT
{
#define PJSTR_HASH2_LOWER(p) (((size_t)(jstr_tolower_ascii((p)[0])) - ((size_t)jstr_tolower_ascii((p)[-1]) << 3)) % 256)
#define PJSTR_STRCASESTR_BMH(_shift_table_type, _ne_iterator_type)                                         \
	do {                                                                                               \
		const unsigned char *_h = (unsigned char *)_hs;                                            \
		const unsigned char *const _n = (unsigned char *)_ne;                                      \
		const unsigned char *const _end = _h + _hslen - _nelen + 1;                                \
		size_t _tmp;                                                                               \
		const size_t _mtc1 = _nelen - 1;                                                           \
		size_t _off = 0;                                                                           \
		_shift_table_type _shift[256];                                                             \
		(sizeof(_shift_table_type) == sizeof(size_t))                                              \
		? memset(_shift, 0, sizeof(_shift))                                                        \
		: (memset(_shift, 0, 64),                                                                  \
		   memset(_shift + 64, 0, 64),                                                             \
		   memset(_shift + 64 + 64, 0, 64),                                                        \
		   memset(_shift + 64 + 64 + 64, 0, 64));                                                  \
		for (_ne_iterator_type _i = 1; _i < (_ne_iterator_type)_mtc1; ++_i)                        \
			_shift[PJSTR_HASH2_LOWER(_n + _i)] = _i;                                           \
		const size_t _shft1 = _mtc1 - _shift[PJSTR_HASH2_LOWER(_n + _mtc1)];                       \
		_shift[PJSTR_HASH2_LOWER(_n + _mtc1)] = _mtc1;                                             \
		do {                                                                                       \
			do {                                                                               \
				_h += _mtc1;                                                               \
				_tmp = _shift[PJSTR_HASH2_LOWER(_h)];                                      \
			} while (!_tmp ^ (_h >= _end));                                                    \
			_h -= _tmp;                                                                        \
			if (_tmp < _mtc1)                                                                  \
				continue;                                                                  \
			if (_mtc1 < 15 || !jstr_strcasecmp_mem((char *)_h + _off, (char *)_n + _off, 8)) { \
				if (!jstr_strcasecmp_mem((char *)_h, (char *)_n, _mtc1))                   \
					return (char *)_h;                                                 \
				_off = (_off >= 8 ? _off : _mtc1) - 8;                                     \
			}                                                                                  \
			_h += _shft1;                                                                      \
		} while (jstr_likely(_h < _end));                                                          \
		return NULL;                                                                               \
	} while (0)
	if (jstr_unlikely(_nelen > 256))
		PJSTR_STRCASESTR_BMH(size_t, size_t);
	PJSTR_STRCASESTR_BMH(uint8_t, int);
#undef PJSTR_HASH2_LOWER
#undef PJSTR_STRCASESTR_BMH
}

/*
   Find C in S case-insensitively.
   S MUST be nul terminated.
   Return value:
   Pointer to NE;
   NULL if not found.
*/
JSTR_PURE
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
JSTR_MAYBE_UNUSED
JSTR_NOTHROW
static char *
pstrcasechr_mem(const char *JSTR_RST _s,
		const int _c,
		const size_t _n) JSTR_NOEXCEPT
{
	char _acc[3];
	switch (_c) {
		JSTR_CASE_UPPER
		_acc[0] = _c;
		_acc[1] = _c - 'A' + 'a';
		break;
		JSTR_CASE_LOWER
		_acc[0] = _c;
		_acc[1] = _c - 'a' + 'A';
		break;
	default:
		return (char *)memchr(_s, _c, _n);
	}
	_acc[2] = '\0';
	return (char *)strpbrk(_s, _acc);
}

/*
   Find C in S case-insensitively.
   S MUST be nul terminated.
   Return value:
   Pointer to NE;
   NULL if not found.
*/
JSTR_PURE
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
JSTR_MAYBE_UNUSED
JSTR_NOTHROW
static char *
pstrcasechr(const char *JSTR_RST _s,
	    const int _c) JSTR_NOEXCEPT
{
	char _acc[3];
	switch (_c) {
		JSTR_CASE_UPPER
		_acc[0] = _c;
		_acc[1] = _c - 'A' + 'a';
		break;
		JSTR_CASE_LOWER
		_acc[0] = _c;
		_acc[1] = _c - 'a' + 'A';
		break;
	default:
		return (char *)strchr(_s, _c);
	}
	_acc[2] = '\0';
	return (char *)strpbrk(_s, _acc);
}

/*
   Find NE in HS case-insensitively.
   HS MUST be nul terminated.
   Will call strcasestr if available.
   Return value:
   Pointer to NE;
   NULL if not found.
*/
JSTR_PURE
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
JSTR_MAYBE_UNUSED
JSTR_NOTHROW
#if JSTR_HAVE_STRCASESTR
JSTR_INLINE
#endif
static char *
jstr_strcasestr_mem(const char *JSTR_RST const _hs,
		    const size_t _hslen,
		    const char *JSTR_RST const _ne,
		    const size_t _nelen) JSTR_NOEXCEPT
{
#if JSTR_HAVE_STRCASESTR
	return (char *)strcasestr(_hs, _ne);
#else
	if (jstr_unlikely(_hslen < _nelen))
		return NULL;
	switch (_nelen) {
	case 0: return (char *)_hs;
	case 1: return pstrcasechr_mem(_hs, *_ne, _hslen);
	case 4:
		if (jstr_isalpha(_ne[3]))
			break;
		/* fallthrough */
	case 3:
		if (jstr_isalpha(_ne[2]))
			break;
		/* fallthrough */
	case 2:
		if (!jstr_isalpha(_ne[0]) && !jstr_isalpha(_ne[1]))
			return (char *)PJSTR_MEMMEM(_hs, _hslen, _ne, _nelen);
		break;
	}
	return pjstr_strcasestr_mem_bmh(_hs, _hslen, _ne, _nelen);
#endif
}

/*
   Find NE in HS case-insensitively.
   Return value:
   Pointer to NE;
   NULL if not found.
*/
JSTR_PURE
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
JSTR_MAYBE_UNUSED
#ifdef JSTR_HAVE_STRCASESTR
JSTR_INLINE
#endif
JSTR_NOTHROW
static char *
jstr_strcasestr(const char *JSTR_RST const _hs,
		const char *JSTR_RST const _ne) JSTR_NOEXCEPT
{
#if JSTR_HAVE_STRCASESTR
	return (char *)strcasestr(_hs, _ne);
#else
	size_t _nelen;
	if (_ne[0] == '\0')
		return (char *)_hs;
	if (_ne[1] == '\0')
		return pstrcasechr(_hs, *_ne);
	if (_ne[2] == '\0') {
do2:
		if (!jstr_isalpha(_ne[0]) && !jstr_isalpha(_ne[1]))
			return (char *)strstr(_hs, _ne);
		_nelen = 2;
	} else if (_ne[3] == '\0') {
do3:
		if (!jstr_isalpha(_ne[2]))
			goto do2;
		_nelen = 3;
	} else if (_ne[4] == '\0') {
		if (!jstr_isalpha(_ne[3]))
			goto do3;
		_nelen = 4;
	} else {
		_nelen = 4 + strlen(_ne + 4);
	}
	return pjstr_strcasestr_mem_bmh(_hs, strlen(_hs), _ne, _nelen);
#endif /* HAVE_STRCASESTR */
}

/*
   Reverse of STRCSPN.
*/
JSTR_PURE
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
JSTR_MAYBE_UNUSED
JSTR_NOTHROW
static size_t
jstr_strrcspn_mem(const char *JSTR_RST const _s,
		  const char *JSTR_RST const _reject,
		  size_t _sz) JSTR_NOEXCEPT
{
	if (jstr_unlikely(_reject[0] == '\0'))
		return 0;
	if (jstr_unlikely(_reject[1] == '\0')) {
		const char *const _p = (char *)jstr_memrchr(_s, *_reject, _sz);
		return _p ? (size_t)((_s + _sz - 1) - _p) : _sz;
	}
	if (jstr_unlikely(_sz == 0))
		return 0;
	unsigned char _t[256];
	memset(_t, 0, 64);
	memset(_t + 64, 0, 64);
	memset(_t + 128, 0, 64);
	memset(_t + 192, 0, 64);
	const unsigned char *_p = (unsigned char *)_reject;
	do
		_t[*_p] = 1;
	while (*_p++);
	_p = (unsigned char *)_s + _sz - 1;
	switch (_sz % 4) {
	case 0: break;
	case 3:
		if (!_t[*_p--])
			return 0;
		/* fallthrough */
	case 2:
		if (!_t[*_p--])
			return ((unsigned char *)_s + _sz - 1) - (_p + 1);
		/* fallthrough */
	case 1:
		if (!_t[*_p--])
			return ((unsigned char *)_s + _sz - 1) - (_p + 1);
	}
	if (jstr_unlikely(_sz < 4))
		return _sz;
	unsigned int _c0, _c1, _c2, _c3;
	do {
		_c0 = _t[_p[0]];
		_c1 = _t[_p[-1]];
		_c2 = _t[_p[-2]];
		_c3 = _t[_p[-3]];
		_p -= 4;
		_sz -= 4;
	} while ((_sz != 0) & (_c0 | _c1 | _c2 | _c3));
	size_t _cnt = ((unsigned char *)_s + _sz - 1) - _p;
	return ((_c0 | _c1) != 0)
	       ? _cnt - _c0 + 1
	       : _cnt - _c2 + 3;
}

/*
   Reverse of STRCSPN.
*/
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
JSTR_MAYBE_UNUSED
JSTR_INLINE
JSTR_NOTHROW
JSTR_PURE
static size_t
jstr_strrcspn(const char *JSTR_RST const _s,
	      const char *JSTR_RST const _reject) JSTR_NOEXCEPT
{
	return jstr_strrcspn_mem(_s, _reject, strlen(_s));
}

/*
   Reverse of STRSPN.
*/
JSTR_PURE
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
JSTR_MAYBE_UNUSED
JSTR_NOTHROW
static size_t
jstr_strrspn_mem(const char *JSTR_RST const _s,
		 const char *JSTR_RST const _accept,
		 size_t _sz) JSTR_NOEXCEPT
{
	if (jstr_unlikely(*_accept == '\0'))
		return 0;
	if (jstr_unlikely(_sz == 0))
		return 0;
	if (jstr_unlikely(_accept[1] == '\0')) {
		const char *_p = _s + _sz - 1;
		while (*_p == *_accept)
			--_p;
		return (_s + _sz - 1) - _p;
	}
	const unsigned char *_p = (unsigned char *)_accept;
	unsigned char _t[256];
	memset(_t, 0, 64);
	memset(_t + 64, 0, 64);
	memset(_t + 128, 0, 64);
	memset(_t + 192, 0, 64);
	do
		_t[*_p++] = 1;
	while (*_p);
	_p = (unsigned char *)_s + _sz - 1;
	switch (_sz % 4) {
	case 3:
		if (!_t[*_p--])
			return 0;
		/* fallthrough */
	case 2:
		if (!_t[*_p--])
			return ((unsigned char *)_s + _sz - 1) - (_p + 1);
		/* fallthrough */
	case 1:
		if (!_t[*_p--])
			return ((unsigned char *)_s + _sz - 1) - (_p + 1);
	case 0: break;
	}
	if (jstr_unlikely(_sz < 4))
		return _sz;
	unsigned int _c0, _c1, _c2, _c3;
	do {
		_c0 = _t[_p[0]];
		_c1 = _t[_p[-1]];
		_c2 = _t[_p[-2]];
		_c3 = _t[_p[-3]];
		_p -= 4;
		_sz -= 4;
	} while ((_sz != 0) & (_c0 & _c1 & _c2 & _c3));
	size_t _cnt = ((unsigned char *)_s + _sz - 1) - _p;
	return ((_c0 & _c1) == 0)
	       ? _cnt + _c0
	       : _cnt + _c2 + 2;
}

/*
   Reverse of STRSPN.
*/
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
JSTR_MAYBE_UNUSED
JSTR_INLINE
JSTR_NOTHROW
JSTR_PURE
static size_t
jstr_strrspn(const char *JSTR_RST const _s,
	     const char *JSTR_RST const _accept) JSTR_NOEXCEPT
{
	return jstr_strrspn_mem(_s, _accept, strlen(_s));
}

/*
   Reverse of STRPBRK.
*/
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
JSTR_MAYBE_UNUSED
JSTR_INLINE
JSTR_NOTHROW
JSTR_PURE
static char *
jstr_strrpbrk_mem(const char *JSTR_RST _s,
		  const char *JSTR_RST const _accept,
		  const size_t _sz) JSTR_NOEXCEPT
{
	_s = _s + _sz - jstr_strrcspn_mem(_s, _accept, _sz);
	return *_s ? (char *)_s : NULL;
}

/*
   Reverse of STRPBRK.
*/
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
JSTR_MAYBE_UNUSED
JSTR_INLINE
JSTR_NOTHROW
JSTR_PURE
static char *
jstr_strrpbrk(const char *JSTR_RST const _s,
	      const char *JSTR_RST const _accept) JSTR_NOEXCEPT
{
	return jstr_strrpbrk_mem(_s, _accept, strlen(_s));
}

/*
  Checks if S2 is in _end of S1.
  Return value:
  0 if true;
  1 if false.
*/
JSTR_INLINE
JSTR_PURE
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
JSTR_NOTHROW
static int
jstr_endswith_mem(const char *JSTR_RST const _hs,
		  const char *JSTR_RST const _ne,
		  const size_t _hsz,
		  const size_t _nelen) JSTR_NOEXCEPT
{
	return (_hsz < _nelen) ? 1 : memcmp(_hs + _hsz - _nelen, _ne, _nelen);
}

/*
  Count occurences of C in S.
  Return value:
  Occurences of C in S.
*/
JSTR_INLINE
JSTR_PURE
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
JSTR_NOTHROW
static size_t
jstr_countc(const char *JSTR_RST _s,
	    const int _c) JSTR_NOEXCEPT
{
	size_t _cnt = 0;
	while ((_s = strchr(_s, _c)))
		++_cnt;
	return _cnt;
}

/*
  Count occurences of C in S.
  Return value:
  Occurences of C in S.
*/
JSTR_INLINE
JSTR_PURE
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
JSTR_NOTHROW
static size_t
jstr_countc_mem(const char *JSTR_RST _s,
		const int _c,
		const size_t _n) JSTR_NOEXCEPT
{
	size_t _cnt = 0;
	const char *const _end = _s + _n;
	while ((_s = (char *)memchr(_s, _c, _end - _s)))
		++_cnt;
	return _cnt;
}

#if JSTR_HAVE_MEMMEM

/*
  Count occurences of NE in HS.
  Return value:
  occurences of NE in HS.
*/
JSTR_MAYBE_UNUSED
JSTR_PURE
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
JSTR_NOTHROW
static size_t
jstr_count_mem(const char *JSTR_RST _s,
	       const char *JSTR_RST const _find,
	       size_t _sz,
	       const size_t _findlen) JSTR_NOEXCEPT
{
	if (jstr_unlikely(_findlen == 1))
		return jstr_countc_mem(_s, *_find, _sz);
	if (jstr_unlikely(_findlen == 0))
		return 0;
	size_t _cnt = 0;
	const char *const _end = _s + _sz;
	while ((_s = (char *)memmem(_s, _end - _s, _find, _findlen)))
		++_cnt, _s += _findlen;
	return _cnt;
}

#endif

/*
  Count occurences of NE in HS.
  Return value:
  occurences of NE in HS.
*/
JSTR_PURE
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
JSTR_MAYBE_UNUSED
JSTR_NOTHROW
static size_t
jstr_count(const char *JSTR_RST _s,
	   const char *JSTR_RST const _find) JSTR_NOEXCEPT
{
	if (jstr_unlikely(*_find == '\0'))
		return 0;
	if (jstr_unlikely(_find[1] == '\0'))
		return jstr_countc(_s, *_find);
	const size_t _findlen = strlen(_find + 1) + 1;
	size_t _cnt = 0;
	while ((_s = (char *)strstr(_s, _find)))
		++_cnt, _s += _findlen;
	return _cnt;
}

#define PJSTR_NUMTOSTR(_max_digits, _base)                                     \
	do {                                                                   \
		JSTR_ASSERT_IS_STR(_dst);                                      \
		JSTR_ASSERT_IS_SIZE(_base);                                    \
		JSTR_ASSERT_IS_SIZE(_max_digits);                              \
		unsigned char *_d = (unsigned char *)_dst;                     \
		unsigned char _sbuf[_max_digits];                              \
		unsigned char *JSTR_RST _s = (unsigned char *)_sbuf;           \
		unsigned int _neg = (_num < 0) ? (_num = -_num, 1) : 0;        \
		unsigned char *const _end = (unsigned char *)_s + _max_digits; \
		_s = _end - 1;                                                 \
		do                                                             \
			*_s-- = _num % _base + '0';                            \
		while (_num /= 10);                                            \
		if (_neg)                                                      \
			*_s = '-';                                             \
		else                                                           \
			++_s;                                                  \
		while (_s < _end)                                              \
			*_d++ = *_s++;                                         \
		*_d = '\0';                                                    \
		return (char *)_d;                                             \
	} while (0)

#define PJSTR_UNUMTOSTR(_max_digits, _base)                   \
	do {                                                  \
		JSTR_ASSERT_IS_STR(_dst);                     \
		JSTR_ASSERT_IS_SIZE(_base);                   \
		JSTR_ASSERT_IS_SIZE(_max_digits);             \
		unsigned char *_d = (unsigned char *)_dst;    \
		unsigned char _sbuf[_max_digits];             \
		unsigned char *JSTR_RST _s = _sbuf;           \
		unsigned char *const _end = _s + _max_digits; \
		_s = _end - 1;                                \
		do                                            \
			*_s-- = _num % _base + '0';           \
		while (_num /= 10);                           \
		++_s;                                         \
		while (_s < _end)                             \
			*_d++ = *_s++;                        \
		*_d = '\0';                                   \
		return (char *)_d;                            \
	} while (0)

#define PJSTR_NUMTOSTR_ATTR  \
	JSTR_MAYBE_UNUSED    \
	JSTR_NONNULL_ALL     \
	JSTR_WARN_UNUSED     \
	JSTR_RETURNS_NONNULL \
	JSTR_NOTHROW

#define PJSTR_DEFINE_NUMTOSTR(_num_type, _func_name, _max) \
	PJSTR_NUMTOSTR_ATTR                                \
	static char *                                      \
	_func_name(char *JSTR_RST const _dst,              \
		   _num_type _num,                         \
		   const unsigned int _base) JSTR_NOEXCEPT \
	{                                                  \
		PJSTR_NUMTOSTR(_max, _base);               \
	}

#define PJSTR_DEFINE_NUMTOSTR_BASE(_num_type, _func_name, _max, _base) \
	PJSTR_NUMTOSTR_ATTR                                            \
	static char *                                                  \
	_func_name(char *JSTR_RST const _dst,                          \
		   _num_type _num) JSTR_NOEXCEPT                       \
	{                                                              \
		PJSTR_NUMTOSTR(_max, _base);                           \
	}

#define PJSTR_DEFINE_UNUMTOSTR(_num_type, _func_name, _max) \
	PJSTR_NUMTOSTR_ATTR                                 \
	static char *                                       \
	_func_name(char *JSTR_RST const _dst,               \
		   _num_type _num,                          \
		   const unsigned int _base) JSTR_NOEXCEPT  \
	{                                                   \
		PJSTR_UNUMTOSTR(_max, _base);               \
	}

#define PJSTR_DEFINE_UNUMTOSTR_BASE(_num_type, _func_name, _max, _base) \
	PJSTR_NUMTOSTR_ATTR                                             \
	static char *                                                   \
	_func_name(char *JSTR_RST const _dst,                           \
		   _num_type _num) JSTR_NOEXCEPT                        \
	{                                                               \
		PJSTR_UNUMTOSTR(_max, _base);                           \
	}

PJSTR_DEFINE_NUMTOSTR(int, jstr_itoa, PJSTR_MAX_INT_DIGITS)
PJSTR_DEFINE_NUMTOSTR(long, jstr_ltoa, PJSTR_MAX_INT_DIGITS)
PJSTR_DEFINE_NUMTOSTR(long long, jstr_lltoa, PJSTR_MAX_INT_DIGITS)
PJSTR_DEFINE_UNUMTOSTR(unsigned int, jstr_utoa, PJSTR_MAX_INT_DIGITS)
PJSTR_DEFINE_UNUMTOSTR(unsigned long, jstr_ultoa, PJSTR_MAX_INT_DIGITS)
PJSTR_DEFINE_UNUMTOSTR(unsigned long long, jstr_ulltoa, PJSTR_MAX_INT_DIGITS)

#define PJSTR_DEFINE_NUMTOSTRALL_BASE(_base)                                                       \
	PJSTR_DEFINE_NUMTOSTR_BASE(int, jstr_itoa##_base, PJSTR_MAX_INT_DIGITS, _base)             \
	PJSTR_DEFINE_NUMTOSTR_BASE(long, jstr_ltoa##_base, PJSTR_MAX_INT_DIGITS, _base)            \
	PJSTR_DEFINE_NUMTOSTR_BASE(long long, jstr_lltoa##_base, PJSTR_MAX_INT_DIGITS, _base)      \
	PJSTR_DEFINE_UNUMTOSTR_BASE(unsigned int, jstr_utoa##_base, PJSTR_MAX_INT_DIGITS, _base)   \
	PJSTR_DEFINE_UNUMTOSTR_BASE(unsigned long, jstr_ultoa##_base, PJSTR_MAX_INT_DIGITS, _base) \
	PJSTR_DEFINE_UNUMTOSTR_BASE(unsigned long long, jstr_ulltoa##_base, PJSTR_MAX_INT_DIGITS, _base)

PJSTR_DEFINE_NUMTOSTRALL_BASE(2);
PJSTR_DEFINE_NUMTOSTRALL_BASE(3);
PJSTR_DEFINE_NUMTOSTRALL_BASE(4);
PJSTR_DEFINE_NUMTOSTRALL_BASE(5);
PJSTR_DEFINE_NUMTOSTRALL_BASE(6);
PJSTR_DEFINE_NUMTOSTRALL_BASE(7);
PJSTR_DEFINE_NUMTOSTRALL_BASE(8);
PJSTR_DEFINE_NUMTOSTRALL_BASE(9);
PJSTR_DEFINE_NUMTOSTRALL_BASE(10);
PJSTR_DEFINE_NUMTOSTRALL_BASE(11);
PJSTR_DEFINE_NUMTOSTRALL_BASE(12);
PJSTR_DEFINE_NUMTOSTRALL_BASE(13);
PJSTR_DEFINE_NUMTOSTRALL_BASE(14);
PJSTR_DEFINE_NUMTOSTRALL_BASE(15);
PJSTR_DEFINE_NUMTOSTRALL_BASE(16);
PJSTR_DEFINE_NUMTOSTRALL_BASE(17);
PJSTR_DEFINE_NUMTOSTRALL_BASE(18);
PJSTR_DEFINE_NUMTOSTRALL_BASE(19);
PJSTR_DEFINE_NUMTOSTRALL_BASE(20);
PJSTR_DEFINE_NUMTOSTRALL_BASE(21);
PJSTR_DEFINE_NUMTOSTRALL_BASE(22);
PJSTR_DEFINE_NUMTOSTRALL_BASE(23);
PJSTR_DEFINE_NUMTOSTRALL_BASE(24);
PJSTR_DEFINE_NUMTOSTRALL_BASE(25);
PJSTR_DEFINE_NUMTOSTRALL_BASE(26);
PJSTR_DEFINE_NUMTOSTRALL_BASE(27);
PJSTR_DEFINE_NUMTOSTRALL_BASE(28);
PJSTR_DEFINE_NUMTOSTRALL_BASE(29);
PJSTR_DEFINE_NUMTOSTRALL_BASE(30);
PJSTR_DEFINE_NUMTOSTRALL_BASE(31);
PJSTR_DEFINE_NUMTOSTRALL_BASE(32);
PJSTR_DEFINE_NUMTOSTRALL_BASE(33);
PJSTR_DEFINE_NUMTOSTRALL_BASE(34);
PJSTR_DEFINE_NUMTOSTRALL_BASE(35);
PJSTR_DEFINE_NUMTOSTRALL_BASE(36);

#undef PJSTR_DEFINE_NUMTOSTRALL_BASE
#undef PJSTR_DEFINE_NUMTOSTRALL
#undef PJSTR_DEFINE_UNUMTOSTR_BASE
#undef PJSTR_DEFINE_NUMTOSTR_BASE
#undef PJSTR_DEFINE_UNUMTOSTR
#undef PJSTR_DEFINE_NUMTOSTR

#ifdef __cplusplus
} /* extern C */
#endif /* __cpluslus */

#undef PJSTR_MAX_INT_DIGITS
#undef PJSTR_MAX_LONG_DIGITS
#undef PJSTR_MAX_LONG_LONG_DIGITS
#undef PJSTR_MAX_UINT_DIGITS
#undef PJSTR_MAX_ULONG_DIGITS
#undef PJSTR_MAX_ULONG_LONG_DIGITS
#undef PJSTR_UNUMTOSTR
#undef PJSTR_NUMTOSTR

#endif /* JSTR_STRING_DEF_H */
