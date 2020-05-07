/*
 * arith_instructions.h
 * Andrea Foo (afoo01) and Isabelle Lai (ilai01)
 * Comp40, HW6 um
 * Due: April 13, 2020
 * Purpose: Interface file for the arithmetic instructions implementation
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <bitpack.h>
#include <assert.h>
#include <uarray.h>

#ifndef ARITH_INSTRUCTIONS_INCLUDED
#define ARITH_INSTRUCTIONS_INCLUDED

/* add instruction */
void add(UArray_T registers, int regA, int regB, int regC);

/* multiply instruction */
void multiply(UArray_T registers, int regA, int regB, int regC);

/* divide instruction */
void divide(UArray_T registers, int regA, int regB, int regC);

/* load value instruction */
void load_value(UArray_T registers, int regA, int value);

/* conditional move instruction */
void conditional_move(UArray_T registers, int regA, int regB, int regC);

/* bitwise NAND instruction */
void bitwise_NAND(UArray_T registers, int regA, int regB, int regC);

#endif
