#ifndef JSTR_REGEX_H
#define JSTR_REGEX_H 1

#include "jstr-macros.h"

PJSTR_BEGIN_DECLS
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
PJSTR_END_DECLS

#include "jstr-builder.h"
#include "jstr-config.h"
#include "jstr-replace.h"
#include "jstr-string.h"

#define R JSTR_RESTRICT

#define PJSTR_REG_COMP_NOW()                                                      \
	do {                                                                      \
		const jstr_reg_errcode_ty ret = jstr_reg_comp(preg, ptn, cflags); \
		if (jstr_unlikely(ret != JSTR_REG_RET_NOERROR))                   \
			return ret;                                               \
	} while (0)

/* POSIX cflags */
#define JSTR_REG_CF_EXTENDED REG_EXTENDED
#define JSTR_REG_CF_ICASE    REG_ICASE
#define JSTR_REG_CF_NEWLINE  REG_NEWLINE

/* POSIX eflags */
#define JSTR_REG_EF_NOSUB  REG_NOSUB
#define JSTR_REG_EF_NOTBOL REG_NOTBOL
#define JSTR_REG_EF_NOTEOL REG_NOTEOL

/* BSD extension */
#ifdef REG_STARTEND
#	define JSTR_REG_EF_STARTEND REG_STARTEND
#endif /* REG_STARTEND */

PJSTR_BEGIN_DECLS

typedef enum {
#ifdef REG_ENOSYS
	JSTR_REG_RET_ENOSYS = REG_ENOSYS,
#endif
#if defined REG_NOERROR
	JSTR_REG_RET_NOERROR = REG_NOERROR,
#else
	JSTR_REG_RET_NOERROR = 0,
#endif
#define JSTR_REG_RET_NOERROR JSTR_REG_RET_NOERROR
	JSTR_REG_RET_NOMATCH = REG_NOMATCH,
#define JSTR_REG_RET_NOMATCH JSTR_REG_RET_NOMATCH
	/* POSIX regcomp return values */
	JSTR_REG_RET_BADPAT = REG_BADPAT,
#define JSTR_REG_RET_BADPAT JSTR_REG_RET_BADPAT
	JSTR_REG_RET_ECOLLATE = REG_ECOLLATE,
#define JSTR_REG_RET_ECOLLATE JSTR_REG_RET_ECOLLATE
	JSTR_REG_RET_ECTYPE = REG_ECTYPE,
#define JSTR_REG_RET_ECTYPE JSTR_REG_RET_ECTYPE
	JSTR_REG_RET_EESCAPE = REG_EESCAPE,
#define JSTR_REG_RET_EESCAPE JSTR_REG_RET_EESCAPE
	JSTR_REG_RET_ESUBREG = REG_ESUBREG,
#define JSTR_REG_RET_ESUBREG JSTR_REG_RET_ESUBREG
	JSTR_REG_RET_EBRACK = REG_EBRACK,
#define JSTR_REG_RET_EBRACK JSTR_REG_RET_EBRACK
	JSTR_REG_RET_EPAREN = REG_EPAREN,
#define JSTR_REG_RET_EPAREN JSTR_REG_RET_EPAREN
	JSTR_REG_RET_EBRACE = REG_EBRACE,
#define JSTR_REG_RET_EBRACE JSTR_REG_RET_EBRACE
	JSTR_REG_RET_BADBR = REG_BADBR,
#define JSTR_REG_RET_BADBR JSTR_REG_RET_BADBR
	JSTR_REG_RET_ERANGE = REG_ERANGE,
#define JSTR_REG_RET_ERANGE JSTR_REG_RET_ERANGE
	JSTR_REG_RET_ESPACE = REG_ESPACE,
#define JSTR_REG_RET_ESPACE JSTR_REG_RET_ESPACE
	JSTR_REG_RET_BADRPT = REG_BADRPT,
#define JSTR_REG_RET_BADRPT JSTR_REG_RET_BADRPT
/* GNU regcomp returns */
#ifdef REG_RET_EEND
	JSTR_REG_EEND = REG_RET_EEND,
#	define JSTR_REG_EEND JSTR_REG_EEND
#endif
#ifdef REG_RET_ESIZE
	JSTR_REG_ESIZE = REG_RET_ESIZE,
#	define JSTR_REG_ESIZE JSTR_REG_ESIZE
#endif
#ifdef REG_RET_ERPAREN
	JSTR_REG_ERPAREN = REG_RET_ERPAREN
#	define JSTR_REG_ERPAREN JSTR_REG_ERPAREN
#endif
} jstr_reg_errcode_ty;

JSTR_INLINE
JSTR_FUNC_VOID
static void
jstr_reg_free(regex_t *R preg)
JSTR_NOEXCEPT
{
	regfree(preg);
}

JSTR_COLD
JSTR_FUNC_VOID
JSTR_NOINLINE
static void
pjstr_reg_err_print(const int errcode,
		    const regex_t *R preg)
JSTR_NOEXCEPT
{
	char buf[64];
	regerror(errcode, preg, buf, 32);
	fprintf(stderr, "%s\n", buf);
}

JSTR_COLD
JSTR_FUNC_VOID
JSTR_NOINLINE
static void
pjstr_reg_err_exit_print(const int errcode,
			 const regex_t *R preg)
JSTR_NOEXCEPT
{
	char buf[64];
	regerror(errcode, preg, buf, 32);
	fprintf(stderr, "%s\n", buf);
	exit(EXIT_FAILURE);
}

