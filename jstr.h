#ifndef JSTR_H_DEF__
#define JSTR_H_DEF__

#ifdef __cplusplus
#	define JSTR_NOEXCEPT__ noexcept
#else
#	define JSTR_NOEXCEPT__
#endif

#define JSTR_ALIGN_POWER_OF_TWO
#define JSTR_64_BIT

#if defined(__GNUC__) || defined(__GLIBC__)
#	ifndef _GNU_SOURCE
#		define _GNU_SOURCE
#	endif
#endif

#ifndef __cplusplus
#	include <stdarg.h>
#	include <stdlib.h>
#	include <string.h>
#endif // ! __cplusplus

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
#	include "macros.h"
#	include <sys/cdefs.h>
#	include "/home/james/c/pp_macros/pp_va_args_macros.h"
#ifdef __cplusplus
}
#	include <type_traits>
#	include <cstring>
#	include <cstdarg>
#	include <cstdlib>
#	include <utility>
#endif // __cplusplus

#define JSTR_MIN_CAP 8
#define JSTR_MULTIPLIER 2

#ifdef __cplusplus
#	define JSTR_CAST(T) (T)
#else
#	define JSTR_CAST(T)
#endif // __cplusplus

typedef struct jstring_t jstring_t;

#ifdef __cplusplus

void private_jstr_constructor_cap(jstring_t *RESTRICT this_, const std::size_t cap, const char *RESTRICT s, const std::size_t slen) JSTR_NOEXCEPT__;
void private_jstr_new_append_void(jstring_t *RESTRICT this_, const size_t srclen, const char *RESTRICT const src_, ...) JSTR_NOEXCEPT__;
void private_jstr_new_alloc_void(jstring_t *RESTRICT this_, const size_t size) JSTR_NOEXCEPT__;

extern "C" {
#endif // __cplusplus

int private_jstr_cat(jstring_t *RESTRICT this_, const size_t len, ...) JSTR_NOEXCEPT__ JSTR_WARN_UNUSED__;
int private_jstr_cat_s(jstring_t *RESTRICT this_, const size_t len, ...) JSTR_NOEXCEPT__ JSTR_WARN_UNUSED__;

void jstr_delete(jstring_t *RESTRICT this_) JSTR_NOEXCEPT__;

void jstr_init(jstring_t *RESTRICT this_) JSTR_NOEXCEPT__;

int jstr_new_alloc(jstring_t *RESTRICT this_, const size_t size) JSTR_NOEXCEPT__ JSTR_WARN_UNUSED__;
int jstr_new_append(jstring_t *RESTRICT this_, const size_t srclen, const char *RESTRICT const src_, ...) JSTR_NOEXCEPT__ JSTR_WARN_UNUSED__;
int private_jstr_new_cat(jstring_t *RESTRICT this_, const size_t arglen, ...) JSTR_NOEXCEPT__ JSTR_WARN_UNUSED__;

void jstr_push_back_noalloc(jstring_t *this_, const char c) JSTR_NOEXCEPT__;
int jstr_push_back_nocheck(jstring_t *this_, const char c) JSTR_NOEXCEPT__ JSTR_WARN_UNUSED__;
int jstr_push_back(jstring_t *this_, const char c) JSTR_NOEXCEPT__ JSTR_WARN_UNUSED__;
int jstr_push_back_s(jstring_t *this_, const char c) JSTR_NOEXCEPT__ JSTR_WARN_UNUSED__;

void jstr_push_front_noalloc(jstring_t *RESTRICT this_, const char c) JSTR_NOEXCEPT__;
int jstr_push_front_nocheck(jstring_t *RESTRICT this_, const char c) JSTR_NOEXCEPT__ JSTR_WARN_UNUSED__;
int jstr_push_front(jstring_t *RESTRICT this_, const char c) JSTR_NOEXCEPT__ JSTR_WARN_UNUSED__;
int jstr_push_front_s(jstring_t *RESTRICT this_, const char c) JSTR_NOEXCEPT__ JSTR_WARN_UNUSED__;

void jstr_pop_back(jstring_t *RESTRICT this_) JSTR_NOEXCEPT__;
void jstr_pop_back_s(jstring_t *RESTRICT this_) JSTR_NOEXCEPT__;

void jstr_pop_front(jstring_t *RESTRICT this_) JSTR_NOEXCEPT__;
void jstr_pop_front_s(jstring_t *RESTRICT this_) JSTR_NOEXCEPT__;

void private_jstr_append_noalloc(jstring_t *dest, const char *RESTRICT src, const size_t srclen) JSTR_NOEXCEPT__;
int private_jstr_append(jstring_t *dest, const char *RESTRICT src, const size_t srclen) JSTR_NOEXCEPT__ JSTR_WARN_UNUSED__;
int private_jstr_append_s(jstring_t *dest, const char *RESTRICT src, const size_t srclen) JSTR_NOEXCEPT__ JSTR_WARN_UNUSED__;

void jstr_swap(jstring_t *RESTRICT this_, jstring_t *RESTRICT other_) JSTR_NOEXCEPT__;
void jstr_swap_str(jstring_t *RESTRICT this_, char **RESTRICT other_, size_t *otherlen, size_t *other_cap) JSTR_NOEXCEPT__;

int jstr_shrink_to_fit_nocheck(jstring_t *RESTRICT this_) JSTR_NOEXCEPT__ JSTR_WARN_UNUSED__;
int jstr_shrink_to_fit(jstring_t *RESTRICT this_) JSTR_NOEXCEPT__ JSTR_WARN_UNUSED__;

int jstr_shrink_to_nocheck(jstring_t *RESTRICT this_, const size_t cap) JSTR_NOEXCEPT__ JSTR_WARN_UNUSED__;
int jstr_shrink_to(jstring_t *RESTRICT this_, const size_t cap) JSTR_NOEXCEPT__ JSTR_WARN_UNUSED__;

void jstr_shrink_to_size_nocheck(jstring_t *RESTRICT this_, const size_t size) JSTR_NOEXCEPT__;
void jstr_shrink_to_size(jstring_t *RESTRICT this_, const size_t size) JSTR_NOEXCEPT__;
void jstr_shrink_to_size_s(jstring_t *RESTRICT this_, const size_t size) JSTR_NOEXCEPT__;

int jstr_reserve(jstring_t *RESTRICT this_, const size_t cap) JSTR_NOEXCEPT__ JSTR_WARN_UNUSED__;
int jstr_reserve_s(jstring_t *RESTRICT this_, const size_t cap) JSTR_NOEXCEPT__ JSTR_WARN_UNUSED__;
int jstr_reserve_nocheck(jstring_t *RESTRICT this_, const size_t cap, ...) JSTR_NOEXCEPT__ JSTR_WARN_UNUSED__;

int private_jstr_replace(jstring_t *RESTRICT dest, const char *RESTRICT src, const size_t srclen, ...) JSTR_NOEXCEPT__ JSTR_WARN_UNUSED__;
void jstr_replace_noalloc(jstring_t *RESTRICT dest, const char *RESTRICT src, const size_t srclen) JSTR_NOEXCEPT__;
int jstr_replace_nocheck(jstring_t *RESTRICT dest, const char *RESTRICT src, const size_t srclen) JSTR_NOEXCEPT__ JSTR_WARN_UNUSED__;

void jstr_replace_jstr_noalloc(jstring_t *RESTRICT dest, const jstring_t *RESTRICT const src) JSTR_NOEXCEPT__;
int jstr_replace_jstr_nocheck(jstring_t *RESTRICT dest, const jstring_t *RESTRICT const src) JSTR_NOEXCEPT__ JSTR_WARN_UNUSED__;
int jstr_replace_jstr(jstring_t *RESTRICT dest, const jstring_t *RESTRICT const src, ...) JSTR_NOEXCEPT__ JSTR_WARN_UNUSED__;

/* if equals returns 0 */
int jstr_cmp(const jstring_t *RESTRICT const this_, const jstring_t *RESTRICT const other_) JSTR_NOEXCEPT__ JSTR_WARN_UNUSED__;
int jstr_cmp_nocheck(const jstring_t *RESTRICT const this_, const jstring_t *RESTRICT const other) JSTR_NOEXCEPT__ JSTR_WARN_UNUSED__;
int jstr_cmp_str(const jstring_t *RESTRICT const this_, const char *RESTRICT const s, const size_t slen) JSTR_NOEXCEPT__ JSTR_WARN_UNUSED__;

#ifdef __USE_GNU
int jstr_case_cmp_nocheck(const jstring_t *RESTRICT const this_, const jstring_t *RESTRICT const other) JSTR_NOEXCEPT__ JSTR_WARN_UNUSED__;
int jstr_case_cmp(const jstring_t *RESTRICT const this_, const jstring_t *RESTRICT const other) JSTR_NOEXCEPT__ JSTR_WARN_UNUSED__;

int jstr_case_cmp_str(const jstring_t *RESTRICT const this_, const char *RESTRICT const s) JSTR_NOEXCEPT__ JSTR_WARN_UNUSED__;
#endif // __USE_GNU

/* finds first occurence of character from end of string */
char *jstr_rchr(const jstring_t *RESTRICT const this_, int c) JSTR_NOEXCEPT__ JSTR_WARN_UNUSED__;
/* memchr */
char *jstr_chr(const jstring_t *RESTRICT const this_, int c) JSTR_NOEXCEPT__ JSTR_WARN_UNUSED__;

#ifdef __USE_GNU
char *private_jstr_str(jstring_t *haystack, const char *RESTRICT const needle, size_t needlelen, ...) JSTR_NOEXCEPT__ JSTR_WARN_UNUSED__;

#	define jstr_str(this_, ...)                                                                             \
		(PP_NARG(__VA_ARGS__) == 1)                                                                     \
			? private_jstr_str(this_, PP_FIRST_ARG(__VA_ARGS__), strlen(PP_FIRST_ARG(__VA_ARGS__))) \
			: private_jstr_str(this_, __VA_ARGS__, 0)

#else

char *jstr_str(jstring_t *haystack, const char *RESTRICT needle) JSTR_NOEXCEPT__ JSTR_WARN_UNUSED__;

#endif

int jstr_dup(jstring_t *RESTRICT this_, jstring_t *RESTRICT other) JSTR_NOEXCEPT__ JSTR_WARN_UNUSED__;
int jstr_dup_s(jstring_t *RESTRICT this_, jstring_t *RESTRICT other) JSTR_NOEXCEPT__ JSTR_WARN_UNUSED__;

int jstr_ndup(jstring_t *RESTRICT this_, jstring_t *RESTRICT other, const size_t n) JSTR_NOEXCEPT__ JSTR_WARN_UNUSED__;
int jstr_ndup_s(jstring_t *RESTRICT this_, jstring_t *RESTRICT other, const size_t n) JSTR_NOEXCEPT__ JSTR_WARN_UNUSED__;

void jstr_rev(jstring_t *RESTRICT this_) JSTR_NOEXCEPT__;

int jstr_rev_dup(jstring_t *RESTRICT src, char **RESTRICT dest) JSTR_NOEXCEPT__ JSTR_WARN_UNUSED__;

#ifdef __cplusplus
}
#endif // __cplusplus

