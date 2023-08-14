#ifndef JSTR_DEF_H
#define JSTR_DEF_H

#include "_jstr_macros.h"
#include <stdlib.h>

#define JTRIE_ASCII_SIZE 256

typedef enum {
	JTRIE_RET_NOERROR = 0,
	JTRIE_RET_MALLOC_ERROR = 1,
} Jtrie_errcode;

struct Jtrie_node {
	struct Jtrie_node *child[JTRIE_ASCII_SIZE];
	unsigned char EOW;
};

JSTR_INLINE
JSTR_WARN_UNUSED
static struct Jtrie_node *
jtrie_init(void) JSTR_NOEXCEPT
{
	return (struct Jtrie_node *)calloc(1, sizeof(struct Jtrie_node));
}

JSTR_MAYBE_UNUSED
static void
private_jtrie_destruct_recur(struct Jtrie_node *JSTR_RST node) JSTR_NOEXCEPT
{
	if (jstr_unlikely(node == NULL))
		return;
	for (int i = 0; i != JTRIE_ASCII_SIZE - 1; ++i)
		private_jtrie_destruct_recur(node->child[i]);
	free(node);
	node = NULL;
}

JSTR_MAYBE_UNUSED
static void
jtrie_free(struct Jtrie_node **JSTR_RST node) JSTR_NOEXCEPT
{
	private_jtrie_destruct_recur(*node);
}

JSTR_INLINE
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
static Jtrie_errcode
jtrie_insert(struct Jtrie_node *JSTR_RST const root,
	     const char *JSTR_RST const word) JSTR_NOEXCEPT
{
	const unsigned char *w = (unsigned char *)word;
	if (jstr_unlikely(*w == '\0'))
		return JTRIE_RET_NOERROR;
	struct Jtrie_node *curr = root;
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

JSTR_INLINE
JSTR_NONNULL_ALL
JSTR_WARN_UNUSED
static Jtrie_errcode
jtrie_insertprefix(struct Jtrie_node *JSTR_RST const root,
		   const char *JSTR_RST const word) JSTR_NOEXCEPT
{
	const unsigned char *w = (unsigned char *)word;
	if (jstr_unlikely(*w == '\0'))
		return JTRIE_RET_NOERROR;
	struct Jtrie_node *curr = root;
	for (; *w; ++w) {
		if (curr->child[*w] == NULL)
			curr->child[*w] = jtrie_init();
		curr = curr->child[*w];
		if (jstr_unlikely(curr == NULL))
			return JTRIE_RET_MALLOC_ERROR;
		curr->EOW = 1;
	}
	curr->EOW = 1;
	return JTRIE_RET_NOERROR;
}

JSTR_NONNULL_ALL
JSTR_INLINE
static void
jtrie_remove(struct Jtrie_node *JSTR_RST const root,
	     const char *JSTR_RST const word) JSTR_NOEXCEPT
{
	const unsigned char *w = (unsigned char *)word;
	if (jstr_unlikely(*w == '\0'))
		return;
	struct Jtrie_node *curr = root->child[*w];
	if (jstr_unlikely(curr == NULL))
		return;
	while (*++w && curr->child[*w])
		curr = curr->child[*w];
	curr->EOW = 0;
}

JSTR_NONNULL_ALL
JSTR_INLINE
static void
jtrie_removeprefix(struct Jtrie_node *JSTR_RST const root,
		   const char *JSTR_RST const word) JSTR_NOEXCEPT
{
	const unsigned char *w = (unsigned char *)word;
	if (jstr_unlikely(*w == '\0'))
		return;
	struct Jtrie_node *curr = root->child[*w];
	if (jstr_unlikely(curr == NULL))
		return;
	while (*++w && curr->child[*w]) {
		curr->EOW = 0;
		curr = curr->child[*w];
	}
	curr->EOW = 0;
}

/*
   Return value:
   1 if matches;
   0 otherwise.
*/
JSTR_NONNULL_ALL
JSTR_INLINE
JSTR_WARN_UNUSED
static int
jtrie_match(const struct Jtrie_node *JSTR_RST const root,
	    const char *JSTR_RST const word) JSTR_NOEXCEPT
{
	const unsigned char *w = (unsigned char *)word;
	if (jstr_unlikely(*w == '\0'))
		return 0;
	const struct Jtrie_node *curr = root->child[*w];
	if (jstr_unlikely(curr == NULL))
		return 0;
	while (*++w && curr->child[*w])
		curr = curr->child[*w];
	return curr->EOW;
}

/*
   Return value:
   Pointer to node with last letter of WORD.
   NULL if not found.
*/
JSTR_NONNULL_ALL
JSTR_INLINE
JSTR_WARN_UNUSED
static struct Jtrie_node *
jtrie_starts_with(const struct Jtrie_node *JSTR_RST const root,
		  const char *JSTR_RST const word) JSTR_NOEXCEPT
{
	const unsigned char *w = (unsigned char *)word;
	if (jstr_unlikely(*w == '\0'))
		return NULL;
	const struct Jtrie_node *curr = root->child[*w];
	if (jstr_unlikely(curr == NULL))
		return NULL;
	while (*++w && curr->child[*w])
		curr = curr->child[*w];
	return (struct Jtrie_node *)curr;
}

#endif /* JSTR_DEF_H */