#define PJSTR_REG_NOERR(errcode) jstr_likely(errcode == JSTR_REG_RET_NOERROR || errcode == JSTR_REG_RET_NOMATCH)

JSTR_FUNC_VOID
JSTR_INLINE
static void
jstr_reg_err_exit(jstr_reg_errcode_ty errcode,
		  const regex_t *R preg)
JSTR_NOEXCEPT
{
	if (PJSTR_REG_NOERR(errcode))
		return;
	pjstr_reg_err_exit_print(errcode, preg);
}

JSTR_FUNC_VOID
JSTR_INLINE
static void
jstr_reg_err_print(jstr_reg_errcode_ty errcode,
		   const regex_t *R preg)
JSTR_NOEXCEPT
{
	if (PJSTR_REG_NOERR(errcode))
		return;
	pjstr_reg_err_print(errcode, preg);
}

JSTR_FUNC_VOID
JSTR_INLINE
static void
jstr_reg_err(jstr_reg_errcode_ty errcode,
	     const regex_t *R preg,
	     char *R errbuf,
	     const size_t errbuf_size)
JSTR_NOEXCEPT
{
	if (PJSTR_REG_NOERR(errcode))
		return;
	regerror(errcode, preg, errbuf, errbuf_size);
}

JSTR_FUNC
JSTR_INLINE
static jstr_reg_errcode_ty
jstr_reg_comp(regex_t *R preg,
	      const char *R ptn,
	      const int cflags)
JSTR_NOEXCEPT
{
	return (jstr_reg_errcode_ty)regcomp(preg, ptn, cflags);
}

JSTR_NONNULL(1)
JSTR_NONNULL(2)
JSTR_INLINE
JSTR_WARN_UNUSED
JSTR_PURE
JSTR_NOTHROW
static jstr_reg_errcode_ty
jstr_reg_exec(const regex_t *R preg,
	      const char *R s,
	      const size_t nmatch,
	      regmatch_t *R pmatch,
	      const int eflags)
JSTR_NOEXCEPT
{
	return (jstr_reg_errcode_ty)regexec(preg, s, nmatch, pmatch, eflags);
}

#ifdef JSTR_REG_EF_STARTEND

JSTR_FUNC_PURE
JSTR_INLINE
static jstr_reg_errcode_ty
jstr_reg_exec_len(const regex_t *R preg,
		  const char *R s,
		  const size_t sz,
		  const size_t nmatch,
		  regmatch_t *R pmatch,
		  const int eflags)
JSTR_NOEXCEPT
{
	pmatch->rm_so = 0;
	pmatch->rm_eo = sz;
	return (jstr_reg_errcode_ty)regexec(preg, s, nmatch, pmatch, eflags | JSTR_REG_EF_STARTEND);
}

#endif /* JSTR_REG_EF_STARTEND */

#ifdef JSTR_REG_EF_STARTEND
#	define PJSTR_REG_EXEC(preg, s, sz, nmatch, pmatch, eflags) \
		jstr_reg_exec_len(preg, s, sz, nmatch, pmatch, eflags)
#else
#	define PJSTR_REG_EXEC(preg, s, sz, nmatch, pmatch, eflags) \
		jstr_reg_exec(preg, s, nmatch, pmatch, eflags)
#endif /* JSTR_REG_EF_STARTEND */

/*
   Check if S matches precompiled regex.
   Return return value of regexec.
*/
JSTR_FUNC_PURE
JSTR_INLINE
static jstr_reg_errcode_ty
jstr_reg_match(const char *R s,
	       regex_t *R preg,
	       const int eflags)
JSTR_NOEXCEPT
{
	return jstr_reg_exec(preg, s, 0, NULL, eflags);
}

/*
   Check if S matches PTN.
   Return return value of regexec or regcomp if it fails.
*/
JSTR_FUNC_PURE
JSTR_INLINE
static jstr_reg_errcode_ty
jstr_reg_match_now(const char *R s,
		   const char *R ptn,
		   regex_t *R preg,
		   const int cflags,
		   const int eflags)
JSTR_NOEXCEPT
{
	PJSTR_REG_COMP_NOW();
	return jstr_reg_match(s, preg, eflags);
}

/*
   Search pattern in S.
   Return return value of regexec.
   Store offset of matched pattern in pmatch.
*/
JSTR_FUNC_PURE
JSTR_INLINE
static jstr_reg_errcode_ty
jstr_reg_search(const char *R s,
		regex_t *R preg,
		regmatch_t *R pmatch,
		const int eflags)
JSTR_NOEXCEPT
{
	return jstr_reg_exec(preg, s, 1, pmatch, eflags);
}

/*
   Search pattern in S.
   Return return value of regexec or regcomp if it fails.
   Store offset of matched pattern in pmatch.
*/
JSTR_FUNC_PURE
JSTR_INLINE
static jstr_reg_errcode_ty
jstr_reg_search_now(const char *R s,
		    const char *R ptn,
		    regex_t *R preg,
		    regmatch_t *R pmatch,
		    const int cflags,
		    const int eflags)
JSTR_NOEXCEPT
{
	PJSTR_REG_COMP_NOW();
	return jstr_reg_search(s, preg, pmatch, eflags);
}

#ifdef JSTR_REG_EF_STARTEND

/*
   Search pattern in S.
   Return return value of regexec.
   Store offset of matched pattern in pmatch.
*/
JSTR_FUNC
JSTR_INLINE
static jstr_reg_errcode_ty
jstr_reg_search_len(const char *R s,
		    const size_t sz,
		    regex_t *R preg,
		    regmatch_t *R pmatch,
		    const int eflags)