typedef struct jstring_t {
#ifndef __cplusplus
	size_t size; 
	size_t capacity;
	char *data;
} jstring_t;
#else
	std::size_t size;
	std::size_t capacity;
	char *data;
	ALWAYS_INLINE
	jstring_t() JSTR_NOEXCEPT__
	{
		jstr_init(this);
	}

	ALWAYS_INLINE
	~jstring_t() JSTR_NOEXCEPT__
	{
		free(this->data);
		this->data = nullptr;
		this->capacity = 0;
		this->size = 0;
	}

	template <std::size_t N>
	ALWAYS_INLINE JSTR_WARN_UNUSED__
	constexpr std::size_t strlen(const char (&s)[N]) JSTR_NOEXCEPT__
	{
		return N - 1;
	}

#	if __cplusplus >= 201703L

	template <typename T, typename... Args>
	ALWAYS_INLINE JSTR_WARN_UNUSED__
	size_t strlen_args(T arg, Args&&... args) JSTR_NOEXCEPT__
	{
		return (std::strlen(arg) + ... + std::strlen(args));
	}

#	else

	template <typename T, typename... Args>
	ALWAYS_INLINE JSTR_WARN_UNUSED__
	size_t strlen_args(T s, Args... args) JSTR_NOEXCEPT__
	{
		return std::strlen(s) + strlen_args(args...);
	}

#	endif // __cplusplus 17

#	if __cplusplus >= 201103L

	template <typename T, typename... Args>
	ALWAYS_INLINE
	jstring_t(T arg1, T arg2, Args&&... args) JSTR_NOEXCEPT__
	{
		static_assert(std::is_same<std::decay_t<T>, const char *>::value, "Args passed must be a C string!");
		const size_t arglen = strlen_args(arg1, arg2, std::forward<Args>(args)...);
		if (unlikely(!alloc(arglen)))
			return;
		char *tmp = this->data;
		while (*arg1)
			*tmp++ = *arg1++;
		while (*arg2)
			*tmp++ = *arg2++;
		for (const auto&& arg : { std::forward<Args>(args)... })
			while (*arg)
				*tmp++ = *arg++;
		*tmp = '\0';
		this->size = arglen;
	}

	template <typename T, typename... Args>
	ALWAYS_INLINE JSTR_WARN_UNUSED__
	int cat(T arg1, T arg2, Args&&... args) JSTR_NOEXCEPT__
	{
		static_assert(std::is_same<std::decay_t<T>, const char *>::value, "Args passed must be a C string!");
		const size_t arglen = strlen_args(arg1, arg2, std::forward<Args>(args)...);
		if (unlikely(!this->reserve_add(arglen)))
			return 0;
		char *tmp = this->data + this->size;
		while (*arg1)
			*tmp++ = *arg1++;
		while (*arg2)
			*tmp++ = *arg2++;
		for (const auto&& arg : { std::forward<Args>(args)... })
			while (*arg)
				*tmp++ = *arg++;
		*tmp = '\0';
		this->size += arglen;
		return 1;
	}

	template <typename T, typename... Args>
	ALWAYS_INLINE JSTR_WARN_UNUSED__
	int cat_s(T arg1, T arg2, Args&&... args) JSTR_NOEXCEPT__
	{
		if (unlikely(!this->cat(arg1, arg2, std::forward<Args>(args)...)))
			return 0;
		return 1;
	}

