/* The following functions are based on newlib's strstr.
   Copyright (c) 2018 Arm Ltd.  All rights reserved.

   BSD-3-Clause

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:
   1. Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
   2. Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
   3. The name of the company may not be used to endorse or promote
      products derived from this software without specific prior written
      permission.

   THIS SOFTWARE IS PROVIDED BY ARM LTD ``AS IS'' AND ANY EXPRESS OR IMPLIED
   WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
   MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
   IN NO EVENT SHALL ARM LTD BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
   TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */

#ifndef PJSTR_STRSTR234_FUNC
#	define PJSTR_STRSTR234_FUNC pjstr_strstr
#endif
#ifndef PJSTR_STRSTR234_CANONIZE
#	define PJSTR_STRSTR234_CANONIZE(x) (x)
#endif
#define L PJSTR_STRSTR234_CANONIZE
#if PJSTR_STRSTR234_STRNSTR
#	define N_PARAM , size_t l
#	define N       l-- &&
#else
#	define N_PARAM
#	define N
#endif

#include "jstr-macros.h"

#if defined PJSTR_STRSTR234_MEMMEM

JSTR_ATTR_ACCESS((__read_only__, 1, 3))
JSTR_FUNC_PURE
static char *
JSTR_CONCAT(PJSTR_STRSTR234_FUNC, 2)(const unsigned char *hs,
                                     const unsigned char *const ne,
                                     size_t l)
JSTR_NOEXCEPT
{
#	if JSTR_LP64
	typedef uint32_t size_ty;
	enum { sh = 16 };
#	else
	typedef uint16_t size_ty;
	enum { sh = 8 };
#	endif
	const size_ty nw = (size_ty)L(ne[0]) << sh | L(ne[1]);
	size_ty hw = (size_ty)L(hs[0]) << sh | L(hs[1]);
	for (hs += 2, l -= 2; l-- && hw != nw; hw = hw << sh | L(*hs++)) {}
	return (hw == nw) ? (char *)(hs - 2) : NULL;
}

JSTR_ATTR_ACCESS((__read_only__, 1, 3))
JSTR_FUNC_PURE
static char *
JSTR_CONCAT(PJSTR_STRSTR234_FUNC, 3)(const unsigned char *hs,
                                     const unsigned char *const ne,
                                     size_t l)
JSTR_NOEXCEPT
{
	typedef uint32_t U32;
	const U32 nw = (U32)L(ne[0]) << 24 | L(ne[1]) << 16 | L(ne[2]) << 8;
	U32 hw = (U32)L(hs[0]) << 24 | L(hs[1]) << 16 | L(hs[2]) << 8;
	for (hs += 3, l -= 3; l-- && hw != nw; hw = (hw | L(*hs++)) << 8) {}
	return (hw == nw) ? (char *)(hs - 3) : NULL;
}

JSTR_ATTR_ACCESS((__read_only__, 1, 3))
JSTR_FUNC_PURE
static char *
JSTR_CONCAT(PJSTR_STRSTR234_FUNC, 4)(const unsigned char *hs,
                                     const unsigned char *const ne,
                                     size_t l)
JSTR_NOEXCEPT
{
	typedef uint32_t U32;
	const U32 nw = (U32)L(ne[0]) << 24 | L(ne[1]) << 16 | L(ne[2]) << 8 | L(ne[3]);
	U32 hw = (U32)L(hs[0]) << 24 | L(hs[1]) << 16 | L(hs[2]) << 8 | L(hs[3]);
	for (hs += 4, l -= 4; l-- && hw != nw; hw = hw << 8 | L(*hs++)) {}
	return (hw == nw) ? (char *)(hs - 4) : NULL;
}

JSTR_ATTR_ACCESS((__read_only__, 1, 3))
JSTR_FUNC_PURE
static char *
JSTR_CONCAT(PJSTR_STRSTR234_FUNC, 5)(const unsigned char *hs,
                                     const unsigned char *const ne,
                                     size_t l)
JSTR_NOEXCEPT
{
	typedef uint64_t U64;
	const U64 nw = (U64)L(ne[0]) << 56 | (U64)L(ne[1]) << 48 | (U64)L(ne[2]) << 40 | (U64)L(ne[3]) << 32 | (U64)L(ne[4]) << 24;
	U64 hw = (U64)L(hs[0]) << 56 | (U64)L(hs[1]) << 48 | (U64)L(hs[2]) << 40 | (U64)L(hs[3]) << 32 | (U64)L(hs[4]) << 24;
	for (hs += 5, l -= 5; l-- && hw != nw; hw = hw << 8 | L(*hs++) << 24) {}
	return (hw == nw) ? (char *)(hs - 5) : NULL;
}