JSTR_NOEXCEPT
{
	return jstr_reg_exec_len(preg, s, sz, 1, pmatch, eflags);
}

/*
   Search pattern in S.
   Return return value of regexec.
   Store offset of matched pattern in pmatch.
*/
JSTR_FUNC_PURE
JSTR_INLINE
static jstr_reg_errcode_ty
jstr_reg_search_len_now(const char *R s,
			const char *R ptn,
			const size_t sz,
			regex_t *R preg,
			regmatch_t *R pmatch,
			const int cflags,
			const int eflags)
JSTR_NOEXCEPT
{
	PJSTR_REG_COMP_NOW();
	return jstr_reg_search_len(s, sz, preg, pmatch, eflags);
}

/*
   Check if S matches PTN.
   Return return value of regexec or regcomp if it fails.
*/
JSTR_FUNC_PURE
JSTR_INLINE
static jstr_reg_errcode_ty
jstr_reg_match_len(const char *R s,
		   const size_t sz,
		   regex_t *R preg,
		   const int eflags)
JSTR_NOEXCEPT
{
	regmatch_t rm;
	return jstr_reg_exec_len(preg, s, sz, 0, &rm, eflags | JSTR_REG_EF_STARTEND);
}

/*
   Check if S matches PTN.
   Return return value of regexec or regcomp if it fails.
*/
JSTR_FUNC_PURE
JSTR_INLINE
static jstr_reg_errcode_ty
jstr_reg_match_len_now(const char *R s,
		       const char *R ptn,
		       const size_t sz,
		       regex_t *R preg,
		       const int cflags,
		       const int eflags)
JSTR_NOEXCEPT
{
	PJSTR_REG_COMP_NOW();
	return jstr_reg_match_len(s, sz, preg, eflags);
}

#endif /* JSTR_REG_EF_STARTEND */

#ifdef __clang__
#	pragma clang diagnostic ignored "-Wunknown-warning-option"
#	pragma clang diagnostic push
#elif defined __GNUC__
#	pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#	pragma GCC diagnostic push
#endif

JSTR_FUNC
JSTR_INLINE
static jstr_reg_errcode_ty
pjstr_reg_remove(char *R s,
		 size_t *R sz,
		 const size_t start_idx,
		 const regex_t *R preg,
		 const int eflags)
JSTR_NOEXCEPT
{
	regmatch_t rm;
	const jstr_reg_errcode_ty ret = PJSTR_REG_EXEC(preg, s + start_idx, *sz - start_idx, 1, &rm, eflags);
	if (jstr_unlikely(ret != JSTR_REG_RET_NOERROR)
	    || jstr_unlikely(rm.rm_eo == rm.rm_so))
		return ret;
	rm.rm_so += start_idx;
	rm.rm_eo += start_idx;
	jstr_strmove_len(s + rm.rm_so,
			 s + rm.rm_eo,
			 *sz - (rm.rm_eo - rm.rm_so));
	*sz -= (rm.rm_eo - rm.rm_so);
	return ret;
}

#ifdef __clang__
#	pragma clang diagnostic pop
#elif defined __GNUC__
#	pragma GCC diagnostic pop
#endif

JSTR_FUNC
static jstr_reg_errcode_ty
jstr_reg_remove(char *R s,
		size_t *R sz,
		const regex_t *R preg,
		const int eflags)
JSTR_NOEXCEPT
{
	return pjstr_reg_remove(s, sz, 0, preg, eflags);
}

JSTR_FUNC
JSTR_INLINE
static jstr_reg_errcode_ty
jstr_reg_remove_now(char *R s,
		    size_t *R sz,
		    const char *R ptn,
		    regex_t *R preg,
		    const int cflags,
		    const int eflags)
JSTR_NOEXCEPT
{
	PJSTR_REG_COMP_NOW();
	return jstr_reg_remove(s, sz, preg, eflags);
}

JSTR_FUNC
static jstr_reg_errcode_ty
jstr_reg_remove_from(char *R s,
		     size_t *R sz,
		     const size_t start_idx,
		     const regex_t *R preg,
		     const int eflags)
JSTR_NOEXCEPT
{
	return pjstr_reg_remove(s, sz, start_idx, preg, eflags);
}

JSTR_FUNC
JSTR_INLINE
static jstr_reg_errcode_ty
jstr_reg_remove_from_now(char *R s,
			 size_t *R sz,
			 const size_t start_idx,
			 const char *R ptn,
			 regex_t *R preg,
			 const int cflags,
			 const int eflags)
JSTR_NOEXCEPT
{
	PJSTR_REG_COMP_NOW();
	return jstr_reg_remove_from(s, sz, start_idx, preg, eflags);
}

JSTR_FUNC
JSTR_INLINE
static jstr_reg_errcode_ty
pjstr_reg_removeall(char *R s,
		    size_t *R sz,
		    const regex_t *R preg,
		    const int eflags,
		    const int use_n,
		    size_t n,
		    const size_t start_idx)
JSTR_NOEXCEPT
{
	regmatch_t rm;
	unsigned char *dst = (unsigned char *)s + start_idx;
	const unsigned char *p = dst;
	const unsigned char *oldp = dst;
	const unsigned char *const end = *(unsigned char **)s + *sz;
	size_t findlen;
	jstr_reg_errcode_ty ret = JSTR_REG_RET_NOMATCH;
	while ((use_n & 1 ? n-- : 1)
	       && PJSTR_REG_EXEC(preg, (char *)p, end - p, 1, &rm, eflags) == JSTR_REG_RET_NOERROR) {
		ret = JSTR_REG_RET_NOERROR;
		findlen = rm.rm_eo - rm.rm_so;
		p = p + rm.rm_so;
		if (jstr_unlikely(findlen == 0))
			++p;
		else
			pjstr_removeall_in_place(&dst, &oldp, &p, findlen);
		if (jstr_unlikely(*p == '\0'))
			break;
	}
	if (jstr_likely(dst != oldp))
		*sz = jstr_stpmove_len(dst, oldp, end - oldp) - s;
	return ret;
}

