#ifndef JSTR_IO_H
#define JSTR_IO_H 1

#include "jstr-macros.h"

P_JSTR_BEGIN_DECLS
#include <dirent.h>
#include <fnmatch.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
P_JSTR_END_DECLS

#include "jstr-builder.h"
#include "jstr-io-table.h"
#include "jstr-string.h"

#define R JSTR_RESTRICT

P_JSTR_BEGIN_DECLS

typedef enum {
	JSTR_IO_FT_UNKNOWN = 0,
	JSTR_IO_FT_TEXT,
	JSTR_IO_FT_BINARY,
} jstr_io_ext_ty;

#define S switch (*ext++)
#define T  \
case '\0': \
	return JSTR_IO_FT_TEXT
#define B  \
case '\0': \
	return JSTR_IO_FT_BINARY
JSTR_FUNC_PURE
JSTR_NOINLINE
static jstr_io_ext_ty
p_jstr_io_ext_type(const char *R ext) JSTR_NOEXCEPT
{
	S {
	case 'a':
		S {
			B; /* a */
		}
		break;
	case 'b':
		S {
		case 'i':
			S {
			case 'n':
				S {
					B; /* bin */
				}
				break;
			}
			break;
		}
		break;
	case 'C':
		S {
			T; /* C */
		}
		break;
	case 'c':
		S {
			T; /* c */
		case 'c':
			S {
				T; /* cc */
			}
			break;
		case 'p':
			S {
			case 'p':
				S {
					T; /* cpp */
				}
			}
			break;
		case 's':
			S {
				T; /* cs */
			}
			break;
		}
		break;
	case 'm':
		S {
		case 'd':
			S {
				T; /* md */
			}
			break;
		case 'k':
			S {
			case 'v':
				S {
					B; /* mkv */
				}
				break;
			}
			break;
		}
		break;
	case 'g':
		S {
		case 'z':
			S {
				B; /* gz */
			}
			break;
		}
		break;
	case 'h':
		S {
			T; /* h */
		case 'h':
			S {
				T; /* hh */
			}
			break;
		case 'p':
			S {
			case 'p':
				S {
					T; /* hpp */
				}
				break;
			}
			break;
		case 't':
			S {
			case 'm':
				S {
				case 'l':
					S {
						T; /* html */
					}
					break;
				}
				break;
			}
			break;
		}
		break;
	case 'j':
		S {
		case 'p':
			S {
			case 'g':
				S {
					B; /* jpg */
				}
				break;
			case 'e':
				S {
				case 'g':
					S {
						B; /* jpeg */
					}
					break;
				}
				break;
			}
			break;
		case 's':
			S {
				T; /* js */
			case 'o':
				S {
				case 'n':
					S {
						T; /* json */
					}
					break;
				}
				break;
			}
			break;
		}
		break;
	case 'o':
		S {
			B; /* o */
		}
		break;
	case 'p':
		S {
		case 'l':
			S {
				T; /* pl */
			}
			break;
		case 'm':
			S {
				T; /* pm */
			}
			break;
		case 'y':
			S {
				T; /* py */
			case 'i':
				S {
					T; /* pyi */
				}
				break;
			case 'c':
				S {
					B; /* pyc */
				}
				break;
			}
			break;
		case 'n':
			S {
			case 'g':
				S {
					B; /* png */
				}
				break;
			}
			break;
		case 'd':
			S {
			case 'f':
				S {
					B; /* pdf */
				}
				break;
			}
			break;
		}
		break;
	case 's':
		S {
		case 'h':
			S {
				T; /* sh */
			}
			break;
		case 'o':
			T; /* s */
			S {
				B; /* so */
			}
			break;
		}
		break;
	case 'r':
		S {
		case 's':
			S {
				T; /* rs */
			}
			break;
		}
		break;
	case 'S':
		S {
			T; /* S */
		}
		break;
	case 't':
		S {
		case 's':
			S {
				T; /* ts */
			}
			break;
		case 'x':
			S {
			case 't':
				S {
					T; /* txt */
				}
				break;
			}
			break;
		}
		break;
	case 'w':
		S {
		case 'a':
			S {
			case 'v':
				S {
					B; /* wav */
				}
				break;
			}
			break;
		}
		break;
	}
	return JSTR_IO_FT_UNKNOWN;
}

#undef S
#undef T
#undef B