#	else

	template <typename T, typename... Args>
	ALWAYS_INLINE
	jstring_t(T arg1, T arg2, Args&&... args) JSTR_NOEXCEPT__
	{
		if (unlikely(!private_jstr_cat(this, strlen_args(arg1, arg2, std::forward<Args>(args)...), arg1, arg2, args...)))
			return 0;
		return 1;
	}

	template <typename T, typename... Args>
	ALWAYS_INLINE JSTR_WARN_UNUSED__
	int cat(T arg1, T arg2, Args&&... args) JSTR_NOEXCEPT__
	{
		if (unlikely(!private_jstr_cat(this, strlen_args(arg1, arg2, std::forward<Args>(args)...), arg1, arg2, args...)))
			return 0;
		return 1;
	}

	template <typename T, typename... Args>
	ALWAYS_INLINE JSTR_WARN_UNUSED__
	int cat_s(T arg1, T arg2, Args&&... args) JSTR_NOEXCEPT__
	{
		if (unlikely(!private_jstr_cat_s(this, strlen_args(arg1, arg2, std::forward<Args>(args)...), arg1, arg2, args...)))
			return 0;
		return 1;
	}

#	endif // __cplusplus 11

	template <typename T, typename... Args>
	ALWAYS_INLINE
	jstring_t(T arg, Args&&... args) JSTR_NOEXCEPT__
	{
		private_jstr_new_cat(this, strlen_args(arg, std::forward<args>(args)...), arg, args...);
	}

	ALWAYS_INLINE
	jstring_t(const char *RESTRICT s) JSTR_NOEXCEPT__
	{
		private_jstr_new_append_void(this, std::strlen(s), s);
	}

	template <std::size_t N>
	ALWAYS_INLINE
	jstring_t(const char (&s)[N]) JSTR_NOEXCEPT__
	{
		jstr_new_append_void(this, N - 1, s);
	}

	ALWAYS_INLINE
	jstring_t(const char *RESTRICT s, const std::size_t slen) JSTR_NOEXCEPT__
	{
		if (unlikely(!jstr_new_append(this, slen, s)))
			return;
		*(this->data + slen) = '\0';
	}

	ALWAYS_INLINE
	jstring_t(const std::size_t size) JSTR_NOEXCEPT__
	{
		private_jstr_new_alloc_void(this, size);
	}

	ALWAYS_INLINE
	jstring_t(const std::size_t cap, const char *RESTRICT s) JSTR_NOEXCEPT__
	{
		private_jstr_constructor_cap(this, cap, s, std::strlen(s));
	}

	ALWAYS_INLINE
	jstring_t(const std::size_t cap, const char *RESTRICT s, const std::size_t slen) JSTR_NOEXCEPT__
	{
		private_jstr_constructor_cap(this, cap, s, slen);
	}

	template <std::size_t N>
	ALWAYS_INLINE
	jstring_t(const std::size_t cap, const char (&s)[N]) JSTR_NOEXCEPT__
	{
		private_jstr_constructor_cap(this, cap, s, N - 1);
	}

	ALWAYS_INLINE
	jstring_t(const jstring_t *RESTRICT const other) JSTR_NOEXCEPT__
	{
		private_jstr_new_append_void(this, other->size, other->data);
	}

	ALWAYS_INLINE JSTR_WARN_UNUSED__
	int alloc() JSTR_NOEXCEPT__
	{
		if (unlikely(!jstr_new_alloc(this, 8)))
			return 0;
		return 1;
	}

	ALWAYS_INLINE JSTR_WARN_UNUSED__
	int alloc(const size_t cap) JSTR_NOEXCEPT__
	{
		if (unlikely(!jstr_new_alloc(this, cap)))
			return 0;
		return 1;
	}

	template <std::size_t N>
	ALWAYS_INLINE JSTR_WARN_UNUSED__
	int alloc(const char (&s)[N]) JSTR_NOEXCEPT__
	{
		if (unlikely(!jstr_new_append(this, N - 1, s)))
			return 0;
		return 1;
	}

	ALWAYS_INLINE JSTR_WARN_UNUSED__
	int alloc(const char *RESTRICT const s, const size_t slen) JSTR_NOEXCEPT__
	{
		if (unlikely(!jstr_new_append(this, slen, s)))
			return 0;
		return 1;
	}

	ALWAYS_INLINE JSTR_WARN_UNUSED__
	int alloc(const char *RESTRICT const s) JSTR_NOEXCEPT__
	{
		if (unlikely(!jstr_new_append(this, std::strlen(s), s)))
			return 0;
		return 1;
	}
		
	ALWAYS_INLINE CONST JSTR_WARN_UNUSED__
	char *begin() JSTR_NOEXCEPT__
	{
		return (char *)this->data;
	}

	ALWAYS_INLINE CONST JSTR_WARN_UNUSED__
	char *end() JSTR_NOEXCEPT__
	{
		return (char *)this->data + this->size;
	}

	ALWAYS_INLINE CONST JSTR_WARN_UNUSED__
	const char *cbegin() JSTR_NOEXCEPT__
	{
		return (const char *)this->data;
	}

	ALWAYS_INLINE CONST JSTR_WARN_UNUSED__
	const char *cend() JSTR_NOEXCEPT__
	{
		return (const char *)this->data + this->size;
	}

	ALWAYS_INLINE
	void pop_back() JSTR_NOEXCEPT__
	{
		jstr_pop_back(this);
	}

	ALWAYS_INLINE
	void pop_back_s() JSTR_NOEXCEPT__
	{
		jstr_pop_back_s(this);
	}

	ALWAYS_INLINE
	void pop_front() JSTR_NOEXCEPT__
	{
		jstr_pop_front(this);
	}

	ALWAYS_INLINE
	void pop_front_s() JSTR_NOEXCEPT__
	{
		jstr_pop_front_s(this);
	}

	ALWAYS_INLINE JSTR_WARN_UNUSED__
	int push_front(const char c) JSTR_NOEXCEPT__
	{
		return jstr_push_front(this, c);
	}

	ALWAYS_INLINE JSTR_WARN_UNUSED__
	int push_front_s(const char c) JSTR_NOEXCEPT__
	{
		return jstr_push_front_s(this, c);
	}

	ALWAYS_INLINE JSTR_WARN_UNUSED__
	int push_back(const char c) JSTR_NOEXCEPT__
	{
		return jstr_push_back(this, c);
	}

	ALWAYS_INLINE JSTR_WARN_UNUSED__
	int push_back_s(const char c) JSTR_NOEXCEPT__
	{
		return jstr_push_back_s(this, c);
	}

	ALWAYS_INLINE JSTR_WARN_UNUSED__
	int operator+=(const char *RESTRICT const s) JSTR_NOEXCEPT__
	{
		return this->append(s);
	}

	ALWAYS_INLINE JSTR_WARN_UNUSED__
	int operator+=(const char c) JSTR_NOEXCEPT__
	{
		return this->push_back(c);
	}

	template <std::size_t N>
	ALWAYS_INLINE JSTR_WARN_UNUSED__
	int operator+=(const char (&s)[N]) JSTR_NOEXCEPT__
	{
		return private_jstr_append(this, s, N - 1);
	}

	template <std::size_t N>
	ALWAYS_INLINE JSTR_WARN_UNUSED__
	int operator=(const char (&s)[N]) JSTR_NOEXCEPT__
	{
		return this->replace(s);
	}

	ALWAYS_INLINE JSTR_WARN_UNUSED__
	int operator=(const char *RESTRICT const s) JSTR_NOEXCEPT__
	{
		return this->replace(s);
	}

	ALWAYS_INLINE JSTR_WARN_UNUSED__
	int operator=(const jstring_t *RESTRICT const other) JSTR_NOEXCEPT__
	{
		return this->replace(other);
	}
	
	ALWAYS_INLINE JSTR_WARN_UNUSED__
	int append(const char *RESTRICT const s, const size_t slen) JSTR_NOEXCEPT__
	{
		if (!private_jstr_append(this, s, slen))
			return 0;
		*(this->data + slen) = '\0';
		return 1;
	}

	template <std::size_t N>
	ALWAYS_INLINE JSTR_WARN_UNUSED__
	int append(const char (&s)[N]) JSTR_NOEXCEPT__
	{
		return private_jstr_append(this, s, N - 1);
	}

	ALWAYS_INLINE JSTR_WARN_UNUSED__
	int append(const char *RESTRICT const s) JSTR_NOEXCEPT__
	{
		return private_jstr_append(this, s, std::strlen(s));
	}

	ALWAYS_INLINE JSTR_WARN_UNUSED__
	int append_s(const char *RESTRICT const s, const size_t slen) JSTR_NOEXCEPT__
	{
		if (!private_jstr_append_s(this, s, slen))
			return 0;
		*(this->data + slen) = '\0';
		return 1;
	}

	template <std::size_t N>
	ALWAYS_INLINE JSTR_WARN_UNUSED__
	int append_s(const char (&s)[N]) JSTR_NOEXCEPT__
	{
		return private_jstr_append_s(this, s, N - 1);
	}

	ALWAYS_INLINE JSTR_WARN_UNUSED__
	int append_s(const char *RESTRICT const s) JSTR_NOEXCEPT__
	{
		return private_jstr_append_s(this, s, std::strlen(s));
	}

	template <std::size_t N>
	ALWAYS_INLINE CONST JSTR_WARN_UNUSED__
	int str(const char (&s)[N]) JSTR_NOEXCEPT__
	{
		return private_jstr_str(this, s, N - 1);
	}

	ALWAYS_INLINE CONST JSTR_WARN_UNUSED__
	char *str(const char *RESTRICT const s) JSTR_NOEXCEPT__
	{
		return private_jstr_str(this, s, std::strlen(s));
	}

	ALWAYS_INLINE CONST JSTR_WARN_UNUSED__
	char *chr(const int c) JSTR_NOEXCEPT__
	{
		return jstr_chr(this, c);
	}

	ALWAYS_INLINE CONST JSTR_WARN_UNUSED__
	char *rchr(const int c) JSTR_NOEXCEPT__
	{
		return jstr_rchr(this, c);
	}

	ALWAYS_INLINE void swap(jstring_t *other) JSTR_NOEXCEPT__
	{
		jstr_swap(this, other);
	}

	ALWAYS_INLINE JSTR_WARN_UNUSED__
	int reserve(const std::size_t cap) JSTR_NOEXCEPT__
	{
		return jstr_reserve(this, cap);
	}

	ALWAYS_INLINE JSTR_WARN_UNUSED__
	int reserve_add(const std::size_t add_cap) JSTR_NOEXCEPT__
	{
		return jstr_reserve(this, this->capacity + add_cap);
	}

	ALWAYS_INLINE JSTR_WARN_UNUSED__
	int replace(const char *RESTRICT const s) JSTR_NOEXCEPT__
	{
		return private_jstr_replace(this, s, std::strlen(s));
	}

	ALWAYS_INLINE JSTR_WARN_UNUSED__
	int replace(const jstring_t *RESTRICT const other) JSTR_NOEXCEPT__
	{
		return jstr_replace_jstr(this, other);
	}

	template <std::size_t N>
	ALWAYS_INLINE JSTR_WARN_UNUSED__
	int replace(const char (&s)[N]) JSTR_NOEXCEPT__
	{
		return private_jstr_replace(this, s, N - 1);
	}

	ALWAYS_INLINE JSTR_WARN_UNUSED__
	int shrink_to_fit() JSTR_NOEXCEPT__
	{
		return jstr_shrink_to_fit(this);
	}

	ALWAYS_INLINE CONST JSTR_WARN_UNUSED__
	int cmp(const jstring_t *RESTRICT const other) JSTR_NOEXCEPT__
	{
		return jstr_cmp_nocheck(this, other);
	}

	template <std::size_t N>
	ALWAYS_INLINE CONST JSTR_WARN_UNUSED__
	int cmp(const char (&s)[N]) JSTR_NOEXCEPT__
	{
		return jstr_cmp_str(this, s, N - 1);
	}

