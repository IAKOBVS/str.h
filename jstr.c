#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "jstr.h"

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
# define RESTRICT restrict
#elif defined(__GNUC__) || defined(__clang__)
# define RESTRICT __restrict__
#elif defined(_MSC_VER)
# define RESTRICT __restrict
#else
# define RESTRICT
#endif

#if defined(__GNUC__) || defined(__clang__)
    #define ALWAYS_INLINE __attribute__((always_inline)) inline
#elif defined(_MSC_VER)
    #define ALWAYS_INLINE __forceinline inline
#else
    #define ALWAYS_INLINE inline
#endif

#if defined(__PRETTY_FUNCTION__)
	#define CURR_FUNC __PRETTY_FUNCTION__
#elif defined(__FUNCTION__)
	#define CURR_FUNC __FUNCTION__
#elif defined(__func__)
	#define CURR_FUNC __func__
#else
	#define CURR_FUNC ""
#endif

#if (defined(__GNUC__) && (__GNUC__ >= 3)) || (defined(__clang__) && __has_builtin(__builtin_expect))
  #define likely(x) __builtin_expect(!!(x), 1)
  #define unlikely(x) __builtin_expect(!!(x), 0)
#else
  #define likely(x) (x)
  #define unlikely(x) (x)
#endif

#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))

ALWAYS_INLINE void jstrInit(Jstring *RESTRICT dest)
{
	dest->data = NULL;
	dest->size = 0;
	dest->capacity = 0;
}

ALWAYS_INLINE void jstrDeleteFast(Jstring *RESTRICT dest)
{
	free(dest->data);
	jstrInit(dest);
}

ALWAYS_INLINE void jstrDelete(Jstring *RESTRICT dest)
{
	if (dest->data) {
		free(dest->data);
		jstrInit(dest);
	}
}

ALWAYS_INLINE int jstrNew(Jstring *RESTRICT dest, const char *RESTRICT src, const size_t srcSize)
{
	if ((dest->data = malloc((dest->capacity = MAX(JSTR_MIN_SIZE, 2 * srcSize)))));
	else goto ERROR;
	dest->size = srcSize;
	memcpy(dest->data, src, srcSize + 1);
	return 1;

ERROR:
	jstrInit(dest);
	return 0;
}

int private_jstrCat(Jstring *RESTRICT dest, ...)
{
	size_t totalSize = dest->size;
	char *RESTRICT tmpDest = dest->data + totalSize;
	va_list ap;
	va_start(ap, dest);
	for (char *RESTRICT argv = va_arg(ap, char *); argv; argv = va_arg(ap, char *))
		totalSize += strlen(argv);
	va_end(ap);
	if (dest->capacity < totalSize) {
		size_t tmpCap = dest->capacity;
		do {
			tmpCap *= 2;
		} while (tmpCap < totalSize);
		if (likely((dest->data = realloc(dest->data, (tmpCap = dest->capacity)))));
		else goto ERROR_FREE;
	}
	va_start(ap, dest);
	for (char *RESTRICT argv = va_arg(ap, char *); argv; argv = va_arg(ap, char *))
		do {
			*tmpDest++ = *argv++;
		} while (*argv);
	va_end(ap);
	*tmpDest = '\0';
	dest->size = totalSize;
	return 1;

ERROR_FREE:
	jstrDeleteFast(dest);
	return 0;
}

inline int jstrAppend(Jstring *RESTRICT dest, const char *RESTRICT src, const size_t srcSize)
{
	const size_t totalSize = dest->size + srcSize;
	if (dest->capacity < totalSize) {
		size_t tmpCap = dest->capacity;
		do {
			tmpCap *= 2;
		} while (tmpCap < totalSize);
		if (likely((dest->data = realloc(dest->data, (dest->capacity = tmpCap)))));
		else goto ERROR_FREE;
	}
	memcpy(dest->data + dest->size, src, srcSize + 1);
	dest->size = totalSize;
	return 1;

ERROR_FREE:
	jstrDeleteFast(dest);
	return 0;
}

inline int jstrPushback(Jstring *RESTRICT dest, const char c)
{
	if (likely((dest->capacity > dest->size)));
	else
		if (likely((dest->data = realloc(dest->data, (dest->capacity *= 2)))));
		else goto ERROR_FREE;
	dest->data[dest->size] = c;
	dest->data[dest->size + 1] = '\0';
	return 1;

ERROR_FREE:
	jstrDeleteFast(dest);
	return 0;
}

ALWAYS_INLINE void jstrPopback(Jstring *RESTRICT dest)
{
	dest->data[--dest->size] = '\0';
}

ALWAYS_INLINE void jstrSwap(Jstring *RESTRICT dest, Jstring *RESTRICT src)
{
	char *RESTRICT tmpSrc = src->data;
	const size_t srcCap = src->capacity;
	const size_t srcSize = src->size;
	src->data = dest->data;
	src->capacity = dest->capacity;
	src->size = dest->size;
	dest->data = tmpSrc;
	dest->capacity = srcSize;
	dest->size = srcSize;
}

ALWAYS_INLINE void jstrSwapStr(Jstring *RESTRICT dest, char **RESTRICT src, size_t *RESTRICT srcSize, size_t *RESTRICT srcCapacity)
{
	char *RESTRICT tmpSrc = *src;
	const size_t tmpSrcSize = *srcSize;
	const size_t tmpSrcCap = *srcCapacity;
	*src = dest->data;
	*srcSize = dest->size;
	*srcCapacity = dest->capacity;
	dest->data = tmpSrc;
	dest->size = tmpSrcSize;
	dest->capacity = tmpSrcCap;
}

ALWAYS_INLINE int jstrCmp(Jstring *RESTRICT dest, Jstring *RESTRICT src)
{
	return (dest->size != src->size) ? 1 : memcmp(dest->data, src->data, dest->size);
}

inline int jstrReplace(Jstring *RESTRICT dest, char *RESTRICT src, const size_t srcSize)
{
	if (dest->capacity > srcSize);
	else
		if (likely((dest->data = realloc(dest->data, srcSize + 1))));
		else goto ERROR_FREE;
	memcpy(dest->data, src, srcSize + 1);
	return 1;

ERROR_FREE:
	jstrDeleteFast(dest);
	return 0;
}

inline int jstrReserve(Jstring *RESTRICT dest, size_t capacity)
{
	if (dest->capacity) {
		if (likely((dest->data = realloc(dest->data, (dest->capacity = capacity) * sizeof dest->data[0]))));
		else goto ERROR_FREE;
	} else {
		if (likely((dest->data = malloc((dest->capacity = capacity) * sizeof dest->data[0]))));
		else goto ERROR;
	}
	return 1;

ERROR_FREE:
	jstrDeleteFast(dest);
ERROR:
	return 0;
}

inline int jstrShrink(Jstring *RESTRICT dest)
{
	if (likely((dest->data = realloc(dest->data, dest->size + 1)))) {
		dest->capacity = dest->size;
		return 1;
	}
	jstrDeleteFast(dest);
	return 0;
}