/*
   Return jstr_io_ext_ty based on the filename extension;
*/
JSTR_FUNC_PURE
JSTR_INLINE
static jstr_io_ext_ty
jstr_io_ext_type_len(const char *R filename,
		     const size_t sz) JSTR_NOEXCEPT
{
	filename = (char *)jstr_memrchr(filename, '.', sz);
	if (filename == NULL)
		return JSTR_IO_FT_UNKNOWN;
	return p_jstr_io_ext_type(filename + 1);
}

/*
   Return jstr_io_ext_ty based on the filename extension;
*/
JSTR_INLINE
JSTR_FUNC_PURE
static jstr_io_ext_ty
jstr_io_ext_type(const char *R filename) JSTR_NOEXCEPT
{
	filename = strrchr(filename, '.');
	if (filename == NULL)
		return JSTR_IO_FT_UNKNOWN;
	return p_jstr_io_ext_type(filename + 1);
}

/*
   Check MIN(N, SZ) bytes for any unprintable character.
*/
JSTR_FUNC
static int
jstr_isbinary(const char *R const buf,
	      const size_t n,
	      const size_t sz) JSTR_NOEXCEPT
{
	if (jstr_unlikely(sz == 0))
		return 0;
	const unsigned char *const end = (unsigned char *)buf + JSTR_MIN(n, sz);
	const unsigned char *s = (unsigned char *)buf;
	while (s < end)
		if (p_jstr_io_reject_table[*s++])
			return 1;
	return 0;
}

#define P_JSTR_ELF    "\x7f\ELF"
#define P_JSTR_ELF_SZ (sizeof("\x7f\ELF") - 1)
#define P_JSTR_UTF    "\xEF\xBB\xBF"
#define P_JSTR_UTF_SZ (sizeof("\xEF\xBB\xBF") - 1)

/*
   Check if the first 64 bytes or fewer contain any unprintable character.
*/
JSTR_FUNC
static int
jstr_io_isbinary_maybe(const char *R const buf,
		       const size_t sz) JSTR_NOEXCEPT
{
#define JSTR_BINARY_CHECK()                                                         \
	do {                                                                        \
		if (jstr_likely(sz > P_JSTR_ELF_SZ - 1)) {                          \
			if (jstr_unlikely(!memcmp(buf, P_JSTR_ELF, P_JSTR_ELF_SZ))) \
				return 1;                                           \
check_utf:;                                                                         \
			if (!memcmp(buf, P_JSTR_UTF, P_JSTR_UTF_SZ))                \
				return 0;                                           \
		} else if (jstr_likely(sz == P_JSTR_UTF_SZ)) {                      \
			goto check_utf;                                             \
		}                                                                   \
	} while (0)
	if (jstr_unlikely(sz == 0))
		return 0;
	JSTR_BINARY_CHECK();
	const unsigned char *const end = (unsigned char *)buf + JSTR_MIN(sz, 64) + 1;
	const unsigned char *s = (unsigned char *)buf;
	while (s < end)
		if (p_jstr_io_reject_table[*s++])
			return 1;
	return 0;
}

/*
   Check if the first 32 bytes or fewer contain any unprintable character.
*/
JSTR_FUNC_PURE
JSTR_INLINE
static int
jstr_io_isbinary_maybe_j(jstr_ty *R const j) JSTR_NOEXCEPT
{
	return jstr_io_isbinary_maybe(j->data, j->size);
}

/*
   Check the whole file for a NUL byte.
   File must be nul terminated.
*/
JSTR_FUNC_PURE
static int
jstr_io_isbinary(const char *R const buf,
		 const size_t sz) JSTR_NOEXCEPT
{
	JSTR_BINARY_CHECK();
	return strlen(buf) != sz;
}

#undef JSTR_BINARY_CHECK
#undef P_JSTR_ELF
#undef P_JSTR_ELF_SZ
#undef P_JSTR_UTF
#undef P_JSTR_UTF_SZ

/*
   Check the whole file for any unprintable character.
   File must be nul terminated.
*/
JSTR_FUNC_PURE
JSTR_INLINE
static int
jstr_io_isbinary_j(jstr_ty *R const j) JSTR_NOEXCEPT
{
	return jstr_io_isbinary(j->data, j->size);
}

JSTR_FUNC
JSTR_INLINE
static int
jstr_io_fwrite(const char *R const s,
	       const size_t sz,
	       FILE *R const fp) JSTR_NOEXCEPT
{
	fwrite(s, 1, sz, fp);
	return ferror(fp) ? 0 : 1;
}