#	ifdef __USE_GNU

	ALWAYS_INLINE CONST JSTR_WARN_UNUSED__
	int casecmp(const jstring_t *RESTRICT const other) JSTR_NOEXCEPT__
	{
		return jstr_case_cmp_str(this, other->data);
	}

	ALWAYS_INLINE CONST JSTR_WARN_UNUSED__
	int casecmp(const char *RESTRICT const s) JSTR_NOEXCEPT__
	{
		return jstr_case_cmp_str(this, s);
	}

#	endif // __USE_GNU

	ALWAYS_INLINE
	void private_jstr_constructor_cap(jstring_t *RESTRICT this_, const std::size_t cap, const char *RESTRICT s, const std::size_t slen) JSTR_NOEXCEPT__
	{
		this_->capacity = MAX(JSTR_NEXT_POW2(cap), JSTR_MIN_CAP);
		this_->data = (char *)std::malloc(this_->capacity);
		if (unlikely(!this_->data)) {
			this_->capacity = 0;
			this_->size = 0;
			return;
		}
		std::memcpy(this_->data, s, slen + 1);
		*(this_->data + slen) = '\0';
		this_->size = slen;
	}

	ALWAYS_INLINE
	void private_jstr_new_append_void(jstring_t *RESTRICT dest, const size_t srclen, const char *RESTRICT const src, ...) JSTR_NOEXCEPT__
	{
		dest->capacity = MAX(JSTR_MIN_CAP, JSTR_NEXT_POW2(2 * srclen));
		dest->data = JSTR_CAST(char *)malloc(dest->capacity);
		if (unlikely(!dest->data)) {
			jstr_init(dest);
			return;
		}
		dest->size = srclen;
		memcpy(dest->data, src, srclen + 1);
	}

	ALWAYS_INLINE
	void private_jstr_new_alloc_void(jstring_t *RESTRICT this_, const size_t size) JSTR_NOEXCEPT__
	{
		this_->size = 0;
		this_->capacity = MAX(JSTR_MIN_CAP, JSTR_NEXT_POW2(2 * size));
		this_->data = JSTR_CAST(char *)malloc(this_->capacity);
		if (unlikely(!this_->data)) {
			this_->capacity = 0;
			this_->data = NULL;
			return;
		}
	}

	ALWAYS_INLINE
	void jstr_init(jstring_t *RESTRICT this_) JSTR_NOEXCEPT__
	{
		this_->capacity = 0;
		this_->size = 0;
		this_->data = NULL;
	}

	ALWAYS_INLINE
	void jstr_delete(jstring_t *RESTRICT this_) JSTR_NOEXCEPT__
	{
		free(this_->data);
		jstr_init(this_);
	}

	ALWAYS_INLINE
	int jstr_reserve_nocheck_exact(jstring_t *RESTRICT this_, const size_t cap) JSTR_NOEXCEPT__
	{
		char *RESTRICT tmp;
		if (unlikely(!(tmp = JSTR_CAST(char *)realloc(this_->data, cap))))
			return 0;
		this_->data = tmp;
		this_->capacity = cap;
		return 1;
	}

	ALWAYS_INLINE
	int jstr_reserve_nocheck(jstring_t *RESTRICT this_, const size_t cap, ...) JSTR_NOEXCEPT__
	{
		size_t tmp_cap = this_->capacity * JSTR_MULTIPLIER;
		while (tmp_cap < cap)
			tmp_cap *= 2;
		char *RESTRICT tmp;
		if (unlikely(!(tmp = JSTR_CAST(char *)realloc(this_->data, tmp_cap))))
			return 0;
		this_->data = tmp;
		this_->capacity = tmp_cap;
		return 1;
	}

	ALWAYS_INLINE
	int jstr_reserve(jstring_t *RESTRICT this_, const size_t cap) JSTR_NOEXCEPT__
	{
		if (cap > this_->capacity)
			return jstr_reserve_nocheck(this_, cap);
		return 1;
	}

	int private_jstr_cat(jstring_t *RESTRICT this_, const size_t len, ...) JSTR_NOEXCEPT__
	{
		if (unlikely(!jstr_reserve(this_, this_->size + len + 1)))
			return 0;
		char *RESTRICT tmp = this_->data + len;
		this_->size += len;
		va_list ap;
		va_start(ap, len);
		for (const char *RESTRICT argv = va_arg(ap, const char *); argv; argv = va_arg(ap, const char *))
			do {
				*tmp++ = *argv++;
			} while (*argv);
		*tmp = '\0';
		va_end(ap);
		return 1;
	}

	int private_jstr_cat_s(jstring_t *RESTRICT this_, const size_t len, ...) JSTR_NOEXCEPT__
	{
		if (unlikely(!this_->capacity))
			return 0;
		if (unlikely(!jstr_reserve(this_, this_->size + len + 1)))
			return 0;
		char *RESTRICT tmp = this_->data + len;
		this_->size += len;
		va_list ap;
		va_start(ap, len);
		for (const char *RESTRICT argv = va_arg(ap, const char *); argv; argv = va_arg(ap, const char *))
			do {
				*tmp++ = *argv++;
			} while (*argv);
		*tmp = '\0';
		va_end(ap);
		return 1;
	}

	ALWAYS_INLINE
	void private_jstr_append_noalloc(jstring_t *RESTRICT dest, const char *RESTRICT const src, const size_t srclen) JSTR_NOEXCEPT__
	{
		memcpy(dest->data + dest->size, src, srclen + 1);
		dest->size = dest->size + srclen;
	}

	ALWAYS_INLINE
	int private_jstr_append(jstring_t *RESTRICT dest, const char *RESTRICT const src, const size_t srclen) JSTR_NOEXCEPT__
	{
		if (unlikely(!jstr_reserve(dest, dest->size + srclen)))
			return 0;
		private_jstr_append_noalloc(dest, src, srclen);
		return 1;
	}

	ALWAYS_INLINE
	int private_jstr_append_s(jstring_t *RESTRICT dest, const char *RESTRICT const src, const size_t srclen) JSTR_NOEXCEPT__
	{
		if (unlikely(!dest->capacity))
			return 0;
		return private_jstr_append(dest, src, srclen);
	}

	ALWAYS_INLINE
	int jstr_new_append(jstring_t *RESTRICT dest, const size_t srclen, const char *RESTRICT const src, ...) JSTR_NOEXCEPT__
	{
		dest->capacity = MAX(JSTR_MIN_CAP, JSTR_NEXT_POW2(2 * srclen));
		dest->data = JSTR_CAST(char *)malloc(dest->capacity);
		if (unlikely(!dest->data)) {
			jstr_init(dest);
			return 0;
		}
		dest->size = srclen;
		memcpy(dest->data, src, srclen + 1);
		return 1;
	}

	int private_jstr_new_cat(jstring_t *RESTRICT this_, const size_t arglen, ...) JSTR_NOEXCEPT__
	{
		this_->capacity = MAX(JSTR_MIN_CAP, JSTR_NEXT_POW2(2 * arglen));
		this_->data = JSTR_CAST(char *)malloc(this_->capacity);
		if (unlikely(!this_->data)) {
			jstr_init(this_);
			return 0;
		}
		this_->size = arglen;
		char *tmp = this_->data;
		va_list ap;
		va_start(ap, arglen);
		for (const char *RESTRICT argv = va_arg(ap, const char *); argv; argv = va_arg(ap, const char *))
			do {
				*tmp++ = *argv++;
			} while (*argv);
		*tmp = '\0';
		va_end(ap);
		return 1;
	}

	ALWAYS_INLINE
	int jstr_new_alloc(jstring_t *RESTRICT this_, const size_t size) JSTR_NOEXCEPT__
	{
		this_->size = 0;
		this_->capacity = MAX(JSTR_MIN_CAP, JSTR_NEXT_POW2(2 * size));
		this_->data = JSTR_CAST(char *)malloc(this_->capacity);
		if (unlikely(!this_->data)) {
			this_->capacity = 0;
			this_->data = NULL;
			return 0;
		}
		return 1;
	}

	ALWAYS_INLINE
	void jstr_swap(jstring_t *RESTRICT this_, jstring_t *RESTRICT other) JSTR_NOEXCEPT__
	{
		const jstring_t tmp = *other;
		*other = *this_;
		*this_ = tmp;
	}

	ALWAYS_INLINE
	void jstr_swap_str(jstring_t *RESTRICT this_, char **RESTRICT other, size_t *RESTRICT otherlen, size_t *RESTRICT othercapacity) JSTR_NOEXCEPT__
	{
		char *RESTRICT const tmp_other = *other;
		const size_t tmp_otherlen = *otherlen;
		const size_t tmp_othercap = *othercapacity;
		*other = this_->data;
		*otherlen = this_->size;
		*othercapacity = this_->capacity;
		this_->data = tmp_other;
		this_->size = tmp_otherlen;
		this_->capacity = tmp_othercap;
	}

	ALWAYS_INLINE
	int jstr_cmp_nocheck(const jstring_t *RESTRICT const this_, const jstring_t *RESTRICT const other) JSTR_NOEXCEPT__
	{
		return memcmp(this_->data, other->data, this_->size);
	}

	ALWAYS_INLINE
	int jstr_cmp(const jstring_t *RESTRICT const this_, const jstring_t *RESTRICT const other) JSTR_NOEXCEPT__
	{
		if (this_->size != other->size)
			return 1;
		return jstr_cmp_nocheck(this_, other);
	}

	ALWAYS_INLINE
	int jstr_cmp_str(const jstring_t *RESTRICT const this_, const char *RESTRICT const s, const size_t slen) JSTR_NOEXCEPT__
	{
		if (this_->size != slen)
			return 1;
		return memcmp(this_, s, slen);
	}