JSTR_FUNC
static jstr_reg_errcode_ty
jstr_reg_removeall(char *R s,
		   size_t *R sz,
		   const regex_t *R preg,
		   const int eflags)
JSTR_NOEXCEPT
{
	return pjstr_reg_removeall(s, sz, preg, eflags, 0, 0, 0);
}

JSTR_FUNC
static jstr_reg_errcode_ty
jstr_reg_removen(char *R s,
		 size_t *R sz,
		 const size_t n,
		 const regex_t *R preg,
		 const int eflags)
JSTR_NOEXCEPT
{
	return pjstr_reg_removeall(s, sz, preg, eflags, 1, n, 0);
}

JSTR_FUNC
static jstr_reg_errcode_ty
jstr_reg_removeall_from(char *R s,
			size_t *R sz,
			const size_t start_idx,
			const regex_t *R preg,
			const int eflags)
JSTR_NOEXCEPT
{
	return pjstr_reg_removeall(s, sz, preg, eflags, 0, 0, start_idx);
}

JSTR_FUNC_VOID
JSTR_INLINE
static void
pjstr_reg_replaceall_small_rplc(unsigned char *s,
				size_t *R sz,
				unsigned char **dst,
				const unsigned char **oldp,
				unsigned char **p,
				const unsigned char *R rplc,
				const size_t rplclen,
				const size_t findlen)
{
	typedef unsigned char u;
	if (*dst != *oldp) {
		JSTR_PRINT_LOG("*dst != *oldp");
		*dst = (u *)jstr_mempmove(*dst, *oldp, *p - *oldp);
		jstr_strmove_len(*dst + rplclen,
				 *p + findlen,
				 (s + *sz) - (*p + findlen));
		*dst = (u *)jstr_mempcpy(*dst, rplc, rplclen);
		*oldp = *dst;
	} else {
		JSTR_PRINT_LOG("*dst == *oldp");
		jstr_strmove_len(*p + rplclen,
				 *p + findlen,
				 s + *sz - (*p + findlen));
		memcpy(*p, rplc, rplclen);
	}
	*sz += rplclen - findlen;
	*p += rplclen;
}

JSTR_FUNC
JSTR_INLINE
static int
pjstr_reg_replaceall_big_rplc(unsigned char **s,
			      size_t *R sz,
			      size_t *R cap,
			      unsigned char **dst,
			      const unsigned char **oldp,
			      unsigned char **p,
			      const unsigned char *R rplc,
			      const size_t rplclen,
			      const size_t findlen)
{
	JSTR_PRINT_LOG("cap <= *sz + rplclen - findlen");
	if (*dst != *oldp)
		memmove(*dst, *oldp, *p - *oldp);
	const unsigned char *tmp = *s;
	JSTR_RESERVE_ALWAYS_NOMALLOC((char **)s, sz, cap, *sz + rplclen - findlen, return 0);
	jstr_strmove_len(*p + rplclen,
			 *p + findlen,
			 (tmp + *sz) - (*p + findlen));
	memcpy(*p, rplc, rplclen);
	*p = *s + (*p - tmp);
	*dst = *s + (*dst - tmp) + rplclen;
	*oldp = *dst;
	*sz += rplclen - findlen;
	*p += rplclen;
	return 1;
}

JSTR_FUNC
static jstr_reg_errcode_ty
pjstr_reg_replaceall_len(char *R *R s,
			 size_t *R sz,
			 size_t *R cap,
			 const char *R rplc,
			 const size_t rplclen,
			 regex_t *R preg,
			 const int eflags,
			 const int use_n,
			 size_t n,
			 const size_t start_idx)
JSTR_NOEXCEPT
{
	typedef unsigned char u;
	if (jstr_unlikely(rplclen == 0))
		return jstr_reg_removeall_from(*s, sz, start_idx, preg, eflags);
	unsigned char *dst = *(u **)s + start_idx;
	unsigned char *p = dst;
	const unsigned char *oldp = dst;
	size_t findlen;
	regmatch_t rm;
	while ((use_n & 1 ? n-- : 1)
	       && *p
	       && PJSTR_REG_EXEC(preg, (char *)p, (*(u **)s + *sz) - p, 1, &rm, eflags) == JSTR_REG_RET_NOERROR) {
		findlen = rm.rm_eo - rm.rm_so;
		p += rm.rm_so;
		if (jstr_unlikely(findlen == 0)) {
			JSTR_PRINT_LOG("jstr_unlikely(findlen == 0)");
			if (jstr_unlikely(*++p == '\0'))
				break;
			continue;
		}
		if (rplclen <= findlen) {
			JSTR_PRINT_LOG("rplclen <= findlen");
			pjstr_replaceall_in_place(&dst, &oldp, (const u **)&p, (u *)rplc, rplclen, findlen);
		} else if (*cap > *sz + rplclen - findlen) {
			JSTR_PRINT_LOG("*cap > *sz + rplclen - findlen");
			pjstr_reg_replaceall_small_rplc(*(u **)s, sz, &dst, &oldp, &p, (u *)rplc, rplclen, findlen);
		} else {
			JSTR_PRINT_LOG("else");
			if (jstr_unlikely(!pjstr_reg_replaceall_big_rplc((u **)s, sz, cap, &dst, &oldp, &p, (u *)rplc, rplclen, findlen)))
				goto err;
		}
	}
	if (dst != oldp)
		*sz = jstr_stpmove_len(dst, oldp, (*(u **)s + *sz) - oldp) - *s;
	return JSTR_REG_RET_NOERROR;
err:
	jstr_reg_free(preg);
	jstr_free(s, sz, cap);
	return JSTR_REG_RET_ESPACE;
}

