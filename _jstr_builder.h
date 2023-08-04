#ifndef JSTR_BUILDER_H_DEF
#define JSTR_BUILDER_H_DEF

#ifdef __cplusplus
extern "C" {
#endif /* __cpluslus */
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef __cplusplus
}
#	include <utility>
#endif /* __cpluslus */

#include "_jstr_config.h"
#include "_jstr_macros.h"
#include "_jstr_templates.h"

#ifdef __cplusplus
#	include "_jstr_traits.h"
#endif /* __cpluslus */

#define JSTR_EXTERN_C  1
#define JSTR_NAMESPACE 0

#if JSTR_NAMESPACE && defined(__cplusplus)
namespace jstr {
#endif /* JSTR_NAMESPACE */
#if JSTR_EXTERN_C && defined(__cplusplus)
/* extern "C" { */
#endif /* JSTR_EXTERN_C */

/*
  exit(1) if ptr is NULL.
*/
JSTR_INLINE
static void jstr_err(char *JSTR_RST const p) JSTR_NOEXCEPT
{
	if (unlikely(!p))
		JSTR_ERR();
}

typedef struct jstr_t {
	size_t size;
	size_t cap;
	char *data;

#ifndef __cplusplus

} jstr_t;

#else

	template <typename Str,
		  typename... StrArgs>
	JSTR_INLINE
	JSTR_NONNULL_ALL
	jstr_t(Str &&arg,
	       StrArgs &&...args)
	{
		jstr::alloc_cat(&this->data, &this->size, &this->cap, std::forward<Str>(arg), std::forward<StrArgs>(args)...);
	}

	JSTR_INLINE
	JSTR_NONNULL_ALL
	jstr_t(const size_t len) JSTR_NOEXCEPT
	{
		jstr_alloc(&this->data, &this->size, &this->cap, len);
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
	       const size_t srclen) JSTR_NOEXCEPT
	{
		jstr_alloc_append_mem(&this->data, &this->size, &this->cap, src, srclen);
	}

#	if JSTR_FREE_ON_DESTRUCTOR_CPP

	JSTR_INLINE
	~jstr_t(void) JSTR_NOEXCEPT
	{
		free(this->data);
#		if JSTR_NULLIFY_PTR_ON_DESTRUCTOR_CPP
		this->data = NULL;
#		endif /* JSTR_NULLIFY_PTR_ON_DESTRUCTOR_CPP */
	}

#	endif /* JSTR_FREE_ON_DESTRUCTOR_CPP */

	/*
	  free(STR) and set STR to NULL.
	*/
	JSTR_INLINE
	void del(void) JSTR_NOEXCEPT
	{
		free(this->data);
#	if JSTR_NULLIFY_PTR_ON_DELETE
		this->data = NULL;
#	endif /* JSTR_NULLIFY_PTR_ON_DELETE */
	}

	/*
	  Execute JSTR_ERR().
	*/
	JSTR_INLINE
	void err(void) JSTR_NOEXCEPT
	{
		if (unlikely(!this->data))
			JSTR_ERR();
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
		fprintf(stderr, "data puts():%s\n", this->data);
		fputs("data:", stderr);
		fwrite(this->data, 1, this->size, stderr);
		fputc('\n', stderr);
	}

    private:
#endif /* __cpluslus */

JSTR_INLINE
JSTR_NONNULL_ALL
static void jstr_alloc(char **JSTR_RST const s,
		       size_t *JSTR_RST const sz,
		       size_t *JSTR_RST const cap,
		       const size_t top) JSTR_NOEXCEPT
{
	*sz = 0;
	*s = (char *)malloc(top * JSTR_GROWTH_MULTIPLIER);
	JSTR_MALLOC_ERR(*s, return);
	*cap = top * JSTR_GROWTH_MULTIPLIER;
}

JSTR_INLINE
JSTR_NONNULL_ALL
static void jstr_alloc_append_mem(char **JSTR_RST const s,
				  size_t *JSTR_RST const sz,
				  size_t *JSTR_RST const cap,
				  const char *JSTR_RST const src,
				  const size_t srclen) JSTR_NOEXCEPT
{
	jstr_alloc(s, sz, cap, srclen * JSTR_GROWTH_MULTIPLIER);
	if (unlikely(!*s))
		return;
	*sz = srclen;
	memcpy(*s, src, srclen + 1);
}

JSTR_INLINE
JSTR_NONNULL_ALL
static void jstr_alloc_append(char **JSTR_RST const s,
			      size_t *JSTR_RST const sz,
			      size_t *JSTR_RST const cap,
			      const char *JSTR_RST const src) JSTR_NOEXCEPT
{
	jstr_alloc_append_mem(s, sz, cap, src, strlen(src));
}

#ifdef __cplusplus
}
jstr_t;
#endif /* __cpluslus */

#ifdef __cplusplus

JSTR_INLINE
JSTR_NONNULL_ALL
static void jstr_alloc(char **JSTR_RST const s,
		       size_t *JSTR_RST const sz,
		       size_t *JSTR_RST const cap,
		       const size_t top) JSTR_NOEXCEPT
{
	*sz = 0;
	*s = (char *)malloc(top * JSTR_GROWTH_MULTIPLIER);
	JSTR_MALLOC_ERR(*s, return);
	*cap = top * JSTR_GROWTH_MULTIPLIER;
}

JSTR_INLINE
JSTR_NONNULL_ALL
static void jstr_alloc_append_mem(char **JSTR_RST const s,
				  size_t *JSTR_RST const sz,
				  size_t *JSTR_RST const cap,
				  const char *JSTR_RST const src,
				  const size_t srclen) JSTR_NOEXCEPT
{
	jstr_alloc(s, sz, cap, srclen * JSTR_GROWTH_MULTIPLIER);
	if (unlikely(!*s))
		return;
	*sz = srclen;
	memcpy(*s, src, srclen + 1);
}