#ifdef __USE_GNU

	ALWAYS_INLINE
	int jstr_case_cmp_nocheck(const jstring_t *RESTRICT const this_, const jstring_t *RESTRICT const other) JSTR_NOEXCEPT__
	{
		return strcasecmp(this_->data, other->data);
	}

	ALWAYS_INLINE
	int jstr_case_cmp_str(const jstring_t *RESTRICT const this_, const char *RESTRICT const s) JSTR_NOEXCEPT__
	{
		return strcasecmp(this_->data, s);
	}

	ALWAYS_INLINE
	int jstr_case_cmp(const jstring_t *RESTRICT const this_, const jstring_t *RESTRICT const other) JSTR_NOEXCEPT__
	{
		if (this_->size != other->size)
			return 1;
		return jstr_case_cmp_nocheck(this_, other);
	}

#endif

	ALWAYS_INLINE
	void jstr_replace_noalloc(jstring_t *RESTRICT dest, const char *RESTRICT src, const size_t srclen) JSTR_NOEXCEPT__
	{
		memcpy(dest->data, src, srclen + 1);
		dest->size = srclen;
	}

	ALWAYS_INLINE
	void jstr_replace_jstr_noalloc(jstring_t *RESTRICT dest, const jstring_t *RESTRICT const src) JSTR_NOEXCEPT__
	{
		memcpy(dest->data, src->data, src->size + 1);
		dest->size = src->size;
	}

	ALWAYS_INLINE
	int jstr_replace_nocheck(jstring_t *RESTRICT dest, const char *RESTRICT src, const size_t srclen) JSTR_NOEXCEPT__
	{
		if (unlikely(!jstr_reserve_nocheck(dest, srclen)))
			return 0;
		jstr_replace_noalloc(dest, src, srclen);
		return 1;
	}

	ALWAYS_INLINE
	int jstr_replace_jstr_nocheck(jstring_t *RESTRICT dest, const jstring_t *RESTRICT const src) JSTR_NOEXCEPT__
	{
		if (unlikely(!jstr_reserve_nocheck(dest, src->size)))
			return 0;
		jstr_replace_jstr_noalloc(dest, src);
		return 1;
	}


	ALWAYS_INLINE
	int private_jstr_replace(jstring_t *RESTRICT dest, const char *RESTRICT src, const size_t srclen, ...) JSTR_NOEXCEPT__
	{
		if (dest->capacity < srclen)
			return jstr_replace_nocheck(dest, src, srclen);
		jstr_replace_noalloc(dest, src, srclen);
		return 1;
	}

	ALWAYS_INLINE
	int jstr_replace_jstr(jstring_t *RESTRICT dest, const jstring_t *RESTRICT const src, ...) JSTR_NOEXCEPT__
	{
		if (dest->capacity < src->capacity)
			return jstr_replace_nocheck(dest, src->data, src->capacity);
		jstr_replace_jstr_noalloc(dest, src);
		return 1;
	}

	ALWAYS_INLINE
	int jstr_reserve_s(jstring_t *RESTRICT this_, const size_t cap) JSTR_NOEXCEPT__
	{
		if (unlikely(!this_->capacity))
			return 0;
		return jstr_reserve(this_, cap);
	}

	ALWAYS_INLINE
	int jstr_shrink_to_fit_nocheck(jstring_t *RESTRICT this_) JSTR_NOEXCEPT__
	{
		char *RESTRICT tmp;
		if (unlikely(!(tmp = JSTR_CAST(char *)realloc(this_->data, this_->size + 1))))
			return 0;
		this_->capacity = this_->size + 1;
		this_->data = tmp;
		return 1;
	}

	ALWAYS_INLINE
	int jstr_shrink_to_fit(jstring_t *RESTRICT this_) JSTR_NOEXCEPT__
	{
		if (unlikely(this_->capacity == this_->size))
			return 1;
		return jstr_shrink_to_fit_nocheck(this_);
	}

	ALWAYS_INLINE
	int jstr_shrink_to_nocheck(jstring_t *RESTRICT this_, const size_t cap) JSTR_NOEXCEPT__
	{
		char *RESTRICT tmp;
		if (unlikely(!(tmp = JSTR_CAST(char *)realloc(this_->data, cap))))
			return 0;
		this_->data = tmp;
		this_->data[this_->size = cap] = '\0';
		this_->capacity = cap;
		return 1;
	}

	ALWAYS_INLINE
	int jstr_shrink_to(jstring_t *RESTRICT this_, const size_t cap) JSTR_NOEXCEPT__
	{
		if (cap > this_->capacity)
			return 1;
		return jstr_shrink_to_nocheck(this_, cap);
	}

	ALWAYS_INLINE
	void jstr_shrink_to_size_nocheck(jstring_t *RESTRICT this_, const size_t size) JSTR_NOEXCEPT__
	{
		this_->data[this_->size = size] = '\0';
	}

	ALWAYS_INLINE
	void jstr_shrink_to_size(jstring_t *RESTRICT this_, const size_t size) JSTR_NOEXCEPT__
	{
		if (size < this_->size)
			jstr_shrink_to_size_nocheck(this_, size);
	}

	ALWAYS_INLINE
	void jstr_shrink_to_size_s(jstring_t *RESTRICT this_, const size_t size) JSTR_NOEXCEPT__
	{
		if (!!this_->size & (size < this_->size))
			jstr_shrink_to_size_nocheck(this_, size);
	}

	ALWAYS_INLINE
	void jstr_push_back_noalloc(jstring_t *RESTRICT this_, const char c) JSTR_NOEXCEPT__
	{
		this_->data[this_->size] = c;
		this_->data[++this_->size] = '\0';
	}

	ALWAYS_INLINE
	int jstr_push_back_nocheck(jstring_t *RESTRICT this_, const char c) JSTR_NOEXCEPT__
	{
		if (unlikely(!jstr_reserve_nocheck(this_, this_->size * JSTR_MULTIPLIER)))
			return 0;
		jstr_push_back_noalloc(this_, c);
		return 1;
	}

	ALWAYS_INLINE
	int jstr_push_back(jstring_t *RESTRICT this_, const char c) JSTR_NOEXCEPT__
	{
		if (unlikely(this_->size == this_->capacity))
			return jstr_push_back_nocheck(this_, c);
		jstr_push_back_noalloc(this_, c);
		return 1;
	}

	ALWAYS_INLINE
	int jstr_push_back_s(jstring_t *RESTRICT this_, const char c) JSTR_NOEXCEPT__
	{
		if (unlikely(!this_->capacity))
			return 0;
		return jstr_push_back(this_, c);
	}

	ALWAYS_INLINE
	void jstr_pop_back(jstring_t *RESTRICT this_) JSTR_NOEXCEPT__
	{
		this_->data[--this_->size] = '\0';
	}

	ALWAYS_INLINE
	void jstr_pop_back_s(jstring_t *RESTRICT this_) JSTR_NOEXCEPT__
	{
		if (unlikely(!this_->size))
			return;
		jstr_pop_back(this_);
	}

	ALWAYS_INLINE
	void jstr_push_front_noalloc(jstring_t *RESTRICT this_, const char c) JSTR_NOEXCEPT__
	{
		memmove(this_->data + 1, this_->data, ++this_->size);
		*this_->data = c;
	}

	ALWAYS_INLINE
	int jstr_push_front_nocheck(jstring_t *RESTRICT this_, const char c) JSTR_NOEXCEPT__
	{
		if (unlikely(!jstr_reserve_nocheck(this_, this_->size * JSTR_MULTIPLIER)))
			return 0;
		jstr_push_front_noalloc(this_, c);
		return 1;
	}

	ALWAYS_INLINE
	int jstr_push_front(jstring_t *RESTRICT this_, const char c) JSTR_NOEXCEPT__
	{
		if (unlikely(this_->capacity == this_->size))
			return jstr_push_front_nocheck(this_, c);
		jstr_push_front_noalloc(this_, c);
		return 1;
	}

	ALWAYS_INLINE
	int jstr_push_front_s(jstring_t *RESTRICT this_, const char c) JSTR_NOEXCEPT__
	{
		if (unlikely(!this_->capacity))
			return 0;
		return jstr_push_front(this_, c);
	}

	ALWAYS_INLINE
	void jstr_pop_front(jstring_t *RESTRICT this_) JSTR_NOEXCEPT__
	{
		memmove(this_->data, this_->data + 1, this_->size--);
	}

	ALWAYS_INLINE
	void jstr_pop_front_s(jstring_t *RESTRICT this_) JSTR_NOEXCEPT__
	{
		if (unlikely(!this_->size))
			return;
		memmove(this_->data, this_->data + 1, this_->size--);
	}