JSTR_FUNC
static jstr_reg_errcode_ty
jstr_reg_replaceall_len_from(char *R *R s,
			     size_t *R sz,
			     size_t *R cap,
			     const size_t start_idx,
			     const char *R rplc,
			     const size_t rplclen,
			     regex_t *R preg,
			     const int eflags)
JSTR_NOEXCEPT
{
	return pjstr_reg_replaceall_len(s, sz, cap, rplc, rplclen, preg, eflags, 0, 0, start_idx);
}

JSTR_FUNC
JSTR_INLINE
static jstr_reg_errcode_ty
jstr_reg_replaceall_len_from_now(char *R *R s,
				 size_t *R sz,
				 size_t *R cap,
				 const size_t start_idx,
				 const char *R ptn,
				 const char *R rplc,
				 const size_t rplclen,
				 regex_t *R preg,
				 const int cflags,
				 const int eflags)
JSTR_NOEXCEPT
{
	PJSTR_REG_COMP_NOW();
	return jstr_reg_replaceall_len_from(s, sz, cap, start_idx, rplc, rplclen, preg, eflags);
}

JSTR_FUNC
static jstr_reg_errcode_ty
jstr_reg_replacen_len_from(char *R *R s,
			   size_t *R sz,
			   size_t *R cap,
			   const size_t start_idx,
			   const size_t n,
			   const char *R rplc,
			   const size_t rplclen,
			   regex_t *R preg,
			   const int eflags)
JSTR_NOEXCEPT
{
	return pjstr_reg_replaceall_len(s, sz, cap, rplc, rplclen, preg, eflags, 1, n, start_idx);
}

JSTR_INLINE
JSTR_FUNC
static jstr_reg_errcode_ty
jstr_reg_replaceall_len(char *R *R s,
			size_t *R sz,
			size_t *R cap,
			const char *R rplc,
			const size_t rplclen,
			regex_t *R preg,
			const int eflags)
JSTR_NOEXCEPT
{
	return pjstr_reg_replaceall_len(s, sz, cap, rplc, rplclen, preg, eflags, 0, 0, 0);
}

JSTR_INLINE
JSTR_FUNC
static jstr_reg_errcode_ty
jstr_reg_replacen_len(char *R *R s,
		      size_t *R sz,
		      size_t *R cap,
		      const char *R rplc,
		      const size_t n,
		      const size_t rplclen,
		      regex_t *R preg,
		      const int eflags)
JSTR_NOEXCEPT
{
	return pjstr_reg_replaceall_len(s, sz, cap, rplc, rplclen, preg, eflags, 1, n, 0);
}

JSTR_INLINE
JSTR_FUNC
static jstr_reg_errcode_ty
jstr_reg_replaceall_len_now(char *R *R s,
			    size_t *R sz,
			    size_t *R cap,
			    const char *R ptn,
			    const char *R rplc,
			    const size_t rplclen,
			    regex_t *R preg,
			    const int cflags,
			    const int eflags)
JSTR_NOEXCEPT
{
	PJSTR_REG_COMP_NOW();
	return jstr_reg_replaceall_len(s, sz, cap, rplc, rplclen, preg, eflags);
}

JSTR_INLINE
JSTR_FUNC
static jstr_reg_errcode_ty
jstr_reg_replacen_len_now(char *R *R s,
			  size_t *R sz,
			  size_t *R cap,
			  const char *R ptn,
			  const char *R rplc,
			  const size_t n,
			  const size_t rplclen,
			  regex_t *R preg,
			  const int cflags,
			  const int eflags)
JSTR_NOEXCEPT
{
	PJSTR_REG_COMP_NOW();
	return jstr_reg_replacen_len(s, sz, cap, rplc, n, rplclen, preg, eflags);
}

JSTR_INLINE
JSTR_FUNC
static jstr_reg_errcode_ty
pjstr_reg_replace_len(char *R *R s,
		      size_t *R sz,
		      size_t *R cap,
		      const char *R rplc,
		      const size_t rplclen,
		      const regex_t *R preg,
		      const int eflags,
		      const size_t start_idx)
JSTR_NOEXCEPT
{
	regmatch_t rm;
	const jstr_reg_errcode_ty ret = PJSTR_REG_EXEC(preg, *s + start_idx, *sz - start_idx, 1, &rm, eflags);
	if (jstr_unlikely(ret != JSTR_REG_RET_NOERROR)
	    || jstr_unlikely(rm.rm_eo == rm.rm_so))
		return ret;
	if (jstr_unlikely(!pjstr_replaceat_len_higher(s, sz, cap, rm.rm_so + start_idx, rplc, rplclen, rm.rm_eo - rm.rm_so)))
		return JSTR_REG_RET_ESPACE;
	return ret;
}

JSTR_INLINE
JSTR_FUNC
static jstr_reg_errcode_ty
jstr_reg_replace_len(char *R *R s,
		     size_t *R sz,
		     size_t *R cap,
		     const char *R rplc,
		     const size_t rplclen,
		     const regex_t *R preg,
		     const int eflags)
