/* Zero byte detection; basics.  RISCV version.
   Copyright (C) 2023 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

#ifndef _RISCV_STRING_FZA_H
#define _RISCV_STRING_FZA_H 1

#ifdef __riscv_zbb
/* With bitmap extension we can use orc.b to find all zero bytes.  */
#include "string-misc.h"
#include "string-optype.h"

/* The functions return a byte mask.  */



/* This function returns 0xff for each byte that is zero in X.  */
static JSTR_INLINE jstr_op_ty
pjstr_find_zero_all (jstr_op_ty x)
{
  jstr_op_ty r;
  asm ("orc.b %0, %1" : "=r" (r) : "r" (x));
  return ~r;
}

/* This function returns 0xff for each byte that is equal between X1 and
   X2.  */
static JSTR_INLINE jstr_op_ty
pjstr_find_eq_all (jstr_op_ty x1, jstr_op_ty x2)
{
  return pjstr_find_zero_all (x1 ^ x2);
}

/* Identify zero bytes in X1 or equality between X1 and X2.  */
static JSTR_INLINE jstr_op_ty
pjstr_find_zero_eq_all (jstr_op_ty x1, jstr_op_ty x2)
{
  return pjstr_find_zero_all (x1) | pjstr_find_eq_all (x1, x2);
}

/* Identify zero bytes in X1 or inequality between X1 and X2.  */
static JSTR_INLINE jstr_op_ty
pjstr_find_zero_ne_all (jstr_op_ty x1, jstr_op_ty x2)
{
  return pjstr_find_zero_all (x1) | ~pjstr_find_eq_all (x1, x2);
}

/* Define the "inexact" versions in terms of the exact versions.  */
# define pjstr_find_zero_low		pjstr_find_zero_all
# define pjstr_find_eq_low		pjstr_find_eq_all
# define pjstr_find_zero_eq_low	pjstr_find_zero_eq_all
#else
#include "string-fza.h"
#endif

#endif /* _RISCV_STRING_FZA_H  */
