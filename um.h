/*
 * um.h
 * Andrea Foo (afoo01) and Isabelle Lai (ilai01)
 * Comp40, HW7 um
 * Due: April 13, 2020
 * Purpose: Interface file for the UM implementation
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <bitpack.h>
#include <assert.h>

#ifndef UM_INCLUDED
#define UM_INCLUDED

/* function to run um machine */
void run_machine(char *filename);

#endif