JSTR_ATTR_ACCESS((__read_only__, 1, 3))
JSTR_FUNC_PURE
static char *
JSTR_CONCAT(PJSTR_STRSTR234_FUNC, 6)(const unsigned char *hs,
                                     const unsigned char *const ne,
                                     size_t l)
JSTR_NOEXCEPT
{
	typedef uint64_t U64;
	const U64 nw = (U64)L(ne[0]) << 56 | (U64)L(ne[1]) << 48 | (U64)L(ne[2]) << 40 | (U64)L(ne[3]) << 32 | (U64)L(ne[4]) << 24 | (U64)L(ne[5]) << 16;
	U64 hw = (U64)L(hs[0]) << 56 | (U64)L(hs[1]) << 48 | (U64)L(hs[2]) << 40 | (U64)L(hs[3]) << 32 | (U64)L(hs[4]) << 24 | (U64)L(hs[5]) << 16;
	for (hs += 6, l -= 6; l-- && hw != nw; hw = hw << 8 | L(*hs++) << 16) {}
	return (hw == nw) ? (char *)(hs - 6) : NULL;
}

JSTR_ATTR_ACCESS((__read_only__, 1, 3))
JSTR_FUNC_PURE
static char *
JSTR_CONCAT(PJSTR_STRSTR234_FUNC, 7)(const unsigned char *hs,
                                     const unsigned char *const ne,
                                     size_t l)
JSTR_NOEXCEPT
{
	typedef uint64_t U64;
	const U64 nw = (U64)L(ne[0]) << 56 | (U64)L(ne[1]) << 48 | (U64)L(ne[2]) << 40 | (U64)L(ne[3]) << 32 | (U64)L(ne[4]) << 24 | (U64)L(ne[5]) << 16 | (U64)L(ne[6]) << 8;
	U64 hw = (U64)L(hs[0]) << 56 | (U64)L(hs[1]) << 48 | (U64)L(hs[2]) << 40 | (U64)L(hs[3]) << 32 | (U64)L(hs[4]) << 24 | (U64)L(hs[5]) << 16 | (U64)L(hs[6]) << 8;
	for (hs += 7, l -= 7; l-- && hw != nw; hw = (hw | L(*hs++)) << 8) {}
	return (hw == nw) ? (char *)(hs - 7) : NULL;
}

JSTR_ATTR_ACCESS((__read_only__, 1, 3))
JSTR_FUNC_PURE
static char *
JSTR_CONCAT(PJSTR_STRSTR234_FUNC, 8)(const unsigned char *hs,
                                     const unsigned char *const ne,
                                     size_t l)
JSTR_NOEXCEPT
{
	typedef uint64_t U64;
	const U64 nw = (U64)L(ne[0]) << 56 | (U64)L(ne[1]) << 48 | (U64)L(ne[2]) << 40 | (U64)L(ne[3]) << 32 | (U64)L(ne[4]) << 24 | (U64)L(ne[5]) << 16 | (U64)L(ne[6]) << 8 | (U64)L(ne[7]);
	U64 hw = (U64)L(hs[0]) << 56 | (U64)L(hs[1]) << 48 | (U64)L(hs[2]) << 40 | (U64)L(hs[3]) << 32 | (U64)L(hs[4]) << 24 | (U64)L(hs[5]) << 16 | (U64)L(hs[6]) << 8 | (U64)L(hs[7]);
	for (hs += 8, l -= 8; l-- && hw != nw; hw = hw << 8 | L(*hs++)) {}
	return (hw == nw) ? (char *)(hs - 8) : NULL;
}

#else

JSTR_FUNC_PURE
static char *
JSTR_CONCAT(PJSTR_STRSTR234_FUNC, 2)(const unsigned char *hs,
                                     const unsigned char *const ne
                                     N_PARAM)
{
#	if JSTR_LP64
	typedef uint32_t size_ty;
	enum { sh = 16 };
#	else
	typedef uint16_t size_ty;
	enum { sh = 8 };
#	endif
	const size_ty nw = (size_ty)(L(ne[0]) << sh) | L(ne[1]);
	size_ty hw = 0;
	unsigned int c;
	for (c = L(hs[0]); N nw != hw && c != 0; c = L(*++hs))
		hw = (hw << sh) | c;
	return nw == hw ? (char *)hs - 2 : NULL;
}