JSTR_NOEXCEPT
{
	return pjstr_reg_replace_len(s, sz, cap, rplc, rplclen, preg, eflags, 0);
}

JSTR_INLINE
JSTR_FUNC
static jstr_reg_errcode_ty
jstr_reg_replace_len_from(char *R *R s,
			  size_t *R sz,
			  size_t *R cap,
			  const size_t start_idx,
			  const char *R rplc,
			  const size_t rplclen,
			  const regex_t *R preg,
			  const int eflags)
JSTR_NOEXCEPT
{
	return pjstr_reg_replace_len(s, sz, cap, rplc, rplclen, preg, eflags, start_idx);
}

JSTR_INLINE
JSTR_FUNC
static jstr_reg_errcode_ty
jstr_reg_replace_len_from_now(char *R *R s,
			      size_t *R sz,
			      size_t *R cap,
			      const size_t start_idx,
			      const char *R ptn,
			      const char *R rplc,
			      const size_t rplclen,
			      regex_t *R preg,
			      const int cflags,
			      const int eflags)
JSTR_NOEXCEPT
{
	PJSTR_REG_COMP_NOW();
	return jstr_reg_replace_len_from(s, sz, cap, start_idx, rplc, rplclen, preg, eflags);
}

JSTR_FUNC
JSTR_INLINE
static jstr_reg_errcode_ty
jstr_reg_replace_len_now(char *R *R s,
			 size_t *R sz,
			 size_t *R cap,
			 const char *R ptn,
			 const char *R rplc,
			 const size_t rplclen,
			 regex_t *R preg,
			 const int cflags,
			 const int eflags)
JSTR_NOEXCEPT
{
	PJSTR_REG_COMP_NOW();
	return jstr_reg_replace_len(s, sz, cap, rplc, rplclen, preg, eflags);
}

JSTR_FUNC_VOID
JSTR_INLINE
static int
pjstr_reg_strlen_rplc_dst(const regmatch_t *R rm,
			  unsigned char *R rplc,
			  const size_t rplclen,
			  size_t *R rdstlen)
JSTR_NOEXCEPT
{
	typedef unsigned char u;
	const unsigned char *const rplc_e = rplc + rplclen;
	int has_bref = 0;
	for (*rdstlen = rplclen; (rplc = (u *)memchr(rplc, '\\', rplc_e - rplc)); ++rplc) {
		if (jstr_likely(jstr_isdigit(*++rplc)))
			*rdstlen += (rm[*rplc - '0'].rm_eo - rm[*rplc - '0'].rm_so) - 2;
		else if (jstr_unlikely(*rplc == '\0')) {
			has_bref = 1;
			break;
		}
	}
	return has_bref;
}

JSTR_FUNC_VOID
JSTR_INLINE
static void
pjstr_reg_creat_rplc_bref(const unsigned char *R mtc,
			  const regmatch_t *R rm,
			  unsigned char *R rdst,
			  unsigned char *R rplc,
			  const size_t rplclen)
JSTR_NOEXCEPT
{
	typedef unsigned char u;
	const unsigned char *rold;
	const unsigned char *const rplc_e = rplc + rplclen;
	for (;; ++rplc) {
		rold = rplc;
		rplc = (u *)memchr(rplc, '\\', rplc_e - rplc);
		if (jstr_likely(jstr_isdigit(*++rplc))) {
			if (jstr_likely(rplc != rold)) {
				rdst = (u *)jstr_mempmove(rdst, rold, (rplc - 1) - rold);
				rplc += (rplc - 1) - rold - 1;
			}
			rdst = (u *)jstr_mempcpy(rdst, mtc + rm[*rplc - '0'].rm_so, rm[*rplc - '0'].rm_eo - rm[*rplc - '0'].rm_so);
		} else if (jstr_unlikely(*rplc == '\0')) {
			break;
		} else {
			rdst[0] = rplc[-1];
			rdst[1] = rplc[0];
			rdst += 2;
		}
	}
	if (jstr_unlikely(rplc == NULL))
		memcpy(rdst, rold, rplc_e - rold);
}

JSTR_FUNC
JSTR_INLINE
static jstr_reg_errcode_ty
pjstr_reg_replace_bref_len(char *R *R s,
			   size_t *R sz,
			   size_t *R cap,
			   const char *R rplc,
			   size_t rplclen,
			   regex_t *R preg,
			   const int eflags,
			   const size_t nmatch,
			   const size_t start_idx)
JSTR_NOEXCEPT
{
	if (jstr_unlikely(rplclen == 0))
		return jstr_reg_remove_from(*s, sz, start_idx, preg, eflags);
	regmatch_t rm[10];
	jstr_reg_errcode_ty ret = PJSTR_REG_EXEC(preg, *s + start_idx, *sz - start_idx, nmatch, rm, eflags);
	const size_t findlen = rm[0].rm_eo - rm[0].rm_so;
	if (jstr_unlikely(ret != JSTR_REG_RET_NOERROR)
	    || jstr_unlikely(findlen == 0))
		return ret;
	rm[0].rm_so += start_idx;
	rm[0].rm_eo += start_idx;
	typedef unsigned char u;
	size_t rdstlen = rplclen;
	const int has_bref = pjstr_reg_strlen_rplc_dst(rm, (u *)rplc, rplclen, &rdstlen);
	if (jstr_unlikely(has_bref == 0))
		return jstr_reg_replace_len_from(s, sz, cap, start_idx, rplc, rplclen, preg, eflags);
	unsigned char *rdst;
	unsigned char rdst_stack[JSTR_PAGE_SIZE];
	if (jstr_unlikely(rdstlen > JSTR_PAGE_SIZE)) {
		rdst = (u *)malloc(rdstlen);
		PJSTR_MALLOC_ERR(rdst, goto err);
	} else {
		rdst = rdst_stack;
	}
	pjstr_reg_creat_rplc_bref(*(u **)s, rm, (u *)rdst, (u *)rplc, rplclen);
	if (jstr_unlikely(!pjstr_replaceat_len_higher(s, sz, cap, rm[0].rm_so, (char *)rdst, rdstlen, findlen)))
		ret = JSTR_REG_RET_ESPACE;
	if (rdst != rdst_stack)
		free(rdst);
	return ret;
err:
	jstr_reg_free(preg);
	jstr_free(s, sz, cap);
	return JSTR_REG_RET_ESPACE;
}

