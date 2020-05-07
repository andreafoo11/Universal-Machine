/*
 * io_instructions.h
 * Andrea Foo (afoo01) and Isabelle Lai (ilai01)
 * Comp40, HW7 um
 * Due: April 13, 2020
 * Purpose: Interface file for the input output instructions implementation
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <bitpack.h>
#include <assert.h>
#include <uarray.h>

#ifndef IO_INSTRUCTIONS_INCLUDED
#define IO_INSTRUCTIONS_INCLUDED


/* read input from stdin */
void read_input(UArray_T registers, int regC);

/* output one byte of value at register to stdout */
void output(UArray_T registers, int regC);

#endif