JSTR_FUNC_PURE
static char *
JSTR_CONCAT(PJSTR_STRSTR234_FUNC, 3)(const unsigned char *hs,
                                     const unsigned char *const ne
                                     N_PARAM)
{
	typedef uint32_t U32;
	const U32 nw = (U32)(L(ne[0]) << 24) | (L(ne[1]) << 16) | (L(ne[2]) << 8);
	U32 hw = 0;
	unsigned int c;
	for (c = L(hs[0]); N nw != hw && c != 0; c = L(*++hs))
		hw = (hw | c) << 8;
	return nw == hw ? (char *)hs - 3 : NULL;
}

JSTR_FUNC_PURE
static char *
JSTR_CONCAT(PJSTR_STRSTR234_FUNC, 4)(const unsigned char *hs,
                                     const unsigned char *const ne
                                     N_PARAM)
{
	typedef uint32_t U32;
	const U32 nw = (U32)(L(ne[0]) << 24) | (L(ne[1]) << 16) | (L(ne[2]) << 8) | L(ne[3]);
	U32 hw = 0;
	unsigned int c;
	for (c = L(hs[0]); N c != 0 && nw != hw; c = L(*++hs))
		hw = (hw << 8) | c;
	return nw == hw ? (char *)hs - 4 : NULL;
}

JSTR_FUNC_PURE
static char *
JSTR_CONCAT(PJSTR_STRSTR234_FUNC, 5)(const unsigned char *hs,
                                     const unsigned char *const ne
                                     N_PARAM)
{
	typedef uint64_t U64;
	const U64 nw = (U64)L(ne[0]) << 56 | (U64)L(ne[1]) << 48 | (U64)L(ne[2]) << 40 | (U64)L(ne[3]) << 32 | (U64)L(ne[4]) << 24;
	U64 hw = 0;
	unsigned int c;
	for (c = L(hs[0]); N c != 0 && nw != hw; c = L(*++hs))
		hw = (hw << 8) | c << 24;
	return nw == hw ? (char *)hs - 5 : NULL;
}

JSTR_FUNC_PURE
static char *
JSTR_CONCAT(PJSTR_STRSTR234_FUNC, 6)(const unsigned char *hs,
                                     const unsigned char *const ne
                                     N_PARAM)
{
	typedef uint64_t U64;
	const U64 nw = (U64)L(ne[0]) << 56 | (U64)L(ne[1]) << 48 | (U64)L(ne[2]) << 40 | (U64)L(ne[3]) << 32 | (U64)L(ne[4]) << 24 | (U64)L(ne[5]) << 16;
	U64 hw = 0;
	unsigned int c;
	for (c = L(hs[0]); N c != 0 && nw != hw; c = L(*++hs))
		hw = (hw << 8) | c << 16;
	return nw == hw ? (char *)hs - 6 : NULL;
}

JSTR_FUNC_PURE
static char *
JSTR_CONCAT(PJSTR_STRSTR234_FUNC, 7)(const unsigned char *hs,
                                     const unsigned char *const ne
                                     N_PARAM)
{
	typedef uint64_t U64;
	const U64 nw = (U64)L(ne[0]) << 56 | (U64)L(ne[1]) << 48 | (U64)L(ne[2]) << 40 | (U64)L(ne[3]) << 32 | (U64)L(ne[4]) << 24 | (U64)L(ne[5]) << 16 | (U64)L(ne[6]) << 8;
	U64 hw = 0;
	unsigned int c;
	for (c = L(hs[0]); N c != 0 && nw != hw; c = L(*++hs))
		hw = (hw | c) << 8;
	return nw == hw ? (char *)hs - 7 : NULL;
}

JSTR_FUNC_PURE
static char *
JSTR_CONCAT(PJSTR_STRSTR234_FUNC, 8)(const unsigned char *hs,
                                     const unsigned char *const ne
                                     N_PARAM)
{
	typedef uint64_t U64;
	const U64 nw = (U64)L(ne[0]) << 56 | (U64)L(ne[1]) << 48 | (U64)L(ne[2]) << 40 | (U64)L(ne[3]) << 32 | (U64)L(ne[4]) << 24 | (U64)L(ne[5]) << 16 | (U64)L(ne[6]) << 8 | (U64)L(ne[7]);
	U64 hw = 0;
	unsigned int c;
	for (c = L(hs[0]); N c != 0 && nw != hw; c = L(*++hs))
		hw = (hw << 8) | c;
	return nw == hw ? (char *)hs - 8 : NULL;
}

#endif

#undef L
#undef N
#undef N_PARAM
#undef PJSTR_STRSTR234_FUNC
#undef PJSTR_STRSTR234_CANONIZE
#undef PJSTR_STRSTR234_MEMMEM
#undef PJSTR_STRSTR234_STRNSTR