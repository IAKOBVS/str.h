#ifndef JSTR_H

#define JSTR_H 1
#include "jstr-macros.h"

P_JSTR_BEGIN_DECLS
#include <stdlib.h>
P_JSTR_END_DECLS

#define JTRIE_ASCII_SIZE 256

#define R JSTR_RESTRICT

P_JSTR_BEGIN_DECLS

typedef enum {
	JTRIE_RET_NOERROR = 0,
	JTRIE_RET_MALLOC_ERROR = 1,
} jtrie_errcode_ty;

typedef struct jtrie_node_ty {
	struct jtrie_node_ty *child[JTRIE_ASCII_SIZE];
	unsigned int EOW;
} jtrie_node_ty;

JSTR_INLINE
JSTR_WARN_UNUSED
JSTR_NOTHROW
static jtrie_node_ty *
jtrie_init(void) JSTR_NOEXCEPT
{
	return (jtrie_node_ty *)calloc(1, sizeof(jtrie_node_ty));
}

JSTR_MAYBE_UNUSED
JSTR_NOTHROW
static void
pjtrie_free_recur(jtrie_node_ty *R node) JSTR_NOEXCEPT
{
	if (jstr_unlikely(node == NULL))
		return;
	for (int i = 0; i != JTRIE_ASCII_SIZE - 1; ++i)
		pjtrie_free_recur(node->child[i]);
	free(node);
	node = NULL;
}

JSTR_MAYBE_UNUSED
JSTR_NOTHROW
static void
jtrie_free(jtrie_node_ty *R *R node) JSTR_NOEXCEPT
{
	pjtrie_free_recur(*node);
}

JSTR_INLINE
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
JSTR_NOTHROW
static jtrie_errcode_ty
jtrie_insert(jtrie_node_ty *R const root,
	     const char *R const word) JSTR_NOEXCEPT
{
	if (jstr_unlikely(*word == '\0'))
		return JTRIE_RET_NOERROR;
	const unsigned char *w = (unsigned char *)word;
	jtrie_node_ty *curr = root;
	for (; *w; ++w) {
		if (curr->child[*w] == NULL)
			curr->child[*w] = jtrie_init();
		curr = curr->child[*w];
		if (jstr_unlikely(curr == NULL))
			return JTRIE_RET_MALLOC_ERROR;
	}
	curr->EOW = 1;
	return JTRIE_RET_NOERROR;
}

typedef enum {
	PJTRIE_FLAG_REMOVE_NOT_PREFIXES = 0,
	PJTRIE_FLAG_REMOVE_PREFIXES = 1,
} pjtrie_flag_rm_prefixes_ty;

JSTR_NONNULL_ALL
JSTR_INLINE
JSTR_NOTHROW
static void
pjtrie_remove(pjtrie_flag_rm_prefixes_ty flag,
	      jtrie_node_ty *R const root,
	      const char *R const word) JSTR_NOEXCEPT
{
	if (jstr_unlikely(*word == '\0'))
		return;
	const unsigned char *w = (unsigned char *)word;
	jtrie_node_ty *curr = root->child[*w];
	if (jstr_unlikely(curr == NULL))
		return;
	while (*++w && curr->child[*w]) {
		if (flag & PJTRIE_FLAG_REMOVE_PREFIXES)
			curr->EOW = 0;
		curr = curr->child[*w];
	}
	curr->EOW = 0;
}

JSTR_NONNULL_ALL
JSTR_INLINE
JSTR_NOTHROW
static void
jtrie_remove(jtrie_node_ty *R const root,
	     const char *R const word) JSTR_NOEXCEPT
{
	return pjtrie_remove(PJTRIE_FLAG_REMOVE_NOT_PREFIXES, root, word);
}

JSTR_NONNULL_ALL
JSTR_INLINE
JSTR_NOTHROW
static void
jtrie_removeprefixes(jtrie_node_ty *R const root,
		     const char *R const word) JSTR_NOEXCEPT
{
	return pjtrie_remove(PJTRIE_FLAG_REMOVE_PREFIXES, root, word);
}

/*
   Return value:
   Pointer to node with last letter of WORD.
   NULL if not found.
*/
JSTR_NONNULL_ALL
JSTR_INLINE
JSTR_WARN_UNUSED
JSTR_NOTHROW
static jtrie_node_ty *
jtrie_starts_with(const jtrie_node_ty *R const root,
		  const char *R const word) JSTR_NOEXCEPT
{
	if (jstr_unlikely(*word == '\0'))
		return NULL;
	const unsigned char *w = (unsigned char *)word;
	const jtrie_node_ty *curr = root->child[*w];
	if (jstr_unlikely(curr == NULL))
		return NULL;
	while (*++w && curr->child[*w])
		curr = curr->child[*w];
	return (jtrie_node_ty *)curr;
}

/*
   Return value:
   1 if matches;
   0 otherwise.
*/
JSTR_NONNULL_ALL
JSTR_INLINE
JSTR_WARN_UNUSED
JSTR_NOTHROW
static int
jtrie_match(const jtrie_node_ty *R const root,
	    const char *R const word) JSTR_NOEXCEPT
{
	return jtrie_starts_with(root, word)->EOW;
}

P_JSTR_END_DECLS

#undef R

#endif /* JSTR_H */