#ifdef __USE_GNU

	ALWAYS_INLINE
	char *private_jstr_str(jstring_t *haystack, const char *RESTRICT const needle, size_t needlelen, ...) JSTR_NOEXCEPT__
	{
		return JSTR_CAST(char *)memmem(haystack->data, haystack->size, needle, needlelen);
	}

#else

	ALWAYS_INLINE
	char *jstr_str(jstring_t *haystack, const char *RESTRICT needle) JSTR_NOEXCEPT__
	{
		return strstr(haystack->data, needle);
	}

#endif

#ifdef __USE_GNU

	ALWAYS_INLINE
	char *jstr_rchr(const jstring_t *RESTRICT const this_, int c) JSTR_NOEXCEPT__
	{
		return JSTR_CAST(char *)memrchr(this_->data, c, this_->size);
	}

#else

	ALWAYS_INLINE
	char *jstr_rchr(const jstring_t *RESTRICT const this_, const int c) JSTR_NOEXCEPT__
	{
		const char *RESTRICT const begin = this_->data;
		char *RESTRICT end = this_->data + this_->size - 1;
		for ( ; end != begin; --end)
			if (*end == c)
				return end;
		return NULL;
	}

#endif // __USE_GNU

	ALWAYS_INLINE
	void jstr_rev(jstring_t *RESTRICT this_) JSTR_NOEXCEPT__
	{
		char *RESTRICT begin = this_->data;
		char *RESTRICT end = this_->data + this_->size - 1;
		for (char tmp_begin; begin < end; ++begin, --end) {
			tmp_begin = *begin;
			*begin = *end;
			*end = tmp_begin;
		}
	}

	ALWAYS_INLINE
	int jstr_rev_dup(jstring_t *RESTRICT src, char **RESTRICT dest) JSTR_NOEXCEPT__
	{
		*dest = JSTR_CAST(char *)malloc(src->size + 1);
		if (unlikely(!*dest))
			return 0;
		char *RESTRICT const begin = src->data - 1;
		char *RESTRICT end = src->data + src->size - 1;
		while (begin < end)
			**dest++ = *end--;
		**dest = '\0';
		return 1;
	}

	ALWAYS_INLINE
	char *jstr_chr(const jstring_t *RESTRICT const this_, int c) JSTR_NOEXCEPT__
	{
		return JSTR_CAST(char *)memchr(this_->data, c, this_->size);
	}

	ALWAYS_INLINE
	int jstr_dup(jstring_t *RESTRICT this_, jstring_t *RESTRICT other) JSTR_NOEXCEPT__
	{
		other->data = JSTR_CAST(char *)malloc(this_->capacity);
		if (unlikely(!other))
			return 0;
		memcpy(other->data, this_->data, this_->size + 1);
		other->capacity = this_->capacity;
		other->size = this_->size;
		return 1;
	}

	ALWAYS_INLINE
	int jstr_dup_s(jstring_t *RESTRICT this_, jstring_t *RESTRICT other) JSTR_NOEXCEPT__
	{
		if (unlikely(!this_->size))
			return 0;
		return jstr_dup(this_, other);
	}

	ALWAYS_INLINE
	int jstr_ndup(jstring_t *RESTRICT this_, jstring_t *RESTRICT other, const size_t n) JSTR_NOEXCEPT__
	{
		if (n > this_->size)
			return 0;
		other->data = JSTR_CAST(char *)malloc(n + 1);
		if (unlikely(!other))
			return 0;
		memcpy(other->data, this_->data, n + 1);
		*(other->data + n) = '\0';
		other->capacity = n + 1;
		other->size = n;
		return 1;
	}

	ALWAYS_INLINE
	int jstr_ndup_s(jstring_t *RESTRICT this_, jstring_t *RESTRICT other, const size_t n) JSTR_NOEXCEPT__
	{
		if (!this_->size
		| (n > this_->size))
			return 0;
		other->data = JSTR_CAST(char *)malloc(n + 1);
		if (unlikely(!other))
			return 0;
		memcpy(other->data, this_->data, n + 1);
		*(other->data + n) = '\0';
		other->capacity = n + 1;
		other->size = n;
		return 1;
	}
} jstring_t;
#endif // __cplusplus

