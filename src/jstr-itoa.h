#ifndef JSTR_ITOA_H
#define JSTR_ITOA_H 1

#include "jstr-macros.h"

PJSTR_BEGIN_DECLS
#include <stdlib.h>
#include <string.h>
PJSTR_END_DECLS

#include "jstr-replace.h"

#define R JSTR_RESTRICT

PJSTR_BEGIN_DECLS

/* Return ptr to '\0' after the last digit in the DEST string. */
JSTR_FUNC
JSTR_RETURNS_NONNULL
static char *
jstr_ulltoa_p(char *R nptr,
	      unsigned long long number,
	      const unsigned int base) JSTR_NOEXCEPT
{
	char *start = nptr;
	do
		*nptr++ = number % base + '0';
	while (number /= base);
	const char *const end = nptr;
	*nptr-- = '\0';
	int c;
	while (start < nptr) {
		c = *start;
		*start++ = *nptr;
		*nptr-- = c;
	}
	return (char *)end;
}

/* Return ptr to '\0' after the last digit in the DEST string. */
JSTR_FUNC
JSTR_RETURNS_NONNULL
JSTR_INLINE
static char *
jstr_lltoa_p(char *R nptr,
	     long long number,
	     const unsigned int base) JSTR_NOEXCEPT
{
	if (number < 0) {
		number = -number;
		*nptr++ = '-';
	}
	return jstr_ulltoa_p(nptr, number, base);
}

#define PJSTR_ULLTOA(type, name, u)                                       \
	/* Return ptr to '\0' after the last digit in the DEST string. */ \
	JSTR_FUNC                                                         \
	JSTR_RETURNS_NONNULL                                              \
	JSTR_INLINE                                                       \
	static char *                                                     \
	jstr_##name##_p(char *R nptr,                                     \
			type number,                                      \
			const unsigned int base) JSTR_NOEXCEPT            \
	{                                                                 \
		return jstr_##u##lltoa_p(nptr, number, base);             \
	}

PJSTR_ULLTOA(unsigned long, ultoa, u)
PJSTR_ULLTOA(unsigned int, utoa, u)
PJSTR_ULLTOA(long, ltoa, )
PJSTR_ULLTOA(int, itoa, )

#undef PJSTR_ULLTOA

/* Return ptr to '\0' after the last digit in the DEST string. */
JSTR_FUNC
JSTR_RETURNS_NONNULL
static char *
jstr_ulltoa_p_sep(char *R nptr,
		  unsigned long long number,
		  const unsigned int base,
		  const int separator)
{
	char *start = nptr;
	int n = 0;
	int c;
	for (unsigned long long loop;;) {
		c = number % base + '0';
		loop = number /= base;
		if (++n != 3) {
			*nptr++ = c;
			if (jstr_unlikely(loop == 0))
				break;
		} else {
			if (loop) {
				*nptr = c;
				*(nptr + 1) = separator;
				nptr += 2;
			} else {
				*nptr++ = c;
				break;
			}
			n = 0;
		}
	}
	const char *const end = nptr;
	*nptr-- = '\0';
	while (start < nptr) {
		c = *start;
		*start++ = *nptr;
		*nptr-- = c;
	}
	return (char *)end;
}

/* Return ptr to '\0' after the last digit in the DEST string. */
JSTR_FUNC
JSTR_INLINE
static char *
jstr_lltoa_p_sep(char *R nptr,
		 long long number,
		 const unsigned int base,
		 const int separator)
{
	if (number < 0) {
		number = -number;
		*nptr++ = '-';
	}
	return jstr_ulltoa_p_sep(nptr, number, base, separator);
}

#define PJSTR_ULLTOA_SEP(type, name, u)                                      \
	/* Return ptr to '\0' after the last digit in the DEST string. */    \
	JSTR_FUNC                                                            \
	JSTR_RETURNS_NONNULL                                                 \
	JSTR_INLINE                                                          \
	static char *                                                        \
	jstr_##name##_p_sep(char *R nptr,                                    \
			    type number,                                     \
			    const unsigned int base,                         \
			    const int separator) JSTR_NOEXCEPT               \
	{                                                                    \
		return jstr_##u##lltoa_p_sep(nptr, number, base, separator); \
	}

PJSTR_ULLTOA_SEP(unsigned long, ultoa, u)
PJSTR_ULLTOA_SEP(unsigned int, utoa, u)
PJSTR_ULLTOA_SEP(long, ltoa, )
PJSTR_ULLTOA_SEP(int, itoa, )

#undef PJSTR_ULLTOA_SEP

PJSTR_END_DECLS

#undef R

#endif /* JSTR_ITOA_H */
