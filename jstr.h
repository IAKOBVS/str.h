#ifndef JSTR_H_DEF__
#define JSTR_H_DEF__

#define JSTR_RELEASE
#define JSTR_ALIGN_POWER_OF_TWO
#define JSTR_64_BIT

#ifdef JSTR_RELEASE
#	undef JSTR_DEBUG
#else
#	define JSTR_DEBUG
#endif // JSTR_RELEASE

#include <sys/cdefs.h>
#include <stddef.h>
#include "macros.h"
#include "/home/james/c/pp_macros/pp_va_args_macros.h"

typedef struct jstring_t {
	size_t size; 
	size_t capacity;
	char *data;
} jstring_t;

int private_jstr_cat(jstring_t *RESTRICT this_jstr, const size_t len, ...) JSTR_WARN_UNUSED;

void jstr_init(jstring_t *RESTRICT this_jstr);
void jstr_delete(jstring_t *RESTRICT this_jstr);

int jstr_new_append(jstring_t *RESTRICT this_jstr, const size_t srclen, const char *RESTRICT const src, ...) JSTR_WARN_UNUSED;
int jstr_new_alloc(jstring_t *RESTRICT this_jstr, const size_t size) JSTR_WARN_UNUSED;

int private_jstr_new_cat(jstring_t *RESTRICT this_jstr, const size_t arglen, ...) JSTR_WARN_UNUSED;

int jstr_push_back(jstring_t *this_jstr, const char c) JSTR_WARN_UNUSED;
int jstr_push_back_nocheck(jstring_t *this_jstr, const char c) JSTR_WARN_UNUSED;
void jstr_push_back_noalloc(jstring_t *this_jstr, const char c);

void jstr_push_front_noalloc(jstring_t *RESTRICT this_jstr, const char c);
int jstr_push_front_nocheck(jstring_t *RESTRICT this_jstr, const char c) JSTR_WARN_UNUSED;
int jstr_push_front(jstring_t *RESTRICT this_jstr, const char c) JSTR_WARN_UNUSED;

void jstr_pop_back(jstring_t *RESTRICT this_jstr);
void jstr_pop_front(jstring_t *RESTRICT this_jstr);

int private_jstr_append(jstring_t *this_jstr, const char *RESTRICT src, const size_t srclen, ...) JSTR_WARN_UNUSED;
void private_jstr_append_noalloc(jstring_t *this_jstr, const char *RESTRICT src, const size_t srclen, ...);

/* swaps this_jstr with src and updates the jstring_t struct members */
void jstr_swap(jstring_t *RESTRICT this_jstr, jstring_t *RESTRICT src);
void jstr_swap_str(jstring_t *RESTRICT this_jstr, char **RESTRICT src, size_t *srclen, size_t *src_cap);

int jstr_shrink_to_fit_nocheck(jstring_t *RESTRICT this_jstr) JSTR_WARN_UNUSED;
int jstr_shrink_to_fit(jstring_t *RESTRICT this_jstr) JSTR_WARN_UNUSED;

int jstr_shrink_to_nocheck(jstring_t *RESTRICT this_jstr, const size_t cap) JSTR_WARN_UNUSED;
int jstr_shrink_to(jstring_t *RESTRICT this_jstr, const size_t cap) JSTR_WARN_UNUSED;

void jstr_shrink_to_size_nocheck(jstring_t *RESTRICT this_jstr, const size_t size);
void jstr_shrink_to_size(jstring_t *RESTRICT this_jstr, const size_t size);

int jstr_reserve(jstring_t *RESTRICT this_jstr, const size_t cap) JSTR_WARN_UNUSED;
int jstr_reserve_nocheck(jstring_t *RESTRICT this_jstr, const size_t cap, ...) JSTR_WARN_UNUSED;

/* replaces this_jstr->data with this_jstr and reallocs if needed */
int jstr_replace(jstring_t *RESTRICT this_jstr, char *RESTRICT src, const size_t srclen) JSTR_WARN_UNUSED;
void jstr_replace_noalloc(jstring_t *RESTRICT this_jstr, char *RESTRICT src, const size_t srclen);
/* compares two Jstr, and if equal, returns 0 */
int jstr_cmp(jstring_t *RESTRICT this_jstr, jstring_t *RESTRICT src) JSTR_WARN_UNUSED;

#define jstr_cat(this_jstr, ...)                                                       \
	generic_jstr_cat(this_jstr, PP_STRLEN_VA_ARGS(__VA_ARGS__), __VA_ARGS__, NULL)

#define generic_jstr_cat(this_jstr, len, arg1, ...) _Generic((PP_FIRST_ARG(__VA_ARGS__)), \
	void *: jstr_append(this_jstr, arg1, len),                                        \
	char *: private_jstr_cat(this_jstr, len, arg1, __VA_ARGS__)                       \
)

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
#endif

#define jstr_append(this_jstr, ...)                                                                            \
(                                                                                                              \
	PP_NARG(__VA_ARGS__) == 2                                                                              \
		? private_jstr_append(this_jstr, __VA_ARGS__, 0)                                               \
		: private_jstr_append(this_jstr, PP_FIRST_ARG(__VA_ARGS__), strlen(PP_FIRST_ARG(__VA_ARGS__))) \
)

#define jstr_append_noalloc(this_jstr, ...)                                                                            \
(                                                                                                                      \
	PP_NARG(__VA_ARGS__) == 2                                                                                      \
		? private_jstr_append_noalloc(this_jstr, __VA_ARGS__, 0)                                               \
		: private_jstr_append_noalloc(this_jstr, PP_FIRST_ARG(__VA_ARGS__), strlen(PP_FIRST_ARG(__VA_ARGS__))) \
)

#define private_jstr_reserve_x(this_jstr, multiplier)                                   \
	jstr_reserve_nocheck_exact(this_jstr, ((multiplier) * ((this_jstr)->capacity)))

#define jstr_reserve_2x(this_jstr) private_jstr_reserve_x(this_jstr, 2)
#define jstr_reserve_4x(this_jstr) private_jstr_reserve_x(this_jstr, 4)
#define jstr_reserve_8x(this_jstr) private_jstr_reserve_x(this_jstr, 8)
#define jstr_reserve_16x(this_jstr) private_jstr_reserve_x(this_jstr, 16)
#define jstr_reserve_32x(this_jstr) private_jstr_reserve_x(this_jstr, 32)
#define jstr_reserve_64x(this_jstr) private_jstr_reserve_x(this_jstr, 64)

#define jstr_replace_auto(this_jstr, src) jstr_replace(this_jstr, src, strlen(src))

#define jstr_replace_noalloc_auto(this_jstr, src) jstr_replace(this_jstr, src, strlen(src))

#define jstr_foreach(elem, jstr)                   \
	for (char *elem = jstr.data; elem; ++elem)

#define jstr_begin(this_jstr) ((this_jstr)->data)
#define jstr_end(this_jstr) (((this_jstr)->data) + ((this_jstr)->size))

#define jstr_cbegin(this_jstr) ((const char *)((this_jstr)->data))
#define jstr_cend(this_jstr) ((const char *)(((this_jstr)->data) + ((this_jstr)->size)))

#define jstr_foreach_index(elem, jstr)                        \
	for (size_t i = 0, end__ = jstr.size; i < end__; ++i)

#endif // JSTR_H_DEF__
