#ifndef JSTR_TEST_H
#define JSTR_TEST_H

#define JSTR_PANIC 1
#define JSTR_TEST  1
#define JSTR_DEBUG 1
/* Force use of slower simple functions. */
#define JSTR_USE_SIMPLE 0

#define JSTR_BUILT

#include <assert.h>
#include <ctype.h>
#include <stddef.h>
#include <string.h>

#include "../src/jstr-macros.h"

#define TOLOWER(c) (unsigned char)(((unsigned char)(c) >= 'A' && (unsigned char)(c) <= 'Z') ? ((unsigned char)(c) - 'A' + 'a') : (unsigned char)(c))
#define TOUPPER(c) (unsigned char)(((unsigned char)(c) >= 'a' && (unsigned char)(c) <= 'z') ? ((unsigned char)(c) - 'a' + 'A') : (unsigned char)(c))

#define SUCCESS()                                                   \
	do {                                                        \
		(void)argc;                                         \
		(void)argv;                                         \
		PRINT("%s ", __FILE__);                             \
		for (; *argv; ++argv)                               \
			if (!strncmp(*argv, "-D", 2))               \
				PRINT("%s ", *argv + strlen("-D")); \
		PRINT("succeeded.\n");                              \
	} while (0)

/* clang-format off */
#define EMPTY(p)          (sizeof(p) == sizeof(const char *) ? ((const char *)p) == (const char *)NULL ? "(null)" : (p) : (p))
#define PRINT(...)    printf(__VA_ARGS__)
#define PRINTERR(...) fprintf(stderr, __VA_ARGS__)
#define START() do{}while(0)

PJSTR_BEGIN_DECLS
JSTR_ATTR_MAYBE_UNUSED
JSTR_ATTR_INLINE
static char *clean_func(const char *func)
{
	for (; !isalpha(*func); ++func);
	return (char *)func;
}
PJSTR_END_DECLS

#if VERBOSE
#	define TESTING(func) PRINT("Testing %s().\n", clean_func(#func))
#else
#	define TESTING(func)
#endif
/* clang-format on */

#define ASSERT_RESULT(func, expr, result, expected)                      \
	do {                                                             \
		if (jstr_unlikely(!(expr))) {                            \
			PRINTERR("Assertion failure: %s().\nResult:%s\n" \
			         "Expected:%s\n",                        \
			         #func,                                  \
			         EMPTY(result),                          \
			         EMPTY(expected));                       \
			assert(expr);                                    \
		}                                                        \
	} while (0)

#define ASSERT_ERRFUNC(func, expr)                                     \
	do {                                                           \
		if (jstr_unlikely(!(expr))) {                          \
			PRINTERR("Assertion failure: %s().\n", #func); \
			assert(expr);                                  \
		}                                                      \
	} while (0)

#endif /* JSTR_TEST_H */