JSTR_INLINE
JSTR_NONNULL_ALL
static void jstr_alloc_append(char **JSTR_RST const s,
			      size_t *JSTR_RST const sz,
			      size_t *JSTR_RST const cap,
			      const char *JSTR_RST const src) JSTR_NOEXCEPT
{
	jstr_alloc_append_mem(s, sz, cap, src, strlen(src));
}

#else

#endif /* __cpluslus */

/*
  free(p) and set p to NULL.
*/
JSTR_INLINE
JSTR_NONNULL_ALL
static void jstr_del(char *JSTR_RST p) JSTR_NOEXCEPT
{
	free(p);
#if JSTR_NULLIFY_PTR_ON_DELETE
	p = NULL;
#endif /* JSTR_NULLIFY_PTR_ON_DELETE */
}

JSTR_INLINE
JSTR_NONNULL_ALL
static void jstr_debug(const jstr_t *JSTR_RST const j)
{
	fprintf(stderr, "size:%zu\ncap:%zu\n", j->size, j->cap);
	fprintf(stderr, "data:%s\n", j->data);
}

#ifndef __cplusplus

/*
   Insert multiple strings to S.
   The last argument MUST be NULL.
*/
JSTR_MAYBE_UNUSED
JSTR_SENTINEL
inline static void jstr_alloc_cat(char **JSTR_RST const s,
				  size_t *JSTR_RST const sz,
				  size_t *JSTR_RST const cap,
				  ...) JSTR_NOEXCEPT
{
	char *JSTR_RST arg;
	*sz = 0;
	va_list ap;
	va_start(ap, cap);
	while ((arg = va_arg(ap, char *)))
		*sz += strlen(arg);
	va_end(ap);
	if (unlikely(!*sz)) {
		*sz = 0;
		*cap = JSTR_MIN_CAP;
		**s = '\0';
		*s = (char *)malloc(JSTR_MIN_CAP);
		JSTR_MALLOC_ERR(*s, return);
		return;
	}
	*cap = *sz * 2;
	*s = (char *)malloc(*cap);
	JSTR_MALLOC_ERR(*s, return);
	char *sp = *s + *sz;
	va_start(ap, cap);
	while ((arg = va_arg(ap, char *)))
#	ifdef JSTR_HAS_STPCPY
		sp = stpcpy(sp, arg);
#	else
		while (*arg)
			*sp++ = *arg++;
#	endif
	va_end(ap);
	*sp = '\0';
}

/*
   Insert multiple strings to S.
   The last argument MUST be NULL.
*/
JSTR_MAYBE_UNUSED
JSTR_SENTINEL
inline static void jstr_alloc_cat_j(jstr_t *JSTR_RST const j,
				    ...) JSTR_NOEXCEPT
{
	char *JSTR_RST arg;
	j->size = 0;
	va_list ap;
	va_start(ap, j);
	while ((arg = va_arg(ap, char *)))
		j->size += strlen(arg);
	va_end(ap);
	if (unlikely(!j->size)) {
		j->size = 0;
		j->cap = JSTR_MIN_CAP;
		*j->data = '\0';
		j->data = (char *)malloc(JSTR_MIN_CAP);
		JSTR_MALLOC_ERR(j->data, return);
		return;
	}
	j->cap = j->size * 2;
	j->data = (char *)malloc(j->cap);
	JSTR_MALLOC_ERR(j->data, return);
	char *sp = j->data + j->size;
	va_start(ap, j);
	while ((arg = va_arg(ap, char *)))
#	ifdef JSTR_HAS_STPCPY
		sp = stpcpy(sp, arg);
#	else
		while (*arg)
			*sp++ = *arg++;
#	endif
	va_end(ap);
	*sp = '\0';
}

#else

#	if JSTR_EXTERN_C && defined(__cplusplus)
/* } */
#	endif /* JSTR_NAMESPACE */
#	if JSTR_NAMESPACE && defined(__cplusplus)
}
#	endif /* JSTR_EXTERN_C */

namespace jstr
{

	namespace priv {

	JSTR_INLINE
	JSTR_NONNULL_ALL
	static void cat_assign(char **JSTR_RST const dst,
			       jstr_t *JSTR_RST const src) JSTR_NOEXCEPT
	{
		memcpy(*dst, src->data, src->size);
		*dst += src->size;
	}

	} /* namespace priv */

	/*
	  Insert multiple strings to S.
	*/
	template <typename Str,
		  typename... StrArgs>
	JSTR_INLINE
	JSTR_NONNULL_ALL static void
	alloc_cat_j(jstr_t * j,
		    Str && arg,
		    StrArgs && ...args) JSTR_NOEXCEPT
	{
		alloc_cat(&j->data, &j->size, &j->cap, std::forward<Str>(arg), std::forward<StrArgs>(args)...);
	}

	/*
	  Append multiple strings to end of S.
	*/
	template <typename Str,
		  typename... StrArgs>
	JSTR_INLINE
	JSTR_NONNULL_ALL static void
	cat_j(jstr_t * j,
	      Str && arg,
	      StrArgs && ...args) JSTR_NOEXCEPT
	{
		cat(&j->data, &j->size, &j->cap, std::forward<Str>(arg), std::forward<StrArgs>(args)...);
	}

} /* namespace jstr */

#endif /* __cpluslus */

#endif /* JSTR_BUILDER H_DEF */
