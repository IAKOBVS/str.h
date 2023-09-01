/* Zero byte detection; indexes.  RISCV version.
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

#ifndef PJSTR_STRING_RISCV_FZI_H
#define PJSTR_STRING_RISCV_FZI_H 1

#ifdef __riscv_zbb
#include "string-fzi.h"
#else
/* Without bitmap pjstr_clz/pjstr_ctz extensions, it is faster to direct test the bits
   instead of calling compiler auxiliary functions.  */
#include "string-optype.h"

static JSTR_INLINE unsigned int
pjstr_first (jstr_op_ty c)
{
  if (c & 0x80U)
    return 0;
  if (c & 0x8000U)
    return 1;
  if (c & 0x800000U)
    return 2;

  if (sizeof (jstr_op_ty) == 4)
    return 3;

  if (c & 0x80000000U)
    return 3;
  if (c & 0x8000000000UL)
    return 4;
  if (c & 0x800000000000UL)
    return 5;
  if (c & 0x80000000000000UL)
    return 6;
  return 7;
}

static JSTR_INLINE unsigned int
pjstr_last (jstr_op_ty c)
{
  if (sizeof (jstr_op_ty) == 8)
    {
      if (c & 0x8000000000000000UL)
	return 7;
      if (c & 0x80000000000000UL)
	return 6;
      if (c & 0x800000000000UL)
	return 5;
      if (c & 0x8000000000UL)
	return 4;
    }
  if (c & 0x80000000U)
    return 3;
  if (c & 0x800000U)
    return 2;
  if (c & 0x8000U)
    return 1;
  return 0;
}
#endif

#endif /* STRING_FZI_H */