JSTR_FUNC
static jstr_reg_errcode_ty
jstr_reg_replace_bref_len(char *R *R s,
			  size_t *R sz,
			  size_t *R cap,
			  const char *R rplc,
			  size_t rplclen,
			  regex_t *R preg,
			  const int eflags,
			  const size_t nmatch)
JSTR_NOEXCEPT
{
	return pjstr_reg_replace_bref_len(s, sz, cap, rplc, rplclen, preg, eflags, nmatch, 0);
}

JSTR_FUNC
static jstr_reg_errcode_ty
jstr_reg_replace_bref_len_from(char *R *R s,
			       size_t *R sz,
			       size_t *R cap,
			       const size_t start_idx,
			       const char *R rplc,
			       size_t rplclen,
			       regex_t *R preg,
			       const int eflags,
			       const size_t nmatch)
JSTR_NOEXCEPT
{
	return pjstr_reg_replace_bref_len(s, sz, cap, rplc, rplclen, preg, eflags, nmatch, start_idx);
}

JSTR_INLINE
JSTR_FUNC
static jstr_reg_errcode_ty
pjstr_reg_replaceall_bref_len(char *R *R s,
			      size_t *R sz,
			      size_t *R cap,
			      const char *R rplc,
			      size_t rplclen,
			      regex_t *R preg,
			      const int eflags,
			      const size_t nmatch,
			      const int use_n,
			      size_t n,
			      const size_t start_idx)
JSTR_NOEXCEPT
{
	typedef unsigned char u;
	unsigned char *p = *(u **)s + start_idx;
	if (jstr_unlikely(rplclen == 0))
		return pjstr_reg_removeall(*s, sz, preg, eflags, use_n, n, start_idx);
	unsigned char *dst = p;
	const unsigned char *oldp = p;
	regmatch_t rm[10];
	size_t rdstlen;
	size_t rdstcap = 0;
	unsigned char rdst_stack[JSTR_PAGE_SIZE];
	unsigned char *rdstp = rdst_stack;
	unsigned char *rdst_heap = NULL;
	size_t findlen;
	int has_bref;
	while ((use_n ? n-- : 1)
	       && *p
	       && PJSTR_REG_EXEC(preg, (char *)p, (*(u **)s + *sz) - p, nmatch, rm, eflags) == JSTR_REG_RET_NOERROR) {
		findlen = rm[0].rm_eo - rm[0].rm_so;
		if (jstr_unlikely(findlen == 0)) {
			++p;
			continue;
		}
		has_bref = pjstr_reg_strlen_rplc_dst(rm, (u *)rplc, rplclen, &rdstlen);
		if (jstr_unlikely(has_bref == 0))
			return jstr_reg_replaceall_len_from(s, sz, cap, start_idx, rplc, rplclen, preg, eflags);
		if (jstr_unlikely(rdstlen > JSTR_PAGE_SIZE)) {
			if (jstr_unlikely(rdst_heap == NULL)) {
				rdstcap = JSTR_PTR_ALIGN_UP(rdstlen, PJSTR_MALLOC_ALIGNMENT);
				rdst_heap = (u *)malloc(rdstcap);
				PJSTR_MALLOC_ERR(rdst_heap, goto err);
				rdstp = rdst_heap;
			} else if (rdstcap < rdstlen) {
				PJSTR_REALLOC(rdst_heap, rdstcap, rdstlen, goto err);
				rdstp = rdst_heap;
			}
		}
		pjstr_reg_creat_rplc_bref((u *)p, rm, (u *)rdstp, (u *)rplc, rplclen);
		p += rm[0].rm_so;
		if (rdstlen <= findlen)
			pjstr_replaceall_in_place(&dst, &oldp, (const u **)&p, rdstp, rdstlen, findlen);
		else if (*cap > *sz + rdstlen - findlen)
			pjstr_reg_replaceall_small_rplc(*(u **)s, sz, &dst, &oldp, &p, rdstp, rdstlen, findlen);
		else if (jstr_unlikely(!pjstr_reg_replaceall_big_rplc((u **)s, sz, cap, &dst, &oldp, &p, rdstp, rdstlen, findlen)))
			goto err_free;
	}
	if (dst != oldp)
		*sz = jstr_stpmove_len(dst, oldp, (*(u **)s + *sz) - oldp) - *s;
	free(rdst_heap);
	return JSTR_REG_RET_NOERROR;
err_free:
	free(rdst_heap);
err:
	jstr_reg_free(preg);
	jstr_free(s, sz, cap);
	return JSTR_REG_RET_ESPACE;
}

