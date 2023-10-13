#ifndef JSTR_CONFIG_H
#define JSTR_CONFIG_H 1

#ifndef JSTR_DEBUG
#	define JSTR_DEBUG 0
#endif

#ifndef JARR_DEBUG
#	define JARR_DEBUG 0
#endif

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

#define PJSTR_MALLOC_ALIGNMENT (sizeof(size_t) + sizeof(size_t))

#ifndef P_JARR_GROWTH
#	define P_JARR_GROWTH 1.5
#endif

#ifndef P_JARR_ALLOC_MULTIPLIER
#	define P_JARR_ALLOC_MULTIPLIER 1.5
#endif

#ifndef JSTR_GROWTH
#	define JSTR_GROWTH 1.5
#endif

#ifndef JSTR_ALLOC_MULTIPLIER
#	define JSTR_ALLOC_MULTIPLIER 2
#endif

#endif /* JSTR_CONFIG_H */
