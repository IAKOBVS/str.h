/* See LICENSE file for copyright and license details. */

#ifndef JSTR_CTYPE_H
#define JSTR_CTYPE_H 1

#include "jstr-macros.h"

JSTRP_BEGIN_DECLS
#include <errno.h>
#include <string.h>
JSTRP_END_DECLS

#include "jstr-ctype-table.h"
#include "jstr-macros.h"
#include "jstr-stdstring.h"

#define R JSTR_RESTRICT

JSTRP_BEGIN_DECLS

/*
   ASCII.
   Will NOT handle EOF correctly.
   toupper(EOF) != EOF;
*/
JSTR_INLINE
JSTR_FUNC_CONST
static int
jstr_toupper(const int c)
JSTR_NOEXCEPT
{
	return jstrp_ctype_toupper[(unsigned char)c];
}

/*
   ASCII.
   Will NOT handle EOF correctly.
   tolower(EOF) != EOF;
*/
JSTR_INLINE
JSTR_FUNC_CONST
static int
jstr_tolower(const int c)
JSTR_NOEXCEPT
{
	return jstrp_ctype_tolower[(unsigned char)c];
}

#define JSTRP_DEFINE_REPEAT_CTYPE(FUNC) \
	FUNC(alpha, JSTR_ISALPHA)       \
	FUNC(lower, JSTR_ISLOWER)       \
	FUNC(upper, JSTR_ISUPPER)       \
	FUNC(space, JSTR_ISSPACE)       \
	FUNC(blank, JSTR_ISBLANK)       \
	FUNC(digit, JSTR_ISDIGIT)       \
	FUNC(xdigit, JSTR_ISXDIGIT)     \
	FUNC(punct, JSTR_ISPUNCT)       \
	FUNC(graph, JSTR_ISGRAPH)       \
	FUNC(print, JSTR_ISPRINT)       \
	FUNC(cntrl, JSTR_ISCNTRL)       \
	FUNC(word, JSTR_ISWORD)

/*
   ASCII.
   For example: jstr_isctype(C, JSTR_ISLOWER | JSTR_ISDIGIT) checks if C is either lowercase or a digit.
*/
JSTR_INLINE
JSTR_FUNC_CONST
static int
jstr_isctype(const int c,
             const jstr_ctype_ty type)
JSTR_NOEXCEPT
{
	return jstrp_ctype[(unsigned char)c] & type;
}

#define JSTRP_DEFINE_ISCTYPE(ctype, ctype_enum)     \
	/* ASCII. */                                \
	JSTR_INLINE                                 \
	JSTR_FUNC_CONST                             \
	static int                                  \
	jstr_is##ctype(const int c)                 \
	JSTR_NOEXCEPT                               \
	{                                           \
		return jstr_isctype(c, ctype_enum); \
	}

JSTRP_DEFINE_REPEAT_CTYPE(JSTRP_DEFINE_ISCTYPE);

#undef JSTRP_DEFINE_ISCTYPE

/* ASCII. */
JSTR_INLINE
JSTR_FUNC_PURE
static char *
jstr_skipctype(const char *R s,
               const jstr_ctype_ty ctype)
JSTR_NOEXCEPT
{
	if (jstr_unlikely(ctype & JSTR_ISCNTRL)) {
		while (*s && jstr_isctype(*s, ctype))
			++s;
		return (char *)s;
	}
	while (jstr_isctype(*s++, ctype))
		;
	return (char *)s - 1;
}

#define JSTRP_DEFINE_SKIP_CTYPE(ctype, ctype_enum)    \
	/* ASCII. */                                  \
	JSTR_INLINE                                   \
	JSTR_FUNC_PURE                                \
	static char *                                 \
	jstr_skip##ctype(const char *R s)             \
	JSTR_NOEXCEPT                                 \
	{                                             \
		return jstr_skipctype(s, ctype_enum); \
	}

JSTRP_DEFINE_REPEAT_CTYPE(JSTRP_DEFINE_SKIP_CTYPE)

#undef JSTRP_DEFINE_SKIP_CTYPE

/* ASCII. */
JSTR_INLINE
JSTR_FUNC_PURE
static int
jstr_isctypestr(const char *R s,
                const jstr_ctype_ty ctype)
JSTR_NOEXCEPT
{
	return jstr_likely(*s) && *(jstr_skipctype(s, ctype) + 1) == '\0';
}

#define JSTRP_DEFINE_ISCTYPE_STR(ctype, ctype_enum)    \
	/* ASCII. */                                   \
	JSTR_INLINE                                    \
	JSTR_FUNC_PURE                                 \
	static int                                     \
	jstr_is##ctype##str(const char *R s)           \
	JSTR_NOEXCEPT                                  \
	{                                              \
		return jstr_isctypestr(s, ctype_enum); \
	}

JSTRP_DEFINE_REPEAT_CTYPE(JSTRP_DEFINE_ISCTYPE_STR)

#undef JSTRP_DEFINE_ISCTYPE_STR

/* ASCII. */
JSTR_INLINE
JSTR_FUNC_PURE
static char *
jstr_skipctype_rev(const char *const start,
                   const char *end,
                   const jstr_ctype_ty ctype)
JSTR_NOEXCEPT
{
	while (start != end
	       && jstr_isctype(*end, ctype))
		--end;
	return (char *)end;
}

#define JSTRP_DEFINE_SKIP_CTYPE_REV(ctype, ctype_enum)             \
	/* ASCII. */                                               \
	JSTR_INLINE                                                \
	JSTR_FUNC_PURE                                             \
	static char *                                              \
	jstr_skip##ctype##_rev(const char *const start,            \
	                       const char *end)                    \
	JSTR_NOEXCEPT                                              \
	{                                                          \
		return jstr_skipctype_rev(start, end, ctype_enum); \
	}

JSTRP_DEFINE_REPEAT_CTYPE(JSTRP_DEFINE_SKIP_CTYPE_REV)

#undef JSTRP_DEFINE_SKIP_CTYPE_REV

/* ASCII. */
JSTR_FUNC_VOID
JSTR_INLINE
static void
jstr_toupperstr(char *R s)
JSTR_NOEXCEPT
{
	while ((*s = jstr_toupper(*s)))
		++s;
}

/* ASCII. */
JSTR_FUNC_VOID
JSTR_INLINE
static void
jstr_tolowerstr(char *R s)
JSTR_NOEXCEPT
{
	while ((*s = jstr_tolower(*s)))
		++s;
}

/*
   Copy SRC to DST touppered.
   Return pointer to '\0' in DST.
*/
JSTR_FUNC
JSTR_INLINE
static char *
jstr_toupperstrcpy_p(char *R dst,
                     const char *R src)
JSTR_NOEXCEPT
{
	while ((*dst++ = jstr_tolower(*src++)))
		;
	return dst - 1;
}

/*
   Copy SRC to DST tolowered.
   Return pointer to '\0' in DST.
*/
JSTR_FUNC
JSTR_INLINE
static char *
jstr_tolowerstrcpy_p(char *R dst,
                     const char *R src)
JSTR_NOEXCEPT
{
	while ((*dst++ = jstr_tolower(*src++)))
		;
	return dst - 1;
}

JSTRP_END_DECLS

#undef JSTRP_DEFINE_REPEAT_CTYPE
#undef R

#endif /* JSTR_CTYPE_H */