JSTR_FUNC
static jstr_reg_errcode_ty
jstr_reg_replaceall_bref_len(char *R *R s,
			     size_t *R sz,
			     size_t *R cap,
			     const char *R rplc,
			     size_t rplclen,
			     regex_t *R preg,
			     const int eflags,
			     const size_t nmatch)
JSTR_NOEXCEPT
{
	return pjstr_reg_replaceall_bref_len(s, sz, cap, rplc, rplclen, preg, eflags, nmatch, 0, 0, 0);
}

JSTR_FUNC
static jstr_reg_errcode_ty
jstr_reg_replacen_bref_len(char *R *R s,
			   size_t *R sz,
			   size_t *R cap,
			   const char *R rplc,
			   size_t n,
			   size_t rplclen,
			   regex_t *R preg,
			   const int eflags,
			   const size_t nmatch)
JSTR_NOEXCEPT
{
	return pjstr_reg_replaceall_bref_len(s, sz, cap, rplc, rplclen, preg, eflags, nmatch, 1, n, 0);
}

JSTR_FUNC
static jstr_reg_errcode_ty
jstr_reg_replaceall_bref_len_from(char *R *R s,
				  size_t *R sz,
				  size_t *R cap,
				  const size_t start_idx,
				  const char *R rplc,
				  size_t rplclen,
				  regex_t *R preg,
				  const int eflags,
				  const size_t nmatch)
JSTR_NOEXCEPT
{
	return pjstr_reg_replaceall_bref_len(s, sz, cap, rplc, rplclen, preg, eflags, nmatch, 0, 0, start_idx);
}

JSTR_FUNC
static jstr_reg_errcode_ty
jstr_reg_replacen_bref_len_from(char *R *R s,
				size_t *R sz,
				size_t *R cap,
				const size_t start_idx,
				const char *R rplc,
				size_t n,
				size_t rplclen,
				regex_t *R preg,
				const int eflags,
				const size_t nmatch)
JSTR_NOEXCEPT
{
	return pjstr_reg_replaceall_bref_len(s, sz, cap, rplc, rplclen, preg, eflags, nmatch, 1, n, start_idx);
}

JSTR_FUNC
static jstr_reg_errcode_ty
jstr_reg_replacen_bref_len_from_now(char *R *R s,
				    size_t *R sz,
				    size_t *R cap,
				    const size_t start_idx,
				    const char *R ptn,
				    const char *R rplc,
				    size_t n,
				    size_t rplclen,
				    regex_t *R preg,
				    const int cflags,
				    const int eflags,
				    const size_t nmatch)
JSTR_NOEXCEPT
{
	PJSTR_REG_COMP_NOW();
	return pjstr_reg_replaceall_bref_len(s, sz, cap, rplc, rplclen, preg, eflags, nmatch, 0, n, start_idx);
}

JSTR_INLINE
JSTR_FUNC
static jstr_reg_errcode_ty
jstr_reg_replacen_bref_len_now(char *R *R s,
			       size_t *R sz,
			       size_t *R cap,
			       const char *R ptn,
			       const char *R rplc,
			       size_t n,
			       size_t rplclen,
			       regex_t *R preg,
			       const int cflags,
			       const int eflags,
			       const size_t nmatch)
JSTR_NOEXCEPT
{
	PJSTR_REG_COMP_NOW();
	return jstr_reg_replacen_bref_len(s, sz, cap, rplc, n, rplclen, preg, eflags, nmatch);
}

JSTR_INLINE
JSTR_FUNC
static jstr_reg_errcode_ty
jstr_reg_replaceall_bref_len_now(char *R *R s,
				 size_t *R sz,
				 size_t *R cap,
				 const char *R ptn,
				 const char *R rplc,
				 size_t rplclen,
				 regex_t *R preg,
				 const int cflags,
				 const int eflags,
				 const size_t nmatch)
JSTR_NOEXCEPT
{
	PJSTR_REG_COMP_NOW();
	return jstr_reg_replaceall_bref_len(s, sz, cap, rplc, rplclen, preg, eflags, nmatch);
}

JSTR_INLINE
JSTR_FUNC
static jstr_reg_errcode_ty
jstr_reg_replace_bref_len_now(char *R *R s,
			      size_t *R sz,
			      size_t *R cap,
			      const char *R ptn,
			      const char *R rplc,
			      size_t rplclen,
			      regex_t *R preg,
			      const int cflags,
			      const int eflags,
			      const size_t nmatch)
JSTR_NOEXCEPT
{
	PJSTR_REG_COMP_NOW();
	return jstr_reg_replace_bref_len(s, sz, cap, rplc, rplclen, preg, eflags, nmatch);
}

JSTR_INLINE
JSTR_FUNC
static jstr_reg_errcode_ty
jstr_reg_replace_bref_len_from_now(char *R *R s,
				   size_t *R sz,
				   size_t *R cap,
				   const size_t start_idx,
				   const char *R ptn,
				   const char *R rplc,
				   size_t rplclen,
				   regex_t *R preg,
				   const int cflags,
				   const int eflags,
				   const size_t nmatch)
JSTR_NOEXCEPT
{
	PJSTR_REG_COMP_NOW();
	return jstr_reg_replace_bref_len_from(s, sz, cap, start_idx, rplc, rplclen, preg, eflags, nmatch);
}

PJSTR_END_DECLS

#undef R

#undef PJSTR_REG_EXEC
#undef JSTR_PRINT_LOG
#undef JSTR_REG_DEBUG

#undef PJSTR_REG_NOERR

#undef PJSTR_REG_COMP_NOW

#endif /* JSTR_REGEX_H */