JSTR_FUNC
JSTR_INLINE
static int
jstr_io_fwrite_j(const jstr_ty *R const j,
		 FILE *R const fp) JSTR_NOEXCEPT
{
	return jstr_io_fwrite(j->data, j->size, fp);
}

JSTR_FUNC
JSTR_INLINE
static int
jstr_io_fwrite_file(const char *R const s,
		    const size_t sz,
		    const char *R const fname,
		    const char *R const modes) JSTR_NOEXCEPT
{
	FILE *R const fp = fopen(fname, modes);
	if (jstr_unlikely(fp == NULL))
		return 0;
	if (jstr_unlikely(!jstr_io_fwrite(s, sz, fp)))
		goto err;
	fclose(fp);
	return 1;
err:
	fclose(fp);
	return 0;
}

JSTR_FUNC
JSTR_INLINE
static int
jstr_io_fwrite_file_j(const jstr_ty *R const j,
		      const char *R const fname,
		      const char *R const modes) JSTR_NOEXCEPT
{
	return jstr_io_fwrite_file(j->data, j->size, fname, modes);
}

#if JSTR_HAVE_POPEN

#	if 0 /* broken */

JSTR_FUNC
static int
jstr_io_alloc_popen(char *R *R const s,
		    size_t *R const sz,
		    size_t *R const cap,
		    const char *R const cmd) JSTR_NOEXCEPT
{
	FILE *R const fp = popen(cmd, "r");
	if (jstr_unlikely(fp == NULL))
		goto err;
	char *p;
	enum { MINBUF = BUFSIZ };
	char buf[MINBUF];
	p = buf;
	int c;
	while (((c = getc(fp)) != EOF)
	       & (p - buf != MINBUF))
		*p++ = c;
	*cap = P_JSTR_MIN_ALLOC(p - buf);
	*cap = JSTR_ALIGN_UP_STR(*cap);
	*s = (char *)malloc(*cap);
	P_JSTR_MALLOC_ERR(*s, goto err_close);
	memcpy(*s, buf, p - buf);
	if (jstr_unlikely(p - buf == MINBUF)) {
		p = *s + (p - buf);
		const char *old;
		while ((c = getc(fp)) != EOF) {
			if (jstr_unlikely((size_t)(p - *s) == *cap)) {
				old = *s;
				P_JSTR_REALLOCEXACT(*s, *cap, (size_t)(*cap * JSTR_GROWTH), goto err_close);
				p = *s + (p - old);
			}
			*p++ = c;
		}
		*p = '\0';
		*sz = p - *s;
	} else {
		(*s)[p - buf] = '\0';
		*sz = p - buf;
	}
	pclose(fp);
	return 1;
err_close:
	pclose(fp);
err:
	return 0;
}

#	else

JSTR_FUNC
static int
jstr_io_alloc_popen(char *R *R const s,
		    size_t *R const sz,
		    size_t *R const cap,
		    const char *R const cmd) JSTR_NOEXCEPT
{
	FILE *R const fp = popen(cmd, "r");
	if (jstr_unlikely(fp == NULL))
		goto err;
	char *p;
	enum { MINBUF = 4096 };
	char buf[MINBUF];
	p = buf + fread(buf, 1, MINBUF, fp);
	if (jstr_unlikely(ferror(fp)))
		goto err_close;
	*cap = P_JSTR_MIN_ALLOC(p - buf);
	*cap = JSTR_ALIGN_UP_STR(*cap);
	*s = (char *)malloc(*cap);
	P_JSTR_MALLOC_ERR(*s, goto err_close);
	memcpy(*s, buf, p - buf);
	*sz = p - buf;
	if (jstr_unlikely(p - buf == MINBUF)) {
		p = *s + (p - buf);
		const char *old;
		size_t req_size;
		for (;;) {
			req_size = *cap - *sz;
			old = p;
			p += fread(p, 1, req_size, fp);
			*sz += (p - old);
			if ((size_t)(p - old) < req_size)
				break;
			if ((size_t)(p - *s) == *cap) {
				old = *s;
				P_JSTR_REALLOCEXACT(*s, *cap, (size_t)(*cap * JSTR_GROWTH), goto err_close);
				p = *s + (p - old);
			}
		}
		if (jstr_unlikely(ferror(fp)))
			goto err_close_free;
		*p = '\0';
	} else {
		(*s)[p - buf] = '\0';
	}
	pclose(fp);
	return 1;
err_close_free:
	free(*s);
err_close:
	pclose(fp);
err:
	return 0;
}

