#ifndef JARRAY_DEF_H
#define JARRAY_DEF_H

#include "_builder.h"
#include "_config.h"
#include "_macros.h"
#include <stdlib.h>

#define PJARR_REALLOC(p, sizeof_elem, old_cap, new_cap, malloc_fail) \
	do {                                                         \
		JSTR_ASSERT_IS_STR(p);                               \
		JSTR_ASSERT_IS_SIZE(old_cap);                        \
		JSTR_ASSERT_IS_SIZE(new_cap);                        \
		PJSTR_GROW(old_cap, new_cap);                        \
		(p) = (char *)realloc(p, old_cap * sizeof_elem);     \
		PJSTR_MALLOC_ERR(p, malloc_fail);                    \
	} while (0)

#define PJARR_REALLOC_EXACT(p, sizeof_elem, old_cap, new_cap, malloc_fail) \
	do {                                                               \
		JSTR_ASSERT_IS_SIZE(old_cap);                              \
		JSTR_ASSERT_IS_SIZE(new_cap);                              \
		old_cap = PJSTR_ALIGN_UP(new_cap, PJSTR_MALLOC_ALIGNMENT); \
		(p) = (char *)realloc(p, old_cap * sizeof_elem);           \
		PJSTR_MALLOC_ERR(p, malloc_fail);                          \
	} while (0)

#define jarr_define(T, typename)          \
	typedef struct jarr_ty_##typename \
	{                                 \
		T *data;                  \
		size_t size;              \
		size_t capacity;          \
	}                                 \
	jarr_##typename##_ty;

JSTR_INLINE
JSTR_NONNULL_ALL
static void
pjarr_init(const void **JSTR_RST const _p,
	  size_t *JSTR_RST const _size,
	  size_t *JSTR_RST const _cap)
{
	*_p = NULL;
	*_size = 0;
	*_cap = 0;
}

#define jarr_init(jarr, newcap) \
	pjarr_init(&((jarr)->data), &((jarr)->size), &((jarr)->capacity))

JSTR_INLINE
static void
pjarr_free(void *JSTR_RST _p)
{
	free(_p);
	_p = NULL;
}

JSTR_INLINE
JSTR_NONNULL_ALL
static void
pjarr_alloc(void **JSTR_RST const _p,
	   size_t *JSTR_RST const _sz,
	   size_t *JSTR_RST const _cap,
	   const size_t _sizeof_p,
	   const size_t _newcap)
{
	*_sz = 0;
	*_cap = PJSTR_MIN_ALLOC(_newcap) / _sizeof_p;
	*_p = malloc(*_cap * _sizeof_p);
}

#define jarr_alloc(jarr, newcap) \
	pjarr_alloc(&((jarr)->data), &((jarr)->size), &((jarr)->capacity), sizeof((jarr)->data), newcap)

#if 0

JSTR_INLINE
JSTR_NONNULL_ALL
static void
pjarr_alloc_append(void **JSTR_RST const _p,
		  size_t *JSTR_RST const _sz,
		  size_t *JSTR_RST const _cap,
		  const size_t _sizeof_p,
		  const size_t _newcap)
{
	*_sz = 0;
	*_cap = PJSTR_MIN_ALLOC(_newcap) / _sizeof_p;
	*_p = malloc(*_cap * _sizeof_p);
}

#	define jarr_alloc_append(jarr, newcap) \
		pjarr_alloc_append(&((jarr)->data), &((jarr)->size), &((jarr)->capacity), sizeof((jarr)->data), newcap)

#endif

JSTR_INLINE
JSTR_NONNULL_ALL
static void
pjarr_pop_back(void *JSTR_RST const _p,
	      size_t *JSTR_RST const _sz,
	      const size_t _sizeof_p)
{
	if (jstr_unlikely(*_sz == 0))
		return;
	memmove(_p,
		(unsigned char *)_p + 1 * _sizeof_p,
		*_sz * _sizeof_p - 1 * _sizeof_p);
}

#define jarr_pop_back(jarr) \
	pjarr_pop_back(&((jarr)->data), &((jarr)->size), &((jarr)->capacity))

JSTR_INLINE
JSTR_NONNULL_ALL
static void
pjarr_push_back(void **JSTR_RST const _p,
		size_t *JSTR_RST const _sz,
		size_t *JSTR_RST const _cap,
		const size_t _sizeof_p,
		const void *JSTR_RST const _c)
{
	if (jstr_unlikely(*_cap == *_sz + 1))
		PJARR_REALLOC_EXACT(*_p, _sizeof_p, *_cap, *_sz * JSTR_CFG_ALLOC_MULTIPLIER, return);
	memcpy((unsigned char *)_p + *_sz * _sizeof_p, _c, _sizeof_p);
}

#define jarr_push_back(jarr, c) \
	pjarr_push_back(&((jarr)->data), &((jarr)->size), &((jarr)->capacity), sizeof((jarr)->data), c)

#endif /* JARRAY_DEF_H */
