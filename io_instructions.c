/*
 * io_instructions.c
 * Andrea Foo (afoo01) and Isabelle Lai (ilai01)
 * Comp40, HW6 um
 * Due: April 13, 2020
 * Purpose: Implementation file for the input output instructions in UM
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <bitpack.h>
#include <assert.h>
#include <uarray.h>
#include "io_instructions.h"

/* 
 * read_input
 * Input: UArray_T registers, int regC
 * Output: none
 * Purpose: Read one byte from std in and store as unsigned int
 *          and store it in register C
 */
void read_input(UArray_T registers, int regC)
{
    /* get pointer to register at given index */
    uint32_t *regC_val = UArray_at(registers, regC);
    
    /* get a char from stdin and store it at regC */
    char byte_val = getchar();
    *regC_val = (uint32_t) byte_val;
}

/* 
 * read_input
 * Input: UArray_T registers, int regC
 * Output: value in Register C (0 to 255)
 * Purpose: Get value in register C and output to stdout
 */
void output(UArray_T registers, int regC)
{
    /* get pointer to register at given index */
    uint32_t *regC_val = UArray_at(registers, regC);
    
    /* output one char from the register to stdout */
    char byte_val = *regC_val;
    putchar(byte_val);
}