#	endif

#endif

JSTR_INLINE
JSTR_FUNC
static int
p_jstr_io_alloc_file(const int alloc_exact,
		     char *R *R const s,
		     size_t *R const sz,
		     size_t *R const cap,
		     const char *R const fname,
		     struct stat *R const st) JSTR_NOEXCEPT
{
	FILE *R const fp = fopen(fname, "r");
	if (jstr_unlikely(fp == NULL))
		goto err;
	if (jstr_unlikely(stat(fname, st)))
		goto err_close;
	if (alloc_exact)
		*cap = P_JSTR_MIN_ALLOCEXACT(st->st_size + 1);
	else
		*cap = P_JSTR_MIN_ALLOC(st->st_size);
	*cap = JSTR_ALIGN_UP_STR(*cap);
	*s = (char *)malloc(*cap);
	P_JSTR_MALLOC_ERR(*s, goto err_close);
	fread(*s, 1, st->st_size, fp);
	if (jstr_unlikely(ferror(fp)))
		goto err_close_free;
	fclose(fp);
	(*s)[st->st_size] = '\0';
	*sz = st->st_size;
	return 1;
err_close_free:
	free(*s);
err_close:
	fclose(fp);
err:
	return 0;
}

/*
   Return value:
   0 on error;
   otherwise 1.
*/
JSTR_FUNC
static int
jstr_io_alloc_file(char *R *R const s,
		   size_t *R const sz,
		   size_t *R const cap,
		   const char *R const fname,
		   struct stat *R const st) JSTR_NOEXCEPT
{
	return p_jstr_io_alloc_file(0, s, sz, cap, fname, st);
}

/*
   Return value:
   0 on error;
   otherwise 1.
*/
JSTR_MAYBE_UNUSED
JSTR_WARN_UNUSED
JSTR_NONNULL_ALL
JSTR_NOTHROW
static int
jstr_io_allocexact_file(char *R *R const s,
			size_t *R const sz,
			size_t *R const cap,
			const char *R const fname,
			struct stat *R const st) JSTR_NOEXCEPT
{
	return p_jstr_io_alloc_file(1, s, sz, cap, fname, st);
}

/*
   Return value:
   0 on error;
   otherwise 1.
*/
JSTR_FUNC
static int
jstr_io_alloc_file_j(jstr_ty *R const j,
		     const char *R const fname,
		     struct stat *R const st) JSTR_NOEXCEPT
{
	return jstr_io_alloc_file(&j->data, &j->size, &j->capacity, fname, st);
}

/*
   Return value:
   0 on error;
   otherwise 1.
*/
JSTR_FUNC
static int
jstr_io_allocexact_file_j(jstr_ty *R const j,
			  const char *R const fname,
			  struct stat *R const st) JSTR_NOEXCEPT
{
	return jstr_io_allocexact_file(&j->data, &j->size, &j->capacity, fname, st);
}

enum {
	JSTR_IO_MAX_FNAME = 4096,
};

JSTR_INLINE
JSTR_FUNC_RET_NONNULL
static char *
jstr_io_append_path_p(char *R const path_end,
		      const char *R fname)
{
	*path_end = '/';
	return jstr_stpcpy(path_end + 1, fname);
}

JSTR_INLINE
JSTR_FUNC_NOWARN
static void
jstr_io_append_path(char *R const path_end,
		    const char *R fname)
{
	*path_end = '/';
	strcpy(path_end + 1, fname);
}

JSTR_INLINE
JSTR_FUNC_NOWARN
static void
jstr_io_append_path_len(char *R const path_end,
			const char *R fname,
			const size_t flen)
{
	*path_end = '/';
	memcpy(path_end + 1, fname, flen + 1);
}

typedef enum jstr_io_ftw_flag_ty {
	/* Match glob with the filename instead of the whole path. */
	JSTR_IO_FTW_MATCH_PATH = (1),
	/* Call FN on regular files. */
	JSTR_IO_FTW_DO_REG = (JSTR_IO_FTW_MATCH_PATH << 1),
	/* Call FN on directories. */
	JSTR_IO_FTW_DO_DIR = (JSTR_IO_FTW_DO_REG << 1),
	/* Do not traverse subdirectories. */
	JSTR_IO_FTW_NO_SUBDIR = (JSTR_IO_FTW_DO_DIR << 1),
	/* Do not call stat. Only sb.st_mode is defined. */
	JSTR_IO_FTW_NO_STAT = (JSTR_IO_FTW_NO_SUBDIR << 1),
	/* Only call stat on regular files. */
	JSTR_IO_FTW_STAT_REG = (JSTR_IO_FTW_NO_STAT << 1),
} jstr_io_ftw_flag_ty;

