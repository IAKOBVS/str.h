/* Copyright (C) 1992-2023 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <https://www.gnu.org/licenses/>.  */

#ifndef JSTR_LGPL_STPCPY_H
#define JSTR_LGPL_STPCPY_H 1

#ifndef JSTR_LGPL_IMPL
#	include "jstr-macros.h"
#	include "jstr-word-at-a-time.h"
#endif

#ifndef JSTR_LGPL_IMPL
PJSTR_BEGIN_DECLS
#	include <stddef.h>
PJSTR_END_DECLS

char *
stpcpy(char *dst, const char *src)
{
#endif
#if JSTR_BYTE_ORDER == JSTR_LITTLE_ENDIAN
#	define JSTR_WORD_MERGE(w0, sh_1, w1, sh_2) (((w0) >> (sh_1)) | ((w1) << (sh_2)))
#elif JSTR_BYTE_ORDER == JSTR_BIG_ENDIAN
#	define JSTR_WORD_MERGE(w0, sh_1, w1, sh_2) (((w0) << (sh_1)) | ((w1) >> (sh_2)))
#endif
	/* Copy just a few bytes to make DEST aligned.  */
	size_t len = (-(uintptr_t)dst) % sizeof(jstr_word_ty);
	for (; len != 0; len--, ++dst) {
		const char c = *src++;
		*dst = c;
		if (c == '\0')
			return dst;
	}
	/* DEST is now aligned to jstr_word_ty , SRC may or may not be.  */
	const uintptr_t ofs = (uintptr_t)src % sizeof(jstr_word_ty);
	jstr_word_ty word;
	jstr_word_ty *d = (jstr_word_ty *)dst;
	const jstr_word_ty *s = (jstr_word_ty *)src;
	/* Aligned loop. */
	if (ofs == 0) {
		for (;;) {
			word = *s++;
			if (jstr_word_has_zero(word))
				break;
			*d++ = word;
		}
	} else {
		/* Unaligned loop. */
		jstr_word_ty w2a = *s++;
		uintptr_t sh_1 = ofs * CHAR_BIT;
		uintptr_t sh_2 = sizeof(jstr_word_ty) * CHAR_BIT - sh_1;
		word = JSTR_WORD_MERGE(w2a, sh_1, (jstr_word_ty)-1, sh_2);
		if (!jstr_word_has_zero(word)) {
			jstr_word_ty w2b;
			/* Unaligned loop.  The invariant is that W2B, which is "ahead" of W1,
			   does not contain end-of-string.  Therefore it is safe (and necessary)
			   to read another word from each while we do not have a difference.  */
			for (;;) {
				w2b = *s++;
				word = JSTR_WORD_MERGE(w2a, sh_1, w2b, sh_2);
				/* Check if there is zero on w2a.  */
				if (jstr_word_has_zero(word))
					goto out;
				*d++ = word;
				if (jstr_word_has_zero(w2b))
					break;
				w2a = w2b;
			}
			/* Align the final partial of P2.  */
			word = JSTR_WORD_MERGE(w2b, sh_1, 0, sh_2);
		}
	}
out:
	dst = (char *)d;
	for (size_t i = 0; i < sizeof(jstr_word_ty); i++, ++dst) {
		const char c = (char)jstr_word_extractbyte(word, i);
		*dst = c;
		if (c == '\0')
			break;
	}
	return dst;
#ifndef JSTR_LGPL_IMPL
}
#endif

#endif /* JSTR_LGPL_STPCPY_H */