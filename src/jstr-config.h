#ifndef JSTR_CONFIG_H
#define JSTR_CONFIG_H 1

#define JSTR_MAX_UINT_DIGITS	   10
#define JSTR_MAX_ULONG_DIGITS	   19
#define JSTR_MAX_ULONG_LONG_DIGITS 19

#define JSTR_MAX_INT_DIGITS	  (JSTR_MAX_UINT_DIGITS + 1)
#define JSTR_MAX_LONG_DIGITS	  (JSTR_MAX_ULONG_DIGITS + 1)
#define JSTR_MAX_LONG_LONG_DIGITS (JSTR_MAX_ULONG_LONG_DIGITS + 1)

#define JSTR_ENDIAN_LITTLE 1

#if JSTR_ENDIAN_LITTLE
#	undef JSTR_ENDIAN_LITTLE
#define JSTR_ENDIAN_LITTLE 1
#	define JSTR_ENDIAN_BIG	   0
#else
#	undef JSTR_ENDIAN_LITTLE
#define JSTR_ENDIAN_LITTLE 1
#	define JSTR_ENDIAN_BIG	   1
#endif

#define JSTR_MIN_CAP ((sizeof(size_t) == 8) ? 24 : 16)

#define P_JSTR_MALLOC_ALIGNMENT (sizeof(size_t) + sizeof(size_t))

#ifndef PJARR_GROWTH
#	define PJARR_GROWTH 1.5
#endif

#ifndef PJARR_ALLOC_MULTIPLIER
#	define PJARR_ALLOC_MULTIPLIER 1.5
#endif

#ifndef JSTR_GROWTH
#	define JSTR_GROWTH 1.5
#endif

#ifndef JSTR_ALLOC_MULTIPLIER
#	define JSTR_ALLOC_MULTIPLIER 2
#endif

#ifndef JSTR_EXIT_ON_MALLOC_ERROR
#	define JSTR_EXIT_ON_MALLOC_ERROR 1
#endif

#ifndef JSTR_ERR_MSG_ON_MALLOC_ERROR
#	define JSTR_ERR_MSG_ON_MALLOC_ERROR 1
#endif

#ifndef JSTR_ERR_MSG_ON_REGEX_ERROR
#	define JSTR_ERR_MSG_ON_REGEX_ERROR 1
#endif

#ifndef JSTR_NULLIFY_PTR_ON_FREE
#	define JSTR_NULLIFY_PTR_ON_FREE 0
#endif

#ifndef PJARR_NULLIFY_PTR_ON_FREE
#	define PJARR_NULLIFY_PTR_ON_FREE 0
#endif

#endif /* JSTR_CONFIG_H */