#ifdef JSTR_HAS_GENERIC
#	define jstr_replace(dest, ...) _Generic((PP_FIRST_ARG(__VA_ARGS__)),                                                        \
		jstring_t *: jstr_replace_jstr(dest, (jstring_t *)PP_FIRST_ARG(__VA_ARGS__)),                                       \
		JSTR_GENERIC_CASE_STR((PP_NARG(__VA_ARGS__) == 2)                                                                   \
			? private_jstr_replace(dest, (char *)__VA_ARGS__, 0)                                                        \
			: private_jstr_replace(dest, (char *)PP_FIRST_ARG(__VA_ARGS__), strlen((char *)PP_FIRST_ARG(__VA_ARGS__)))) \
	)
#else
#	define jstr_replace(dest, src, srclen)          \
		private_jstr_replace(dest, src, srclen)
#endif // JSTR_HAS_GENERIC

#ifdef JSTR_HAS_GENERIC
#	define jstr_cat(this_jstr, ...)                                                          \
		generic_jstr_cat(this_jstr, PP_STRLEN_VA_ARGS(__VA_ARGS__), __VA_ARGS__, NULL)

#	define generic_jstr_cat(this_jstr, len, arg1, ...) _Generic((PP_FIRST_ARG(__VA_ARGS__)), \
		void *: jstr_append(this_jstr, arg1, len),                                       \
		JSTR_GENERIC_CASE_STR(private_jstr_cat(this_jstr, len, arg1, __VA_ARGS__))       \
	)

#	define jstr_cat_s(this_jstr, ...)                                                          \
		generic_jstr_cat_s(this_jstr, PP_STRLEN_VA_ARGS(__VA_ARGS__), __VA_ARGS__, NULL)

