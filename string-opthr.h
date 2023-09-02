/* Define a threshold for word access.  Generic version.
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

#ifndef PJSTR_STRING_OPTHR_H
#define PJSTR_STRING_OPTHR_H 1

/* Threshold value for when to enter the unrolled loops.  */
#ifdef JSTR_ARCH_x86_32
#	define JSTR_OP_THRES 8
#else
#	define JSTR_OP_THRES 16
#endif

#endif /* string-opthr.h */