#define GET_PATH()                                      \
	do {                                            \
		if (jstr_unlikely(fulpath[0] == '\0'))  \
			memcpy(fulpath, dirpath, dlen); \
	} while (0)

#if JSTR_HAVE_DIRENT_D_TYPE
#	define GET_STAT_MODE_MAYBE()                    \
		do {                                     \
			sb.st_mode = DTTOIF(ep->d_type); \
		} while (0)
#	define GET_PATH_MAYBE()    \
		do {                \
			GET_PATH(); \
		} while (0)
#	define STAT_MAYBE(filename, st)                  \
		do {                                      \
			if (jflags & JSTR_IO_FTW_NO_STAT) \
				GET_STAT_MODE_MAYBE();    \
			else                              \
				stat(filename, st);       \
		} while (0)
#else
#	define GET_STAT_MODE_MAYBE() \
		do {                  \
		} while (0)
#	define GET_PATH_MAYBE() \
		do {             \
		} while (0)
#	define STAT_MAYBE(filename, st) \
		do {                     \
		} while (0)
#endif

#ifdef _DIRENT_HAVE_D_NAMLEN
#	ifndef _D_EXACT_NAMLEN
#		define _D_EXACT_NAMLEN(d) ((d)->d_namlen)
#	endif
#	ifndef _D_ALLOC_NAMLEN
#		define _D_ALLOC_NAMLEN(d) (_D_EXACT_NAMLEN(d) + 1)
#	endif
#else
#	ifndef _D_EXACT_NAMLEN
#		define _D_EXACT_NAMLEN(d) (strlen((d)->d_name))
#	endif
#	ifndef _D_ALLOC_NAMLEN
#		ifdef _DIRENT_HAVE_D_RECLEN
#			define _D_ALLOC_NAMLEN(d) (((char *)(d) + (d)->d_reclen) - &(d)->d_name[0])
#		else
#			define _D_ALLOC_NAMLEN(d) (sizeof(d)->d_name > 1 ? sizeof(d)->d_name \
									  : _D_EXACT_NAMLEN(d) + 1)
#		endif
#	endif
#endif