#	define generic_jstr_cat_s(this_jstr, len, arg1, ...) _Generic((PP_FIRST_ARG(__VA_ARGS__)), \
		void *: jstr_append_s(this_jstr, arg1, len),                                       \
		JSTR_GENERIC_CASE_STR(private_jstr_cat_s(this_jstr, len, arg1, __VA_ARGS__))       \
	)
#else
#	define jstr_cat(this_jstr, ...)                                                                                           \
		(PP_NARG(__VA_ARGS__) > 1)                                                                                        \
			? private_jstr_cat(this_jstr, PP_STRLEN_VA_ARGS(__VA_ARGS__), __VA_ARGS__, NULL)                          \
			: private_jstr_append(this_jstr, PP_FIRST_ARG(__VA_ARGS__), PP_STRLEN_VA_ARGS(PP_FIRST_ARG(__VA_ARGS__)))

#	define jstr_cat_s(this_jstr, ...)                                                                                           \
		(PP_NARG(__VA_ARGS__) > 1)                                                                                          \
			? private_jstr_cat_s(this_jstr, PP_STRLEN_VA_ARGS(__VA_ARGS__), __VA_ARGS__, NULL)                          \
			: private_jstr_append_s(this_jstr, PP_FIRST_ARG(__VA_ARGS__), PP_STRLEN_VA_ARGS(PP_FIRST_ARG(__VA_ARGS__)))
#endif // JSTR_HAS_GENERIC

#define jstr_new_cat(this_jstr, ...) private_jstr_new_cat(this_jstr, PP_STRLEN_VA_ARGS(__VA_ARGS__), __VA_ARGS__, NULL)

#define PRIVATE_JSTR_NEW_FIRST_INT(this_jstr, ...)                         \
	(PP_NARG(__VA_ARGS__) == 1)                                        \
		? PRIVATE_JSTR_NEW_ALLOC(this_jstr, __VA_ARGS__)           \
		: PRIVATE_JSTR_NEW_APPEND_WITH_LEN(this_jstr, __VA_ARGS__)

#define PRIVATE_JSTR_NEW_ALLOC(this_jstr, ...)                       \
	jstr_new_alloc(this_jstr, (size_t)PP_FIRST_ARG(__VA_ARGS__))

#define PRIVATE_JSTR_NEW_APPEND(this_jstr, ...)                                                                  \
	jstr_new_append(this_jstr, strlen((char *)PP_FIRST_ARG(__VA_ARGS__)), (char *)PP_FIRST_ARG(__VA_ARGS__))

#define PRIVATE_JSTR_NEW_APPEND_WITH_LEN(this_jstr, ...)      \
	jstr_new_append(this_jstr, (size_t)__VA_ARGS__, NULL)

#define PRIVATE_JSTR_NEW_CAT(this_jstr, ...)                                                       \
	private_jstr_new_cat(this_jstr, PP_STRLEN_VA_ARGS((char *)__VA_ARGS__), __VA_ARGS__, NULL)

#define PRIVATE_JSTR_NEW_ADD_STR(this_jstr, ...)                  \
(                                                                 \
	(PP_NARG(__VA_ARGS__) == 1)                               \
		? PRIVATE_JSTR_NEW_APPEND(this_jstr, __VA_ARGS__) \
		: PRIVATE_JSTR_NEW_CAT(this_jstr, __VA_ARGS__)    \
)

#ifdef JSTR_HAS_GENERIC
#	define jstr_new(this_jstr, ...) _Generic((PP_FIRST_ARG(__VA_ARGS__)),               \
		JSTR_GENERIC_CASE_SIZE(PRIVATE_JSTR_NEW_FIRST_INT(this_jstr, __VA_ARGS__)), \
		JSTR_GENERIC_CASE_STR(PRIVATE_JSTR_NEW_ADD_STR(this_jstr, __VA_ARGS__))     \
	)
#else
#	define jstr_new(this_jstr, size) jstr_new_alloc(this_jstr, size)
#endif // JSTR_HAS_GENERIC

#ifdef JSTR_HAS_GENERIC
#	define jstr_add(this_jstr, ...) _Generic((PP_FIRST_ARG(__VA_ARGS__)),                                                         \
		JSTR_GENERIC_CASE_SIZE(jstr_reserve_nocheck(this_jstr, ((this_jstr)->capacity) + (size_t)PP_FIRST_ARG(__VA_ARGS__))), \
		JSTR_GENERIC_CASE_STR(jstr_append(this_jstr, (char *)__VA_ARGS__))                                                    \
	)
#endif // JSTR_HAS_GENERIC

#define jstr_append(this_jstr, ...)                                                                            \
(                                                                                                              \
	PP_NARG(__VA_ARGS__) == 2                                                                              \
		? private_jstr_append(this_jstr, __VA_ARGS__, 0)                                               \
		: private_jstr_append(this_jstr, PP_FIRST_ARG(__VA_ARGS__), strlen(PP_FIRST_ARG(__VA_ARGS__))) \
)

#define private_jstr_reserve_x(this_jstr, multiplier)                                   \
	jstr_reserve_nocheck_exact(this_jstr, ((multiplier) * ((this_jstr)->capacity)))

#define jstr_reserve_2x(this_jstr) private_jstr_reserve_x(this_jstr, 2)
#define jstr_reserve_4x(this_jstr) private_jstr_reserve_x(this_jstr, 4)
#define jstr_reserve_8x(this_jstr) private_jstr_reserve_x(this_jstr, 8)
#define jstr_reserve_16x(this_jstr) private_jstr_reserve_x(this_jstr, 16)
#define jstr_reserve_32x(this_jstr) private_jstr_reserve_x(this_jstr, 32)
#define jstr_reserve_64x(this_jstr) private_jstr_reserve_x(this_jstr, 64)

#define private_jstr_reserve_s_x(this_jstr, multiplier)                                   \
	jstr_reserve_s_nocheck_exact(this_jstr, ((multiplier) * ((this_jstr)->capacity)))

#define jstr_reserve_s_2x(this_jstr) private_jstr_reserve_s_x(this_jstr, 2)
#define jstr_reserve_s_4x(this_jstr) private_jstr_reserve_s_x(this_jstr, 4)
#define jstr_reserve_s_8x(this_jstr) private_jstr_reserve_s_x(this_jstr, 8)
#define jstr_reserve_s_16x(this_jstr) private_jstr_reserve_s_x(this_jstr, 16)
#define jstr_reserve_s_32x(this_jstr) private_jstr_reserve_s_x(this_jstr, 32)
#define jstr_reserve_s_64x(this_jstr) private_jstr_reserve_s_x(this_jstr, 64)

#ifndef __cplusplus
#	define jstr_foreach(elem, jstr)                          \
		for (char *elem = ((jstr)->data); *elem; ++elem)
#	define jstr_begin(this_jstr) ((this_jstr)->data)
#	define jstr_end(this_jstr) (((this_jstr)->data) + ((this_jstr)->size))

#	define jstr_cbegin(this_jstr) ((const char *)((this_jstr)->data))
#	define jstr_cend(this_jstr) ((const char *)(((this_jstr)->data) + ((this_jstr)->size)))
#endif // __cplusplus

#define jstr_typeof_data char *

#define jstr_foreach_index(elem, jstr)                        \
	for (size_t i = 0, end__ = jstr.size; i < end__; ++i)

#endif // JSTR_H_DEF__
