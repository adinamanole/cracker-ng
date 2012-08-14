
/*!
 * \file rijndael.h
 * \brief Part of CPT Cracker-ng.
 * \author Mickaël 'Tiger-222' Schoentgen
 * \date 2011.08.14
 *
 * Copyright (C) 2000-2009 Peter Selinger.
 * Copyright (C) 2012 Mickaël 'Tiger-222' Schoentgen.
 * This file is part of ccrypt. It is free software and it is covered
 * by the GNU general public license. See the file COPYING for details.
 * 
 * $Id: rijndael.h 258 2009-08-26 17:46:10Z selinger $
 * 
 * derived from original source: rijndael-alg-ref.h   v2.0   August '99
 * Reference ANSI C code for NIST competition
 * authors: Paulo Barreto
 *          Vincent Rijmen
 * 
 * This is an optimized version for Cracker-ng.
 */


#ifndef RIJNDAEL_H
#define RIJNDAEL_H

typedef unsigned char word8;
typedef int word32;
union word8x4_u {
	word8 w8[4];
	word32 w32;
};
typedef union word8x4_u word8x4;

#include "tables.h"

#define MAXBC (256/32)
#define MAXRK (15*MAXBC)

typedef struct {
	word32 rk[MAXRK];
	int shift[2][4];
} roundkey;

/* keys and blocks are externally treated as word32 arrays, to
   make sure they are aligned on 4-byte boundaries on architectures
   that require it. */

/* make a roundkey rkk from key. key must have appropriate size given
   by keyBits. keyBits and blockBits may only be 128, 196, or
   256. Returns non-zero if arguments are invalid. */

int xrijndaelKeySched(word32 key[], roundkey *rkk);

/* encrypt, resp. decrypt, block using rijndael roundkey rkk. rkk must
   have been created with xrijndaelKeySched. Size of block, in bits,
   must be equal to blockBits parameter that was used to make rkk. In
   all other cases, behavior is undefined - for reasons of speed, no
   check for error conditions is done. */

void xrijndaelEncrypt(word32 block[], roundkey *rkk);
void xrijndaelDecrypt(word32 block[], roundkey *rkk);

#endif /* RIJNDAEL_H */