/*
   Call FN on entries found recursively that matches GLOB.
   If either the DO_REG or DO_DIR flag is used, FN will not be called on other filetypes.
   If GLOB is NULL, all entries match.
   ARG is a ptr to struct which contains additional arguments to be passed to FN.
   FN therefore must correctly interpret ARG.
   Jflags (prefixed with JSTR_IO_FTW_):
   MATCH_PATH: match dirpath instead of fulpath.
   NO_SUBDIR: do not traverse subdirectories.
   NO_STAT: do not call stat. Only sb.mode is defined.
   STAT_REG: only call stat on regular files.
   DO_REG: avoid calling FN on other filetypes.
   DO_DIR: avoid calling FN on other filetypes.
*/
JSTR_FUNC_MAY_NULL
static void
jstr_io_ftw_len(const char *R const dirpath,
		const size_t dlen,
		const char *R const fn_glob,
		const int fn_flags,
		const jstr_io_ftw_flag_ty jflags,
		void (*fn)(const char *dirpath, const struct stat *sb, const void *arg),
		const void *R const arg)
{
	DIR *R const dp = opendir(dirpath);
	if (jstr_unlikely(dp == NULL))
		return;
	char fulpath[JSTR_IO_MAX_FNAME];
	fulpath[0] = '\0';
	struct stat sb;
	size_t fulpathlen;
	for (const struct dirent *R ep; (ep = readdir(dp));) {
		/* Ignore . and .. . */
		if (jstr_unlikely((ep->d_name)[0] == '.')
		    && (jstr_unlikely(((ep->d_name)[1] == '\0'))
			|| ((jstr_unlikely((ep->d_name)[1] == '.'))
			    && jstr_unlikely((ep->d_name)[2] == '\0'))))
			continue;
#if JSTR_HAVE_DIRENT_D_TYPE
		if (ep->d_type == DT_REG)
			goto do_reg;
		if (ep->d_type == DT_DIR)
			goto do_dir;
#else
		GET_PATH();
		fulpathlen = jstr_io_append_path_p(fulpath + dlen, ep->d_name) - fulpath;
		if (jstr_unlikely(stat(fulpath, &sb)))
			continue;
		if (S_ISREG(sb.st_mode))
			goto do_reg;
		if (S_ISDIR(sb.st_mode))
			goto do_dir;
#endif /* HAVE_D_TYPE */
		/* If either DO_DIR or DO_REG is used, ignore other filetypes. */
		if (jflags & (JSTR_IO_FTW_DO_DIR | JSTR_IO_FTW_DO_REG))
			continue;
do_reg:
		if ((jflags & JSTR_IO_FTW_DO_DIR)
		    && !(jflags & JSTR_IO_FTW_DO_REG))
			continue;
		if (fn_glob) {
			if (jflags & JSTR_IO_FTW_MATCH_PATH) {
				GET_PATH_MAYBE();
				if (fnmatch(fn_glob, fulpath, fn_flags))
					continue;
			} else {
				if (fnmatch(fn_glob, ep->d_name, fn_flags))
					continue;
				GET_PATH_MAYBE();
			}
		} else {
			GET_PATH_MAYBE();
		}
#if JSTR_HAVE_DIRENT_D_TYPE
#	if JSTR_HAVE_DIRENT_D_NAMLEN
		jstr_io_append_path_len(fulpath + dlen, ep->d_name, ep->d_namlen);
#	else
		jstr_io_append_path(fulpath + dlen, ep->d_name);
#	endif
#endif
		if (jflags & JSTR_IO_FTW_STAT_REG) {
#if JSTR_HAVE_DIRENT_D_TYPE
			if (ep->d_type == DT_REG)
#else
			if (S_ISREG(sb.st_mode))
#endif
				STAT_MAYBE(fulpath, &sb);
			else
				GET_STAT_MODE_MAYBE();
		} else {
			STAT_MAYBE(fulpath, &sb);
		}
		fn(fulpath, &sb, arg);
		continue;
do_dir:
		if (jflags & JSTR_IO_FTW_NO_SUBDIR)
			continue;
		GET_PATH_MAYBE();
#if JSTR_HAVE_DIRENT_D_TYPE
#	if JSTR_HAVE_DIRENT_D_NAMLEN
		jstr_io_append_path_len(fulpath + dlen, ep->d_name, ep->d_namlen) - dirpath;
		fulpathlen = dlen + 1 + ep->d_namlen;
#	else
		fulpathlen = jstr_io_append_path_p(fulpath + dlen, ep->d_name) - dirpath;
#	endif
#endif
		if (!(jflags & JSTR_IO_FTW_STAT_REG))
			STAT_MAYBE(fulpath, &sb);
		if ((jflags & JSTR_IO_FTW_DO_REG)
		    && (jflags & JSTR_IO_FTW_DO_DIR))
			fn(fulpath, &sb, arg);
		else
			fn(fulpath, &sb, arg);
		jstr_io_ftw_len(fulpath, fulpathlen, fn_glob, fn_flags, jflags, fn, arg);
		continue;
	}
	closedir(dp);
}

#undef GET_PATH
#undef GET_PATH_MAYBE
#undef STAT_MAYBE
#undef GET_STAT_MODE_MAYBE

/*
   Call FN on entries found recursively that matches GLOB.
   If either the DO_REG or DO_DIR flag is used, FN will not be called on other filetypes.
   If GLOB is NULL, all entries match.
   ARG is a ptr to struct which contains additional arguments to be passed to FN.
   FN therefore must correctly interpret ARG.
   Jflags (prefixed with JSTR_IO_FTW_):
   MATCH_PATH: match FULPATH instead of filename.
   NO_SUBDIR: do not traverse subdirectories.
   NO_STAT: do not call stat. Only sb.mode is defined.
   STAT_REG: only call stat on regular files.
   DO_REG: avoid calling FN on other filetypes.
   DO_DIR: avoid calling FN on other filetypes.
*/
JSTR_FUNC_MAY_NULL
JSTR_INLINE
static void
jstr_io_ftw(const char *R const dirpath,
	    const char *R const fn_glob,
	    const int fn_flags,
	    const jstr_io_ftw_flag_ty jflags,
	    void (*fn)(const char *dirpath, const struct stat *sb, const void *arg),
	    const void *R const arg)
{
	jstr_io_ftw_len(dirpath, strlen(dirpath), fn_glob, fn_flags, jflags, fn, arg);
}

P_JSTR_END_DECLS

#undef R

#endif /* JSTR_IO_H */
